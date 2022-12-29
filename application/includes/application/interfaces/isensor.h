#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_ISENSOR_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_ISENSOR_H_

#include "middleware/data/enums/return-codes.h"

namespace Application
{
	/**
	 * Интерфейс драйвера сенсора.
	 */
	class ISensor
	{
	 public:
		/**
		 * Получить данные от сенсора.
		 * @param data Данные сенсора
		 * @return Статус операции
		 */
		virtual Middleware::ReturnCode GetMeasureData(double* data) = 0;

		virtual ~ISensor() = default;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_ISENSOR_H_
