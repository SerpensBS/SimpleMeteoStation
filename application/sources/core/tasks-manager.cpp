#include <cstdint>
#include "tasks-manager.h"

namespace Application
{
	Middleware::ReturnCode TasksManager::AddTask(Application::ITask& task)
	{
		// Если нет нового места под подписку - выходим.
		if (nullptr != tasks.back())
		{
			return Middleware::ReturnCode::ERROR;
		}

		// Не даем подписаться дважды - ищем дубли.
		for (auto& i : tasks)
		{
			if (i == &task)
			{
				return Middleware::ReturnCode::ERROR;
			}

			if (i == nullptr)
			{
				i = &task;
				return Middleware::ReturnCode::OK;
			}
		}

		return Middleware::ReturnCode::ERROR;
	}

	Middleware::ReturnCode TasksManager::RemoveTask(ITask& task)
	{
		for (uint32_t i = 0; i < tasks.size(); ++i)
		{
			// Если нашли элемент, то удаляем его и сдвигаем все последующие оставшиеся элементы влево.
			if (tasks[i] == &task)
			{
				tasks[i] = nullptr;

				for (uint32_t j = i + 1; j < tasks.size(); ++j, ++i)
				{
					tasks[i] = tasks[j];
					tasks[j] = nullptr;
				}

				return Middleware::ReturnCode::OK;
			}
		}

		return Middleware::ReturnCode::ERROR;
	}

	Middleware::ReturnCode TasksManager::RunTasks()
	{
		for (auto* task : tasks)
		{
			if (!task)
			{
				return Middleware::ReturnCode::OK;
			}

			task->Execute();
		}

		return Middleware::ReturnCode::OK;
	}
}

