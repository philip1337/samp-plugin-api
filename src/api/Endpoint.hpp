#ifndef SRC_API_ENDPOINT_HPP
#define SRC_API_ENDPOINT_HPP

#pragma once

#include <Config.hpp>

#include <boost/asio.hpp>
#include "SessionManager.hpp"
#include "Handler.hpp"

SAMP_API_BEGIN_NS

class Endpoint {
public:
	Endpoint(boost::asio::io_service* service, uint32_t port);

	void Listen();
	Handler& GetHandler() { return handler_; };

private:
	boost::asio::ip::tcp::endpoint endpoint_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::io_service* service_;

	Handler handler_;
	SessionManager manager_;
};

SAMP_API_END_NS

#endif
