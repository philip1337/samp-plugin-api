#ifndef SRC_API_CONNECTION_HPP
#define SRC_API_CONNECTION_HPP

#pragma once

#include <Config.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>

#include "FieldsAlloc.hpp"

SAMP_API_BEGIN_NS

namespace ip = boost::asio::ip;         // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

class SessionManager;
class Handler;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket, Handler& handler, SessionManager& manager)
        : socket_(std::move(socket))
		, handler_(handler)
		, manager_(manager)
    {}

    // Initiate the asynchronous operations associated with the connection.
	void Start();

	// Stop all operations for this session
	void Stop();

private:
	Handler& handler_;
	SessionManager& manager_;

    // The socket for the currently connected client.
    tcp::socket socket_;

    // The buffer for performing reads.
    boost::beast::flat_buffer buffer_{8192};

    // The request message.
    http::request<http::dynamic_body> request_;

    // The response message.
    http::response<http::dynamic_body> response_;

    // The timer for putting a deadline on connection processing.
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline_{
        socket_.get_io_service(), std::chrono::seconds(60)};

    // Asynchronously receive a complete request message.
	void ReadRequest();

    // Determine what needs to be done with the request message.
	void ProcessRequest();

    // Construct a response message based on the program state.
	void CreateResponse();

    // Asynchronously transmit the response message.
	void Write();

    // Check whether we have spent enough time on this connection.
	void CheckDeadline();

	// Verify
	bool VerifyRequest();
};

typedef std::shared_ptr<Session> SessionPtr;

SAMP_API_END_NS
#endif
