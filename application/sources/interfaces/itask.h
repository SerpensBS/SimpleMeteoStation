#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_INTERFACES_ITASK_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_INTERFACES_ITASK_H_

#include "middleware/data/return-code.h"

namespace Application
{
	/**
	 * Интерфейс для задач.
	 */
	class ITask
	{
	 public:
		/**
		 * Выполнить задачу.
		 * @return
		 */
		virtual Middleware::ReturnCode Execute() = 0;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_INTERFACES_ITASK_H_
