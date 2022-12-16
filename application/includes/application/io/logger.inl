namespace Application
{
	template<uint32_t buffer_size>
	const char* Logger<buffer_size>::GetLevelPrefix(Middleware::LogLevel log_level)
	{
		switch (log_level)
		{
			case Middleware::LogLevel::Debug:
				return "[DEBUG] ";
			case Middleware::LogLevel::Trace:
				return "[TRACE] ";
			case Middleware::LogLevel::Info:
				return "[INFO] ";
			case Middleware::LogLevel::Warn:
				return "[WARN] ";
			case Middleware::LogLevel::Error:
				return "[ERROR] ";
			default:
				return "[UNDEF LEVEL] ";
		}
	}

	template<uint32_t buffer_size>
	template<typename... Args>
	void Logger<buffer_size>::Log(Middleware::LogLevel log_level, const char* log_format_message, Args... args)
	{
		if (log_level >= configuration_.MinLogLevel && log_level <= configuration_.MaxLogLevel)
		{
			int32_t chars_added = sprintf(buffer_, log_format_message, args...);

			if (0 > chars_added || buffer_size < (static_cast<uint32_t>(chars_added) + 1))
			{
				// Похоже что у компиляторов GCC Linux x86 и ARM M3 отличается длина int, и мы получаем warning из-за
				// несоответствия ожидаемого типа аргумента sprintf и того, что передается в параметры функции.
				#pragma clang diagnostic push
				#pragma ide diagnostic ignored "Simplify"
				#pragma ide diagnostic ignored "UnreachableCode"
				const char* formatted_error_message = sizeof(long int) == sizeof(int32_t)
					? "%sLogger Buffer Corrupted! Sprintf return value: %ld. Buffer size: %lu\n\r"
					: "%sLogger Buffer Corrupted! Sprintf return value: %d. Buffer size: %u\n\r";
				#pragma clang diagnostic pop

				char error_message[80] = {};
				sprintf(error_message, formatted_error_message,
					GetLevelPrefix(Middleware::LogLevel::Error),
					chars_added,
					buffer_size);
				output_driver_.SendMessage(error_message);
				return;
			}

			output_driver_.SendMessage(GetLevelPrefix(log_level));
			output_driver_.SendMessage(buffer_);
			output_driver_.SendMessage("\n\r");
		}
	}
}