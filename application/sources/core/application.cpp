#include "core/application.h"

namespace Application
{
	Application* Application::instance = nullptr;

	/**
	 * Запускает приложение.
	 */
	void Application::Run()
	{
		while (true)
		{
		}
	}

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