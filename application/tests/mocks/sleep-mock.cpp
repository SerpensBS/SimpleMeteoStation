#include "sleep-mock.h"

namespace ApplicationTests
{
	/**
	 * Переводит микроконтроллер в режим сна на определенное время.
	 * @param sleep_time_s Время сна в секундах
	*/
	void SleepMock::Sleep(uint32_t sleep_time_s)
	{
		std::this_thread::sleep_for(std::chrono::seconds(sleep_time_s));
	}
}