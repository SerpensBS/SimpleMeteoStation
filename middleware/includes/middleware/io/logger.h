#ifndef SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_IO_LOGGER_H_
#define SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_IO_LOGGER_H_

#include <cstdint>
#include <cstdio>
#include "middleware/data/configurations/logger-configuration.h"
#include "middleware/interfaces/ioutput.h"

namespace Middleware
{
	/**
	 * Записывает сообщения в лог.
	 */
	template<uint32_t buffer_size>
	class Logger
	{
	 private:
		/**
		 * Драйвер вывода лога.
		 */
		IOutput& output_driver_;

		/**
		 * Конфигурация logger'а.
		 */
		LoggerConfiguration configuration_;

		/**
		 * Буфер.
		 */
		char buffer_[buffer_size] = {};
	 public:
		explicit Logger(IOutput& output_driver, LoggerConfiguration configuration)
			: output_driver_(output_driver), configuration_(configuration)
		{
		}

		/**
		 * Записать сообщение в лог
		 * @param log_level Уровень лога
		 * @param log_format_message Текст сообщения с нуль-терминатором
		 */
		template<typename ... Args>
		inline void Log(LogLevel log_level, const char* log_format_message, Args ... args);
	};
}

#include "logger.inl"
#endif //SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_IO_LOGGER_H_
