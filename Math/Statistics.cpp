#include "StdAfx.h"
#include "Statistics.h"
#include "ISort.h"
#include "Utility\ILogger.h"

using namespace Math::Statistics;

/* ==================== Stats begin ==================== */
template <typename T>
Stats<T>::Stats(void)
{
}

template <typename T>
Stats<T>::~Stats(void)
{
}

template <typename T>
void Stats<T>::Push(StatsID statsID, T sample)
{
	m_samples[statsID].push_back(sample);
}

template <typename T>
int Stats<T>::Size() const
{
	int totalSize = 0;
	for (std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.begin(); mapItr != m_samples.end(); ++mapItr)
	{
		totalSize += mapItr->second.size();
	}
	return totalSize;
}

template <typename T>
int Stats<T>::Size(StatsID statsID) const
{
	std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.find(statsID);
	if (mapItr == m_samples.end())
	{
		return 0;
	}
	return mapItr->second.size();
}

template <typename T>
T Stats<T>::CalculateMean(StatsID statsID) const
{
	std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.find(statsID);
	if (mapItr == m_samples.end())
	{
		LOG(Utility::Debug, LOGPLACE, "Mean cannot be calculated for %d statsID. The specific entry in statistics map has not been created.", statsID);
		return 0;
	}
	if (mapItr->second.empty())
	{
		LOG(Utility::Info, LOGPLACE, "Mean cannot be calculated for %d statsID. There are no samples stored.", statsID);
		return static_cast<T>(0);
	}
	T sum = 0;
	for (std::vector<T>::const_iterator samplesItr = mapItr->second.begin(); samplesItr != mapItr->second.end(); ++samplesItr)
	{
		sum += *samplesItr;
	}
	return sum / mapItr->second.size();
}

template <typename T>
T Stats<T>::CalculateMedian(StatsID statsID)
{
	if (m_samples[statsID].empty())
	{
		LOG(Utility::Info, LOGPLACE, "Median cannot be calculated for %d statsID. There are no samples stored.", statsID);
		return static_cast<T>(0);
	}

	Math::Sorting::ISort::GetSortingObject(Math::Sorting::MERGE_SORT)->Sort(&m_samples[statsID][0], m_samples[statsID].size(), Sorting::ASCENDING);

	if ((m_samples[statsID].size() % 2) == 0)
	{
		T medianMean = m_samples[statsID].at(m_samples[statsID].size() / 2) + m_samples[statsID].at((m_samples[statsID].size() / 2) - 1);
		return static_cast<T>(medianMean / 2.0f);
	}
	else
	{
		return m_samples[statsID].at(m_samples[statsID].size() / 2);
	}
}

template MATH_API class Stats<Math::Real>;
template MATH_API class Stats<int>;
/* ==================== Stats end ==================== */


/* ==================== MethodStats begin ==================== */
MethodStats::MethodStats(void) :
	m_totalTime(REAL_ZERO),
	m_invocationsCount(0),
	m_isProfiling(false),
	m_isNestedWithinAnotherProfiledMethod(false)
{
	QueryPerformanceFrequency(&m_frequency); // get ticks per second;
}

MethodStats::~MethodStats(void)
{
}

void MethodStats::Push(Math::Real timeSample)
{
	m_totalTime += timeSample;
	m_timeSamples.push_back(std::make_pair<bool, Math::Real>(m_isNestedWithinAnotherProfiledMethod, timeSample));
}

Math::Real MethodStats::CalculateMean() const
{
	//const Math::Real ONE_THOUSAND = 1000.0f;
	//const Math::Real ONE_MILION = 1000000.0f;

	CHECK_CONDITION(m_invocationsCount == m_timeSamples.size(), Utility::Error, "There have been %d method invocations performed, but %d samples are stored", m_invocationsCount, m_timeSamples.size());
	if (m_timeSamples.empty())
	{
		CHECK_CONDITION(Math::AlmostEqual(m_totalTime, REAL_ZERO), Utility::Warning, "Although no time samples are stored the total time is not zero (%.4f).", m_totalTime);
		LOG(Utility::Debug, LOGPLACE, "Mean cannot be calculated. No time samples are stored.");
		return REAL_ZERO;
	}

	//Math::Real totalTime = REAL_ZERO;
	//for (std::vector<Math::Real>::const_iterator timeSamplesItr = m_timeSamples.begin(); timeSamplesItr != m_timeSamples.end(); ++timeSamplesItr)
	//{
	//	totalTime += (*timeSamplesItr);
	//}
	//CHECK_CONDITION(Math::AlmostEqual(m_totalTime, totalTime), Utility::Error, "The calculated total time (%.4f) is different than the directly stored value in the class (%.4f).", totalTime, m_totalTime);
	//return totalTime / m_timeSamples.size();

	return m_totalTime / m_invocationsCount;
}

