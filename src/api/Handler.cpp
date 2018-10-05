#include "Handler.hpp"

#include "Types.hpp"
#include "SampTypes.hpp"
#include "PlayerManager.hpp"

#include <xxhash.h>
#include <a_samp.h>
#include <boost/beast/core.hpp>
#include <log/Log.hpp>

SAMP_API_BEGIN_NS

namespace player {
	bool list(Request& req, Response& response, Match& match, PlayerMessage& msg) {
		playerManager->GetPlayerData(msg.players);
		msg.count = msg.players.size();
		return true;
	}

	bool kick(Request& req, Response& response, Match& match, Message& msg) {
		auto str = match.get("name");

		if (!playerManager->Kick(str)) {
			msg.message = fmt::format("Invalid player {}", str);
			return false;
		}

		msg.message = fmt::format("Player {} got kicked", str);
		return true;
	}

	bool ban(Request& req, Response& response, Match& match, Message& msg) {
		auto str = match.get("name");

		if (!playerManager->Ban(str)) {
			msg.message = fmt::format("Invalid player {}", str);
			return false;
		}

		msg.message = fmt::format("Player {} got banned", str);
		return true;
	}
}

namespace misc {
	bool say(Request& req, Response& response, Match& match, Message& msg) {
		auto data = boost::beast::buffers_to_string(req.body().data());
		const auto request = spotify::json::decode<SayRequest>(data);

		// Message
		auto message = fmt::format("{}: {}", request.name, request.message);

		// Send message to all
		sampgdk::SendPlayerMessageToAll(request.color, message.c_str());

		msg.message = "OK";
		return true;
	}

	bool echo(Request& req, Response& response, Match& match, Message& msg) {
		auto data = boost::beast::buffers_to_string(req.body().data());
		const auto request = spotify::json::decode<SayRequest>(data);

		SAMP_INFO("Message from {}: {}", request.name, request.message);

		msg.message = "OK";
		return true;
	}

	bool ipban(Request& req, Response& response, Match& match, Message& msg) {
		auto data = boost::beast::buffers_to_string(req.body().data());
		const auto request = spotify::json::decode<IpRequest>(data);

		// Request failed
		if (sampgdk::BlockIpAddress(request.address.c_str(), request.time * 1000)) {
			msg.message = fmt::format("Failed to add ip to banlist: {}", request.address.c_str());
			return false;
		}

		msg.message = fmt::format("Added ip to banlist: {} time: {} seconds", request.address, request.time * 1000);
		return true;
	}

	bool ipunban(Request& req, Response& response, Match& match, Message& msg) {
		auto data = boost::beast::buffers_to_string(req.body().data());
		const auto request = spotify::json::decode<IpRequest>(data);

		// Request failed
		if (sampgdk::UnBlockIpAddress(request.address.c_str())) {
			msg.message = fmt::format("Failed to remove ip from banlist: {}", request.address.c_str());
			return false;
		}

		msg.message = fmt::format("Removed ip from banlist: {}", request.address);
		return true;
	}
}

namespace settings {
	bool gamemodetext(Request& req, Response& response, Match& match, Message& msg) {
		auto data = boost::beast::buffers_to_string(req.body().data());
		const auto request = spotify::json::decode<SimpleRequest>(data);

		// Request failed
		if (!sampgdk::SetGameModeText(request.value.c_str())) {
			msg.message = "Failed to update gamemode text";
			return false;
		}

		msg.message = "Gamemode text changed";
		return true;
	}

	bool gravity(Request& req, Response& response, Match& match, Message& msg) {
		auto data = boost::beast::buffers_to_string(req.body().data());
		const auto request = spotify::json::decode<GravityRequest>(data);

		// Request failed
		if (!sampgdk::SetGravity(request.gravity)) {
			msg.message = "Failed to update gravity";
			return false;
		}

		msg.message = "Gravity changed";
		return true;
	}

	bool weather(Request& req, Response& response, Match& match, Message& msg) {
		auto data = boost::beast::buffers_to_string(req.body().data());
		const auto request = spotify::json::decode<SimpleIntRequest>(data);
		
		// Request failed
		if (!sampgdk::SetWeather(request.value)) {
			msg.message = "Failed to update weather";
			return false;
		}

		msg.message = "Weather changed";
		return true;
	}

	bool list(Request& req, Response& response, Match& match, SettingsMessage& message) {
		message.tickcount = sampgdk::GetTickCount();
		message.tickrate = sampgdk::GetServerTickRate();
		message.gravity = sampgdk::GetGravity();
		message.apoolsize = sampgdk::GetActorPoolSize();
		message.vpoolsize = sampgdk::GetVehiclePoolSize();
		message.maxplayers = sampgdk::GetMaxPlayers();
		return true;
	}
}

#define CreateRoute(path, returntype, function) routes_.emplace(path, std::unique_ptr<SimpleRouteHandler>(new RouteHandler<returntype>(function)))

void Handler::RegisterFunctions() {
	CreateRoute("/player/list", PlayerMessage, player::list);

	CreateRoute("/player/kick/:name", Message, player::kick);
	CreateRoute("/player/ban/:name", Message, player::ban);

	CreateRoute("/misc/say", Message, misc::say);
	CreateRoute("/misc/echo", Message, misc::echo);
	CreateRoute("/misc/ip/ban", Message, misc::ipban);
	CreateRoute("/misc/ip/unban", Message, misc::ipunban);

	CreateRoute("/settings/gamemodetext", Message, settings::gamemodetext);
	CreateRoute("/settings/gravity", Message, settings::gravity);
	CreateRoute("/settings/weather", Message, settings::weather);

	CreateRoute("/settings/list", SettingsMessage, settings::list);
}

#undef CreateRoute

void Handler::ShowError(Request& req, Response& response, uint32_t code, std::string message, std::chrono::system_clock::time_point* p) {
	Message msg;
	msg.type = "error";
	msg.code = code;
	msg.message = message;

	auto match = route_.set(req.target().to_string());

	RequestInvalidHandler handler;
	handler.HandleRequest(req, response, match, p, msg);
}

void Handler::HandleRequest(Request& req, Response& response, std::string& remote, std::chrono::system_clock::time_point* p) {
	auto match = route_.set(req.target().to_string());
	if (!Authorize(req, remote)) {
		ShowError(req, response, 401, "Unauthorized", p);
		return;
	}
	
	for (auto& route : routes_) {
		if (match.test(route.first)) {
			response.result(http::status::ok);
			route.second.get()->HandleRequest(req, response, match, p);
			return;
		}
	}

	ShowError(req, response, 404, "Invalid route", p);
}

bool Handler::Authorize(Request& req, std::string& remote)
{
	auto uri = req.target().to_string();

	// Skip verification for debug builds
	//#ifdef _DEBUG
	//	SAMP_DEBUG("[{}] Skipping authorization.. uri: {}", remote, uri);
	//	return true;
	//#endif

	// Get body
	auto data = boost::beast::buffers_to_string(req.body().data());

	try {
		// Parse body
		const auto body = spotify::json::decode<BodyRequest>(data);
		auto h = XXH32(uri.data(), uri.size(), seed_);

		// Invalid password
		if (password_.compare(body.password) != 0 || h != body.hash) {
			SAMP_WARN("[{}] Invalid request password: {}  hash: {} required hash: {} uri: {}", 
				      remote, body.password, body.hash, h, uri);
			return false;
		}

		return true;
	}
	catch (const spotify::json::decode_exception &e) {}
	return false;
}

SAMP_API_END_NS
