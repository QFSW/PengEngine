#pragma once

#include <string>

#include <utils/strtools.h>

enum class LogVerbosity
{
	Log,
	Warning,
	Error,
	Success
};

class Logger
{
public:
	static Logger& get();

	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;

	void log(LogVerbosity verbosity, const std::string& message);

	template <typename...Args>
	void logf(LogVerbosity verbosity, const char* format, Args&&...args);

	consteval static bool enabled() { return true; }

private:
	Logger() = default;
};

template <typename...Args>
void Logger::logf(LogVerbosity verbosity, const char* format, Args&&... args)
{
	if constexpr (enabled())
	{
		log(verbosity, strtools::catf(format, std::forward<Args>(args)...));
	}
}
