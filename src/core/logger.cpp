#include "logger.h"

#include <iostream>

Logger::Logger()
    : Singleton()
#ifndef NO_LOGGING
    , _worker_thread("Logger")
#endif
{ }

#ifdef NO_LOGGING
void Logger::log(LogSeverity, const std::string&) {}
#else
void Logger::log(LogSeverity severity, const std::string& message)
{
    _worker_thread.schedule_job(threading::Job([severity, message]
	{
		log_internal(severity, message);
	}));
}
#endif

void Logger::log_internal(LogSeverity severity, const std::string& message)
{
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


void Logger::log(const std::string& message)
{
	get().log(LogSeverity::log, message);
}

void Logger::warning(const std::string& message)
{
	get().log(LogSeverity::warning, message);
}

void Logger::error(const std::string& message)
{
	get().log(LogSeverity::error, message);
}

void Logger::success(const std::string& message)
{
	get().log(LogSeverity::success, message);
}
