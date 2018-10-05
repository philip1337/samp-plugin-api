#ifndef SRC_API_PLAYER_HPP
#define SRC_API_PLAYER_HPP

#pragma once

#include <Config.hpp>

#include <string>

SAMP_API_BEGIN_NS

class Player {
public:
	Player(std::string& name, uint32_t id);
	std::string& GetLogin();

	bool GetPosition(float& x, float& y, float& z);
	uint32_t GetId();

	bool Disconnect();
	bool Ban();

private:
	uint32_t id_;
	std::string name_;
};

SAMP_API_END_NS
#endif 
