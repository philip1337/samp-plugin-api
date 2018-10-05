#ifndef SRC_API_SESSIONMANAGER_HPP
#define SRC_API_SESSIONMANAGER_HPP

#pragma once

#include <Config.hpp>

#include <set>
#include "Session.hpp"

SAMP_API_BEGIN_NS

class SessionManager
{
public:
  SessionManager(const SessionManager&) = delete;
  SessionManager& operator=(const SessionManager&) = delete;

  SessionManager();

  void Start(SessionPtr c);
  void Stop(SessionPtr c);
  void StopAll();

private:
  std::set<SessionPtr> sessions_;
};

SAMP_API_END_NS
#endif 
