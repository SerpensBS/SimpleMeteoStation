#include "application/core/core.h"
#include "gtest/gtest.h"
#include "mocks/clock-mock.h"
#include "mocks/display-mock.h"
#include "mocks/sensor-mock.h"
#include "mocks/sleep-mock.h"
#include "utils/console-logger.h"

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
			sleep_manager = new SleepMock();
			display = new DisplayMock<12, 4>();
			clock = new ClockMock();

		}

		void TearDown() override
		{
			delete sleep_manager;
			sleep_manager = nullptr;

			delete display;
			display = nullptr;
		}
	 public:
		static SleepMock* sleep_manager;
		static DisplayMock<12, 4>* display;
		static ClockMock* clock;
	};

	SleepMock* CoreTests::sleep_manager = nullptr;
	DisplayMock<12, 4>* CoreTests::display = nullptr;
	ClockMock* CoreTests::clock = nullptr;

	/**
	 * Запускает функциональный тест.
	 */
	TEST_F(CoreTests, Run)
	{
		SensorMock temperature_sensor(23.7);
		SensorMock pressure_sensor(756.3);

		Application::LogLevelLimitsConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Application::LogLevel::Error;
		logger_configuration.MinLogLevel = Application::LogLevel::Trace;
		ConsoleLogger logger(logger_configuration);

		auto status = Application::Core::Run(&temperature_sensor, &pressure_sensor, sleep_manager, display, clock, &logger);
		ASSERT_EQ(Middleware::ReturnCode::OK, status);
	}
}
#pragma GCC pop_options