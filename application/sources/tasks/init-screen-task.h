#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_TASKS_INIT_SCREEN_TASK_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_TASKS_INIT_SCREEN_TASK_H_

#include "middleware/data/enums/return-codes.h"
#include "sources/controllers/display-controller.h"

namespace Application
{
	/**
	 * Контейнер для входящих параметров.
	 */
	struct DisplayInputParameter
	{
		/**
		 * Контроллер дисплея.
		 */
		DisplayController& display_controller;
	};

	/**
	 * Инициализация экрана - очищаем и заполняем основные поля, которые останутся неизменными.
	 * @param param Контейнер для входящих параметров.
	 * @return Статус операции
	 */
	Middleware::ReturnCode InitScreenTask(DisplayInputParameter& param)
	{
		param.display_controller.ClearScreen();
		param.display_controller.PrintHeaders();

		return Middleware::ReturnCode::OK;
	}
}

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_TASKS_INIT_SCREEN_TASK_H_
