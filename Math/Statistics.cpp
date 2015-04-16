#include "StdAfx.h"
#include "Statistics.h"
#include "ISort.h"

using namespace Math::Statistics;

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
	
	//return m_samples.at(statsID).size();

	//try
	//{
	//	return m_samples.at(statsID).size();
	//}
	//catch (std::out_of_range exc)
	//{
	//	LOG(Utility::Error, LOGPLACE, "Unknown statsID specified (%d)", statsID);
	//	return 0;
	//}
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
		Real medianMean = m_samples[statsID].at(m_samples[statsID].size() / 2) + m_samples[statsID].at((m_samples[statsID].size() / 2) - 1);
		return medianMean / 2;
	}
	else
	{
		return m_samples[statsID].at(m_samples[statsID].size() / 2);
	}
}

template MATH_API class Stats<Math::Real>;
template MATH_API class Stats<int>;