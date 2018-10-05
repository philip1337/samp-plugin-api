#include <Config.hpp>
#include "Log.hpp"

#include <boost/filesystem.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

SAMP_API_BEGIN_NS

Log* Logger = nullptr;

bool Log::CheckPath(const std::string& path)
{
	// Initialize boost path
	boost::filesystem::path p(path);

	// If file already exists do nothing
	if (boost::filesystem::is_regular_file(p))
		return true;

	// Parent path
	auto parent = p.parent_path();

	// Empty
	if (parent.empty())
		return true;

	// If directory already exists do nothing
	if (boost::filesystem::is_directory(parent))
		return true;

	// If path is a file
	if (boost::filesystem::is_regular_file(parent)) {
		return true;
	}

	return boost::filesystem::create_directories(parent);
}

Log::Log() {
	// Register console sink (we use samp logsink)
	//m_Sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	//m_Sinks[0]->set_level(spdlog::level::info);
}

void Log::SetupDefaultLogger(const std::string &name) {
	m_Logger = new spdlog::logger(name, m_Sinks.begin(), m_Sinks.end());
	m_Logger->flush_on(spdlog::level::err);
}

void Log::DropLoggers() {
	spdlog::drop_all();	// Flush and drop
}

bool Log::RegisterLogSink(spdlog::sink_ptr ptr) {
	m_Sinks.push_back(ptr);
	return true;
}

bool Log::RegisterLogSink(const std::string & name) {
	try {
		// If invalid path
		if (!CheckPath(name))
			return false;

		// Register sink
		m_Sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(name, 0, 0, false));
	} catch (const spdlog::spdlog_ex& ex) {
		return false;
	}
	return true;
}

spdlog::logger * Log::GetLogger() {
	return m_Logger;
}

SAMP_API_END_NS
