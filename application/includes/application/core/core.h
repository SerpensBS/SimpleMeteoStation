#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_CORE_CORE_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_CORE_CORE_H_

#include "application/data/abstract/logger.h"
#include "middleware/data/enums/return-codes.h"
#include "application/interfaces/iclock.h"
#include "middleware/interfaces/idisplay.h"
#include "middleware/interfaces/isensor.h"
#include "application/interfaces/isleep.h"

namespace Application
{
	/**
	 * Приложение. Здесь выполняется основная бизнес-логика программы.
	 */
	class Core
	{
	 public:
		/**
		 * Запускает приложение.
		 * @param temperature_sensor Драйвер датчика температуры
		 * @param pressure_sensor Драйвер датчика атмосферного давления
		 * @param sleep_manager Драйвер управления сном
		 * @param display Драйвер дисплея
		 * @param clock Драйвер часов реального времени
		 * @param debug_output Драйвер вывода отладочной информации
		 * @return Статус операции
		 */
		static Middleware::ReturnCode Run(
			Middleware::ISensor* temperature_sensor,
			Middleware::ISensor* pressure_sensor,
			Application::ISleep* sleep_manager,
			Middleware::IDisplay* display,
			Application::IClock* clock,
			Logger* logger
		);
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_CORE_CORE_H_
