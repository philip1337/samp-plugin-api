#include "Handler.hpp"
#include "Types.hpp"

#include <xxhash.h>
#include <boost/beast/core.hpp>
#include <log/Log.hpp>

SAMP_API_BEGIN_NS

void kick(Request& req, Response& response, Match& match) {
	response.result(http::status::not_found);
	response.set(http::field::content_type, "text/plain");
	boost::beast::ostream(response.body()) << "OK\r\n";
}

void Handler::RegisterFunctions() {
	routes_.emplace("/kick/:name", kick);
}

void Handler::InvalidRequest(Response& response) {
	response.result(http::status::not_found);
	response.set(http::field::content_type, "text/plain");
	boost::beast::ostream(response.body()) << "Invalid request\r\n";
}

void Handler::HandleRequest(Request& req, Response& response, std::string& remote) {
	auto match = route_.set(req.target().to_string());
	for (auto& route : routes_) {
		if (match.test(route.first)) {
			route.second(req, response, match);
			return;
		}
	}
	InvalidRequest(response);
}

void Handler::HandleInvalidRequest(Request & req, Response & response, std::string& remote) {
	InvalidRequest(response);
}

bool IsNumber(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool Handler::VerifyRequest(Request& req, std::string& remote)
{
	// Get body
	auto data = boost::beast::buffers_to_string(req.body().data());
	auto uri = req.target().to_string();

	try {
		// Parse body
		const auto body = spotify::json::decode<BodyRequest>(data);
		auto h = XXH32(uri.data(), uri.size(), seed_);

		if (!IsNumber(body.hash))
			return false;

		// Invalid password
		if (password_.compare(body.password) != 0 || h != stoul(body.hash)) {
			SAMP_WARN("[{}] Invalid request password: {}  hash: {} required hash: {} uri: {}", remote, body.password, body.hash, h, uri);
			return false;
		}

		return true;
	}
	catch (const spotify::json::decode_exception &e) {}
	return false;
}

SAMP_API_END_NS
