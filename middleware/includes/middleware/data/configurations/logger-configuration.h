#ifndef SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_DATA_CONFIGURATIONS_LOGGER_CONFIGURATION_H_
#define SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_DATA_CONFIGURATIONS_LOGGER_CONFIGURATION_H_

#include "middleware/data/enums/log-level.h"

namespace Middleware
{
	struct LoggerConfiguration
	{
		/**
		 * Минимальный уровень сообщения. которое будет выводиться в лог.
		 */
		LogLevel MinLogLevel = LogLevel::Trace;

		/**
		 * Максимальный уровень сообщения, который будет выводиться в лог.
		 */
		LogLevel MaxLogLevel = LogLevel::Error;
	};
}

#endif //SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_DATA_CONFIGURATIONS_LOGGER_CONFIGURATION_H_
