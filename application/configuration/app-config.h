#ifndef SIMPLEMETEOSTATION_APPLICATION_CONFIGURATION_APP_CONFIG_H_
#define SIMPLEMETEOSTATION_APPLICATION_CONFIGURATION_APP_CONFIG_H_

#include <cstdint>

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
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_CONFIGURATION_APP_CONFIG_H_
