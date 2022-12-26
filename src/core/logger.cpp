#include "logger.h"

#include <iostream>

Logger& Logger::get()
{
	static Logger logger;
	return logger;
}

void Logger::log(LogVerbosity verbosity, const std::string& message)
{
	if constexpr (!enabled())
	{
		return;
	}

	switch (verbosity)
	{
		case LogVerbosity::Log: break;
		case LogVerbosity::Warning:
		{
			std::cout << "\033[0;93m";
			break;
		}
		case LogVerbosity::Error:
		{
			std::cout << "\033[1;31m";
			break;
		}
		case LogVerbosity::Success:
		{
			std::cout << "\033[1;32m";
			break;
		}
	}

	std::cout << message << "\n";

	switch (verbosity)
	{
		case LogVerbosity::Log: break;
		case LogVerbosity::Warning:
		case LogVerbosity::Error:
		case LogVerbosity::Success:
		{
			std::cout << "\033[0m";
			break;
		}
	}
}
