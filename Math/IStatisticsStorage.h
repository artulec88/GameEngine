#ifndef __MATH_ISTATISTICS_STORAGE_H__
#define __MATH_ISTATISTICS_STORAGE_H__

#include "Math.h"
#include "Statistics.h"

#include "Utility\Time.h"

#include <map>
#include <string>

#define STATS_STORAGE Math::Statistics::IStatisticsStorage::GetStatisticsStorage()

namespace Math {
	namespace Statistics
	{

		class IStatisticsStorage
		{
			typedef std::map<std::string, std::unique_ptr<ClassStats>> ClassNames2ClassStatsMap;
			/* ==================== Static variables and functions begin ==================== */
		public:
			MATH_API static IStatisticsStorage& GetStatisticsStorage();
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		private:
			IStatisticsStorage();
			~IStatisticsStorage(void);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			MATH_API ClassStats& GetClassStats(const std::string& className);

			MATH_API void StartTimer() { m_timer.Start(); }
			MATH_API void StopTimer() { m_timer.Stop(); }

			MATH_API void PrintSimpleReport() const;
			MATH_API void PrintReport() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			ClassNames2ClassStatsMap m_classStatistics;
			Utility::Timing::Timer m_timer;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class IStatisticsStorage */

	} /* end namespace Statistics */

} /* end namespace Math */

#endif // __MATH_ISTATISTICS_STORAGE_H__