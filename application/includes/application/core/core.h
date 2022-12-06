#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_CORE_CORE_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_CORE_CORE_H_

#include "middleware/enums/return-codes.h"
#include "middleware/interfaces/iclock.h"
#include "middleware/interfaces/isensor.h"
#include "middleware/interfaces/isleep.h"
#include "middleware/interfaces/idisplay.h"

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
		 * @return Функция должна выполняться бесконечно, и любой выход из нее является ошибкой.
		 */
		static void Run(
			Middleware::ISensor* temperature_sensor,
			Middleware::ISensor* pressure_sensor,
			Middleware::ISleep* sleep_manager,
			Middleware::IDisplay* display,
			Middleware::IClock* clock
		);
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_CORE_CORE_H_
