#include "configuration/app-config.h"
#include "configuration/logger-config.h"
#include "core/core.h"
#include "middleware/io/logger.h"
#include "sources/controllers/display-controller.h"
#include "sources/tasks/task.h"
#include "sources/tasks/get-and-print-measures-task.h"
#include "sources/tasks/init-screen-task.h"
#include "tasks-manager.h"

namespace Application
{
	Middleware::ReturnCode Core::Run(Middleware::ISensor* temperature_sensor,
		Middleware::ISensor* pressure_sensor,
		Middleware::ISleep* sleep_manager,
		Middleware::IDisplay* display,
		Middleware::IClock* clock,
		Middleware::IOutput* debug_output)
	{
		// Настраиваем logger.
		Middleware::Logger<Application::LoggerConfiguration::LogBufferSize> logger(
			*debug_output,
			Application::LoggerConfiguration::LevelConfiguration);

		// Отказываемся работать без устройства вывода и драйвера управления сном.
		if (!sleep_manager || !display)
		{
			logger.Log(Middleware::LogLevel::Error, "%s", "Undefined SleepManager Driver or Display Driver!");
			return Middleware::ReturnCode::ERROR;
		}

		DisplayController display_controller(*display);

		// Инициализируем задачи.
		TasksManager<ApplicationConfiguration::TaskQueueMaxSize> taskManager(*clock);

		DisplayInputParameter initScreenTaskParam = { display_controller };
		Task<DisplayInputParameter&> initScreenTask(InitScreenTask, initScreenTaskParam);

		GetDataInputParameter getAndPrintMeasuresParam = { temperature_sensor, pressure_sensor, display_controller };
		Task<GetDataInputParameter&> getAndPrintMeasuresTask(GetAndPrintMeasuresTask, getAndPrintMeasuresParam);

		// Стартовая инициализация дисплея.
		taskManager.AddTask(initScreenTask);
		// Добавляем задачу опроса сенсоров и вывода данных на дисплей.
		taskManager.AddTask(getAndPrintMeasuresTask, 0, 5);

		// Запускаем основной цикл приложения.
		while (true)
		{
			// Запускаем процесс выполнения задач.
			auto status = taskManager.RunTasks();

			// Если последний запуск был с ошибкой - выходим с ошибкой.
			if (Middleware::ReturnCode::OK != status)
			{
				return status;
			}

			// Если в пуле больше нет задач - выходим.
			if (0 == taskManager.GetTasksQueueSize())
			{
				break;
			}

			// Если следующая задача не стоит на запуск вот прям сейчас - спим до момента ее запланированного запуска.
			if (taskManager.GetTimeToCallTheNearestTask() > clock->GetCurrentTime())
			{
				sleep_manager->Sleep(taskManager.GetTimeToCallTheNearestTask() - clock->GetCurrentTime());
			}
		}

		return Middleware::ReturnCode::OK;
	}
}