#ifndef __MATH_STATISTICS_STORAGE_H__
#define __MATH_STATISTICS_STORAGE_H__

#include "Math.h"
#include "Statistics.h"

#include "Utility/Time.h"

#include <map>
#include <string>

#define STATS_STORAGE math::statistics::StatisticsStorage::GetStatisticsStorage()

namespace math {
	namespace statistics
	{

		class StatisticsStorage
		{
			using ClassNames2ClassStatsMap = std::map<std::string, std::unique_ptr<ClassStats>>;
			/* ==================== Static variables and functions begin ==================== */
			static StatisticsStorage s_statsStorage;
		public:
			MATH_API static constexpr StatisticsStorage& GetStatisticsStorage()
			{
				return s_statsStorage;
			}
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		// TODO: It would be more appropriate to make all constructors private and prevent any more instances of that class to be created.
			/// <summary> Statistics storage constructor. </summary>
			StatisticsStorage();

			/// <summary> Statistics storage destructor. </summary>
			~StatisticsStorage();

			/// <summary> Statistics storage copy constructor. </summary>
			/// <param name="statsStorage"> The statistics storage to copy construct from. </param>
			StatisticsStorage(const StatisticsStorage& statsStorage) = delete;

			/// <summary> Statistics storage move constructor. </summary>
			/// <param name="statsStorage"> The statistics storage to move construct from. </param>
			StatisticsStorage(StatisticsStorage&& statsStorage) = delete;

			/// <summary> Statistics storage copy assignment operator. </summary>
			/// <param name="statsStorage"> The statistics storage to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned statistics storage. </returns>
			StatisticsStorage& operator=(const StatisticsStorage& statsStorage) = delete;

			/// <summary> Statistics storage move assignment operator. </summary>
			/// <param name="statsStorage"> The statistics storage to move assign from. </param>
			/// <returns> The reference to the newly move-assigned statistics storage. </returns>
			StatisticsStorage& operator=(StatisticsStorage&& statsStorage) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			MATH_API ClassStats& GetClassStats(const std::string& className);

			MATH_API void StartTimer() { m_timer.Start(); }
			MATH_API void StopTimer() { m_timer.Stop(); }

			MATH_API void PrintSimpleReport() const;
			MATH_API void PrintReport() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			ClassNames2ClassStatsMap m_classStatistics;
			utility::timing::Timer m_timer;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class StatisticsStorage */

	} /* end namespace statistics */

} /* end namespace math */

#endif // __MATH_STATISTICS_STORAGE_H__