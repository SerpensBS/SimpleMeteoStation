#ifndef SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_ISLEEP_H_
#define SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_ISLEEP_H_

#include <cstdint>

namespace Middleware
{
	/**
	 * Интерфейс драйвера управления режимом сна.
	 */
	class ISleep
	{
	 public:
		virtual ~ISleep() = default;

		/**
		 * Переводит микроконтроллер в режим сна на определенное время.
		 * @param sleep_time_s Время сна в секундах
		 */
		virtual void Sleep(uint32_t sleep_time_s) = 0;
	};
}

#endif //SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_ISLEEP_H_
