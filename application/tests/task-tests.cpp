#include "gtest/gtest.h"
#include "sources/tasks/task.h"

namespace ApplicationTests
{
	/**
	 * Тесты для Task.
	 */
	class TaskTests : public testing::Test
	{
	};

	#pragma region Data
	/**
	 * Входящий параметр для тестовых функций. Значение полей должно меняться в зависимости от вызываемых методов.
	 */
	struct TaskInputParameters
	{
		uint32_t function_result = 0;
		uint32_t success_result = 0;
		uint32_t failure_result = 0;
	};

	#pragma endregion

	#pragma region TestFunctions
	/**
	 * Возвращает OK и инкрементирует FunctionResult поле.
	 */
	Middleware::ReturnCode GetOkFuncAndIncrementValue(TaskInputParameters* value)
	{
		value->function_result++;
		return Middleware::ReturnCode::OK;
	}

	/**
	 * Возвращает ERROR и инкрементирует FunctionResult поле.
	 */
	Middleware::ReturnCode GetErrorFuncAndIncrementValue(TaskInputParameters* value)
	{
		value->function_result++;
		return Middleware::ReturnCode::ERROR;
	}

	/**
	 * Инкрементирует FailureResult поле.
	 */
	void FailureCallback(TaskInputParameters* value)
	{
		value->failure_result++;
	}

	/**
	 * Инкрементирует SuccessResult поле.
	 */
	void SuccessCallback(TaskInputParameters* value)
	{
		value->success_result++;
	}

	#pragma endregion

	TEST_F(TaskTests, RunWithoutCallbacksPositive)
	{
		uint32_t start_value = 0x00;
		uint32_t expected_value = start_value + 1;

		TaskInputParameters input_parameters;
		input_parameters.function_result = start_value;
		input_parameters.success_result = start_value;
		input_parameters.failure_result = start_value;

		Application::Task task(GetOkFuncAndIncrementValue, &input_parameters);
		auto status = task.Execute();

		ASSERT_EQ(Middleware::ReturnCode::OK, status);
		ASSERT_EQ(expected_value, input_parameters.function_result);
		ASSERT_EQ(start_value, input_parameters.success_result);
		ASSERT_EQ(start_value, input_parameters.failure_result);
	}

	TEST_F(TaskTests, RunWithoutCallbacksNegative)
	{
		uint32_t start_value = 0x00;
		uint32_t expected_value = start_value + 1;

		TaskInputParameters input_parameters;
		input_parameters.function_result = start_value;
		input_parameters.success_result = start_value;
		input_parameters.failure_result = start_value;

		Application::Task task(GetErrorFuncAndIncrementValue, &input_parameters);
		auto status = task.Execute();

		ASSERT_EQ(Middleware::ReturnCode::ERROR, status);
		ASSERT_EQ(expected_value, input_parameters.function_result);
		ASSERT_EQ(start_value, input_parameters.success_result);
		ASSERT_EQ(start_value, input_parameters.failure_result);
	}

	TEST_F(TaskTests, RunWithCallbacsPositive)
	{
		uint32_t start_value = 0x00;
		uint32_t expected_value = start_value + 1;

		TaskInputParameters input_parameters;
		input_parameters.function_result = start_value;
		input_parameters.success_result = start_value;
		input_parameters.failure_result = start_value;

		Application::Task<TaskInputParameters*> task(GetOkFuncAndIncrementValue, &input_parameters, FailureCallback, SuccessCallback);
		auto status = task.Execute();

		ASSERT_EQ(Middleware::ReturnCode::OK, status);
		ASSERT_EQ(expected_value, input_parameters.function_result);
		ASSERT_EQ(expected_value, input_parameters.success_result);
		ASSERT_EQ(start_value, input_parameters.failure_result);
	}

	TEST_F(TaskTests, RunWithCallbacsNegative)
	{
		uint32_t start_value = 0x00;
		uint32_t expected_value = start_value + 1;

		TaskInputParameters input_parameters;
		input_parameters.function_result = start_value;
		input_parameters.success_result = start_value;
		input_parameters.failure_result = start_value;

		Application::Task<TaskInputParameters*> task(GetErrorFuncAndIncrementValue, &input_parameters, FailureCallback, SuccessCallback);
		auto status = task.Execute();

		ASSERT_EQ(Middleware::ReturnCode::ERROR, status);
		ASSERT_EQ(expected_value, input_parameters.function_result);
		ASSERT_EQ(start_value, input_parameters.success_result);
		ASSERT_EQ(expected_value, input_parameters.failure_result);
	}

	TEST_F(TaskTests, RunWithOnlyErrorCallbackPositive)
	{
		uint32_t start_value = 0x00;
		uint32_t expected_value = start_value + 1;

		TaskInputParameters input_parameters;
		input_parameters.function_result = start_value;
		input_parameters.success_result = start_value;
		input_parameters.failure_result = start_value;

		Application::Task<TaskInputParameters*> task(GetOkFuncAndIncrementValue, &input_parameters, FailureCallback);
		auto status = task.Execute();

		ASSERT_EQ(Middleware::ReturnCode::OK, status);
		ASSERT_EQ(expected_value, input_parameters.function_result);
		ASSERT_EQ(start_value, input_parameters.success_result);
		ASSERT_EQ(start_value, input_parameters.failure_result);
	}

	TEST_F(TaskTests, RunWithOnlyErrorCallbackNegative)
	{
		uint32_t start_value = 0x00;
		uint32_t expected_value = start_value + 1;

		TaskInputParameters input_parameters;
		input_parameters.function_result = start_value;
		input_parameters.success_result = start_value;
		input_parameters.failure_result = start_value;

		Application::Task<TaskInputParameters*> task(GetErrorFuncAndIncrementValue, &input_parameters, FailureCallback);
		auto status = task.Execute();

		ASSERT_EQ(Middleware::ReturnCode::ERROR, status);
		ASSERT_EQ(expected_value, input_parameters.function_result);
		ASSERT_EQ(start_value, input_parameters.success_result);
		ASSERT_EQ(expected_value, input_parameters.failure_result);
	}
}