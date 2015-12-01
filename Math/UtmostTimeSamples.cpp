#include "stdafx.h"
#include "UtmostTimeSamples.h"
#include <sstream>

using namespace Math::Statistics;

UtmostTimeSamples::UtmostTimeSamples()
{
	for (int i = 0; i < NUMBER_OF_SAMPLES; ++i)
	{
		m_minTime[i] = Utility::Timing::TimeSpan::MAX_TIME_SPAN;
		m_maxTime[i] = Utility::Timing::TimeSpan::MIN_TIME_SPAN;
	}
}


UtmostTimeSamples::~UtmostTimeSamples()
{
}

void UtmostTimeSamples::ProcessTime(const Utility::Timing::TimeSpan& timeSpan)
{
	bool maxFound = false;
	bool minFound = false;
	for (int i = 0; i < NUMBER_OF_SAMPLES; ++i)
	{
		if ((minFound) && (maxFound))
			return;
		if ((!minFound) && (timeSpan < m_minTime[i]))
		{
			for (int j = i; j < NUMBER_OF_SAMPLES - 1; ++j)
			{
				m_minTime[j + 1] = m_minTime[j];
			}
			m_minTime[i] = timeSpan;
			minFound = true;
		}
		if ((!maxFound) && (timeSpan > m_maxTime[i]))
		{
			for (int j = i; j < NUMBER_OF_SAMPLES - 1; ++j)
			{
				m_maxTime[j + 1] = m_maxTime[j];
			}
			m_maxTime[i] = timeSpan;
			maxFound = true;
		}
	}
}

std::string UtmostTimeSamples::ToString()
{
	std::stringstream ss("");
	ss << "minTimes = { ";
	for (int i = 0; i < NUMBER_OF_SAMPLES; ++i)
	{
		ss << m_minTime[i].ToString() << "; ";
	}
	ss << " } maxTimes = { ";
	for (int i = 0; i < NUMBER_OF_SAMPLES; ++i)
	{
		ss << m_maxTime[i].ToString() << "; ";
	}
	ss << " }";
	return ss.str();
}