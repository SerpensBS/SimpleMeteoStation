#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_ICLOCK_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_ICLOCK_H_

#include <cstdint>

namespace Application
{
	/**
	 * Часы реального времени.
	 */
	class IClock
	{
	 public:
		/**
		 * Получить время, прошедшее с момента запуска устройства.
		 * @return Количество секунд, прошедших с момента запуска устройства
		 */
		virtual uint32_t GetCurrentTime() = 0;

		IClock() = default;
		virtual ~IClock() = default;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_ICLOCK_H_
