#include "Handler.hpp"

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

void Handler::HandleRequest(Request& req, Response& response) {
	auto match = route_.set(req.target().to_string());
	for (auto& route : routes_) {
		if (match.test(route.first)) {
			route.second(req, response, match);
			return;
		}
	}
	InvalidRequest(response);
}

void Handler::HandleInvalidRequest(Request & req, Response & response) {
	// TODO: access.log
	InvalidRequest(response);
}

SAMP_API_END_NS
