#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_ABSTRACT_PLANNED_TASK_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_ABSTRACT_PLANNED_TASK_H_

#include <cstdint>
#include "sources/interfaces/itask.h"

namespace Application
{
	/**
	 * Данные о времени запуска и необходимости повторения для планировщика задач.
	 */
	class PlannedTask
	{
	 public:
		/**
		 * Задача, запланированная к запуску.
		 */
		ITask* task = nullptr;

		/**
		 * Время запуска задачи (сек).
		 */
		uint32_t execute_task_time_s = UINT32_MAX;

		/**
		 * Время, через которое надо повторить запуск задачи после предыдущего успешного запуска (сек).
		 * Если 0 - значит, не повторяем.
		 */
		uint32_t repeat_task_time_s = 0;

		bool operator<(const PlannedTask& another) const
		{
			return execute_task_time_s < another.execute_task_time_s;
		}
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_ABSTRACT_PLANNED_TASK_H_
