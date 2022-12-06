#include <algorithm>
#include <cstdint>

namespace Application
{
	template<uint32_t max_tasks_in_queue_count>
	Middleware::ReturnCode TasksManager<max_tasks_in_queue_count>::AddTask(
		Application::ITask& added_task,
		uint32_t execute_time_s,
		int32_t repeat_task_time_s)
	{
		// Если нет нового места под подписку - выходим.
		if (tasks_queue.size() > max_tasks_in_queue_count)
		{
			return Middleware::ReturnCode::ERROR;
		}

		// Не даем подписаться дважды - ищем дубли.
		for (auto& planned_task : tasks_queue)
		{
			// Выходим с ошибкой, если дубль.
			if (planned_task.task == &added_task)
			{
				return Middleware::ReturnCode::ERROR;
			}

			// Добавляем задачу в очередь, когда нашли свободное место.
			if (planned_task.task == nullptr)
			{
				planned_task.task = &added_task;
				planned_task.execute_task_time_s = execute_time_s > 0
					? execute_time_s
					: clock.GetCurrentTime();
				planned_task.repeat_task_time_s = repeat_task_time_s;

				++tasks_in_queue_count;

				std::sort(tasks_queue.begin(), tasks_queue.end());
				return Middleware::ReturnCode::OK;
			}
		}

		return Middleware::ReturnCode::ERROR;
	}

	template<uint32_t max_tasks_in_queue_count>
	Middleware::ReturnCode TasksManager<max_tasks_in_queue_count>::RemoveTask(ITask& task_for_remove)
	{
		for (uint32_t i = 0; i < tasks_queue.size(); ++i)
		{
			// Если нашли элемент, то удаляем его и сдвигаем все последующие оставшиеся элементы влево.
			if (tasks_queue[i].task == &task_for_remove)
			{
				tasks_queue[i] = {};

				for (uint32_t j = i + 1; j < tasks_queue.size(); ++j, ++i)
				{
					tasks_queue[i] = tasks_queue[j];
					tasks_queue[j] = {};
				}

				--tasks_in_queue_count;

				// Если идет выполнение задач из очереди - не торопимся сортировать, т.к. это будет сделано в конце
				// цикла поочередного выполнения всех запланированных к этому времени задач.
				if (!isRunning)
				{
					std::sort(tasks_queue.begin(), tasks_queue.end());
				}

				return Middleware::ReturnCode::OK;
			}
		}

		return Middleware::ReturnCode::ERROR;
	}

	template<uint32_t max_tasks_in_queue_count>
	Middleware::ReturnCode TasksManager<max_tasks_in_queue_count>::RunTasks()
	{
		isRunning = true;
		for (auto& planned_task : tasks_queue)
		{
			// Если больше задач не найдено - выходим.
			if (!planned_task.task)
			{
				break;
			}

			// Если задача должна быть выполнена к этому моменту - выполняем.
			if (planned_task.execute_task_time_s <= clock.GetCurrentTime())
			{
				planned_task.task->Execute();

				// Если задача запланирована на повтор - задаем новое время, если нет - удаляем.
				if (planned_task.repeat_task_time_s < 0)
				{
					RemoveTask(*planned_task.task);
				}
				else
				{
					planned_task.execute_task_time_s += static_cast<uint32_t>(planned_task.repeat_task_time_s);
				}
			}
		}

		isRunning = false;

		// Сортируем оставшиеся задачи и выходим.
		std::sort(tasks_queue.begin(), tasks_queue.end());
		return Middleware::ReturnCode::OK;
	}

	template<uint32_t max_tasks_in_queue_count>
	uint32_t TasksManager<max_tasks_in_queue_count>::GetTasksQueueSize() const
	{
		return tasks_in_queue_count;
	}

	template<uint32_t max_tasks_in_queue_count>
	uint32_t TasksManager<max_tasks_in_queue_count>::GetTimeToCallTheNearestTask()
	{
		return tasks_queue[0].execute_task_time_s;
	}
}