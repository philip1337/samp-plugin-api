#include "Session.hpp"
#include "Handler.hpp"

SAMP_API_BEGIN_NS

void Session::Start() {
	ReadRequest();
	CheckDeadline();
}

void Session::Stop() {
	socket_.close();
}

void Session::ReadRequest()
{
	auto self = shared_from_this();

	http::async_read(
		socket_,
		buffer_,
		request_,
		[self](boost::beast::error_code ec,
			std::size_t bytes_transferred)
	{
		boost::ignore_unused(bytes_transferred);
		if (!ec)
			self->ProcessRequest();
	});
}

void Session::ProcessRequest()
{
	response_.version(request_.version());
	response_.keep_alive(false);

	switch (request_.method())
	{
	case http::verb::post:
		CreateResponse();
		break;

	default:	// We don't accept get methods currently
		auto ip = socket_.remote_endpoint().address().to_string();
		handler_.HandleInvalidRequest(request_, response_, ip);
		break;
	}

	Write();
}

void Session::CreateResponse()
{
	auto ip = socket_.remote_endpoint().address().to_string();
	if (handler_.VerifyRequest(request_, ip))
		handler_.HandleRequest(request_, response_, ip);
	else
		handler_.HandleInvalidRequest(request_, response_, ip);
}

void Session::Write()
{
	auto self = shared_from_this();
	response_.set(http::field::content_length, response_.body().size());
	http::async_write(
		socket_,
		response_,
		[self](boost::beast::error_code ec, std::size_t)
	{
		self->socket_.shutdown(tcp::socket::shutdown_send, ec);
		self->deadline_.cancel();
	});
}

void Session::CheckDeadline()
{
	auto self = shared_from_this();
	deadline_.async_wait(
		[self](boost::beast::error_code ec)
	{
		if (!ec)
		{
			// Close socket to cancel any outstanding operation.
			self->socket_.close(ec);
		}
	});
}

SAMP_API_END_NS
