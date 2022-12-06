#include <chrono>
#include <ctime>

namespace ApplicationTests
{
	template<uint32_t width_display, uint32_t height_display>
	Middleware::ReturnCode DisplayMock<width_display, height_display>::PrintMessage(
		const char* message,
		uint32_t message_length,
		uint8_t row,
		uint8_t column)
	{
		for (uint32_t i = 0; i < message_length; ++i)
		{
			display_screen[row][column + i] = message[i];
		}

		PrintScreen();

		return Middleware::ReturnCode::OK;
	}

	template<uint32_t width_display, uint32_t height_display>
	Middleware::ReturnCode DisplayMock<width_display, height_display>::ClearScreen()
	{
		for (uint32_t row_position = 0; row_position < height_display; ++row_position)
		{
			for (uint32_t column_position = 0; column_position < width_display; ++column_position)
			{
				display_screen[row_position][column_position] = ' ';
			}
		}

		PrintScreen();

		return Middleware::ReturnCode::OK;
	}

	template<uint32_t width_strings_count, uint32_t height_strings_count>
	void DisplayMock<width_strings_count, height_strings_count>::PrintScreen()
	{
		time_t now = time(nullptr);
		tm *ltm = localtime(&now);

		std::cout << "Screen state changed: " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;

		for (uint32_t row_position = 0; row_position < height_strings_count; ++row_position)
		{
			for (uint32_t column_position = 0; column_position < width_strings_count; ++column_position)
			{
				std::cout << display_screen[row_position][column_position];
			}

			std::cout << std::endl;
		}
	}
}

