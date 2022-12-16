#include "core/core.h"
#include "gtest/gtest.h"
#include "mocks/clock-mock.h"
#include "mocks/display-mock.h"
#include "mocks/output-mock.h"
#include "mocks/sensor-mock.h"
#include "mocks/sleep-mock.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")
namespace ApplicationTests
{
	/**
	 * Функциональный тест. Так как программа зациклена - тест возможен только в ручном режиме.
	 */
	class CoreTests : public ::testing::Test
	{
	 protected:
		void SetUp() override
		{
			temperature_sensor = new SensorMock(23.7);
			pressure_sensor = new SensorMock(756.3);
			sleep_manager = new SleepMock();
			display = new DisplayMock<12, 4>();
			clock = new ClockMock();
			output = new OutputMock();
		}

		void TearDown() override
		{
			delete temperature_sensor;
			temperature_sensor = nullptr;

			delete pressure_sensor;
			pressure_sensor = nullptr;

			delete sleep_manager;
			sleep_manager = nullptr;

			delete display;
			display = nullptr;

			delete output;
			output = nullptr;
		}
	 public:
		static SensorMock* temperature_sensor;
		static SensorMock* pressure_sensor;
		static SleepMock* sleep_manager;
		static DisplayMock<12, 4>* display;
		static ClockMock* clock;
		static OutputMock* output;
	};

	SensorMock* CoreTests::temperature_sensor = nullptr;
	SensorMock* CoreTests::pressure_sensor = nullptr;
	SleepMock* CoreTests::sleep_manager = nullptr;
	DisplayMock<12, 4>* CoreTests::display = nullptr;
	ClockMock* CoreTests::clock = nullptr;
	OutputMock* CoreTests::output = nullptr;

	/**
	 * Запускает функциональный тест.
	 */
	TEST_F(CoreTests, Run)
	{
		auto status = Application::Core::Run(temperature_sensor, pressure_sensor, sleep_manager, display, clock, output);
		ASSERT_EQ(Middleware::ReturnCode::OK, status);
	}
}
#pragma GCC pop_options