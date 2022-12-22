#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_CORE_TASKS_MANAGER_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_CORE_TASKS_MANAGER_H_

#include <array>
#include "middleware/interfaces/iclock.h"
#include "sources/abstract/planned-task.h"
#include "sources/interfaces/itask.h"

namespace Application
{
	/**
	 * Менеджер по работе с задачами.
	 */
	template<uint32_t max_tasks_in_queue_count>
	class TasksManager
	{
	 private:
		/**
		 * Очередь задач.
		 */
		std::array<PlannedTask, max_tasks_in_queue_count> tasks_queue_ = {};

		/**
		 * Количество задач в очереди задач.
		 */
		uint32_t tasks_in_queue_count_ = 0;

		/**
		 * Часы реального времени.
		 */
		Middleware::IClock& clock_;

		/**
		 * Запущен ли в данный момент процесс выполнения задач из очереди планировщика.
		 */
		bool isRunning_ = false;
	 public:
		explicit TasksManager(Middleware::IClock& clock)
		: clock_(clock)
		{
		};

		/**
		 * Добавить задачу в очередь на выполнение.
		 * @param added_task Задача, которая будет добавлена в очередь
		 * @param execute_time_s Через сколько секунд запустить задачу
		 * @param repeat_task_time_s Через сколько секунд после запуска повторить задачу
		 * @return Статус операции
		 */
		Middleware::ReturnCode AddTask(ITask& added_task, uint32_t execute_time_s = 0, uint32_t repeat_task_time_s = 0);

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

#include "tasks-manager.inl"

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_CORE_TASKS_MANAGER_H_
