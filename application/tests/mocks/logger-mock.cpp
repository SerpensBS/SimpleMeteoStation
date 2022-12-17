#include "logger-mock.h"

namespace ApplicationTests
{
	void LoggerMock::WriteLog(Application::LogLevel log_level, const char* log_format_message)
	{
		buffered_message_.LogMessage = std::string(log_format_message);
		buffered_message_.LogLevel = log_level;
	}

	ApplicationTests::Log LoggerMock::GetBufferedMessageData()
	{
		return buffered_message_;
	}
}

