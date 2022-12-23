#include "uart-logger.h"

namespace STM32F103XB
{
	const char* UARTLogger::GetLevelPrefix(Application::LogLevel log_level)
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

	void UARTLogger::WriteLog(Application::LogLevel log_level, const char* log_format_message)
	{
		output_driver_.SendMessage(GetLevelPrefix(log_level));
		output_driver_.SendMessage(log_format_message);
		output_driver_.SendMessage("\n\r");
	}
}