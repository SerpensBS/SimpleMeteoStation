#include "core/core.h"
#include "sources/controllers/display-controller.h"
#include "sources/tasks/task.h"
#include "tasks-manager.h"
#include "sources/tasks/get-and-print-measures-task.h"
#include "sources/tasks/init-screen-task.h"

namespace Application
{
	void Core::Run(Middleware::ISensor* temperature_sensor,
		Middleware::ISensor* pressure_sensor,
		Middleware::ISleep* sleep_manager,
		Middleware::IDisplay* display)
	{
		// Отказываемся работать без устройства вывода и драйвера управления сном.
		if (!sleep_manager || !display)
		{
			return;
		}

		DisplayController display_controller(*display);

		// Инициализируем задачи.
		TasksManager taskManager;

		DisplayInputParameter initScreenTaskParam = { display_controller };
		Task<DisplayInputParameter&> initScreenTask(InitScreenTask, initScreenTaskParam);

		GetDataInputParameter getAndPrintMeasuresParam = { temperature_sensor, pressure_sensor, display_controller };
		Task<GetDataInputParameter&> getAndPrintMeasuresTask(GetAndPrintMeasuresTask, getAndPrintMeasuresParam);

		// Стартовая инициализация дисплея.
		taskManager.AddTask(initScreenTask);
		taskManager.RunTasks();
		taskManager.RemoveTask(initScreenTask);

		// Добавляем задачу опроса сенсоров и вывода данных на дисплей.
		taskManager.AddTask(getAndPrintMeasuresTask);

		// Запускаем основной цикл приложения.
		auto status = Middleware::ReturnCode::OK;
		while (Middleware::ReturnCode::OK == status)
		{
			status = taskManager.RunTasks();
			sleep_manager->Sleep(5);
		}
	}
}