#include "StdAfx.h"
#include "Statistics.h"
#include "ISort.h"
#include "Utility\ILogger.h"
#include "Utility\Time.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <fstream>
#include <sstream>

/* ==================== Stats begin ==================== */
template <typename T>
Math::Statistics::Stats<T>::Stats(int level /* = 0 */) :
	m_level(level),
	m_child(nullptr)
{
}

template <typename T>
Math::Statistics::Stats<T>::~Stats(void)
{
}

template <typename T>
void Math::Statistics::Stats<T>::Push(StatsID statsID, T sample)
{
	if (Size(statsID) == MAX_SAMPLES_COUNT)
	{
		//CRITICAL_LOG_MATH("Pushing ", sample, " to ", statsID, " statsID.");
		if (m_level == MAX_STATS_LEVEL)
		{
			WARNING_LOG_MATH("Maximum number of samples reached for stats ID: ", statsID);
		}
		else
		{
			T mean = CalculateMean(statsID);
			if (m_child == nullptr)
			{
				m_child = std::make_unique<Stats<T>>(m_level + 1);
			}
			m_child->Push(statsID, mean);
		}
		m_samples[statsID].clear();
	}
	else
	{
		//EMERGENCY_LOG_MATH("Pushing ", sample, " to ", statsID, " statsID.");
		m_samples[statsID].push_back(sample);
	}
}

template <typename T>
int Math::Statistics::Stats<T>::Size() const
{
	int totalSize = 0;
	for (std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.begin(); mapItr != m_samples.end(); ++mapItr)
	{
		totalSize += static_cast<int>(mapItr->second.size());
	}
	return totalSize;
}

template <typename T>
int Math::Statistics::Stats<T>::Size(StatsID statsID) const
{
	std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.find(statsID);
	if (mapItr == m_samples.end())
	{
		return 0;
	}
	return static_cast<int>(mapItr->second.size());
}

template <typename T>
T Math::Statistics::Stats<T>::CalculateSum(StatsID statsID) const
{
	std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.find(statsID);
	if (mapItr == m_samples.end())
	{
		WARNING_LOG_MATH("Sum cannot be calculated for statsID = ", statsID, ". The specific entry in statistics map has not been created.");
		return 0;
	}
	
	T childSum = 0;
	if (m_child != NULL)
	{
		childSum = m_child->CalculateSum(statsID);
	}
	T sum = 0;
	for (std::vector<T>::const_iterator samplesItr = mapItr->second.begin(); samplesItr != mapItr->second.end(); ++samplesItr)
	{
		sum += (*samplesItr);
	}
	const int levelFactor = static_cast<int>(pow(static_cast<Math::Real>(MAX_SAMPLES_COUNT), static_cast<Math::Real>(m_level)));
	return (sum * levelFactor) + childSum;
}

template <typename T>
int Math::Statistics::Stats<T>::CalculateSamplesCount(StatsID statsID) const
{
	std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.find(statsID);
	if (mapItr == m_samples.end())
	{
		DEBUG_LOG_MATH("Samples cannot be counted for statsID", statsID, ". The specific entry in statistics map has not been created.");
		return 0;
	}
	
	int childSamplesCount = 0;
	if (m_child != NULL)
	{
		childSamplesCount = m_child->CalculateSamplesCount(statsID);
	}
	const int levelFactor = static_cast<int>(pow(static_cast<Math::Real>(MAX_SAMPLES_COUNT), static_cast<Math::Real>(m_level)));
	return Size(statsID) * levelFactor + childSamplesCount;
}

template <typename T>
T Math::Statistics::Stats<T>::CalculateMean(StatsID statsID) const
{
	std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.find(statsID);
	if (mapItr == m_samples.end())
	{
		WARNING_LOG_MATH("Mean cannot be calculated for statsID = ", statsID, ". The specific entry in statistics map has not been created.");
		return 0;
	}

	T sum = CalculateSum(statsID);
	int samplesCount = CalculateSamplesCount(statsID);
	CHECK_CONDITION_MATH(samplesCount > 0, Utility::Logging::EMERGENCY, "Samples count (", samplesCount, ") must be positive.");

	DEBUG_LOG_MATH("Sum = ", sum, ", samplesCount = ", samplesCount);

	return sum / samplesCount;
}

