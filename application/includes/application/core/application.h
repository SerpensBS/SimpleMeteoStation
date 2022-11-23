#ifndef SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_CORE_APPLICATION_H_
#define SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_CORE_APPLICATION_H_

#include "middleware/data/return-code.h"

namespace Application
{
	/**
	 * Приложение. Здесь выполняется основная бизнес-логика программы.
	 */
	class Application
	{
	 private:
		/**
		 * Singleton экземпляр приложения.
		 */
		static Application* instance;

		Application() = default;
		~Application() = default;
	 public:
		[[noreturn]]void Run();

		static Middleware::ReturnCode CreateSingleInstance(Application*& out_application);

		Application(const Application&) = delete;
		Application(const Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(const Application&&) = delete;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_INCLUDES_APPLICATION_CORE_APPLICATION_H_
