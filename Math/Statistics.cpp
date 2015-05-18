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
	m_timeSamples(),
	m_totalTime(REAL_ZERO),
	m_invocationsCount(0),
	m_isProfiling(false)
{
	QueryPerformanceFrequency(&m_frequency); // get ticks per second;
}

MethodStats::~MethodStats(void)
{
}

void MethodStats::Push(Math::Real timeSample)
{
	m_totalTime += timeSample;
	m_timeSamples.push_back(timeSample);
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

Math::Real MethodStats::CalculateMedian()
{
	if (m_timeSamples.empty())
	{
		LOG(Utility::Info, LOGPLACE, "Median cannot be calculated. There are no time samples stored.");
		return REAL_ZERO;
	}

	Math::Sorting::ISort::GetSortingObject(Math::Sorting::MERGE_SORT)->Sort(&m_timeSamples[0], m_timeSamples.size(), Sorting::ASCENDING);

	if ((m_timeSamples.size() % 2) == 0)
	{
		Math::Real medianMean = m_timeSamples[m_timeSamples.size() / 2] + m_timeSamples[(m_timeSamples.size() / 2) - 1];
		return medianMean / 2.0f;
	}
	else
	{
		return m_timeSamples[m_timeSamples.size() / 2];
	}
}

void MethodStats::StartProfiling()
{
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
/* ==================== MethodStats end ==================== */

/* ==================== ClassStats begin ==================== */
ClassStats::ClassStats(const char* className) :
	m_className(className)
{
}

ClassStats::~ClassStats()
{
	SAFE_DELETE_JUST_TABLE(m_className);
}

void ClassStats::StartProfiling(const char* methodName)
{
	//LOG(Utility::Debug, LOGPLACE, "Started profiling the function \"%s\"", methodName);
	m_methodsStats[methodName].StartProfiling();
}

void ClassStats::StopProfiling(const char* methodName)
{
	//LOG(Utility::Debug, LOGPLACE, "Stopped profiling the function \"%s\"", methodName);
	m_methodsStats[methodName].StopProfiling();
}

void ClassStats::PrintReport() const
{
	static const Math::Real ONE_THOUSAND = 1000.0f;
	static const Math::Real ONE_MILION = 1000000.0f;

	LOG(Utility::Info, LOGPLACE, "Class: \"%s\"", m_className);
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
		
		
		//LOG(Utility::Info, LOGPLACE, "\t\tMedian time: %.2f [us]", methodStatsItr->second.CalculateMedian());
	}
}
/* ==================== ClassStats end ==================== */