template <typename T>
T Math::Statistics::Stats<T>::CalculateMedian(StatsID statsID)
{
	if (m_child != NULL)
	{
		EMERGENCY_LOG_MATH("Median shouldn't be used for the hierarchical stats storage as it is now used. The result will not be correct.");
	}
	if (m_samples[statsID].empty())
	{
		INFO_LOG_MATH("Median cannot be calculated for statsID = ", statsID, ". There are no samples stored.");
		return static_cast<T>(0);
	}

	Math::Sorting::ISort::GetSortingObject(Math::Sorting::SortingAlgorithms::MERGE_SORT)->Sort(&m_samples[statsID][0],
		static_cast<int>(m_samples[statsID].size()), Sorting::Orders::ASCENDING);

	const int index = static_cast<int>(m_samples[statsID].size()) / 2;
	if ((m_samples[statsID].size() % 2) == 0)
	{
		T medianMean = m_samples[statsID][index] + m_samples[statsID][index - 1];
		return static_cast<T>(medianMean / 2.0f);
	}
	else
	{
		return m_samples[statsID][index];
	}
}

template <typename T>
int Math::Statistics::Stats<T>::GetHierachyDepth() const
{
	return (m_child == NULL) ? m_level : m_child->GetHierachyDepth();
}

template class Math::Statistics::Stats<Math::Real>;
template class Math::Statistics::Stats<int>;
/* ==================== Stats end ==================== */


/* ==================== MethodStats begin ==================== */
Math::Statistics::MethodStats::MethodStats(void) :
	m_totalTime(),
#ifdef METHOD_STATS_VARIANT_1
#else
	m_totalTimeNestedProfiling(),
	m_invocationsCountNestedProfiling(0),
#endif
	m_invocationsCount(0),
	m_isProfiling(false),
	m_isNestedWithinAnotherProfiledMethod(false),
	m_timer()
{
	DELOCUST_LOG_MATH("MethodStats constructor");
}

Math::Statistics::MethodStats::~MethodStats(void)
{
}

void Math::Statistics::MethodStats::Push(const Utility::Timing::TimeSpan& elapsedTimeSpan)
{
	m_totalTime += elapsedTimeSpan;

#ifdef METHOD_STATS_VARIANT_1
	m_timeSamples.push_back(std::make_pair<bool, Utility::Timing::TimeSpan>(m_isNestedWithinAnotherProfiledMethod, elapsedTimeSpan));
#else
	if (m_isNestedWithinAnotherProfiledMethod)
	{
		m_totalTimeNestedProfiling += elapsedTimeSpan;
		++m_invocationsCountNestedProfiling;
	}
#endif
	++m_invocationsCount;
}

Utility::Timing::TimeSpan Math::Statistics::MethodStats::CalculateMean() const
{
	//const Math::Real ONE_THOUSAND = 1000.0f;
	//const Math::Real ONE_MILION = 1000000.0f;
#ifdef METHOD_STATS_VARIANT_1
	CHECK_CONDITION_MATH(m_invocationsCount == m_timeSamples.size(), Utility::ERR, "There have been ", m_invocationsCount, " method invocations performed, but ", m_timeSamples.size(), " samples are stored");
	if (m_timeSamples.empty())
	{
		CHECK_CONDITION_MATH(Math::AlmostEqual(m_totalTime, REAL_ZERO), Utility::WARNING, "Although no time samples are stored the total time is not zero (", m_totalTime, ").");
		DEBUG_LOG_MATH("Mean cannot be calculated. No time samples are stored.");
		return REAL_ZERO;
	}
#endif

	//Math::Real totalTime = REAL_ZERO;
	//for (std::vector<Math::Real>::const_iterator timeSamplesItr = m_timeSamples.begin(); timeSamplesItr != m_timeSamples.end(); ++timeSamplesItr)
	//{
	//	totalTime += (*timeSamplesItr);
	//}
	//CHECK_CONDITION_MATH(Math::AlmostEqual(m_totalTime, totalTime), Utility::ERR, "The calculated total time (", totalTime, ") is different than the directly stored value in the class (", m_totalTime, ").");
	//return totalTime / m_timeSamples.size();

	return m_totalTime / m_invocationsCount;
}

