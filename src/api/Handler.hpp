#ifndef SRC_API_HANDLER_HPP
#define SRC_API_HANDLER_HPP

#pragma once

#include <Config.hpp>
#include <log/Log.hpp>

#include <boost/beast/http.hpp>
#include <chrono>
#include "Route.hpp"
#include "Types.hpp"

SAMP_API_BEGIN_NS

namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

typedef http::request<http::dynamic_body> Request;
typedef http::response<http::dynamic_body> Response;


class SimpleRouteHandler {
public:
	SimpleRouteHandler() {}

	//virtual void Handle(Request& req, Response& response, Match& match, std::chrono::system_clock::time_point* p) {
	//	HandleRequest(req, response, match, p);
	//};
	//
	//virtual void HandleRequest(Request& req, Response& response, Match& match, std::chrono::system_clock::time_point* p) {
	//	Message data;
	//	data.type = "response";
	//	data.code = 200;
	//	data.message = "OK";
	//	Finalize(response, data, p);
	//
	//	const auto json = spotify::json::encode(data);
	//	boost::beast::ostream(response.body()) << json;
	//};

	virtual void HandleRequest(Request& req, Response& response, Match& match, std::chrono::system_clock::time_point* p) = 0;

	virtual void Finalize(Response& response, Message& message, std::chrono::system_clock::time_point* p) {
		response.set(http::field::content_type, "application/json");
		response.result(message.code);

		// Request response time
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - *p);
		message.time = milliseconds.count();
	}
};

class RequestInvalidHandler : public SimpleRouteHandler {
public:
	RequestInvalidHandler() : SimpleRouteHandler() {

	}

	void HandleRequest(Request& req, Response& response, Match& match, std::chrono::system_clock::time_point* p) {
		Message data;
		data.type = "invalid";
		data.code = 422;
		data.message = "Invalid";

		// Finalize
		Finalize(response, data, p);

		const auto json = spotify::json::encode(data);
		boost::beast::ostream(response.body()) << json;
	}
	
	void HandleRequest(Request& req, Response& response, Match& match, std::chrono::system_clock::time_point* p, Message& data) {
		// Finalize
		Finalize(response, data, p);

		const auto json = spotify::json::encode(data);
		boost::beast::ostream(response.body()) << json;
	}
};

template <class T>
class RouteHandler : public SimpleRouteHandler {
	typedef bool(*handler)(Request& req, Response& response, Match& match, T& ResponseData);
public:
	RouteHandler(handler func) : SimpleRouteHandler(), handler_(func) {

	}

	void HandleRequest(Request& req, Response& response, Match& match, std::chrono::system_clock::time_point* p) {
		T data;
		data.message = "OK";

		try {
			if (handler_(req, response, match, data)) {
				data.type = "response";
				data.code = 200;
			} else {
				data.code = 417;
				data.type = "error";
			}
		} catch (const spotify::json::decode_exception &e) {
			data.code = 500;
			data.message = fmt::format("Invalid request error: {}", e.what());
		}
	
		// Finalize
		Finalize(response, data, p);

		SAMP_DEBUG("Response message header: {} {} {} {}", 
			      data.type, data.code, data.message, data.time);

		const auto json = spotify::json::encode(data);
		boost::beast::ostream(response.body()) << json;
	}
private:
	handler handler_;
};



class Handler {
public:
	void RegisterFunctions();

	void HandleRequest(Request& req, Response& response, std::string& remote, std::chrono::system_clock::time_point* p);
	bool Authorize(Request& req, std::string& remote);
	void ShowError(Request& req, Response& response, uint32_t code, std::string message, std::chrono::system_clock::time_point* p);

	void SetPassword(std::string& password) { password_ = password; }
	void SetSeed(uint32_t seed) { seed_ = seed; }
private:
	std::string password_;
	uint32_t seed_;

	//std::map<std::string, SimpleRouteHandler> routes_;
	std::map<std::string, std::unique_ptr<SimpleRouteHandler>> routes_;
	Route route_;
};

SAMP_API_END_NS

#endif
