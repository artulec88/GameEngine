#ifndef __MATH_STATISTICS_H__
#define __MATH_STATISTICS_H__

#include "Math.h"
#include <map>
#include <vector>
#include <string>
#include "Utility/ILogger.h"
#include "Utility/Time.h"

//#define PROFILING_ENABLED

#ifdef PROFILING_ENABLED
#define START_PROFILING_STATIC(moduleName, measureTime, param) do { s_classStats.StartProfiling(measureTime, __FUNCTION__##param); } while (0)
#define STOP_PROFILING_STATIC(moduleName, param) do { s_classStats.StopProfiling(__FUNCTION__##param); } while (0)
#define START_PROFILING(moduleName, measureTime, param) do { m_classStats.StartProfiling(measureTime, __FUNCTION__##param); } while (0)
#define STOP_PROFILING(moduleName, param) do { m_classStats.StopProfiling(__FUNCTION__##param); } while (0)
#else
#define START_PROFILING_STATIC(moduleName, measureTime, param)
#define STOP_PROFILING_STATIC(moduleName, param)
#define START_PROFILING(moduleName, measureTime, param)
#define STOP_PROFILING(moduleName, param)
#endif

namespace math {
	namespace statistics
	{
		extern const int STATS_SAMPLES_FIRST_LEVEL;
		extern const int STATS_SAMPLES_SECOND_LEVEL;
		extern const int STATS_SAMPLES_THIRD_LEVEL;

		enum StatsID
		{
			SPF = 0
		};

		template <typename T>
		class Stats
		{
			/* ==================== Static variables and functions begin ==================== */
			static constexpr int MAX_STATS_LEVEL = 3;
			static constexpr int MAX_SAMPLES_COUNT = 1000;
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Stats constructor. </summary>
			/// <param name="level"> The stats hierarchy level. </param>
			MATH_API explicit Stats(const int level = 0);

			/// <summary> Stats destructor. </summary>
			MATH_API ~Stats();

			/// <summary> Stats copy constructor. </summary>
			/// <param name="stats"> The stats to copy construct from. </param>
			MATH_API Stats(const Stats& stats) = delete;

			/// <summary> Stats move constructor. </summary>
			/// <param name="stats"> The stats to move construct from. </param>
			MATH_API Stats(Stats&& stats) = delete;

			/// <summary> Stats copy assignment operator. </summary>
			/// <param name="stats"> The stats to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned stats. </returns>
			MATH_API Stats& operator=(const Stats& stats) = delete;

			/// <summary> Stats move assignment operator. </summary>
			/// <param name="stats"> The stats to move assign from. </param>
			/// <returns> The reference to the newly move-assigned stats. </returns>
			MATH_API Stats& operator=(Stats&& stats) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			MATH_API void Push(StatsID statsId, T sample);
			MATH_API int Size() const;
			MATH_API int Size(StatsID statsId) const;
			MATH_API T CalculateSum(StatsID statsId) const;
			MATH_API int CalculateSamplesCount(StatsID statsId) const;
			MATH_API T CalculateMean(StatsID statsId) const;
			MATH_API T CalculateMedian(StatsID statsId);
			MATH_API int GetHierachyDepth() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			const int m_level;
			std::unique_ptr<Stats<T>> m_child;
			std::map<StatsID, std::vector<T>> m_samples;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Stats */

		class MethodStats
		{
			/* ==================== Constructors and destructors begin ==================== */
		public:
			MATH_API MethodStats();
			MATH_API virtual ~MethodStats();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Push(const utility::timing::TimeSpan& elapsedTimeSpan);

			utility::timing::TimeSpan CalculateMean() const;
#ifdef METHOD_STATS_VARIANT_1
			Utility::timing::TimeSpan CalculateMedian() const;
#endif

			void StartProfiling(bool isMeasureTimeEnabled, bool isNestedWithinAnotherProfiledMethod);
			void StopProfiling();

			const utility::timing::TimeSpan& GetTotalTime() const { return m_totalTime; }
			utility::timing::TimeSpan GetTotalTimeWithoutNestedStats() const;
			int GetInvocationsCount() const { return m_invocationsCount; }
			int GetInvocationsCountWithoutNestedCalls() const { return m_invocationsCount - m_invocationsCountNestedProfiling; }
			bool IsProfiling() const { return m_isProfiling; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
#ifdef METHOD_STATS_VARIANT_1
			/// <summary>
			/// Time samples stored along with the information whether the method has been nested within another method in the same class.
			/// </summary>
			std::vector<std::pair<bool, Utility::timing::TimeSpan>> m_timeSamples;
#else
			utility::timing::TimeSpan m_totalTimeNestedProfiling;
			int m_invocationsCountNestedProfiling;
#endif
			utility::timing::TimeSpan m_totalTime;
			int m_invocationsCount;
			bool m_isProfiling;
			bool m_isNestedWithinAnotherProfiledMethod;
			utility::timing::Timer m_timer;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class MethodStats */

		class ClassStats
		{
			/* ==================== Constructors and destructors begin ==================== */
		public:
			MATH_API explicit ClassStats(const std::string& className);
			MATH_API ~ClassStats();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			//MATH_API void Push(const std::string& statsID, Real sample);
			//MATH_API int Size() const;
			//MATH_API int Size(const std::string& statsID) const;

			//MATH_API Real CalculateMean(const std::string& statsID) const;
			//MATH_API Real CalculateMedian(const std::string& statsID);

			MATH_API void PrintReport(const utility::timing::TimeSpan& applicationTimeSpan, std::fstream& appStatsFile) const;

			MATH_API void StartProfiling(bool isMeasureTimeEnabled, const std::string& methodName);
			MATH_API void StopProfiling(const std::string& methodName);

			MATH_API bool IsProfiling(const std::string& methodName) const
			{
				const auto methodStatsIterator = m_methodsStats.find(methodName);
				return methodStatsIterator == m_methodsStats.end() ? false : methodStatsIterator->second.IsProfiling();
			}

			MATH_API bool IsEmpty() const
			{
				return m_methodsStats.empty();
			}

			MATH_API int GetTotalNumberOfSamples() const;
		private:
			void LogTime(const std::string& text, const utility::timing::TimeSpan& timeSpan) const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			const std::string m_className;

			/// <summary> The number of methods which are currently being profiled within the class </summary>
			/// When equal to zero we know that no class method is currently running.
			/// When bigger than zero we know that there is some nesting involved, i.e. class A is running its method A1
			/// and A::A1 started the A::A2 method. If both A1 and A2 are enabled for profiling then we should only store the A::A1
			/// running time in the class A total time calculation.
			unsigned int m_profilingMethodsCount;
			std::map<std::string, MethodStats> m_methodsStats;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ClassStats */

	} /* end namespace statistics */

} /* end namespace math */

#endif // __MATH_STATISTICS_H__