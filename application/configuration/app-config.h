#ifndef SIMPLEMETEOSTATION_APPLICATION_CONFIGURATION_APP_CONFIG_H_
#define SIMPLEMETEOSTATION_APPLICATION_CONFIGURATION_APP_CONFIG_H_

#include <cstdint>
#include "application/data/configurations/log-level-limits-configuration.h"

namespace Application
{
	/**
	 * Конфигурация приложения.
	 */
	struct ApplicationConfiguration
	{
		/**
		 * Максимальное количество задач в очереди планировщика задач.
		 */
		static constexpr uint32_t TaskQueueMaxSize = 5;

		/**
		 * Минимальный и максимальный уровень логирования.
 		*/
		static constexpr LogLevelLimitsConfiguration LoggerLevelConfiguration
		{
			/**
			 * Минимальный уровень логирования.
			 */
			Application::LogLevel::Trace,

			/**
			 * Максимальный уровень логирования.
			 */
			Application::LogLevel::Error
		};

		/**
		 * Размер буфера для функции-formatter'а логов. В него записывается отформатированное сообщение, которое
		 * передается дальше в реализацию класса logger'а.
		 * Примерная длина сообщения об ошибке - 80 символов. Мы должны гарантировать, что оно поместится в буфер.
		 */
		static constexpr uint32_t LogFormatterBufferSize = 128;
		static_assert(80 < ApplicationConfiguration::LogFormatterBufferSize, "Недостаточный размер log-буфера");
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_CONFIGURATION_APP_CONFIG_H_