#ifdef METHOD_STATS_VARIANT_1
Utility::Timing::TimeSpan Math::Statistics::MethodStats::CalculateMedian() const
{
	if (m_timeSamples.empty())
	{
		INFO_LOG_MATH("Median cannot be calculated. There are no time samples stored.");
		return REAL_ZERO;
	}

	Utility::Timing::TimeSpan* timeSamples = new Utility::Timing::TimeSpan[m_timeSamples.size()]; // TODO: Get rid of all "new" operator calls. Replace it with some better C++11 solution.
	unsigned int i = 0;
	for (std::vector<std::pair<bool, Utility::Timing::TimeSpan>>::const_iterator timeSampleItr = m_timeSamples.begin(); timeSampleItr != m_timeSamples.end(); ++timeSampleItr, ++i)
	{
		timeSamples[i] = timeSampleItr->second;
	}

	Math::Sorting::ISort::GetSortingObject(Math::Sorting::MERGE_SORT)->Sort(&timeSamples[0], m_timeSamples.size(), Sorting::ASCENDING);

	Utility::Timing::TimeSpan result = timeSamples[m_timeSamples.size() / 2];
	if ((m_timeSamples.size() % 2) == 0)
	{
		result += timeSamples[(m_timeSamples.size() / 2) - 1];
		result /= 2.0f;
	}
	SAFE_DELETE_JUST_TABLE(timeSamples);
	return result;
}
#endif

void Math::Statistics::MethodStats::StartProfiling(bool isMeasureTimeEnabled, bool isNestedWithinAnotherProfiledMethod)
{
	m_isNestedWithinAnotherProfiledMethod = isNestedWithinAnotherProfiledMethod;
	m_isProfiling = true;
	CHECK_CONDITION_MATH(!m_timer.IsRunning(), Utility::Logging::ERR, "Cannot start profiling the method. The method's timer is still running.");
	if (isMeasureTimeEnabled)
	{
		m_timer.Start();
	}
}

void Math::Statistics::MethodStats::StopProfiling()
{
	if (m_timer.IsRunning())
	{
		m_timer.Stop();
	}
	Utility::Timing::TimeSpan elapsedTimeSpan = m_timer.GetTimeSpan();
	//DEBUG_LOG_MATH("Stopped profiling the method. ", elapsedTime, " [us] has passed.");
	Push(elapsedTimeSpan);
	m_isProfiling = false;
}

Utility::Timing::TimeSpan Math::Statistics::MethodStats::GetTotalTimeWithoutNestedStats() const
{
#ifdef METHOD_STATS_VARIANT_1
	CHECK_CONDITION_MATH(m_invocationsCount == m_timeSamples.size(), Utility::ERR, "There have been ", m_invocationsCount, " method invocations performed, but ", m_timeSamples.size(), " samples are stored");
	if (m_timeSamples.empty())
	{
		CHECK_CONDITION_MATH(Math::AlmostEqual(m_totalTime, REAL_ZERO), Utility::WARNING, "Although no time samples are stored the total time is not zero (", m_totalTime, ").");
		DEBUG_LOG_MATH("Total time (with no \"nested\" stats) cannot be calculated. No time samples are stored.");
		return REAL_ZERO;
	}

	Math::Real totalTimeWithoutNestedStats = REAL_ZERO;
	for (std::vector<std::pair<bool, Utility::Timing::TimeSpan>>::const_iterator timeSampleItr = m_timeSamples.begin(); timeSampleItr != m_timeSamples.end(); ++timeSampleItr)
	{
		if (!timeSampleItr->first)
		{
			totalTimeWithoutNestedStats += timeSampleItr->second;
		}
	}
	return totalTimeWithoutNestedStats;
#else
	// INFO_LOG_MATH("Total time = ", m_totalTime, ". Total time nested profiling = ", m_totalTimeNestedProfiling);
	return m_totalTime - m_totalTimeNestedProfiling;
#endif
}
/* ==================== MethodStats end ==================== */

