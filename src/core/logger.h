#pragma once

#include <string>
#include <fstream>

#ifndef NO_LOGGING
#include <threading/worker_thread.h>
#endif

#include <utils/strtools.h>
#include <utils/singleton.h>

enum class LogSeverity
{
	log,
	warning,
	error,
	success
};

// TODO: automatically enable virtual terminal to get colors on standard windows terminal
// TODO: automatically dump logs to a file as well as the terminal
// TODO: timestamp logs
class Logger : public utils::Singleton<Logger>
{
	friend Singleton;

public:
	void log(LogSeverity severity, const std::string& message);

	template <typename...Args>
	void logf(LogSeverity severity, const char* format, Args&&...args);

	static void log(const std::string& message);
	static void warning(const std::string& message);
	static void error(const std::string& message);
	static void success(const std::string& message);

	template <typename...Args>
	static void log(const char* format, Args&&...args);

	template <typename...Args>
	static void warning(const char* format, Args&&...args);

	template <typename...Args>
	static void error(const char* format, Args&&...args);

	template <typename...Args>
	static void success(const char* format, Args&&...args);

	consteval static bool enabled();

private:
	struct Log
	{
		LogSeverity severity;
		std::string message;
		tm timestamp;
		int32_t frame_number;
	};

	Logger();
	~Logger();

    void log_internal(const Log& log);
	[[nodiscard]] tm time_now_info() const;

#ifndef NO_LOGGING
	std::ofstream _log_file;
	threading::WorkerThread _worker_thread;
#endif
};

consteval bool Logger::enabled()
{
#ifdef NO_LOGGING
	return false;
#else
	return true;
#endif
}

template <typename...Args>
void Logger::logf(LogSeverity severity, const char* format, Args&&... args)
{
	if constexpr (enabled())
	{
		if constexpr (sizeof...(args) == 0)
		{
			log(severity, std::string(format));
		}
		else
		{
			log(severity, strtools::catf(format, std::forward<Args>(args)...));
		}
	}
	else
	{
		// Suppress unused warnings on parameters
		static_cast<void>(severity);
		static_cast<void>(format);
		static_cast<void>((args,...));
	}
}

template <typename ... Args>
void Logger::log(const char* format, Args&&... args)
{
	get().logf(LogSeverity::log, format, std::forward<Args>(args)...);
}

template <typename ... Args>
void Logger::warning(const char* format, Args&&... args)
{
	get().logf(LogSeverity::warning, format, std::forward<Args>(args)...);
}

template <typename ... Args>
void Logger::error(const char* format, Args&&... args)
{
	get().logf(LogSeverity::error, format, std::forward<Args>(args)...);
}

template <typename ... Args>
void Logger::success(const char* format, Args&&... args)
{
	get().logf(LogSeverity::success, format, std::forward<Args>(args)...);
}
