#pragma once

#include <string>

#include <utils/strtools.h>

enum class LogSeverity
{
	log,
	warning,
	error,
	success
};

class Logger
{
public:
	static Logger& get();

	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;

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
	Logger() = default;
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
