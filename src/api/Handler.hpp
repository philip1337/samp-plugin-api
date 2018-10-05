#ifndef SRC_API_HANDLER_HPP
#define SRC_API_HANDLER_HPP

#pragma once

#include <Config.hpp>
#include "Route.hpp"

#include "Request.hpp"
#include "Reply.hpp"

SAMP_API_BEGIN_NS

typedef void(*apiFunc)(const Request& req, Match m, std::string& response);

class Handler {
public:
	void RegisterFunctions();
	void HandleRequest(const Request& req, Reply& rep);

	Reply::status_type OnRequest(Match& match, const Request& request, std::string& response, const std::string& type);

private:
	Route router_;

	std::map<std::string, apiFunc> routes_;
};

SAMP_API_END_NS

#endif
