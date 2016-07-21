#ifndef __MATH_STATISTICS_H__
#define __MATH_STATISTICS_H__

#include "Math.h"
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "Utility\ILogger.h"
#include "Utility\Time.h"

//#define CALCULATE_STATS
//#ifdef CALCULATE_STATS
#define START_PROFILING_STATIC do { s_classStats.StartProfiling(__FUNCTION__); } while (0)
#define STOP_PROFILING_STATIC do { s_classStats.StopProfiling(__FUNCTION__); } while (0)
#define START_PROFILING do { m_classStats.StartProfiling(__FUNCTION__); } while (0)
#define STOP_PROFILING do { m_classStats.StopProfiling(__FUNCTION__); } while (0)
//#define START_PROFILING_STATIC // just temporary to disable stats storing.
//#define STOP_PROFILING_STATIC // just temporary to disable stats storing.
//#define START_PROFILING // just temporary to disable stats storing.
//#define STOP_PROFILING // just temporary to disable stats storing.
//#else
//#define START_PROFILING_STATIC
//#define STOP_PROFILING_STATIC
//#define START_PROFILING
//#define STOP_PROFILING
//#endif

namespace Math {
	namespace Statistics
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
		private:
			static const int MAX_STATS_LEVEL;
			static const int MAX_SAMPLES_COUNT;
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			MATH_API Stats(int level = 0);
			MATH_API ~Stats(void);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			MATH_API void Push(StatsID statsID, T sample);
			MATH_API int Size() const;
			MATH_API int Size(StatsID statsID) const;
			MATH_API T CalculateSum(StatsID statsID) const;
			MATH_API int CalculateSamplesCount(StatsID statsID) const;
			MATH_API T CalculateMean(StatsID statsID) const;
			MATH_API T CalculateMedian(StatsID statsID);
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
			MATH_API ~MethodStats(void);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			void Push(Math::Real sample);

			Math::Real CalculateMean() const;
#ifdef METHOD_STATS_VARIANT_1
			Math::Real CalculateMedian() const;
#endif

			void StartProfiling(bool isNestedWithinAnotherProfiledMethod);
			void StopProfiling();

			Math::Real GetTotalTime() const { return m_totalTime; }
			Math::Real GetTotalTimeWithoutNestedStats() const;
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
			std::vector<std::pair<bool, Math::Real>> m_timeSamples;
#else
			Math::Real m_totalTimeNestedProfiling;
			int m_invocationsCountNestedProfiling;
#endif
			Math::Real m_totalTime;
			int m_invocationsCount;
			bool m_isProfiling;
			bool m_isNestedWithinAnotherProfiledMethod;
			Utility::Timing::Timer m_timer;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class MethodStats */

		class ClassStats
		{
			/* ==================== Constructors and destructors begin ==================== */
		public:
			MATH_API ClassStats(const char* className);
			MATH_API ~ClassStats(void);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			//MATH_API void Push(const std::string& statsID, Math::Real sample);
			//MATH_API int Size() const;
			//MATH_API int Size(const std::string& statsID) const;

			//MATH_API Math::Real CalculateMean(const std::string& statsID) const;
			//MATH_API Math::Real CalculateMedian(const std::string& statsID);

			MATH_API void PrintReport(const Utility::Timing::TimeSpan& timeSpan, std::fstream& appStatsFile) const;

			MATH_API void StartProfiling(const char* methodName);
			MATH_API void StopProfiling(const char* methodName);

			MATH_API bool IsProfiling(const char* methodName) const
			{
				std::map<const char*, MethodStats>::const_iterator methodStatsIterator = m_methodsStats.find(methodName);
				if (methodStatsIterator == m_methodsStats.end())
				{
					return false;
				}
				return methodStatsIterator->second.IsProfiling();
			}

			MATH_API bool IsEmpty() const
			{
				return m_methodsStats.empty();
			}

			MATH_API int GetTotalNumberOfSamples() const;
		private:
			void LogTime(const char* text, Math::Real time) const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			const char* m_className;

			/// <summary> The number of methods which are currently being profiled within the class </summary>
			/// When equal to zero we know that no class method is currently running.
			/// When bigger than zero we know that there is some nesting involved, i.e. class A is running its method A1
			/// and A::A1 started the A::A2 method. If both A1 and A2 are enabled for profiling then we should only store the A::A1
			/// running time in the class A total time calculation.
			unsigned int m_profilingMethodsCount;
			std::map<const char*, MethodStats> m_methodsStats;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ClassStats */

	} /* end namespace Statistics */

} /* end namespace Math */

#endif // __MATH_STATISTICS_H__