#include "Endpoint.hpp"
#include "Session.hpp"

SAMP_API_BEGIN_NS

Endpoint::Endpoint(boost::asio::io_service* service, uint32_t port) 
	:endpoint_(boost::asio::ip::tcp::v4(), port)
	,acceptor_(*service, endpoint_)
{
	handler_.RegisterFunctions();

	service_ = service;
	acceptor_.listen();
}

void Endpoint::Listen()
{
	auto sesh = std::make_shared<Session>(*service_, manager_, handler_);
	acceptor_.async_accept(sesh->socket_,
		[sesh, this](const boost::system::error_code& accept_error)
	{
		Listen();
		if (!accept_error) {
			manager_.Start(sesh);
		}
	});
}

SAMP_API_END_NS
