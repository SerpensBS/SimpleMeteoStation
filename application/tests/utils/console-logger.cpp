#include <iostream>
#include "console-logger.h"

namespace ApplicationTests
{
	const char* ConsoleLogger::GetLogLevelPrefix(Application::LogLevel log_level)
	{
		switch (log_level)
		{
			case Application::LogLevel::Debug:
				return "[DEBUG] ";
			case Application::LogLevel::Trace:
				return "[TRACE] ";
			case Application::LogLevel::Info:
				return "[INFO] ";
			case Application::LogLevel::Warn:
				return "[WARN] ";
			case Application::LogLevel::Error:
				return "[ERROR] ";
			default:
				return "[UNDEF LEVEL] ";
		}
	}

	void ConsoleLogger::WriteLog(Application::LogLevel log_level, const char* log_format_message)
	{
		std::cout << GetLogLevelPrefix(log_level) << log_format_message << std::endl;
	}
}

