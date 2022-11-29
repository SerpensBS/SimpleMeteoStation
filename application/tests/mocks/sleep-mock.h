#ifndef SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_SLEEP_MOCK_H_
#define SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_SLEEP_MOCK_H_

#include <chrono>
#include <thread>

#include "middleware/interfaces/isleep.h"

namespace ApplicationTests
{
	class SleepMock : public Middleware::ISleep
	{
	 public:
		void Sleep(uint32_t sleep_time_s) override;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_SLEEP_MOCK_H_