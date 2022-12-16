#ifndef SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_OUTPUT_MOCK_H_
#define SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_OUTPUT_MOCK_H_

#include "middleware/interfaces/ioutput.h"

namespace ApplicationTests
{
	/**
	 * Mock output драйвера.
	 */
	class OutputMock : public Middleware::IOutput
	{
	 public:
		/**
		 * Отправить сообщение. Если сообщение оканчивается нуль-терминатором, то размер сообщения должен подсчитываться
		 * автоматически. В противном случае - необходимо указать размер сообщения.
		 * @param message_text Текст сообщения. Если не оканчивается нуль-терминатором, нужно указать длину сообщения
		 * @param length Длина сообщения
		 */
		void SendMessage(const char message_text[]) override;

		~OutputMock() override = default;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_OUTPUT_MOCK_H_
