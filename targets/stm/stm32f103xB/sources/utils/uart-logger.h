#ifndef SIMPLEMETEOSTATION_STM32F103XB_SOURCES_UTILS_UART_LOGGER_H_
#define SIMPLEMETEOSTATION_STM32F103XB_SOURCES_UTILS_UART_LOGGER_H_

#include "io/uart-driver.h"
#include "application/data/abstract/logger.h"
#include "application/data/configurations/log-level-limits-configuration.h"

namespace STM32F103XB
{
	/**
	 * Записывает сообщения в лог.
	 */
	class UARTLogger : public Application::Logger
	{
	 private:
		/**
		 * Драйвер вывода лога.
		 */
		UARTDriver& output_driver_;

		/**
		 * Получить текстовый префикс для LogLevel'а.
		 * @param log_level Уровень лога.
		 * @return Текстовое представление префикса уровня лога.
		 */
		static const char* GetLevelPrefix(Application::LogLevel log_level);

		/**
		 * Записать сообщение в лог.
		 * @param log_level Уровень лога
		 * @param log_format_message Текст сообщения с нуль-терминатором
		 */
		void WriteLog(Application::LogLevel log_level, const char* log_format_message) override;
	 public:
		explicit UARTLogger(UARTDriver& output_driver, Application::LogLevelLimitsConfiguration configuration)
			: Application::Logger(configuration), output_driver_(output_driver)
		{
		}
	};
}

#endif //SIMPLEMETEOSTATION_STM32F103XB_SOURCES_UTILS_UART_LOGGER_H_
