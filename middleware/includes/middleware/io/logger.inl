namespace Middleware
{
	template<uint32_t buffer_size>
	template<typename... Args>
	void Logger<buffer_size>::Log(LogLevel log_level, const char* log_format_message, Args... args)
	{
		if (log_level >= configuration_.MinLogLevel && log_level <= configuration_.MaxLogLevel)
		{
			sprintf(buffer_, log_format_message, args...);

			output_driver_.SendMessage(buffer_);
			output_driver_.SendMessage("\n\r");
		}
	}
}