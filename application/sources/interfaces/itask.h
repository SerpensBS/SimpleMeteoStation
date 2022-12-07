#ifndef SIMPLEMETEOSTATION_APPLICATION_SOURCES_INTERFACES_ITASK_H_
#define SIMPLEMETEOSTATION_APPLICATION_SOURCES_INTERFACES_ITASK_H_

#include "middleware/enums/return-codes.h"

namespace Application
{
	/**
	 * Интерфейс для задач.
	 */
	class ITask
	{
	 public:
		virtual ~ITask() = default;

		/**
		 * Выполнить задачу.
		 * @return
		 */
		virtual Middleware::ReturnCode Execute() = 0;
	};
}

#endif //SIMPLEMETEOSTATION_APPLICATION_SOURCES_INTERFACES_ITASK_H_
