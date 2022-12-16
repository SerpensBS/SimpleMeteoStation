#include "gtest/gtest.h"
#include "io/logger.h"
#include "mocks/output-mock.h"

namespace ApplicationTests
{
	/**
	 * Тесты для logger'а
	 */
	class LoggerTests : public testing::Test
	{
	};

	#pragma region UtilityFunctions
	/**
	 * Получить из строки LogLevel
	 * @param message Строка лога
	 * @return LogLevel
	 */
	Middleware::LogLevel GetLogLevel(const char* message)
	{
		std::string message_string = message;

		uint64_t end_of_prefix = message_string.find(']');
		std::string log_level = message_string.substr(1, end_of_prefix - 1);

		if (log_level == "DEBUG")
		{
			return Middleware::LogLevel::Debug;
		}
		if (log_level == "TRACE")
		{
			return Middleware::LogLevel::Trace;
		}
		if (log_level == "INFO")
		{
			return Middleware::LogLevel::Info;
		}
		if (log_level == "WARN")
		{
			return Middleware::LogLevel::Warn;
		}
		if (log_level == "ERROR")
		{
			return Middleware::LogLevel::Error;
		}

		return Middleware::LogLevel::Error;
	}
	#pragma endregion

	TEST_F(LoggerTests, LogPositive)
	{
		const char* expected_message = "[TRACE] Hello World!\n\r";
		const uint32_t buffer_size = 32;

		OutputMock output;

		Middleware::LoggerConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Middleware::LogLevel::Error;
		logger_configuration.MinLogLevel = Middleware::LogLevel::Trace;

		Application::Logger<buffer_size> logger(output, logger_configuration);
		logger.Log(Middleware::LogLevel::Trace, "%s", "Hello World!");

		ASSERT_STREQ(expected_message, output.GetBufferedMessage());
		ASSERT_EQ(Middleware::LogLevel::Trace, GetLogLevel(output.GetBufferedMessage()));
	}

	TEST_F(LoggerTests, BufferSizePositive)
	{
		const char* expected_message = "[TRACE] Hello World!\n\r";
		const uint32_t buffer_size = 13;

		OutputMock output;

		Middleware::LoggerConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Middleware::LogLevel::Error;
		logger_configuration.MinLogLevel = Middleware::LogLevel::Trace;

		Application::Logger<buffer_size> logger(output, logger_configuration);
		logger.Log(Middleware::LogLevel::Trace, "%s", "Hello World!");

		ASSERT_STREQ(expected_message, output.GetBufferedMessage());
		ASSERT_EQ(Middleware::LogLevel::Trace, GetLogLevel(output.GetBufferedMessage()));
	}

	TEST_F(LoggerTests, BufferSizeNegative)
	{
		const char* expected_message = "[TRACE] Hello World!\n\r";
		const uint32_t buffer_size = 12;

		OutputMock output;

		Middleware::LoggerConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Middleware::LogLevel::Error;
		logger_configuration.MinLogLevel = Middleware::LogLevel::Trace;

		Application::Logger<buffer_size> logger(output, logger_configuration);
		logger.Log(Middleware::LogLevel::Trace, "%s", "Hello World!");

		ASSERT_STRNE(expected_message, output.GetBufferedMessage());
		ASSERT_EQ(Middleware::LogLevel::Error, GetLogLevel(output.GetBufferedMessage()));
	}

	TEST_F(LoggerTests, LogLevelPositive)
	{
		OutputMock output;

		Middleware::LoggerConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Middleware::LogLevel::Error;
		logger_configuration.MinLogLevel = Middleware::LogLevel::Trace;

		const uint32_t buffer_size = 32;
		Application::Logger<buffer_size> logger(output, logger_configuration);

		logger.Log(Middleware::LogLevel::Debug, "%s", "Hello World!");
		ASSERT_EQ(Middleware::LogLevel::Debug, GetLogLevel(output.GetBufferedMessage()));

		output.ClearBufferedMessage();
		logger.Log(Middleware::LogLevel::Trace, "%s", "Hello World!");
		ASSERT_EQ(Middleware::LogLevel::Trace, GetLogLevel(output.GetBufferedMessage()));

		output.ClearBufferedMessage();
		logger.Log(Middleware::LogLevel::Info, "%s", "Hello World!");
		ASSERT_EQ(Middleware::LogLevel::Info, GetLogLevel(output.GetBufferedMessage()));

		output.ClearBufferedMessage();
		logger.Log(Middleware::LogLevel::Warn, "%s", "Hello World!");
		ASSERT_EQ(Middleware::LogLevel::Warn, GetLogLevel(output.GetBufferedMessage()));

		output.ClearBufferedMessage();
		logger.Log(Middleware::LogLevel::Error, "%s", "Hello World!");
		ASSERT_EQ(Middleware::LogLevel::Error, GetLogLevel(output.GetBufferedMessage()));
	}

	TEST_F(LoggerTests, ParseArgumentsPositive)
	{
		const char* expected_message = "[TRACE] Hello 1 2 2.1 3.4\n\r";
		const uint32_t buffer_size = 32;

		OutputMock output;

		Middleware::LoggerConfiguration logger_configuration;
		logger_configuration.MaxLogLevel = Middleware::LogLevel::Error;
		logger_configuration.MinLogLevel = Middleware::LogLevel::Trace;

		Application::Logger<buffer_size> logger(output, logger_configuration);
		logger.Log(Middleware::LogLevel::Trace, "%s %lu %ld %1.1f %1.1f", "Hello",
			static_cast<uint32_t>(1),
			static_cast<int32_t>(2),
			static_cast<float>(2.1f),
			static_cast<double>(3.4));

		ASSERT_STREQ(expected_message, output.GetBufferedMessage());
		ASSERT_EQ(Middleware::LogLevel::Trace, GetLogLevel(output.GetBufferedMessage()));
	}
}