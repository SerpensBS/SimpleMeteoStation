#ifndef SIMPLEMETEOSTATION_APPLICATION_CONFIGURATION_LOGGER_CONFIG_H_
#define SIMPLEMETEOSTATION_APPLICATION_CONFIGURATION_LOGGER_CONFIG_H_

#include "middleware/data/configurations/logger-configuration.h"

namespace Application
{
	/**
	 * Конфигурация loggerr'а.
	 */
	struct LoggerConfiguration
	{
		/**
		* Минимальный и максимальный уровень логирования.
		*/
		static constexpr Middleware::LoggerConfiguration LevelConfiguration
		{
			/**
			 * Минимальный уровень логирования.
			 */
			Middleware::LogLevel::Trace,

			/**
			 * Максимальный уровень логирования.
			 */
			Middleware::LogLevel::Error
		};

		/**
		 * Размер буфера logger'а.
		 */
		static constexpr uint32_t LogBufferSize = 128;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_CONFIGURATION_LOGGER_CONFIG_H_
