#include <ctime>
#include "clock-mock.h"

namespace ApplicationTests
{
	uint32_t ClockMock::GetCurrentTime()
	{
		time_t now = time(nullptr);
		tm* ltm = localtime(&now);

		return static_cast<uint32_t>(ltm->tm_hour * 3600 + ltm->tm_min * 60 + ltm->tm_sec);
	}
}