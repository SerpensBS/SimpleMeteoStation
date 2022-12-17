#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_DATA_INTERFACES_LOGGER_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_DATA_INTERFACES_LOGGER_H_

#include <cstdio>
#include <utility>
#include "app-config.h"
#include "application/data/enums/log-level.h"

namespace Application
{
	/**
	 * Абстрактный logger.
	 */
	class Logger
	{
	 protected:
		/**
		 * Конфигурация logger'а.
		 */
		LogLevelLimitsConfiguration configuration_;

		/**
		 * Записать сообщение в лог.
		 * @param log_level Уровень сообщения
		 */
		virtual void WriteLog(LogLevel log_level, const char*) = 0;
	 public:
		/**
		 * Отформатировать сообщение и отправить его в лог.
		 * @tparam Args Произвольные аргументы
		 * @param log_level Уровень сообщения
		 * @param formatted_log_message Сообщение лога
		 * @param args Произвольные аргументы
		 */
		template<typename ...Args>
		void Log(LogLevel log_level, const char* formatted_log_message, Args&& ...args)
		{
			// Выходим если настройки уровень лога не входит в диапазон, указанный в конфигурации.
			if (log_level < configuration_.MinLogLevel || log_level > configuration_.MaxLogLevel)
			{
				return;
			}

			char log_text_buffer[ApplicationConfiguration::LogFormatterBufferSize];

			int32_t formatter_result = snprintf(
				log_text_buffer,
				ApplicationConfiguration::LogFormatterBufferSize,
				formatted_log_message,
				args...);

			// Проверяем, что formatter смог сформировать сообщение и что оно не вышло за рамки буфера.
			if (0 > formatter_result
			|| ApplicationConfiguration::LogFormatterBufferSize < (static_cast<uint32_t>(formatter_result) + 1))
			{
				// Похоже, что у компиляторов GCC Linux x86 и ARM M3 отличается длина int, и мы получаем warning из-за
				// несоответствия ожидаемого типа аргумента sprintf и того, что передается в параметры функции.
				#pragma clang diagnostic push
				#pragma ide diagnostic ignored "Simplify"
				#pragma ide diagnostic ignored "UnreachableCode"
				static const char* formatted_error_message = sizeof(long int) == sizeof(int32_t)
					? "Logger Buffer Corrupted! Sprintf return value: %ld. Buffer size: %lu\n\r"
					: "Logger Buffer Corrupted! Sprintf return value: %d. Buffer size: %u\n\r";
				#pragma clang diagnostic pop

				sprintf(log_text_buffer,
					formatted_error_message,
					formatter_result,
					ApplicationConfiguration::LogFormatterBufferSize);

				log_level = LogLevel::Error;
			}

			WriteLog(log_level, log_text_buffer);
		}

		explicit Logger(LogLevelLimitsConfiguration configuration)
		: configuration_(configuration)
		{
		}

		virtual ~Logger() = default;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_DATA_INTERFACES_LOGGER_H_
