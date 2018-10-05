#ifndef SRC_API_ENDPOINT_HPP
#define SRC_API_ENDPOINT_HPP

#pragma once

#include <Config.hpp>

#include <boost/asio.hpp>

#include "Handler.hpp"
#include "Route.hpp"
#include "SessionManager.hpp"

SAMP_API_BEGIN_NS

class Endpoint {
public:
	Endpoint(boost::asio::io_service* service, uint32_t port);

	void Listen();

private:
	boost::asio::ip::tcp::endpoint endpoint_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::io_service* service_;

	SessionManager manager_;
	Route route_;
	Handler handler_;
};

SAMP_API_END_NS

#endif
