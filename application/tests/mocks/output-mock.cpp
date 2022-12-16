#include <iostream>
#include "output-mock.h"

namespace ApplicationTests
{
	void OutputMock::SendMessage(const char* message_text)
	{
		buffered_message_.append(message_text);
		std::cout << message_text;
	}

	char* OutputMock::GetBufferedMessage()
	{
		return buffered_message_.data();
	}

	void OutputMock::ClearBufferedMessage()
	{
		buffered_message_.clear();
	}
}