/* ==================== ClassStats begin ==================== */
Math::Statistics::ClassStats::ClassStats(const std::string& className) :
	m_className(className),
	m_profilingMethodsCount(0)
{
	DELOCUST_LOG_MATH("ClassStats \"", className, "\" constructor");
}

Math::Statistics::ClassStats::~ClassStats()
{
}

void Math::Statistics::ClassStats::StartProfiling(bool isMeasureTimeEnabled, const std::string& methodName)
{
	CHECK_CONDITION_MATH(!methodName.empty(), Utility::Logging::ERR, "Cannot start profiling the method in class \"", m_className, "\". The method's name is empty.");
	//DEBUG_LOG_MATH("Started profiling the function \"", m_className, "::", methodName, "\". ", m_profilingMethodsCount, " method(-s) within this class is/are currently being profiled.");
	CHECK_CONDITION_RETURN_VOID_MATH(!m_methodsStats[methodName].IsProfiling(), Utility::Logging::ERR, "The method \"", methodName, "\" is already being profiled.");
	m_methodsStats[methodName].StartProfiling(isMeasureTimeEnabled, m_profilingMethodsCount > 0);
	++m_profilingMethodsCount;
}

void Math::Statistics::ClassStats::StopProfiling(const std::string& methodName)
{
	CHECK_CONDITION_MATH(!methodName.empty(), Utility::Logging::ERR, "Cannot stop profiling the method in class \"", m_className, "\". The method's name is empty.");
	//DEBUG_LOG_MATH("Stopped profiling the function \"", methodName, "\"");
	CHECK_CONDITION_RETURN_VOID_ALWAYS_MATH(m_methodsStats[methodName].IsProfiling(), Utility::Logging::ERR, "The method \"", methodName, "\" is not being profiled.");
	m_methodsStats[methodName].StopProfiling();
	--m_profilingMethodsCount;
}

