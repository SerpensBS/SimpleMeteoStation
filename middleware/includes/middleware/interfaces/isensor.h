#ifndef SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_ISENSOR_H_
#define SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_ISENSOR_H_

#include <cstdint>
#include "middleware/data/enums/return-codes.h"

namespace Middleware
{
	/**
	 * Интерфейс драйвера сенсора.
	 */
	class ISensor
	{
	 public:
		virtual ~ISensor() = default;

		/**
		 * Получить данные от сенсора.
		 * @param data Данные сенсора
		 * @return Статус операции
		 */
		virtual Middleware::ReturnCode GetData(double* data) = 0;
	};
}

#endif //SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_ISENSOR_H_
