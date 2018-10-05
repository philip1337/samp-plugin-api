#include <amx/amx.h>
#include <plugincommon.h>

#include <boost/asio/io_service.hpp>
#include "Endpoint.hpp"

typedef void(*logprintf_t)(char* format, ...);

logprintf_t logprintf;
extern void *pAMXFunctions;
boost::asio::io_service* io_service = nullptr;
sampapi::Endpoint* e = nullptr;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	io_service = new boost::asio::io_service();
	e = new sampapi::Endpoint(io_service, 80);
	e->Listen();

	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	io_service->stop();
	io_service = nullptr;
}

AMX_NATIVE_INFO PluginNatives[] =
{
	{ 0, 0 }
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	io_service->poll_one();
	io_service->reset();
}
