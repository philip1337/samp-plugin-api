#ifndef SRC_API_SESSION_HPP
#define SRC_API_SESSION_HPP

#pragma once

#include <Config.hpp>

#include <memory>
#include <boost/asio.hpp>

#include "RequestParser.hpp"
#include "Request.hpp"
#include "Reply.hpp"
#include "Route.hpp"
#include "Handler.hpp"

SAMP_API_BEGIN_NS

class SessionManager;

/// Represents a single connection from a client.
class Session
	: public std::enable_shared_from_this<Session>
{
	friend class Endpoint;

public:
	Session(const Session&) = delete;
	Session& operator=(const Session&) = delete;

	/// Construct a connection with the given socket.
	explicit Session(boost::asio::io_service& io_service,
		SessionManager& manager, Handler& handler);

	/// Start the first asynchronous operation for the connection.
	void Start();

	/// Stop all asynchronous operations associated with the connection.
	void Stop();

private:
	/// Perform an asynchronous read operation.
	void Read();

	/// Perform an asynchronous write operation.
	void Write();

	/// Socket for the connection.
	boost::asio::ip::tcp::socket socket_;

	/// The manager for this connection.
	SessionManager& session_manager_;

	/// The handler used to process the incoming request.
	Handler& handler_;

	/// Buffer for incoming data.
	//boost::asio::streambuf buffer_;
	std::array<char, 8192> buffer_;

	/// The incoming request.
	Request request_;

	/// The parser for the incoming request.
	RequestParser request_parser_;

	/// The reply to be sent back to the client.
	Reply reply_;

	/// Result
	RequestParser::result_type result;

	uint32_t offset_;
};

typedef std::shared_ptr<Session> SessionPtr;

SAMP_API_END_NS
#endif
