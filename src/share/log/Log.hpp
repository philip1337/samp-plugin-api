#ifndef SRC_SHARE_LOG_LOG_HPP
#define SRC_SHARE_LOG_LOG_HPP

#include <Config.hpp>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>
#include <vector>

SAMP_API_BEGIN_NS

class Log {
public:
	/** Log */
	Log();

	/** Default logger */
	void SetupDefaultLogger(const std::string &name);

	/** Close default logger */
	void DropLoggers();

	/** Register log sink */
	bool RegisterLogSink(const std::string & name);

	/** Register log sink */
	bool RegisterLogSink(spdlog::sink_ptr ptr);

	/** Logger */
	spdlog::logger* GetLogger();

private:
	/** Default logger console */
	std::shared_ptr<spdlog::logger> m_Console;

	/** Log sinks */
	std::vector<spdlog::sink_ptr> m_Sinks;

	/** Path */
	bool CheckPath(const std::string& path);

	/** Sink handler */
	spdlog::logger* m_Logger;
};

extern Log* Logger;

SAMP_API_END_NS

#define SAMP_LOG_INIT()  SAMP_API_NS::Logger = new SAMP_API_NS::Log()
#define SAMP_LOG_SETUP(name)  SAMP_API_NS::Logger->SetupDefaultLogger(name)
#define SAMP_LOG_SHUTDOWN()  SAMP_API_NS::Logger->DropLoggers(); delete SAMP_API_NS::Logger

#ifdef _DEBUG
	#define SAMP_DEBUG(...) SAMP_API_NS::Logger->GetLogger()->info(__VA_ARGS__)	
#else
	#define SAMP_DEBUG(...)
#endif

#define SAMP_INFO(...) SAMP_API_NS::Logger->GetLogger()->info(__VA_ARGS__)
#define SAMP_WARN(...) SAMP_API_NS::Logger->GetLogger()->warn(__VA_ARGS__)
#define SAMP_ERROR(...) SAMP_API_NS::Logger->GetLogger()->error(__VA_ARGS__)

#endif