Math::Real MethodStats::CalculateMedian() const
{
	if (m_timeSamples.empty())
	{
		LOG(Utility::Info, LOGPLACE, "Median cannot be calculated. There are no time samples stored.");
		return REAL_ZERO;
	}

	Math::Real* timeSamples = new Math::Real[m_timeSamples.size()];
	unsigned int i = 0;
	for (std::vector<std::pair<bool, Math::Real>>::const_iterator timeSampleItr = m_timeSamples.begin(); timeSampleItr != m_timeSamples.end(); ++timeSampleItr, ++i)
	{
		timeSamples[i] = timeSampleItr->second;
	}

	Math::Sorting::ISort::GetSortingObject(Math::Sorting::MERGE_SORT)->Sort(&timeSamples[0], m_timeSamples.size(), Sorting::ASCENDING);

	Math::Real result = timeSamples[m_timeSamples.size() / 2];
	if ((m_timeSamples.size() % 2) == 0)
	{
		result += timeSamples[(m_timeSamples.size() / 2) - 1];
		result /= 2.0f;
	}
	SAFE_DELETE_JUST_TABLE(timeSamples);
	return result;
}

void MethodStats::StartProfiling(bool isNestedWithinAnotherProfiledMethod)
{
	m_isNestedWithinAnotherProfiledMethod = isNestedWithinAnotherProfiledMethod;
	m_isProfiling = true;
	++m_invocationsCount;
	QueryPerformanceCounter(&m_startTimer);
}

void MethodStats::StopProfiling()
{
	static const Math::Real ONE_MILLION = static_cast<Math::Real>(1000000.0f);
	LARGE_INTEGER endTimer;
	QueryPerformanceCounter(&endTimer);
	Math::Real elapsedTime = static_cast<Math::Real>(ONE_MILLION * (endTimer.QuadPart - m_startTimer.QuadPart)) / m_frequency.QuadPart; // in [us]
	Push(elapsedTime);
	m_isProfiling = false;
}

Math::Real MethodStats::GetTotalTimeWithoutNestedStats() const
{
	CHECK_CONDITION(m_invocationsCount == m_timeSamples.size(), Utility::Error, "There have been %d method invocations performed, but %d samples are stored", m_invocationsCount, m_timeSamples.size());
	if (m_timeSamples.empty())
	{
		CHECK_CONDITION(Math::AlmostEqual(m_totalTime, REAL_ZERO), Utility::Warning, "Although no time samples are stored the total time is not zero (%.4f).", m_totalTime);
		LOG(Utility::Debug, LOGPLACE, "Total time (with no \"nested\" stats) cannot be calculated. No time samples are stored.");
		return REAL_ZERO;
	}

	Math::Real totalTimeWithoutNestedStats = REAL_ZERO;
	for (std::vector<std::pair<bool, Math::Real>>::const_iterator timeSampleItr = m_timeSamples.begin(); timeSampleItr != m_timeSamples.end(); ++timeSampleItr)
	{
		if (!timeSampleItr->first)
		{
			totalTimeWithoutNestedStats += timeSampleItr->second;
		}
	}
	return totalTimeWithoutNestedStats;
}
/* ==================== MethodStats end ==================== */

/* ==================== ClassStats begin ==================== */
ClassStats::ClassStats(const char* className) :
	m_className(className),
	m_profilingMethodsCount(0)
{
}

ClassStats::~ClassStats()
{
	SAFE_DELETE_JUST_TABLE(m_className);
}

