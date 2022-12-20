#ifndef SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_LOGGER_MOCK_H_
#define SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_LOGGER_MOCK_H_

#include <string>
#include "application/data/abstract/logger.h"

namespace ApplicationTests
{
	/**
	 * Данные лог-сообщения.
	 */
	struct Log
	{
		Application::LogLevel LogLevel = Application::LogLevel::Trace;
		std::string LogMessage;
	};

	/**
	 * Mock logger'а.
	 */
	class LoggerMock : public Application::Logger
	{
	 private:
		ApplicationTests::Log buffered_message_;
		/**
		 * Записать сообщение в лог.
		 * @param log_level Уровень лога
		 * @param log_format_message Текст сообщения с нуль-терминатором
		 */
		void WriteLog(Application::LogLevel log_level, const char* log_format_message) override;
	 public:

		/**
		 * Получить данные последнего отправленного в лог сообщения.
		 * @return Данные лог-сообщения
		 */
		ApplicationTests::Log GetBufferedMessageData();

		explicit LoggerMock(Application::LogLevelLimitsConfiguration configuration)
		: Application::Logger(configuration)
		{
		}
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_TESTS_MOCKS_LOGGER_MOCK_H_
