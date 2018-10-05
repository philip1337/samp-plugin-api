#ifndef SRC_API_REQUEST_HPP
#define SRC_API_REQUEST_HPP

#pragma once

#include <Config.hpp>

#include <string>
#include <vector>

#include "Header.hpp"

SAMP_API_BEGIN_NS

/// A request received from a client.
struct Request
{
	std::string method;
	std::string uri;
	int http_version_major;
	int http_version_minor;
	std::vector<Header> headers;
	std::vector<char> body;

	uint32_t GetContentLenght() const {
		for (auto& header : headers) {
			if (header.name == "Content-Length") {
				return stoi(header.value);
			}
		}
		return 0;
	}
};

SAMP_API_END_NS
#endif
