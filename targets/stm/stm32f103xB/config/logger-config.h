#ifndef SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_LOGGER_CONFIG_H_
#define SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_LOGGER_CONFIG_H_

#include "middleware/data/configurations/logger-configuration.h"

namespace STM32F103XB
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

#endif //SIMPLEMETEOSTATION_TARGETS_STM_STM32F103XB_CONFIG_LOGGER_CONFIG_H_
