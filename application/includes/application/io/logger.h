#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_IO_LOGGER_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_IO_LOGGER_H_

#include <cstdint>
#include <cstdio>
#include "middleware/data/configurations/logger-configuration.h"
#include "middleware/interfaces/ioutput.h"

namespace Application
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
		Middleware::IOutput& output_driver_;

		/**
		 * Конфигурация logger'а.
		 */
		Middleware::LoggerConfiguration configuration_;

		/**
		 * Буфер.
		 */
		char buffer_[buffer_size] = {};

		/**
		 * Получить текстовый префикс для LogLevel'а.
		 * @param log_level Уровень лога.
		 * @return Текстовое представление префикса уровня лога.
		 */
		const char* GetLevelPrefix(Middleware::LogLevel log_level);
	 public:
		explicit Logger(Middleware::IOutput& output_driver, Middleware::LoggerConfiguration configuration)
			: output_driver_(output_driver), configuration_(configuration)
		{
		}

		/**
		 * Записать сообщение в лог.
		 * @param log_level Уровень лога
		 * @param log_format_message Текст сообщения с нуль-терминатором
		 */
		template<typename ... Args>
		void Log(Middleware::LogLevel log_level, const char* log_format_message, Args ... args);
	};
}

#include "logger.inl"
#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_IO_LOGGER_H_
