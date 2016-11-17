#ifndef __MATH_STATISTICS_STORAGE_H__
#define __MATH_STATISTICS_STORAGE_H__

#include "Math.h"
#include "Statistics.h"

#include "Utility\Time.h"

#include <map>
#include <string>

#define STATS_STORAGE Math::Statistics::StatisticsStorage::GetStatisticsStorage()

namespace Math {
	namespace Statistics
	{

		class StatisticsStorage
		{
			using ClassNames2ClassStatsMap = std::map<std::string, std::unique_ptr<ClassStats>>;
			/* ==================== Static variables and functions begin ==================== */
		private:
			static StatisticsStorage statsStorage;
		public:
			MATH_API static constexpr StatisticsStorage& GetStatisticsStorage()
			{
				return statsStorage;
			}
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public: // TODO: It would be more appropriate to make all constructors private and prevent any more instances of that class to be created.
			StatisticsStorage();
			~StatisticsStorage(void);
			StatisticsStorage(const StatisticsStorage& statsStorage) = delete;
			StatisticsStorage(StatisticsStorage&& statsStorage) = delete;
			StatisticsStorage& operator=(const StatisticsStorage& statsStorage) = delete;
			StatisticsStorage& operator=(StatisticsStorage&& statsStorage) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			MATH_API ClassStats& GetClassStats(const std::string& className);

			MATH_API inline void StartTimer() { m_timer.Start(); }
			MATH_API inline void StopTimer() { m_timer.Stop(); }

			MATH_API void PrintSimpleReport() const;
			MATH_API void PrintReport() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			ClassNames2ClassStatsMap m_classStatistics;
			Utility::Timing::Timer m_timer;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class StatisticsStorage */

	} /* end namespace Statistics */

} /* end namespace Math */

#endif // __MATH_STATISTICS_STORAGE_H__