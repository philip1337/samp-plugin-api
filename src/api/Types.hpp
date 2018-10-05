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
				request.optional("hash", &BodyRequest::hash);
				return request;
			}
		};
	}  // namespace json
}  // namespace spotify

#endif
