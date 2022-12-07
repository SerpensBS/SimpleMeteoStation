#include <cstdio>
#include "display-controller.h"

namespace Application
{
	void DisplayController::ClearScreen()
	{
		display_driver_->ClearScreen();
	}

	void DisplayController::PrintHeaders()
	{
		constexpr char temp_message[] = "TEMP:";
		display_driver_->PrintMessage(temp_message, sizeof(temp_message) / sizeof(char), 0, 0);

		constexpr char press_message[] = "PRES:";
		display_driver_->PrintMessage(press_message, sizeof(press_message) / sizeof(char), 1, 0);
	}

	void DisplayController::PrintTemperature(double temp_value_in_C)
	{
		// Выделяем массив ровно такой длины, чтобы хватило символов для нашего формата вывода + /0.
		char temp_value_in_string[sizeof("-xx.x")] = {};
		sprintf(temp_value_in_string, "%0.1f", temp_value_in_C);

		// Рассчитываем длину сообщения.
		uint32_t temp_numbers_count = temp_value_in_C > 10.0 || temp_value_in_C < 0.0
			? 4
			: 3;
		display_driver_->PrintMessage(temp_value_in_string, temp_numbers_count, 0, 6);
	}

	void DisplayController::PrintPressure(double pressure_value_in_mmHg)
	{
		// Выделяем массив ровно такой длины, чтобы хватило символов для нашего формата вывода + /0.
		char press_value_in_string[sizeof("xxx.x")] = {};
		sprintf(press_value_in_string, "%0.1f", pressure_value_in_mmHg);

		display_driver_->PrintMessage(press_value_in_string, 6, 1, 6);
	}
}


