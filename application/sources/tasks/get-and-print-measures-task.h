#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_TASKS_GET_AND_PRINT_MEASURES_TASK_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_TASKS_GET_AND_PRINT_MEASURES_TASK_H_

#include "middleware/enums/return-codes.h"
#include "middleware/interfaces/isensor.h"
#include "sources/controllers/display-controller.h"

namespace Application
{
	/**
	 * Контейнер для входящих параметров.
	 */
	struct GetDataInputParameter
	{
		/**
		 * Драйвер датчика температуры.
		 */
		Middleware::ISensor* temperature_sensor;

		/**
		 * Драйвер датчика атмосферного давления.
		 */
		Middleware::ISensor* pressure_sensor;

		/**
		 * Контроллер дисплея.
		 */
		DisplayController& display_controller;
	};

	/**
	 * Получить данные с сенсоров и вывести их на дисплей.
	 * @param param Контейнер для входящих параметров.
	 * @return Статус операции
	 */
	Middleware::ReturnCode GetAndPrintMeasuresTask(GetDataInputParameter& param)
	{
		auto status = Middleware::ReturnCode::OK;

		if (param.temperature_sensor)
		{
			double temp_data;
			status = param.temperature_sensor->GetData(&temp_data);

			if (Middleware::ReturnCode::OK != status)
			{
				return status;
			}

			param.display_controller.PrintTemperature(temp_data);
		}

		if(param.pressure_sensor)
		{
			double press_data;
			status = param.pressure_sensor->GetData(&press_data);

			if (Middleware::ReturnCode::OK != status)
			{
				return status;
			}

			param.display_controller.PrintPressure(press_data);
		}

		return status;
	}
}

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_TASKS_GET_AND_PRINT_MEASURES_TASK_H_
