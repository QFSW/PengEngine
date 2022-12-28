#include "logger.h"

#include <iostream>

Logger& Logger::get()
{
	static Logger logger;
	return logger;
}

void Logger::log(LogSeverity severity, const std::string& message)
{
	if constexpr (!enabled())
	{
		// Suppress unused warnings on parameters
		static_cast<void>(severity);
		static_cast<void>(message);
		return;
	}

	switch (severity)
	{
		case LogSeverity::log: break;
		case LogSeverity::warning:
		{
			std::cout << "\033[0;93m";
			break;
		}
		case LogSeverity::error:
		{
			std::cout << "\033[1;31m";
			break;
		}
		case LogSeverity::success:
		{
			std::cout << "\033[1;32m";
			break;
		}
	}

	std::cout << message << "\n";

	switch (severity)
	{
		case LogSeverity::log: break;
		case LogSeverity::warning:
		case LogSeverity::error:
		case LogSeverity::success:
		{
			std::cout << "\033[0m";
			break;
		}
	}
}
