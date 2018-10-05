#ifndef SRC_API_LOGSINK_HPP
#define SRC_API_LOGSINK_HPP

#pragma once

#include <Config.hpp>

#include <spdlog/sinks/sink.h>
#include <spdlog/details/console_globals.h>
#include <spdlog/details/pattern_formatter.h>

SAMP_API_BEGIN_NS

typedef void(*logprintf_t)(char* format, ...);
extern logprintf_t logprintf;

template<typename ConsoleMutex>
class LogSink : public spdlog::sinks::sink {
public:
	LogSink() : mutex_(ConsoleMutex::mutex()) {
		set_pattern("%+");
	}

	void log(const spdlog::details::log_msg &msg) final override
	{
		std::lock_guard<mutex_t> lock(mutex_);
		fmt::memory_buffer formatted;
		formatter_->format(msg, formatted);
		print_range_(formatted, 0, formatted.size());
	};

	void flush() final override
	{
		// windows console always flushed?
	}

	void set_pattern(const std::string &pattern) override final
	{
		std::lock_guard<mutex_t> lock(mutex_);
		formatter_ = std::unique_ptr<spdlog::formatter>(new spdlog::pattern_formatter(pattern, spdlog::pattern_time_type::local, ""));
	}

	void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override final
	{
		std::lock_guard<mutex_t> lock(mutex_);
		formatter_ = std::move(sink_formatter);
	}


private:
	using mutex_t = typename ConsoleMutex::mutex_t;
	mutex_t & mutex_;

	// print a range of formatted message to console
	void print_range_(fmt::memory_buffer &formatted, size_t start, size_t end)
	{
		auto size = static_cast<DWORD>(end - start);

		auto data = formatted.data();
		std::string s(data + start, data + end);
		
		logprintf("%s", s.c_str());
	}
};

using samp_log_sink_mt = LogSink<spdlog::details::console_mutex>;
using samp_log_sink_st = LogSink<spdlog::details::console_nullmutex>;

SAMP_API_END_NS

#endif
