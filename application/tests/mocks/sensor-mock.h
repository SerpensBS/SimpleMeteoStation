#ifndef SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_SENSOR_MOCK_H_
#define SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_SENSOR_MOCK_H_

#include "application/interfaces/isensor.h"
#include "middleware/data/enums/return-codes.h"


namespace ApplicationTests
{
	/**
	 * Mock драйвера сенсора.
	 */
	class SensorMock : public Application::ISensor
	{
	 private:
		/**
		 * Значение, которое будет возвращаться как результат измерения.
		 */
		double measured_value_;
	 public:
		explicit SensorMock(double measured_value)
		: Application::ISensor(), measured_value_(measured_value)
		{
		}

		/**
		 * Получить фейковые данные с сенсора
		 * @param out_value Данные, которые вернет сенсор
		 * @return Статус операции
		 */
		Middleware::ReturnCode GetMeasureData(double* out_value) override;
	};
}

#include "sensor-mock.inl"

#endif //SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_SENSOR_MOCK_H_
