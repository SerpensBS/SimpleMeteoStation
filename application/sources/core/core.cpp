#include "core/core.h"
#include "sources/controllers/display-controller.h"

namespace Application
{
	void Core::Run(Middleware::ISensor* temperature_sensor,
		Middleware::ISensor* pressure_sensor,
		Middleware::ISleep* sleep_manager,
		Middleware::IDisplay* display)
	{
		if (!sleep_manager || !display)
		{
			return;
		}

		DisplayController display_controller(*display);
		display_controller.ClearScreen();
		display_controller.PrintHeaders();

		auto status = Middleware::ReturnCode::OK;

		// TODO: Пока делаем суперцикл. В дальнейшем от него надо избавиться, сделав какую-никакую
		//  диспетчеризацию задач.
		while (Middleware::ReturnCode::OK == status)
		{
			double temp_data;
			status = temperature_sensor->GetData(&temp_data);

			if (Middleware::ReturnCode::OK != status)
			{
				return;
			}

			double press_data;
			status = pressure_sensor->GetData(&press_data);

			if (Middleware::ReturnCode::OK != status)
			{
				return;
			}

			display_controller.PrintTemperature(temp_data);
			display_controller.PrintPressure(press_data);

			sleep_manager->Sleep(5);
		}
	}
}