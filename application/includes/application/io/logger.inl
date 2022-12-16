namespace Middleware
{
	template<uint32_t buffer_size>
	const char* Logger<buffer_size>::GetLevelPrefix(LogLevel log_level)
	{
		switch (log_level)
		{
			case LogLevel::Debug:
				return "[DEBUG] ";
			case LogLevel::Trace:
				return "[TRACE] ";
			case LogLevel::Info:
				return "[INFO] ";
			case LogLevel::Warn:
				return "[WARN] ";
			case LogLevel::Error:
				return "[ERROR] ";
			default:
				return "[UNDEF LEVEL] ";
		}
	}

	template<uint32_t buffer_size>
	template<typename... Args>
	void Logger<buffer_size>::Log(LogLevel log_level, const char* log_format_message, Args... args)
	{
		if (log_level >= configuration_.MinLogLevel && log_level <= configuration_.MaxLogLevel)
		{
			sprintf(buffer_, log_format_message, args...);

			output_driver_.SendMessage(GetLevelPrefix(log_level));
			output_driver_.SendMessage(buffer_);
			output_driver_.SendMessage("\n\r");
		}
	}
}