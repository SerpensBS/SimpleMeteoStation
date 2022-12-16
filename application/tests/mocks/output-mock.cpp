#include <iostream>
#include "output-mock.h"

namespace ApplicationTests
{
	void OutputMock::SendMessage(const char* message_text)
	{
		std::cout << message_text;
	}
}
