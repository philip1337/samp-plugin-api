#ifndef SRC_API_SAMPTYPES_HPP
#define SRC_API_SAMPTYPES_HPP

#pragma once

#include <Config.hpp>

#include <iostream>
#include <map>
#include <string>

#include <spotify/json.hpp>
#include <spotify/json/codec.hpp>

#include "Types.hpp"

SAMP_API_BEGIN_NS

struct PlayerData {
	std::string login;
	std::string name;

	float x;
	float y;
	float z;
	float health;
	float armor;

	uint16_t skin;
	uint32_t score;
	
	std::string ip;
	uint16_t ping;

	uint32_t vehicleID;
	bool isNpc;
};

struct PlayerMessage : public Message {
	std::vector<PlayerData> players;
	uint32_t count;
};

struct SayRequest {
	int color;
	std::string name;
	std::string message;
};

struct SimpleRequest {
	std::string value;
};

struct SimpleIntRequest {
	uint32_t value;
};

struct GravityRequest {
	float gravity;
};

struct IpRequest {
	std::string address;
	uint32_t time;
};

struct SettingsMessage : public Message {
	uint32_t tickcount;
	uint32_t tickrate;
	float gravity;
	uint32_t apoolsize;
	uint32_t vpoolsize;
	uint32_t maxplayers;
};

SAMP_API_END_NS

namespace spotify {
	namespace json {
		using namespace sampapi;

		template <>
		struct default_codec_t<SettingsMessage> {
			static codec::object_t<SettingsMessage> codec() {
				auto s = codec::object<SettingsMessage>();
				s.required("type", &SettingsMessage::type);
				s.required("code", &SettingsMessage::code);
				s.optional("message", &SettingsMessage::message);
				s.optional("time", &SettingsMessage::time);

				s.required("tickcount", &SettingsMessage::tickcount);
				s.required("tickrate", &SettingsMessage::tickrate);
				s.required("gravity", &SettingsMessage::gravity);
				s.required("apoolsize", &SettingsMessage::apoolsize);
				s.required("vpoolsize", &SettingsMessage::vpoolsize);
				s.required("maxplayers", &SettingsMessage::maxplayers);
				return s;
			}
		};

		template <>
		struct default_codec_t<PlayerData> {
			static codec::object_t<PlayerData> codec() {
				auto p = codec::object<PlayerData>();
				p.required("name", &PlayerData::name);
		
				p.required("x", &PlayerData::x);
				p.required("y", &PlayerData::y);
				p.required("z", &PlayerData::z);
				
				p.required("health", &PlayerData::health);
				p.required("armor", &PlayerData::armor);
				p.required("skin", &PlayerData::skin);
				p.required("score", &PlayerData::score);
				p.required("ip", &PlayerData::ip);
				p.required("ping", &PlayerData::ping);

				p.required("vehicleID", &PlayerData::vehicleID);
				p.required("isNpc", &PlayerData::isNpc);
				return p;
			}
		};
		
		template <>
		struct default_codec_t<PlayerMessage> {
			static codec::object_t<PlayerMessage> codec() {
				auto msg = codec::object<PlayerMessage>();
				msg.required("type", &PlayerMessage::type);
				msg.required("code", &PlayerMessage::code);
				msg.required("count", &PlayerMessage::count);
				msg.optional("message", &PlayerMessage::message);
				msg.optional("time", &PlayerMessage::time);
				msg.optional("players", &PlayerMessage::players);
				return msg;
			}
		};

		template <>
		struct default_codec_t<SayRequest> {
			static codec::object_t<SayRequest> codec() {
				auto msg = codec::object<SayRequest>();
				msg.required("color", &SayRequest::color);
				msg.required("name", &SayRequest::name);
				msg.required("message", &SayRequest::message);
				return msg;
			}
		};

		template <>
		struct default_codec_t<SimpleRequest> {
			static codec::object_t<SimpleRequest> codec() {
				auto s = codec::object<SimpleRequest>();
				s.required("value", &SimpleRequest::value);
				return s;
			}
		};


		template <>
		struct default_codec_t<SimpleIntRequest> {
			static codec::object_t<SimpleIntRequest> codec() {
				auto s = codec::object<SimpleIntRequest>();
				s.required("value", &SimpleIntRequest::value);
				return s;
			}
		};

		template <>
		struct default_codec_t<GravityRequest> {
			static codec::object_t<GravityRequest> codec() {
				auto s = codec::object<GravityRequest>();
				s.required("gravity", &GravityRequest::gravity);
				return s;
			}
		};

		template <>
		struct default_codec_t<IpRequest> {
			static codec::object_t<IpRequest> codec() {
				auto s = codec::object<IpRequest>();
				s.required("address", &IpRequest::address);
				s.optional("time", &IpRequest::time);
				return s;
			}
		};
	}  // namespace json
}  // namespace spotify

#endif
