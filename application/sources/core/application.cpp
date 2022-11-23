#include "core/application.h"

namespace Application
{
	Application* Application::instance = nullptr;

/**
 * Я не хотел, чтобы данный код можно было вызывать из любой точки программы, поэтому он не должен иметь
 * глобальную точку доступа.
 */
#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
	/**
	 * Запускает приложение.
	 */
	[[noreturn]]void Application::Run()
	{
		while (true)
		{
		}
	}
#pragma clang diagnostic pop

	/**
	 * Создает единственный экземпляр приложения.
	 * @param out_application Экземпляр приложения.
	 * @return
	 */
	Middleware::ReturnCode Application::CreateSingleInstance(Application*& out_application)
	{
		// Контролируем уникальность экземпляра приложения.
		if (nullptr == instance)
		{
			static Application static_instance;
			*&instance = &static_instance;

			out_application = instance;
			return Middleware::ReturnCode::OK;
		}

		return Middleware::ReturnCode::ERROR;
	}
}