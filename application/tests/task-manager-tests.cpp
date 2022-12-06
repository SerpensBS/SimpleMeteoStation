#include <thread>
#include "gtest/gtest.h"
#include "middleware/enums/return-codes.h"
#include "mocks/clock-mock.h"
#include "sources/core/tasks-manager.h"
#include "sources/tasks/task.h"

namespace ApplicationTests
{
	/**
	 * Тесты для диспетчера задач.
	 */
	class TaskManagerTests : public testing::Test
	{
	};

	/**
	 * Контейнер для входящих параметров, по которому мы поймем порядок выполнения задач.
	 */
	struct InputParametersContainer
	{
		uint32_t first_value = 0;
		uint32_t second_value = 0;
		uint32_t third_value = 0;
		uint32_t four_value = 0;
	};

	/**
	 * Тестовая функция для инкремента числа.
	 * @param value
	 * @return
	 */
	Middleware::ReturnCode IncrementValue(uint32_t& value)
	{
		++value;
		return Middleware::ReturnCode::OK;
	}

	TEST_F(TaskManagerTests, AddDifferentTasksPositive)
	{
		constexpr uint32_t task_manager_queue_max_size = 3;
		constexpr uint32_t expected_tasks_in_queue_count = task_manager_queue_max_size;

		ClockMock clock;
		Application::TasksManager<task_manager_queue_max_size> taskManager(clock);

		ASSERT_EQ(0, taskManager.GetTasksQueueSize());

		uint32_t value = 0;
		Application::Task<uint32_t&> firstTask(IncrementValue, value);
		Application::Task<uint32_t&> secondTask(IncrementValue, value);
		Application::Task<uint32_t&> thirdTask(IncrementValue, value);

		auto first_status = taskManager.AddTask(firstTask);
		auto second_status = taskManager.AddTask(secondTask);
		auto third_status = taskManager.AddTask(thirdTask);

		ASSERT_EQ(Middleware::ReturnCode::OK, first_status);
		ASSERT_EQ(Middleware::ReturnCode::OK, second_status);
		ASSERT_EQ(Middleware::ReturnCode::OK, third_status);
		ASSERT_EQ(expected_tasks_in_queue_count, taskManager.GetTasksQueueSize());
	}

	TEST_F(TaskManagerTests, AddDoubleTaskNegative)
	{
		constexpr uint32_t task_manager_queue_max_size = 3;
		constexpr uint32_t expected_tasks_in_queue_count = 1;
		ClockMock clock;

		Application::TasksManager<task_manager_queue_max_size> taskManager(clock);

		ASSERT_EQ(0, taskManager.GetTasksQueueSize());

		uint32_t value = 0;
		Application::Task<uint32_t&> testTask(IncrementValue, value);
		auto first_status = taskManager.AddTask(testTask);
		auto second_status = taskManager.AddTask(testTask);

		ASSERT_EQ(expected_tasks_in_queue_count, taskManager.GetTasksQueueSize());
		ASSERT_EQ(Middleware::ReturnCode::OK, first_status);
		ASSERT_EQ(Middleware::ReturnCode::ERROR, second_status);
	}

	TEST_F(TaskManagerTests, AddTooManyTaskNegative)
	{
		constexpr uint32_t task_manager_queue_max_size = 2;
		constexpr uint32_t expected_tasks_in_queue_count = task_manager_queue_max_size;
		ClockMock clock;

		Application::TasksManager<task_manager_queue_max_size> taskManager(clock);

		ASSERT_EQ(0, taskManager.GetTasksQueueSize());

		uint32_t value = 0;
		Application::Task<uint32_t&> firstTask(IncrementValue, value);
		Application::Task<uint32_t&> secondTask(IncrementValue, value);
		Application::Task<uint32_t&> thirdTask(IncrementValue, value);

		auto first_status = taskManager.AddTask(firstTask);
		auto second_status = taskManager.AddTask(secondTask);
		auto third_status = taskManager.AddTask(thirdTask);

		ASSERT_EQ(expected_tasks_in_queue_count, taskManager.GetTasksQueueSize());
		ASSERT_EQ(Middleware::ReturnCode::OK, first_status);
		ASSERT_EQ(Middleware::ReturnCode::OK, second_status);
		ASSERT_EQ(Middleware::ReturnCode::ERROR, third_status);
	}

