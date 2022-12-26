#include "logger.h"

#include <iostream>

Logger& Logger::get()
{
	static Logger logger;
	return logger;
}

void Logger::log(LogVerbosity verbosity, const std::string& message)
{
	std::cout << message << "\n";
}
