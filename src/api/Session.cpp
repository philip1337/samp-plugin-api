#include "Session.hpp"
#include "Handler.hpp"

#include "Types.hpp"

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
		response_.result(http::status::ok);
		response_.set(http::field::server, "Beast");
		CreateResponse();
		break;

	default:
		// We return responses indicating an error if
		// we do not recognize the request method.
		response_.result(http::status::bad_request);
		response_.set(http::field::content_type, "text/plain");
		boost::beast::ostream(response_.body())
			<< "Invalid request-method '"
			<< request_.method_string().to_string()
			<< "'";
		break;
	}

	Write();
}

bool Session::VerifyRequest()
{
	// Get body
	auto data = boost::beast::buffers_to_string(request_.body().data());

	try {
		const auto body = spotify::json::decode<BodyRequest>(data);
		// TODO: Verification process
		return true;
	} catch (const spotify::json::decode_exception &e) {}
	return false;
}

void Session::CreateResponse()
{
	if (VerifyRequest())
		handler_.HandleRequest(request_, response_);
	else
		handler_.HandleInvalidRequest(request_, response_);
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
