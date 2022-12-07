#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_TASKS_TASK_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_TASKS_TASK_H_

#include <cstdint>
#include "middleware/enums/return-codes.h"
#include "sources/interfaces/itask.h"

namespace Application
{
	/**
	 * Контейнер для вызова функции с callback'ами.
	 * @tparam PT Тип передаваемого в функции параметра
	 */
	template<typename PT>
	class Task : public ITask
	{
	 private:
		/**
		 * Основная функция, которая будет вызвана при выполнении Execute().
		 * @return Статус операции
		 */
		Middleware::ReturnCode (*run_function)(PT) = nullptr;

		/**
		 * Входящий параметр во всех функциях.
		 */
		PT function_input_parameter;

		/**
		 * Функция, которая будет вызвана, если RunFunction вернет ERROR.
		 */
		void (*error_callback_func)(PT) = nullptr;

		/**
		 * Функция, которая будет вызвана, если RunFunction вернет OK.
		 */
		void (*success_callback_func)(PT) = nullptr;
	 public:
		explicit Task(
			Middleware::ReturnCode (*run_function)(PT),
			PT function_input_parameter = nullptr,
			void error_callback_func(PT) = nullptr,
			void success_callback_func(PT) = nullptr
			)
			:
			run_function(run_function),
			function_input_parameter(function_input_parameter),
			error_callback_func(error_callback_func),
			success_callback_func(success_callback_func)
		{
		}

		/**
		 * Выполнить RunFunction и вызвать callback'и при их наличии.
		 * @return
		 */
		Middleware::ReturnCode Execute() override;


	};
}

#include "task.inl"

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_TASKS_TASK_H_
