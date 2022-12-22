#include "sleep-mock.h"

namespace ApplicationTests
{
	void SleepMock::Sleep(uint32_t sleep_time_s)
	{
		std::this_thread::sleep_for(std::chrono::seconds (sleep_time_s));
	}
}