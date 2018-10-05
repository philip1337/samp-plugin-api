#include <sampgdk/core.h>
#include <sampgdk/sdk.h>
#include <a_players.h>
#include <a_samp.h>

#include <boost/asio/io_service.hpp>

#include "Config.hpp"
#include "Endpoint.hpp"

#include <log/Log.hpp>
#include "PlayerManager.hpp"
#include "LogSink.hpp"

using namespace sampapi;

extern void *pAMXFunctions;

boost::asio::io_service* io_service = nullptr;
Endpoint* e = nullptr;
bool ready = false;

bool OpenLog() {
	// Init log
	SAMP_LOG_INIT();

	if (!Logger->RegisterLogSink(std::make_shared<samp_log_sink_mt>()))
		return false;

	// Client output log
	if (!Logger->RegisterLogSink("logs/api.log"))
		return false;

	// Setup
	SAMP_LOG_SETUP("SA-MP Api");
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit() {
	SAMP_DEBUG("[SA-MP API] Initializing gamemode");
	ready = true;
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeExit() {
	SAMP_DEBUG("[SA-MP API] Gamemode shutdown");
	ready = false;
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid) {
	playerManager->AddPlayer(playerid);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason) {
	playerManager->RemovePlayer(playerid);
	return true;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	auto ret = sampgdk::Load(ppData);

	// SA-MP Api functions
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	playerManager = new PlayerManager();

	// Failed to initialize log
	if (!OpenLog()) {
		// If no log we have to use the samp default output
		logprintf("[SA-MP API] Failed to initialize log.");
		return false;
	}

	// Config
	sampConfig = new Config();
	if (!sampConfig->Load("server.cfg")) {
		SAMP_INFO("[SA-MP API] Failed to read server.cfg.");
		return false;
	}

	// IO Service
	io_service = new boost::asio::io_service();

	// Get api_port
	std::string port;
	sampConfig->Get("api_port", port, "7778");

	// Get api_hash
	std::string seed;
	sampConfig->Get("api_seed", seed, "17733194");

	// Get api password
	std::string password;
	if (!sampConfig->Get("api_password", password)) {
		SAMP_INFO("[SA-MP API] Required server config not found: api_password");
		return false;
	}

	// Start endpoint
	e = new sampapi::Endpoint(io_service, stoi(port));
	auto& handler = e->GetHandler();
	handler.SetPassword(password);
	handler.SetSeed(stoul(seed));

	// Start listening
	SAMP_INFO("[SA-MP API] Starting listening on port {}.", port);

	e->Listen();
	return ret;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	io_service->stop();
	io_service = nullptr;
	sampConfig = nullptr;
	playerManager = nullptr;
	SAMP_LOG_SHUTDOWN();
	sampgdk::Unload();
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{ 0, 0 }
};

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	if (!ready)
		return;

	io_service->poll_one();
	io_service->reset();
}
