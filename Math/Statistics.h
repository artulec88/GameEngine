#ifndef __MATH_STATISTICS_H__
#define __MATH_STATISTICS_H__

#include "Math.h"
#include <map>
#include <vector>
#include <string>
#include "Utility\ILogger.h"

namespace Math { namespace Statistics
{
//enum TimeUnit
//{
//	SECONDS = 0,
//	MILISECONDS = 1,
//	MICROSECONDS = 2,
//	NANOSECONDS = 3
//};

//const char* timeUnitStr[] = 
//{
//	"[s]",
//	"[ms]",
//	"[us]",
//	"[ns]",
//	NULL
//};

enum MATH_API StatsID
{
	SPF = 0
};

template <typename T>
class Stats
{
/* ==================== Constructors and destructors begin ==================== */
public:
	MATH_API Stats();
	MATH_API ~Stats(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	MATH_API void Push(StatsID statsID, T sample);
	MATH_API int Size() const;
	MATH_API int Size(StatsID statsID) const;
	
	MATH_API T CalculateMean(StatsID statsID) const;
	MATH_API T CalculateMedian(StatsID statsID);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
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
	Math::Real CalculateMedian() const;
	
	void StartProfiling(bool isNestedWithinAnotherProfiledMethod);
	void StopProfiling();

	Math::Real GetTotalTime() const { return m_totalTime; }
	Math::Real GetTotalTimeWithoutNestedStats() const;
	int GetInvocationsCount() const { return m_invocationsCount; }
	bool IsProfiling() const { return m_isProfiling; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	/// <summary> Time samples stored along with the information whether the method has been nested within another method in the same class. </summary>
	std::vector<std::pair<bool, Math::Real>> m_timeSamples;
	Math::Real m_totalTime;
	int m_invocationsCount;
	bool m_isProfiling;
	bool m_isNestedWithinAnotherProfiledMethod;
	LARGE_INTEGER m_startTimer;
	LARGE_INTEGER m_frequency; // TODO: Probably unnecessary to store this information in every MethodStats instance
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

	MATH_API void PrintReport(Math::Real totalElapsedTime) const;

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

	int GetTotalNumberOfSamples() const;
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