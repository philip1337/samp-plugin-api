#ifndef SRC_API_TYPES_HPP
#define SRC_API_TYPES_HPP

#pragma once

#include <Config.hpp>

#include <iostream>
#include <map>
#include <string>

#include <spotify/json.hpp>
#include <spotify/json/codec.hpp>

SAMP_API_BEGIN_NS

struct BodyRequest {
	std::string password;
	std::string hash;
};

struct ErrorMessage {
	uint32_t code;
	std::string message;
};


SAMP_API_END_NS

namespace spotify {
	namespace json {
		using namespace sampapi;

		// Specialize spotify::json::default_codec_t to specify default behavior when
		// encoding and decoding objects of certain types.
		template <>
		struct default_codec_t<BodyRequest> {
			static codec::object_t<BodyRequest> codec() {
				auto request = codec::object<BodyRequest>();
				request.required("password", &BodyRequest::password);
				request.required("hash", &BodyRequest::hash);
				return request;
			}
		};

		template <>
		struct default_codec_t<ErrorMessage> {
			static codec::object_t<ErrorMessage> codec() {
				auto msg = codec::object<ErrorMessage>();
				msg.required("code", &ErrorMessage::code);
				msg.optional("message", &ErrorMessage::message);
				return msg;
			}
		};
	}  // namespace json
}  // namespace spotify

#endif
