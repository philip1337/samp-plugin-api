#include "Reply.hpp"

#include <string>

SAMP_API_BEGIN_NS

const std::string ok =
  "HTTP/1.0 200 OK\r\n";
const std::string created =
  "HTTP/1.0 201 Created\r\n";
const std::string accepted =
  "HTTP/1.0 202 Accepted\r\n";
const std::string no_content =
  "HTTP/1.0 204 No Content\r\n";
const std::string multiple_choices =
  "HTTP/1.0 300 Multiple Choices\r\n";
const std::string moved_permanently =
  "HTTP/1.0 301 Moved Permanently\r\n";
const std::string moved_temporarily =
  "HTTP/1.0 302 Moved Temporarily\r\n";
const std::string not_modified =
  "HTTP/1.0 304 Not Modified\r\n";
const std::string bad_request =
  "HTTP/1.0 400 Bad Request\r\n";
const std::string unauthorized =
  "HTTP/1.0 401 Unauthorized\r\n";
const std::string forbidden =
  "HTTP/1.0 403 Forbidden\r\n";
const std::string not_found =
  "HTTP/1.0 404 Not Found\r\n";
const std::string internal_server_error =
  "HTTP/1.0 500 Internal Server Error\r\n";
const std::string not_implemented =
  "HTTP/1.0 501 Not Implemented\r\n";
const std::string bad_gateway =
  "HTTP/1.0 502 Bad Gateway\r\n";
const std::string service_unavailable =
  "HTTP/1.0 503 Service Unavailable\r\n";

boost::asio::const_buffer to_buffer(Reply::status_type status)
{
  switch (status)
  {
  case Reply::ok:
    return boost::asio::buffer(ok);
  case Reply::created:
    return boost::asio::buffer(created);
  case Reply::accepted:
    return boost::asio::buffer(accepted);
  case Reply::no_content:
    return boost::asio::buffer(no_content);
  case Reply::multiple_choices:
    return boost::asio::buffer(multiple_choices);
  case Reply::moved_permanently:
    return boost::asio::buffer(moved_permanently);
  case Reply::moved_temporarily:
    return boost::asio::buffer(moved_temporarily);
  case Reply::not_modified:
    return boost::asio::buffer(not_modified);
  case Reply::bad_request:
    return boost::asio::buffer(bad_request);
  case Reply::unauthorized:
    return boost::asio::buffer(unauthorized);
  case Reply::forbidden:
    return boost::asio::buffer(forbidden);
  case Reply::not_found:
    return boost::asio::buffer(not_found);
  case Reply::internal_server_error:
    return boost::asio::buffer(internal_server_error);
  case Reply::not_implemented:
    return boost::asio::buffer(not_implemented);
  case Reply::bad_gateway:
    return boost::asio::buffer(bad_gateway);
  case Reply::service_unavailable:
    return boost::asio::buffer(service_unavailable);
  default:
    return boost::asio::buffer(internal_server_error);
  }
}

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

std::vector<boost::asio::const_buffer> Reply::to_buffers()
{
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(to_buffer(status));
  for (std::size_t i = 0; i < headers.size(); ++i)
  {
    Header& h = headers[i];
    buffers.push_back(boost::asio::buffer(h.name));
    buffers.push_back(boost::asio::buffer(name_value_separator));
    buffers.push_back(boost::asio::buffer(h.value));
    buffers.push_back(boost::asio::buffer(crlf));
  }
  buffers.push_back(boost::asio::buffer(crlf));
  buffers.push_back(boost::asio::buffer(content));
  return buffers;
}

std::string to_string(Reply::status_type status)
{
	return "";
}

Reply Reply::stock_reply(Reply::status_type status)
{
  Reply rep;
  rep.status = status;
  rep.content = to_string(status);
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/html";
  return rep;
}

SAMP_API_END_NS
