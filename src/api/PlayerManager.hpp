#ifndef SRC_API_PLAYERMANAGER_HPP
#define SRC_API_PLAYERMANAGER_HPP

#pragma once

#include <Config.hpp>
#include <unordered_map>

#include "Player.hpp"

SAMP_API_BEGIN_NS

struct PlayerData;

class PlayerManager
{
public:
	Player* AddPlayer(uint32_t id);
	void RemovePlayer(uint32_t id);
	Player* GetPlayer(uint32_t id);

	void GetPlayerData(std::vector<PlayerData>& data);

	bool Kick(std::string& name);
	bool Ban(std::string& name);

private:
	std::unordered_map<uint32_t, Player> players_;
};

extern PlayerManager* playerManager;

SAMP_API_END_NS
#endif 
