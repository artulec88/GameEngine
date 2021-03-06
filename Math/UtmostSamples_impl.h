#include "stdafx.h"
#ifndef __MATH_UTMOST_TIME_SAMPLES_IMPL_H__
#define __MATH_UTMOST_TIME_SAMPLES_IMPL_H__

#include "UtmostSamples.h"
#include <sstream>

template <class T>
math::statistics::UtmostSamples<T>::UtmostSamples() :
	m_minSamples(),
	m_maxSamples()
{
	for (auto i = 0; i < NUMBER_OF_SAMPLES; ++i)
	{
		m_minSamples[i] = 999999999;
		m_maxSamples[i] = 0;
	}
}

template <class T>
math::statistics::UtmostSamples<T>::~UtmostSamples()
{
}

template <class T>
void math::statistics::UtmostSamples<T>::ProcessSample(const T& sample)
{
	bool maxFound = false;
	bool minFound = false;
	for (auto i = 0; i < NUMBER_OF_SAMPLES; ++i)
	{
		if (minFound && maxFound)
		{
			return;
		}
		if (!minFound && sample < m_minSamples[i])
		{
			for (auto j = i; j < NUMBER_OF_SAMPLES - 1; ++j)
			{
				m_minSamples[j + 1] = m_minSamples[j];
			}
			m_minSamples[i] = sample;
			minFound = true;
		}
		if (!maxFound && sample > m_maxSamples[i])
		{
			for (auto j = i; j < NUMBER_OF_SAMPLES - 1; ++j)
			{
				m_maxSamples[j + 1] = m_maxSamples[j];
			}
			m_maxSamples[i] = sample;
			maxFound = true;
		}
	}
}

template <class T>
std::string math::statistics::UtmostSamples<T>::ToString()
{
	std::stringstream ss("");
	ss << "minSamples = { ";
	for (auto i = 0; i < NUMBER_OF_SAMPLES; ++i)
	{
		ss << m_minSamples[i] << "; ";
	}
	ss << " } maxTimes = { ";
	for (auto i = 0; i < NUMBER_OF_SAMPLES; ++i)
	{
		ss << m_maxSamples[i] << "; ";
	}
	ss << " }";
	return ss.str();
}

#endif /* __MATH_UTMOST_TIME_SAMPLES_IMPL_H__ */