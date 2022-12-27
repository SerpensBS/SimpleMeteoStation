#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_ISLEEP_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_ISLEEP_H_

#include <cstdint>

namespace Application
{
	/**
	 * Интерфейс драйвера управления режимом сна.
	 */
	class ISleep
	{
	 public:
		/**
		 * Переводит микроконтроллер в режим сна на определенное время.
		 * @param sleep_time_s Время сна в секундах
		 */
		virtual void Sleep(uint32_t sleep_time_s) = 0;

		ISleep() = default;
		virtual ~ISleep() = default;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_INTERFACES_ISLEEP_H_
