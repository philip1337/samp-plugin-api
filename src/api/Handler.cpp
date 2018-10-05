#include "Handler.hpp"

SAMP_API_BEGIN_NS


void kick(const Request& req, Match m, std::string& response) {
	response = "OK";

	/* Print path vector to console */
	std::copy(req.body.begin(), req.body.end(), std::ostream_iterator<char>(std::cout, ""));
}

void Handler::RegisterFunctions() {
	routes_.emplace("/kick/:name", kick);
}

void Handler::HandleRequest(const Request& req, Reply& rep)
{
	// Router
	auto match = router_.set(req.uri);

	// Handle request
	rep.status = OnRequest(match, req, rep.content, req.method);
}

Reply::status_type Handler::OnRequest(Match& match, const Request& request, std::string& response, const std::string& type)
{
	for (auto& req : routes_) {
		if (match.test(req.first)) {
			req.second(request, match, response);
			return Reply::status_type::ok;
		}
	}

	return Reply::status_type::not_found;
}



SAMP_API_END_NS
