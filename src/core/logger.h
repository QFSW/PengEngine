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

	consteval static bool enabled() { return true; }

private:
	Logger() = default;
};

template <typename...Args>
void Logger::logf(LogSeverity severity, const char* format, Args&&... args)
{
	if constexpr (enabled())
	{
		log(severity, strtools::catf(format, std::forward<Args>(args)...));
	}
}
