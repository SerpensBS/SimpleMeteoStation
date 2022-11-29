#ifndef SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_SENSOR_MOCK_H_
#define SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_SENSOR_MOCK_H_

#include "middleware/interfaces/isensor.h"
#include "middleware/data/return-code.h"

namespace ApplicationTests
{
	/**
	 * Mock драйвера сенсора.
	 */
	class SensorMock : public Middleware::ISensor
	{
		double measured_value;
	 public:
		explicit SensorMock(double measured_value)
		: Middleware::ISensor(), measured_value(measured_value)
		{
		}

		Middleware::ReturnCode GetData(double* out_value) override;
	};
}

#include "sensor-mock.inl"

#endif //SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_SENSOR_MOCK_H_
