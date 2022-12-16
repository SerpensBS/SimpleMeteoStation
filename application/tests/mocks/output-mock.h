#ifndef SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_OUTPUT_MOCK_H_
#define SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_OUTPUT_MOCK_H_

#include <cstdint>
#include <string>
#include "middleware/interfaces/ioutput.h"

namespace ApplicationTests
{
	/**
	 * Mock output драйвера.
	 */
	class OutputMock : public Middleware::IOutput
	{
	 private:
		std::string buffered_message_;
	 public:
		/**
		 * Отправить сообщение. Если сообщение оканчивается нуль-терминатором, то размер сообщения должен подсчитываться
		 * автоматически. В противном случае - необходимо указать размер сообщения.
		 * @param message_text Текст сообщения. Если не оканчивается нуль-терминатором, нужно указать длину сообщения
		 * @param length Длина сообщения
		 */
		void SendMessage(const char message_text[]) override;

		char* GetBufferedMessage();
		void ClearBufferedMessage();

		OutputMock() = default;
		~OutputMock() override = default;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_OUTPUT_MOCK_H_
