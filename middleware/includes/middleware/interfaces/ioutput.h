#ifndef SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_IOUTPUT_H_
#define SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_IOUTPUT_H_

#include <cstdint>

namespace Middleware
{
	/**
	 * Отправляет массив байт по шине.
	 */
	class IOutput
	{
	 public:
		/**
		 * Отправить сообщение. Если сообщение оканчивается нуль-терминатором, то размер сообщения должен подсчитываться
		 * автоматически. В противном случае - необходимо указать размер сообщения.
		 * @param message_text Текст сообщения. Если не оканчивается нуль-терминатором, нужно указать длину сообщения
		 * @param length Длина сообщения
		 */
		virtual void SendMessage(const char message_text[]) = 0;

		virtual ~IOutput() = default;
	};
}

#endif //SIMPLEMETEOSTATION_MIDDLEWARE_INCLUDES_MIDDLEWARE_INTERFACES_IOUTPUT_H_
