#include "StdAfx.h"
#include "Statistics.h"
#include "ISort.h"
#include "Utility\ILogger.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <fstream>

using namespace Math::Statistics;

/* ==================== Stats begin ==================== */
template <typename T>
Stats<T>::Stats(int level /* = 0 */) :
	m_level(level),
	m_child(NULL)
{
}

template <typename T>
Stats<T>::~Stats(void)
{
	SAFE_DELETE(m_child);
}

template <typename T>
void Stats<T>::Push(StatsID statsID, T sample)
{
	if (Size(statsID) == MAX_SAMPLES_COUNT)
	{
		if (m_level == MAX_STATS_LEVEL)
		{
			LOG(Utility::Warning, LOGPLACE, "Maximum number of samples reached for stats ID: %d", statsID);
		}
		else
		{
			T mean = CalculateMean(statsID);
			if (m_child == NULL)
			{
				m_child = new Stats<T>(m_level + 1);
			}
			m_child->Push(statsID, mean);
		}
		m_samples[statsID].clear();
	}
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
T Stats<T>::CalculateSum(StatsID statsID) const
{
	std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.find(statsID);
	if (mapItr == m_samples.end())
	{
		LOG(Utility::Warning, LOGPLACE, "Sum cannot be calculated for %d statsID. The specific entry in statistics map has not been created.", statsID);
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
int Stats<T>::CalculateSamplesCount(StatsID statsID) const
{
	std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.find(statsID);
	if (mapItr == m_samples.end())
	{
		LOG(Utility::Debug, LOGPLACE, "Samples cannot be counted for %d statsID. The specific entry in statistics map has not been created.", statsID);
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
T Stats<T>::CalculateMean(StatsID statsID) const
{
	std::map<StatsID, std::vector<T>>::const_iterator mapItr = m_samples.find(statsID);
	if (mapItr == m_samples.end())
	{
		LOG(Utility::Warning, LOGPLACE, "Mean cannot be calculated for %d statsID. The specific entry in statistics map has not been created.", statsID);
		return 0;
	}

	T sum = CalculateSum(statsID);
	int samplesCount = CalculateSamplesCount(statsID);
	CHECK_CONDITION(samplesCount > 0);

	LOG(Utility::Debug, LOGPLACE, "Sum = %.3f, samplesCount = %d", sum, samplesCount);

	return sum / samplesCount;
}

template <typename T>
T Stats<T>::CalculateMedian(StatsID statsID)
{
	if (m_child != NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Median shouldn't be used for the hierarchical stats storage as it is now used. The result will not be correct.");
	}
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

template <typename T>
int Stats<T>::GetHierachyDepth() const
{
	return (m_child == NULL) ? m_level : m_child->GetHierachyDepth();
}

template MATH_API class Stats<Math::Real>;
template MATH_API class Stats<int>;
/* ==================== Stats end ==================== */


/* ==================== MethodStats begin ==================== */
MethodStats::MethodStats(void) :
	m_totalTime(REAL_ZERO),
#ifdef METHOD_STATS_VARIANT_1
#else
	m_totalTimeNestedProfiling(REAL_ZERO),
	m_invocationsCountNestedProfiling(0),
#endif
	m_invocationsCount(0),
	m_isProfiling(false),
	m_isNestedWithinAnotherProfiledMethod(false)
{
	QueryPerformanceFrequency(&m_frequency); // get ticks per second;
	LOG(Utility::Debug, LOGPLACE, "MethodStats constructor");
}

MethodStats::~MethodStats(void)
{
}

void MethodStats::Push(Math::Real timeSample)
{
	m_totalTime += timeSample;

#ifdef METHOD_STATS_VARIANT_1
	m_timeSamples.push_back(std::make_pair<bool, Math::Real>(m_isNestedWithinAnotherProfiledMethod, timeSample));
#else
	if (m_isNestedWithinAnotherProfiledMethod)
	{
		m_totalTimeNestedProfiling += timeSample;
		++m_invocationsCountNestedProfiling;
	}
#endif
	++m_invocationsCount;
}

Math::Real MethodStats::CalculateMean() const
{
	//const Math::Real ONE_THOUSAND = 1000.0f;
	//const Math::Real ONE_MILION = 1000000.0f;
#ifdef METHOD_STATS_VARIANT_1
	CHECK_CONDITION(m_invocationsCount == m_timeSamples.size(), Utility::Error, "There have been %d method invocations performed, but %d samples are stored", m_invocationsCount, m_timeSamples.size());
	if (m_timeSamples.empty())
	{
		CHECK_CONDITION(Math::AlmostEqual(m_totalTime, REAL_ZERO), Utility::Warning, "Although no time samples are stored the total time is not zero (%.4f).", m_totalTime);
		LOG(Utility::Debug, LOGPLACE, "Mean cannot be calculated. No time samples are stored.");
		return REAL_ZERO;
	}
#endif

	//Math::Real totalTime = REAL_ZERO;
	//for (std::vector<Math::Real>::const_iterator timeSamplesItr = m_timeSamples.begin(); timeSamplesItr != m_timeSamples.end(); ++timeSamplesItr)
	//{
	//	totalTime += (*timeSamplesItr);
	//}
	//CHECK_CONDITION(Math::AlmostEqual(m_totalTime, totalTime), Utility::Error, "The calculated total time (%.4f) is different than the directly stored value in the class (%.4f).", totalTime, m_totalTime);
	//return totalTime / m_timeSamples.size();

	return m_totalTime / m_invocationsCount;
}

#ifdef METHOD_STATS_VARIANT_1
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
#endif

void MethodStats::StartProfiling(bool isNestedWithinAnotherProfiledMethod)
{
	m_isNestedWithinAnotherProfiledMethod = isNestedWithinAnotherProfiledMethod;
	m_isProfiling = true;
	QueryPerformanceCounter(&m_startTimer);
}

void MethodStats::StopProfiling()
{
	static const Math::Real NUMBER_OF_MICROSECONDS_IN_SECOND = static_cast<Math::Real>(1000000.0f);
	LARGE_INTEGER endTimer;
	QueryPerformanceCounter(&endTimer);
	Math::Real elapsedTime = static_cast<Math::Real>(NUMBER_OF_MICROSECONDS_IN_SECOND * (endTimer.QuadPart - m_startTimer.QuadPart)) / m_frequency.QuadPart; // in [us]
	//LOG(Utility::Debug, LOGPLACE, "Stopped profiling the method. %.3f [us] has passed.", elapsedTime);
	Push(elapsedTime);
	m_isProfiling = false;
}

Math::Real MethodStats::GetTotalTimeWithoutNestedStats() const
{
#ifdef METHOD_STATS_VARIANT_1
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
#else
	//LOG(Utility::Info, LOGPLACE, "Total time = %f. Total time nested profiling = %f", m_totalTime, m_totalTimeNestedProfiling);
	return m_totalTime - m_totalTimeNestedProfiling;
#endif
}
/* ==================== MethodStats end ==================== */

/* ==================== ClassStats begin ==================== */
ClassStats::ClassStats(const char* className) :
	m_className(className),
	m_profilingMethodsCount(0)
{
	LOG(Utility::Debug, LOGPLACE, "ClassStats \"%s\" constructor", className);
}

ClassStats::~ClassStats()
{
	SAFE_DELETE_JUST_TABLE(m_className);
}

void ClassStats::StartProfiling(const char* methodName)
{
	if (methodName == NULL)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot stop profiling the method in class \"%s\". The method's name is NULL.", m_className);
		return;
	}
	//LOG(Utility::Debug, LOGPLACE, "Started profiling the function \"%s::%s\". %d method(-s) within this class is/are currently being profiled.", m_className, methodName, m_profilingMethodsCount);
	m_methodsStats[methodName].StartProfiling(m_profilingMethodsCount > 0);
	++m_profilingMethodsCount;
}

void ClassStats::StopProfiling(const char* methodName)
{
	if (methodName == NULL)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot stop profiling the method in class \"%s\". The method's name is NULL.", m_className);
		return;
	}
	//LOG(Utility::Debug, LOGPLACE, "Stopped profiling the function \"%s\"", methodName);
	m_methodsStats[methodName].StopProfiling();
	--m_profilingMethodsCount;
}

void ClassStats::PrintReport(Math::Real totalElapsedTime /* given in seconds */, std::fstream& appStatsFile) const
{
	static const Math::Real ONE_MILION = 1000000.0f;
	
	std::fstream classStatsFile;
	if (!m_methodsStats.empty())
	{
		std::stringstream ss;
		ss << "..\\Docs\\ClassStats\\" << m_className << ".dat";
		classStatsFile.open(ss.str().c_str(), std::ios::out);
		classStatsFile << "\"Method name\"\t\"Invocations count\"\t\"Invocation count excluding nested calls\"\t\"Total time\"\t\"Total time excluding nested calls\"\t\"Average time\"\n";
	}

	LOG(Utility::Info, LOGPLACE, "Class: \"%s\"", m_className);
	Math::Real classTotalTimeExcludingNestedCalls = REAL_ZERO;
	Math::Real classTotalTime = REAL_ZERO;
	for (std::map<const char*, MethodStats>::const_iterator methodStatsItr = m_methodsStats.begin(); methodStatsItr != m_methodsStats.end(); ++methodStatsItr)
	{
		//LOG(Utility::Info, LOGPLACE, "classTotalTime = %f. Method's total time without nested stats %f", classTotalTime, methodStatsItr->second.GetTotalTimeWithoutNestedStats());
		classTotalTimeExcludingNestedCalls += methodStatsItr->second.GetTotalTimeWithoutNestedStats();
		classTotalTime += methodStatsItr->second.GetTotalTime();
	}
	appStatsFile << m_className << "\t" << std::setprecision(1) << std::fixed << classTotalTime << "\t" << classTotalTimeExcludingNestedCalls << "\n";
	LogTime(classTotalTime, "\tTotal time: %.2f %s");
	LogTime(classTotalTimeExcludingNestedCalls, "\tTotal time excluding nested calls: %.2f %s");
	LOG(Utility::Info, LOGPLACE, "\tApplication usage: %.1f%%", 100.0f * classTotalTimeExcludingNestedCalls / (ONE_MILION * totalElapsedTime));

	for (std::map<const char*, MethodStats>::const_iterator methodStatsItr = m_methodsStats.begin(); methodStatsItr != m_methodsStats.end(); ++methodStatsItr)
	{
		LOG(Utility::Info, LOGPLACE, "\tMethod: \"%s\"", methodStatsItr->first);
		LOG(Utility::Info, LOGPLACE, "\t\tInvocations count: %d", methodStatsItr->second.GetInvocationsCount());
		
		Math::Real totalTimeExcludingNestedCalls = methodStatsItr->second.GetTotalTimeWithoutNestedStats();
		LogTime(totalTimeExcludingNestedCalls, "\t\tTotal time: %.2f %s");
		Math::Real totalTimeIncludingNestedCalls = methodStatsItr->second.GetTotalTime();
		LogTime(totalTimeIncludingNestedCalls, "\t\tTotal time including nested calls: %.2f %s");

		Math::Real meanTime = methodStatsItr->second.CalculateMean();
		LogTime(meanTime, "\t\tAverage time: %.2f %s");
		
		LOG(Utility::Info, LOGPLACE, "\t\tClass usage: %.1f%%", 100.0f * totalTimeIncludingNestedCalls / classTotalTimeExcludingNestedCalls);
		LOG(Utility::Info, LOGPLACE, "\t\tApplication usage: %.1f%%", 100.0f * totalTimeIncludingNestedCalls / (ONE_MILION * totalElapsedTime));
		
		//LOG(Utility::Info, LOGPLACE, "\t\tMedian time: %.2f [us]", methodStatsItr->second.CalculateMedian());
		std::string methodNameStr(methodStatsItr->first);
		methodNameStr = methodNameStr.substr(methodNameStr.rfind(":") + 1); // to remove "::" (e.g. display "Render" instead of "Rendering::Renderer::Render")
		int spacePos = methodNameStr.find(' '); // removing whitespace in the method's name (e.g. "operator =" will be modified to "operator=")
		if (spacePos != std::string::npos)
		{
			methodNameStr = methodNameStr.substr(0, spacePos) + methodNameStr.substr(spacePos + 1);
		}
		methodNameStr.erase(std::remove_if(methodNameStr.begin(), methodNameStr.end(), std::bind( std::isspace<char>, -1, std::locale::classic() )), methodNameStr.end());
		classStatsFile << methodNameStr << "\t" << methodStatsItr->second.GetInvocationsCount() << "\t" <<
			methodStatsItr->second.GetInvocationsCountWithoutNestedCalls() << "\t" << methodStatsItr->second.GetTotalTime() <<
			"\t" << methodStatsItr->second.GetTotalTimeWithoutNestedStats() << "\t" << meanTime << "\n";
	}
	if (classStatsFile.is_open())
	{
		classStatsFile.close();
	}
}

int ClassStats::GetTotalNumberOfSamples() const
{
	int totalNumberOfSamples = 0;
	for (std::map<const char*, MethodStats>::const_iterator methodStatsItr = m_methodsStats.begin(); methodStatsItr != m_methodsStats.end(); ++methodStatsItr)
	{
		totalNumberOfSamples += methodStatsItr->second.GetInvocationsCount();
	}
	return totalNumberOfSamples;
}

void ClassStats::LogTime(Math::Real time, const char* logTimeTextFormat) const
{
	static const Math::Real ONE_THOUSAND = 1000.0f;
	static const Math::Real ONE_MILION = 1000000.0f;
	if (time > ONE_THOUSAND)
	{
		if (time > ONE_MILION)
		{
			LOG(Utility::Info, LOGPLACE, logTimeTextFormat, time / ONE_MILION, "[s]");
		}
		else
		{
			LOG(Utility::Info, LOGPLACE, logTimeTextFormat, time / ONE_THOUSAND, "[ms]");
		}
	}
	else
	{
		LOG(Utility::Info, LOGPLACE, logTimeTextFormat, time, "[us]");
	}
}
/* ==================== ClassStats end ==================== */