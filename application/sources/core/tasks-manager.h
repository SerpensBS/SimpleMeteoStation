#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_CORE_TASKS_MANAGER_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_CORE_TASKS_MANAGER_H_

#include <array>
#include "configuration/app-config.h"
#include "middleware/interfaces/iclock.h"
#include "sources/data/planned-task.h"
#include "sources/interfaces/itask.h"

namespace Application
{
	/**
	 * Менеджер по работе с задачами.
	 */
	class TasksManager
	{
	 private:
		/**
		 * Очередь задач.
		 */
		std::array<PlannedTask, ApplicationConfiguration::TaskQueueMaxSize> tasks_queue = {};

		/**
		 * Количество задач в очереди задач.
		 */
		uint32_t tasks_in_queue_count = 0;

		/**
		 * Часы реального времени.
		 */
		Middleware::IClock& clock;

		/**
		 * Запущен ли в данный момент процесс выполнения задач из очереди планировщика.
		 */
		bool isRunning = false;
	 public:
		explicit TasksManager(Middleware::IClock& clock)
		: clock(clock)
		{
		};

		/**
		 * Добавить задачу в очередь на выполнение.
		 * @param added_task Задача, которая будет добавлена в очередь
		 * @return Статус операции
		 */
		Middleware::ReturnCode AddTask(ITask& added_task, uint32_t execute_time_s = 0, int32_t repeat_task_time_s = -1);

		/**
		 * Убрать задачу из очереди на выполнение.
		 * @param task_for_remove Задача, которая будет удалена из очереди
		 * @return Статус операции
		 */
		Middleware::ReturnCode RemoveTask(ITask& task_for_remove);

		/**
		 * Запустить выполнение всех запланированных задач.
		 * @return Статус операции
		 */
		Middleware::ReturnCode RunTasks();

		/**
		 * Возвращает количество запланированных задач в очереди.
		 * @return Количество запланированных задач в очереди.
		 */
		[[nodiscard]] uint32_t GetTasksQueueSize() const;

		/**
		 * Возвращает время запуска ближайшей задачи.
		 * @return Время запуска ближайшей задачи
		 */
		uint32_t GetTimeToCallTheNearestTask();
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_CORE_TASKS_MANAGER_H_
