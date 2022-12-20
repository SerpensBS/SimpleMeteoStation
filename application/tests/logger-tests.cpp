#include <cstring>
#include "config/app-config.h"
#include "gtest/gtest.h"
#include "mocks/logger-mock.h"

namespace ApplicationTests
{
	/**
	 * Тесты для logger'а
	 */
	class LoggerTests : public testing::Test
	{
	};

	TEST_F(LoggerTests, LogPositive)
	{
		const char* expected_message = "Hello World!";

		Application::LogLevelLimitsConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Application::LogLevel::Error;
		logger_configuration.MinLogLevel = Application::LogLevel::Trace;

		LoggerMock logger(logger_configuration);
		logger.Log(Application::LogLevel::Trace, "%s", expected_message);

		Log result = logger.GetBufferedMessageData();
		ASSERT_STREQ(expected_message, result.LogMessage.data());
		ASSERT_EQ(Application::LogLevel::Trace, result.LogLevel);
	}

	TEST_F(LoggerTests, BufferSizePositive)
	{
		char expected_message[Application::ApplicationConfiguration::LogFormatterBufferSize];
		std::memset(expected_message, 'a', Application::ApplicationConfiguration::LogFormatterBufferSize - 1);

		Application::LogLevelLimitsConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Application::LogLevel::Error;
		logger_configuration.MinLogLevel = Application::LogLevel::Trace;

		LoggerMock logger(logger_configuration);
		logger.Log(Application::LogLevel::Trace, "%s", expected_message);

		Log result = logger.GetBufferedMessageData();
		ASSERT_STREQ(expected_message, result.LogMessage.data());
		ASSERT_EQ(Application::LogLevel::Trace, result.LogLevel);
	}

	TEST_F(LoggerTests, BufferSizeNegative)
	{
		char expected_message[Application::ApplicationConfiguration::LogFormatterBufferSize + 1];
		std::memset(expected_message, 'a', Application::ApplicationConfiguration::LogFormatterBufferSize);

		Application::LogLevelLimitsConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Application::LogLevel::Error;
		logger_configuration.MinLogLevel = Application::LogLevel::Trace;

		LoggerMock logger(logger_configuration);
		logger.Log(Application::LogLevel::Trace, "%s", expected_message);

		Log result = logger.GetBufferedMessageData();
		ASSERT_STRNE(expected_message, result.LogMessage.data());
		ASSERT_EQ(Application::LogLevel::Error, result.LogLevel);
	}

	TEST_F(LoggerTests, LogLevelPositive)
	{
		const char* expected_message = "Hello World!";

		Application::LogLevelLimitsConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Application::LogLevel::Error;
		logger_configuration.MinLogLevel = Application::LogLevel::Trace;
		LoggerMock logger(logger_configuration);

		for (int i = 0; i <= static_cast<int>(Application::LogLevel::Error); ++i)
		{
			auto expected_log_level = static_cast<Application::LogLevel>(i);
			logger.Log(expected_log_level, "%s", expected_message);
			Log result = logger.GetBufferedMessageData();

			ASSERT_STREQ(expected_message, result.LogMessage.data());
			ASSERT_EQ(expected_log_level, result.LogLevel);
		}
	}

	TEST_F(LoggerTests, ParseArgumentsPositive)
	{
		const char* expected_message = "Hello 1 2 2.1 3.4";

		Application::LogLevelLimitsConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Application::LogLevel::Error;
		logger_configuration.MinLogLevel = Application::LogLevel::Trace;
		LoggerMock logger(logger_configuration);

		logger.Log(Application::LogLevel::Trace, "%s %lu %ld %1.1f %1.1f", "Hello",
			static_cast<uint32_t>(1),
			static_cast<int32_t>(2),
			static_cast<float>(2.1f),
			static_cast<double>(3.4));

		Log result = logger.GetBufferedMessageData();

		ASSERT_STREQ(expected_message, result.LogMessage.data());
		ASSERT_EQ(Application::LogLevel::Trace, result.LogLevel);
	}
}