	TEST_F(TaskManagerTests, GetTimeToCallTheNearestTaskPositive)
	{
		constexpr uint32_t task_manager_queue_max_size = 1;
		constexpr uint32_t expected_time = 3214134;

		ClockMock clock;
		Application::TasksManager<task_manager_queue_max_size> taskManager(clock);

		ASSERT_EQ(0, taskManager.GetTasksQueueSize());

		uint32_t value = 0;
		Application::Task<uint32_t&> firstTask(IncrementValue, value);

		taskManager.AddTask(firstTask, expected_time);

		ASSERT_EQ(expected_time, taskManager.GetTimeToCallTheNearestTask());
	}

	TEST_F(TaskManagerTests, RunTasksPositive)
	{
		constexpr uint32_t task_manager_queue_max_size = 4;
		uint32_t expected_tasks_in_queue_count = task_manager_queue_max_size;

		ClockMock clock;
		Application::TasksManager<task_manager_queue_max_size> taskManager(clock);

		ASSERT_EQ(0, taskManager.GetTasksQueueSize());

		InputParametersContainer data;
		Application::Task<uint32_t&> firstTask(IncrementValue, data.first_value);
		Application::Task<uint32_t&> secondTask(IncrementValue, data.second_value);
		Application::Task<uint32_t&> thirdTask(IncrementValue, data.third_value);
		Application::Task<uint32_t&> fourTask(IncrementValue, data.four_value);

		uint32_t current_time = clock.GetCurrentTime();

		taskManager.AddTask(firstTask, current_time + 2);
		taskManager.AddTask(secondTask, current_time);
		taskManager.AddTask(thirdTask, current_time + 6);
		taskManager.AddTask(fourTask, current_time + 4);

		taskManager.RunTasks();

		--expected_tasks_in_queue_count;
		ASSERT_EQ(expected_tasks_in_queue_count, taskManager.GetTasksQueueSize());

		ASSERT_EQ(0, data.first_value);
		ASSERT_EQ(1, data.second_value);
		ASSERT_EQ(0, data.third_value);
		ASSERT_EQ(0, data.four_value);

		std::this_thread::sleep_for(std::chrono::seconds(3));

		taskManager.RunTasks();

		--expected_tasks_in_queue_count;
		ASSERT_EQ(expected_tasks_in_queue_count, taskManager.GetTasksQueueSize());

		ASSERT_EQ(1, data.first_value);
		ASSERT_EQ(1, data.second_value);
		ASSERT_EQ(0, data.third_value);
		ASSERT_EQ(0, data.four_value);

		std::this_thread::sleep_for(std::chrono::seconds(2));

		taskManager.RunTasks();

		--expected_tasks_in_queue_count;
		ASSERT_EQ(expected_tasks_in_queue_count, taskManager.GetTasksQueueSize());

		ASSERT_EQ(1, data.first_value);
		ASSERT_EQ(1, data.second_value);
		ASSERT_EQ(0, data.third_value);
		ASSERT_EQ(1, data.four_value);

		std::this_thread::sleep_for(std::chrono::seconds(2));

		taskManager.RunTasks();

		--expected_tasks_in_queue_count;
		ASSERT_EQ(expected_tasks_in_queue_count, taskManager.GetTasksQueueSize());

		ASSERT_EQ(1, data.first_value);
		ASSERT_EQ(1, data.second_value);
		ASSERT_EQ(1, data.third_value);
		ASSERT_EQ(1, data.four_value);
	}
}