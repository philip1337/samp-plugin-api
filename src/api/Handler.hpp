#ifndef SRC_API_HANDLER_HPP
#define SRC_API_HANDLER_HPP

#pragma once

#include <Config.hpp>

#include <boost/beast/http.hpp>
#include "Route.hpp"

SAMP_API_BEGIN_NS

namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

typedef http::request<http::dynamic_body> Request;
typedef http::response<http::dynamic_body> Response;

typedef void(*apiFunc)(Request& req, Response& response, Match& match);

class Handler {
public:
	void RegisterFunctions();

	void HandleRequest(Request& req, Response& response, std::string& remote);
	void HandleInvalidRequest(Request& req, Response& response, std::string& remote);

	bool VerifyRequest(Request& req, std::string& remote);
	void InvalidRequest(Response& response);

	//uint32_t OnRequest(Match& match, const Request& request, std::string& response, const std::string& type);

	void SetPassword(std::string& password) { password_ = password; }
	void SetSeed(uint32_t seed) { seed_ = seed; }

private:
	std::string password_;
	uint32_t seed_;

	std::map<std::string, apiFunc> routes_;
	Route route_;
};

SAMP_API_END_NS

#endif
