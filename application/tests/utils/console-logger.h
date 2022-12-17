#ifndef SIMPLEMETEOSTATION_APPLICATION_TESTS_UTILS_CONSOLE_LOGGER_H_
#define SIMPLEMETEOSTATION_APPLICATION_TESTS_UTILS_CONSOLE_LOGGER_H_

#include "application/data/abstract/logger.h"

namespace ApplicationTests
{
	class ConsoleLogger : public Application::Logger
	{
	 private:
		/**
		 * Получить текстовый префикс для LogLevel'а.
		 * @param log_level Уровень лога.
		 * @return Текстовое представление префикса уровня лога.
		 */
		static const char* GetLogLevelPrefix(Application::LogLevel log_level);
	 public:
		/**
		 * Записать сообщение в лог.
		 * @param log_level Уровень лога
		 * @param log_format_message Текст сообщения с нуль-терминатором
		 */
		void WriteLog(Application::LogLevel log_level, const char* log_format_message) override;

		explicit ConsoleLogger(Application::LogLevelLimitsConfiguration configuration)
		: Application::Logger(configuration)
		{
		}
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_TESTS_UTILS_CONSOLE_LOGGER_H_
