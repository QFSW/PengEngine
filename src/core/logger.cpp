#include "logger.h"

#include <iostream>

#include <utils/io.h>

#include "peng_engine.h"

Logger::Logger()
    : Singleton()
#ifndef NO_LOGGING
    , _worker_thread("Logger")
#endif
{ }

Logger::~Logger()
{
#ifndef NO_LOGGING
	_worker_thread.shutdown();
#endif
}

#ifdef NO_LOGGING
void Logger::log(LogSeverity, const std::string&) {}
#else
void Logger::log(LogSeverity severity, const std::string& message)
{
    _worker_thread.schedule_job(threading::Job([this, severity, message]
	{
		log_internal(severity, message);
	}));
}
#endif

void Logger::log_internal(LogSeverity severity, const std::string& message)
{
	const tm time_info = time_now_info();

	// Open the log file if we haven't already
	// TODO: we might want to limit the number of old log files to keep
	if (!_log_file.is_open())
	{
		// Logger path = logs/YYYY-MM-DD/HH-MM-SS.log
		const std::string log_path = strtools::catf(
			"logs/%04d-%02d-%02d/%02d-%02d-%02d.log",
			1900 + time_info.tm_year, 1 + time_info.tm_mon, time_info.tm_mday,
			time_info.tm_hour, time_info.tm_min, time_info.tm_sec
		);

		io::create_directories_for_file(log_path);

		_log_file.open(log_path);
		if (!_log_file.is_open())
		{
			throw std::runtime_error("Could not open file " + log_path);
		}
	}

	switch (severity)
	{
	    case LogSeverity::log:
	    {
			_log_file << "[LOG]";
			break;
	    }
	    case LogSeverity::warning:
	    {
		    std::cout << "\033[0;93m";
			_log_file << "[WRN]";
		    break;
	    }
	    case LogSeverity::error:
	    {
		    std::cout << "\033[1;31m";
			_log_file << "[ERR]";
		    break;
	    }
	    case LogSeverity::success:
	    {
		    std::cout << "\033[1;32m";
			_log_file << "[SCS]";
		    break;
	    }
		default:
	    {
			_log_file << "[???]";
			break;
	    }
	}

	// [HH:MM:SS][F]
	const std::string time_code = strtools::catf(
		"[%02d:%02d:%02d][%d] ",
		time_info.tm_hour, time_info.tm_min, time_info.tm_sec,
		PengEngine::get().frame_number()
	);

	std::cout << message;
	_log_file << time_code << message << std::endl;

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

	std::cout << "\n";
}

tm Logger::time_now_info() const
{
	const time_t time_now = time(nullptr);
	tm time_info = {};
	localtime_s(&time_info, &time_now);

	return time_info;
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
