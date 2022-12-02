#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_CORE_TASKS_MANAGER_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_CORE_TASKS_MANAGER_H_

#include <array>
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
		std::array<ITask*, 255> tasks = {};
	 public:
		TasksManager() = default;

		/**
		 * Добавить задачу в очередь на выполнение.
		 * @param task Задача
		 * @return Статус операции
		 */
		Middleware::ReturnCode AddTask(ITask& task);

		/**
		 * Убрать задачу из очереди на выполнение.
		 * @param task Задача
		 * @return Статус операции
		 */
		Middleware::ReturnCode RemoveTask(ITask& task);

		/**
		 * Запустить выполнение всех запланированных задач.
		 * @return Статус операции
		 */
		Middleware::ReturnCode RunTasks();
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_CORE_TASKS_MANAGER_H_
