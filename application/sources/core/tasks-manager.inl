#include <algorithm>
#include <cstdint>

namespace Application
{
	template<uint32_t max_tasks_in_queue_count>
	Middleware::ReturnCode TasksManager<max_tasks_in_queue_count>::AddTask(
		Application::ITask& added_task,
		uint32_t execute_time_s,
		uint32_t repeat_task_time_s)
	{
		// Если нет нового места под подписку - выходим.
		if (tasks_queue_.size() > max_tasks_in_queue_count)
		{
			return Middleware::ReturnCode::ERROR;
		}

		// Не даем подписаться дважды - ищем дубли.
		for (auto& planned_task : tasks_queue_)
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
					: clock_.GetCurrentTime();
				planned_task.repeat_task_time_s = repeat_task_time_s;

				++tasks_in_queue_count_;

				std::sort(tasks_queue_.begin(), tasks_queue_.end());
				return Middleware::ReturnCode::OK;
			}
		}

		return Middleware::ReturnCode::ERROR;
	}

	template<uint32_t max_tasks_in_queue_count>
	Middleware::ReturnCode TasksManager<max_tasks_in_queue_count>::RemoveTask(ITask& task_for_remove)
	{
		for (uint32_t i = 0; i < tasks_queue_.size(); ++i)
		{
			// Если нашли элемент, то удаляем его и сдвигаем все последующие оставшиеся элементы влево.
			if (tasks_queue_[i].task == &task_for_remove)
			{
				tasks_queue_[i] = {};

				for (uint32_t j = i + 1; j < tasks_queue_.size(); ++j, ++i)
				{
					tasks_queue_[i] = tasks_queue_[j];
					tasks_queue_[j] = {};
				}

				--tasks_in_queue_count_;

				// Если идет выполнение задач из очереди - не торопимся сортировать, т.к. это будет сделано в конце
				// цикла поочередного выполнения всех запланированных к этому времени задач.
				if (!isRunning_)
				{
					std::sort(tasks_queue_.begin(), tasks_queue_.end());
				}

				return Middleware::ReturnCode::OK;
			}
		}

		return Middleware::ReturnCode::ERROR;
	}

	template<uint32_t max_tasks_in_queue_count>
	Middleware::ReturnCode TasksManager<max_tasks_in_queue_count>::RunTasks()
	{
		isRunning_ = true;
		for (auto& planned_task : tasks_queue_)
		{
			// Если больше задач не найдено - выходим.
			if (!planned_task.task)
			{
				break;
			}

			// Если задача должна быть выполнена к этому моменту - выполняем.
			if (planned_task.execute_task_time_s <= clock_.GetCurrentTime())
			{
				planned_task.task->Execute();

				// Если задача запланирована на повтор - задаем новое время, если нет - удаляем.
				if (0 ==planned_task.repeat_task_time_s)
				{
					RemoveTask(*planned_task.task);
				}
				else
				{
					planned_task.execute_task_time_s += planned_task.repeat_task_time_s;
				}
			}
		}

		isRunning_ = false;

		// Сортируем оставшиеся задачи и выходим.
		std::sort(tasks_queue_.begin(), tasks_queue_.end());
		return Middleware::ReturnCode::OK;
	}

	template<uint32_t max_tasks_in_queue_count>
	uint32_t TasksManager<max_tasks_in_queue_count>::GetTasksQueueSize() const
	{
		return tasks_in_queue_count_;
	}

	template<uint32_t max_tasks_in_queue_count>
	uint32_t TasksManager<max_tasks_in_queue_count>::GetTimeToCallTheNearestTask()
	{
		return tasks_queue_[0].execute_task_time_s;
	}
}