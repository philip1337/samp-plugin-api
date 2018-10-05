#include "Player.hpp"

#include <a_players.h>
#include <a_samp.h>

SAMP_API_BEGIN_NS

Player::Player(std::string & name, uint32_t id)
	:id_(id)
	,name_(name)
{
}

uint32_t Player::GetId()
{
	return id_;
}

std::string& Player::GetLogin()
{
	return name_;
}

bool Player::GetPosition(float & x, float & y, float & z)
{
	return sampgdk::GetPlayerPos(GetId(), &x, &y, &z);
}

bool Player::Disconnect()
{
	return sampgdk::Kick(GetId());
}

bool Player::Ban()
{
	return sampgdk::Ban(GetId());
}


SAMP_API_END_NS
