#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_CONTROLLERS_DISPLAY_CONTROLLER_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_CONTROLLERS_DISPLAY_CONTROLLER_H_

#include "middleware/interfaces/idisplay.h"

namespace Application
{
	/**
	 * Контроллер, управляющий выводом на дисплей данных.
	 */
	class DisplayController
	{
	 private:
		/**
		 * Экземпляр драйвера.
		 */
		Middleware::IDisplay* display_driver_ = nullptr;
	 public:
		/**
		 * Очистить дисплей.
		 */
		void ClearScreen();

		/**
		 * Вывести заголовки на дисплей.
		 */
		void PrintHeaders();

		/**
		 * Вывести температуру в С на дисплей.
		 * @param temp_value_in_C Температура в градусах Цельсия
		 */
		void PrintTemperature(double temp_value_in_C);

		/**
		 * Вывести давление в мм.рт.ст на дисплей.
		 * @param pressure_value_in_mmHg Давление в мм.рт.ст.
		 */
		void PrintPressure(double pressure_value_in_mmHg);

		explicit DisplayController(Middleware::IDisplay& display_driver)
			: display_driver_(&display_driver)
		{
		}
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_CONTROLLERS_DISPLAY_CONTROLLER_H_
