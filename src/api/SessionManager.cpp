#include "SessionManager.hpp"

SAMP_API_BEGIN_NS


SessionManager::SessionManager()
{
}

void SessionManager::Start(SessionPtr c)
{
	sessions_.insert(c);
	c->Start();
}

void SessionManager::Stop(SessionPtr c)
{
	sessions_.erase(c);
	c->Stop();
}

void SessionManager::StopAll()
{
	for (auto c : sessions_)
		c->Stop();
	sessions_.clear();
}

SAMP_API_END_NS
