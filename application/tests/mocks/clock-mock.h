#ifndef SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_CLOCK_MOCK_H_
#define SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_CLOCK_MOCK_H_

#include "middleware/interfaces/iclock.h"

namespace ApplicationTests
{
	/**
	 * Mock для часов реального времени.
	 */
	class ClockMock : public Middleware::IClock
	{
	 public:
		/**
		 * Получить реальное время в секундах
		 * @return Количество секунд, прошедших с 00:00 текущего дня
		 */
		uint32_t GetCurrentTime() override;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_CLOCK_MOCK_H_