void Math::Statistics::ClassStats::PrintReport(const Utility::Timing::TimeSpan& applicationTimeSpan, std::fstream& appStatsFile) const
{
	std::fstream classStatsFile;
	if (!m_methodsStats.empty())
	{
		std::stringstream ss;
		ss << "..\\Docs\\ClassStats\\" << m_className << ".dat";
		classStatsFile.open(ss.str().c_str(), std::ios::out);
		classStatsFile << "\"Method name\"\t\"Invocations count\"\t\"Invocation count excluding nested calls\"\t\"Total time\"\t\"Total time excluding nested calls\"\t\"Average time\"\n";
	}

	INFO_LOG_MATH("Class: \"", m_className, "\"");
	Utility::Timing::TimeSpan classTotalTimeExcludingNestedCalls;
	Utility::Timing::TimeSpan classTotalTime;
	for (std::map<std::string, MethodStats>::const_iterator methodStatsItr = m_methodsStats.begin(); methodStatsItr != m_methodsStats.end(); ++methodStatsItr)
	{
		//INFO_LOG_MATH("Method \"", methodStatsItr->first, "\": total time without nested stats = ", methodStatsItr->second.GetTotalTimeWithoutNestedStats());
		classTotalTimeExcludingNestedCalls += methodStatsItr->second.GetTotalTimeWithoutNestedStats();
		classTotalTime += methodStatsItr->second.GetTotalTime();
	}
	appStatsFile << m_className << "\t" << std::setprecision(1) << std::fixed << classTotalTime.ToString(Utility::Timing::NANOSECOND) <<
		"\t" << classTotalTimeExcludingNestedCalls.ToString(Utility::Timing::NANOSECOND) << "\n";
	LogTime("\tTotal time: ", classTotalTime);
	LogTime("\tTotal time excluding nested calls: ", classTotalTimeExcludingNestedCalls);
	DEBUG_LOG_MATH("\tApplication usage: ", classTotalTimeExcludingNestedCalls / applicationTimeSpan * 100, "%");

	for (std::map<std::string, MethodStats>::const_iterator methodStatsItr = m_methodsStats.begin(); methodStatsItr != m_methodsStats.end(); ++methodStatsItr)
	{
		DEBUG_LOG_MATH("\tMethod: \"", methodStatsItr->first, "\"");
		DEBUG_LOG_MATH("\t\tInvocations count: ", methodStatsItr->second.GetInvocationsCount());
		
		Utility::Timing::TimeSpan totalTimeExcludingNestedCalls = methodStatsItr->second.GetTotalTimeWithoutNestedStats();
		LogTime("\t\tTotal time: ", totalTimeExcludingNestedCalls);
		Utility::Timing::TimeSpan totalTimeIncludingNestedCalls = methodStatsItr->second.GetTotalTime();
		LogTime("\t\tTotal time including nested calls: ", totalTimeIncludingNestedCalls);

		Utility::Timing::TimeSpan meanTime = methodStatsItr->second.CalculateMean();
		LogTime("\t\tAverage time: ", meanTime);
		
		DEBUG_LOG_MATH("\t\tClass usage: ", totalTimeIncludingNestedCalls / classTotalTimeExcludingNestedCalls * 100, "%");
		DEBUG_LOG_MATH("\t\tApplication usage: ", totalTimeIncludingNestedCalls / applicationTimeSpan * 100, "%");
		
		//INFO_LOG_MATH("\t\tMedian time: ", methodStatsItr->second.CalculateMedian(), " [us]");
		std::string methodNameStr(methodStatsItr->first);
		methodNameStr = methodNameStr.substr(methodNameStr.rfind(":") + 1); // to remove "::" (e.g. display "Render" instead of "Rendering::Renderer::Render")
		std::size_t spacePos = methodNameStr.find(' '); // removing whitespace in the method's name (e.g. "operator =" will be modified to "operator=")
		if (spacePos != std::string::npos)
		{
			methodNameStr = methodNameStr.substr(0, spacePos) + methodNameStr.substr(spacePos + 1);
		}
		methodNameStr.erase(std::remove_if(methodNameStr.begin(), methodNameStr.end(), std::bind( std::isspace<char>, -1, std::locale::classic() )), methodNameStr.end());
		classStatsFile << methodNameStr << "\t" << methodStatsItr->second.GetInvocationsCount() << "\t" << methodStatsItr->second.GetInvocationsCountWithoutNestedCalls() <<
			"\t" << methodStatsItr->second.GetTotalTime().ToString(Utility::Timing::NANOSECOND) << "\t" << methodStatsItr->second.GetTotalTimeWithoutNestedStats().ToString(Utility::Timing::NANOSECOND) <<
			"\t" << meanTime.ToString(Utility::Timing::NANOSECOND) << "\n";
	}
	if (classStatsFile.is_open())
	{
		classStatsFile.close();
	}
}

int Math::Statistics::ClassStats::GetTotalNumberOfSamples() const
{
	int totalNumberOfSamples = 0;
	for (std::map<std::string, MethodStats>::const_iterator methodStatsItr = m_methodsStats.begin(); methodStatsItr != m_methodsStats.end(); ++methodStatsItr)
	{
		totalNumberOfSamples += methodStatsItr->second.GetInvocationsCount();
	}
	return totalNumberOfSamples;
}

void Math::Statistics::ClassStats::LogTime(const std::string& text, const Utility::Timing::TimeSpan& timeSpan) const
{
	if (timeSpan < Utility::Timing::MICROSECOND)
	{
		DEBUG_LOG_MATH(text, timeSpan.ToString(Utility::Timing::NANOSECOND));
	}
	else if (timeSpan < Utility::Timing::MILLISECOND)
	{
		DEBUG_LOG_MATH(text, timeSpan.ToString(Utility::Timing::MICROSECOND));
	}
	else if (timeSpan < Utility::Timing::MINUTE)
	{
		DEBUG_LOG_MATH(text, timeSpan.ToString(Utility::Timing::MILLISECOND));
	}
	else
	{
		DEBUG_LOG_MATH(text, timeSpan.ToString(Utility::Timing::SECOND));
	}
}
/* ==================== ClassStats end ==================== */