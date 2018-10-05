#include "PlayerManager.hpp"
#include "SampTypes.hpp"

#include <a_samp.h>
#include <a_players.h>

SAMP_API_BEGIN_NS

PlayerManager* playerManager = nullptr;

Player* PlayerManager::AddPlayer(uint32_t id)
{
	// Register player
	char name[MAX_PLAYER_NAME];
	sampgdk::GetPlayerName(id, name, sizeof(name));

	Player p(std::string(name), id);
	players_.emplace(id, std::move(p));
	return nullptr;
}

void PlayerManager::RemovePlayer(uint32_t id)
{
	auto it = players_.find(id);
	if (it != players_.end())
		players_.erase(it);
}

Player* PlayerManager::GetPlayer(uint32_t id)
{
	auto it = players_.find(id);
	if (it != players_.end())
		return &it->second;
	return nullptr;
}

void PlayerManager::GetPlayerData(std::vector<PlayerData>& data)
{
	for (auto& it : players_) {
		auto& p = it.second;
		
		PlayerData d;
		d.login = p.GetLogin();
		
		// Get position
		p.GetPosition(d.x, d.y, d.z);

		// Name
		char name[MAX_PLAYER_NAME];
		sampgdk::GetPlayerName(p.GetId(), name, sizeof(name));
		d.name = name;
		
		sampgdk::GetPlayerHealth(p.GetId(), &d.health);
		sampgdk::GetPlayerArmour(p.GetId(), &d.armor);

		// Skin
		d.skin = sampgdk::GetPlayerSkin(p.GetId());
		d.score = sampgdk::GetPlayerScore(p.GetId());

		// Address
		char addr[15];
		sampgdk::GetPlayerIp(p.GetId(), addr, sizeof(addr));

		d.ip = addr;
		d.ping = sampgdk::GetPlayerPing(p.GetId());
		d.vehicleID = sampgdk::GetPlayerVehicleID(p.GetId());
		d.isNpc = sampgdk::IsPlayerNPC(p.GetId());

		// Return data
		data.push_back(std::move(d));
	}
}

bool PlayerManager::Kick(std::string& name)
{
	for (auto& it : players_) {
		auto& p = it.second;
		if (p.GetLogin().compare(name))
			continue;

		return p.Disconnect();
	}

	return false;
}

bool PlayerManager::Ban(std::string& name)
{
	for (auto& it : players_) {
		auto& p = it.second;
		if (p.GetLogin().compare(name))
			continue;

		return p.Ban();
	}

	return false;
}

SAMP_API_END_NS
