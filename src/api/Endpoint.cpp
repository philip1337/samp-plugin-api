#include "Endpoint.hpp"
#include "Session.hpp"

#include <boost/beast.hpp>

SAMP_API_BEGIN_NS

Endpoint::Endpoint(boost::asio::io_service* service, uint32_t port) 
	:endpoint_(boost::asio::ip::tcp::v4(), port)
	,acceptor_(*service, endpoint_)
	,socket_(*service)
{
	handler_.RegisterFunctions();

	service_ = service;
	acceptor_.listen();
}

void Endpoint::Listen()
{
	acceptor_.async_accept(socket_,
		[this](boost::beast::error_code ec)
	{
		if (!ec)
			manager_.Start(std::make_shared<Session>(std::move(socket_), handler_, manager_));
		Listen();
	});
}

SAMP_API_END_NS