void ClassStats::StartProfiling(const char* methodName)
{
	//LOG(Utility::Debug, LOGPLACE, "Started profiling the function \"%s::%s\". %d method(-s) within this class is/are currently being profiled.",
	//	m_className, methodName, m_profilingMethodsCount);
	m_methodsStats[methodName].StartProfiling(m_profilingMethodsCount > 0);
	++m_profilingMethodsCount;
}

void ClassStats::StopProfiling(const char* methodName)
{
	//LOG(Utility::Debug, LOGPLACE, "Stopped profiling the function \"%s\"", methodName);
	m_methodsStats[methodName].StopProfiling();
	--m_profilingMethodsCount;
}

void ClassStats::PrintReport(Math::Real totalElapsedTime /* given in seconds */) const
{
	static const Math::Real ONE_THOUSAND = 1000.0f;
	static const Math::Real ONE_MILION = 1000000.0f;

	LOG(Utility::Info, LOGPLACE, "Class: \"%s\"", m_className);
	Math::Real classTotalTime = REAL_ZERO;
	for (std::map<const char*, MethodStats>::const_iterator methodStatsItr = m_methodsStats.begin(); methodStatsItr != m_methodsStats.end(); ++methodStatsItr)
	{
		classTotalTime += methodStatsItr->second.GetTotalTimeWithoutNestedStats();
	}
	if (classTotalTime > ONE_THOUSAND)
	{
		if (classTotalTime > ONE_MILION)
		{
			LOG(Utility::Info, LOGPLACE, "\tClass total time: %.3f [s]", classTotalTime / ONE_MILION);
		}
		else
		{
			LOG(Utility::Info, LOGPLACE, "\tClass total time: %.3f [ms]", classTotalTime / ONE_THOUSAND);
		}
	}
	else
	{
		LOG(Utility::Info, LOGPLACE, "\tTotal time: %.3f [us]", classTotalTime);
	}
	LOG(Utility::Info, LOGPLACE, "\tApplication usage: %.1f%%", 100.0f * classTotalTime / (ONE_MILION * totalElapsedTime));

	for (std::map<const char*, MethodStats>::const_iterator methodStatsItr = m_methodsStats.begin(); methodStatsItr != m_methodsStats.end(); ++methodStatsItr)
	{
		LOG(Utility::Info, LOGPLACE, "\tMethod: \"%s\"", methodStatsItr->first);
		LOG(Utility::Info, LOGPLACE, "\t\tInvocations count: %d", methodStatsItr->second.GetInvocationsCount());
		
		Math::Real totalTime = methodStatsItr->second.GetTotalTime();
		if (totalTime > ONE_THOUSAND)
		{
			if (totalTime > ONE_MILION)
			{
				LOG(Utility::Info, LOGPLACE, "\t\tTotal time: %.3f [s]", totalTime / ONE_MILION);
			}
			else
			{
				LOG(Utility::Info, LOGPLACE, "\t\tTotal time: %.3f [ms]", totalTime / ONE_THOUSAND);
			}
		}
		else
		{
			LOG(Utility::Info, LOGPLACE, "\t\tTotal time: %.3f [us]", totalTime);
		}

		Math::Real meanTime = methodStatsItr->second.CalculateMean();
		if (meanTime > ONE_THOUSAND)
		{
			if (meanTime > ONE_MILION)
			{
				LOG(Utility::Info, LOGPLACE, "\t\tAverage time: %.3f [s]", meanTime / ONE_MILION);
			}
			else
			{
				LOG(Utility::Info, LOGPLACE, "\t\tAverage time: %.3f [ms]", meanTime / ONE_THOUSAND);
			}
		}
		else
		{
			LOG(Utility::Info, LOGPLACE, "\t\tAverage time: %.3f [us]", meanTime);
		}
		
		LOG(Utility::Info, LOGPLACE, "\t\tClass usage: %.1f%%", 100.0f * totalTime / classTotalTime);
		LOG(Utility::Info, LOGPLACE, "\t\tApplication usage: %.1f%%", 100.0f * totalTime / (ONE_MILION * totalElapsedTime));
		
		//LOG(Utility::Info, LOGPLACE, "\t\tMedian time: %.2f [us]", methodStatsItr->second.CalculateMedian());
	}
}
/* ==================== ClassStats end ==================== */