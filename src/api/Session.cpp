#include "Session.hpp"
#include "SessionManager.hpp"
#include "RequestParser.hpp"

#include <utility>
#include <vector>

SAMP_API_BEGIN_NS

Session::Session(boost::asio::io_service& io_service,
	SessionManager& manager, Handler& handler)
	: socket_(io_service)
	, session_manager_(manager)
	, handler_(handler)
{
	result = RequestParser::indeterminate;
}

void Session::Start()
{
	Read();
}

void Session::Stop()
{
	socket_.close();
}

void Session::Read()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(buffer_),
		[this, self](boost::system::error_code ec, std::size_t bytes_transferred)
	{
		if (!ec) {
			std::tie(result, std::ignore) = request_parser_.parse(
				request_, buffer_.data(), buffer_.data() + bytes_transferred);
			if (result == RequestParser::good) {
				handler_.HandleRequest(request_, reply_);
				Write();
			} else if (result == RequestParser::bad) {
				reply_ = Reply::stock_reply(Reply::bad_request);
				Write();
			} else {
				Read();
			}
		} else if (ec != boost::asio::error::operation_aborted) {
			session_manager_.Stop(shared_from_this());
		}
	});
}

void Session::Write()
{
	auto self(shared_from_this());
	boost::asio::async_write(socket_, reply_.to_buffers(),
		[this, self](boost::system::error_code ec, std::size_t)
	{
		if (!ec) {
			// Initiate graceful connection closure.
			boost::system::error_code ignored_ec;
			socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		}

		if (ec != boost::asio::error::operation_aborted) {
			session_manager_.Stop(shared_from_this());
		}
	});
}

SAMP_API_END_NS