#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_DATA_CONFIGURATIONS_LOG_LEVEL_LIMITS_CONFIGURATION_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_DATA_CONFIGURATIONS_LOG_LEVEL_LIMITS_CONFIGURATION_H_

#include "application/data/enums/log-level.h"

namespace Application
{
	struct LogLevelLimitsConfiguration
	{
		/**
		 * Минимальный уровень сообщения. которое будет выводиться в лог.
		 */
		Application::LogLevel MinLogLevel = Application::LogLevel::Trace;

		/**
		 * Максимальный уровень сообщения, который будет выводиться в лог.
		 */
		Application::LogLevel MaxLogLevel = Application::LogLevel::Error;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_DATA_CONFIGURATIONS_LOG_LEVEL_LIMITS_CONFIGURATION_H_
