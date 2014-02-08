#pragma once

#include "Utility.h"
#include "Log.h"
//#include <assert.h>

namespace Utility
{
	template <typename T> UTILITY_API class Singleton
	{
		static T* m_singleton;

	public: // constructors and destructors
		Singleton(void)
		{
			stdlog(Info, LOGPLACE, "Singleton construction started");
			assert(m_singleton == NULL);
			if (m_singleton != NULL)
			{
				stdlog(Warning, LOGPLACE, "Singleton already initialized");
			}
			/**
			 * Figure out the relative address of the derived instance and stores the result in the singleton pointer.
			 * This is because "this" from MyClass (which derives from Singleton<MyClass>) might be different from the Singleton "this".
			 * The solution is to take a nonexistent object sitting at address 0x1 in memory, cast it to both types, and see the difference.
			 * This difference will effectively be the distance between Singleton<MyClass> and its derived type MyClass, which it can use to calculate the singleton pointer.
			 */
			//int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
			//stdlog(Notice, LOGPLACE, "Intializing Singleton with offset %d", offset);
			//m_singleton = (T*)((int)this + offset);
			m_singleton = static_cast<T*>(this);
			stdlog(Info, LOGPLACE, "Singleton construction finished");
		}

		~Singleton(void)
		{
			stdlog(Info, LOGPLACE, "Singleton destruction started");
			assert(m_singleton != NULL);
			if (m_singleton == NULL)
			{
				stdlog(Warning, LOGPLACE, "Singleton is being destructed but has not been intialized");
			}
			//delete m_singleton; // TODO: Check if this is necessary
			m_singleton = NULL;
			stdlog(Info, LOGPLACE, "Singleton destruction finished");
		}

	public:
		static T& GetSingleton(void)
		{
			assert(m_singleton);
			if (m_singleton == NULL)
				stdlog(Error, LOGPLACE, "Singleton not initialized");

			return (*m_singleton);
		}
		static T* GetSingletonPtr(void)
		{
			return m_singleton;
		}
	}; /* end class Singleton */
} /* end namespace Utility */

template <typename T> T* Utility::Singleton<T>::m_singleton = NULL;