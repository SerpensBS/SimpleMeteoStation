#ifndef SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_ICLOCK_H_
#define SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_ICLOCK_H_

#include <cstdint>

namespace Middleware
{
	/**
	 * Часы реального времени.
	 */
	class IClock
	{
	 public:
		/**
		 * Получить реальное время в секундах
		 * @return Количество секунд, прошедших с 00:00 текущего дня
		 */
		virtual uint32_t GetCurrentTime() = 0;
	};
}

#endif //SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_ICLOCK_H_
