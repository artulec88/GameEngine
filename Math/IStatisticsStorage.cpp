#include "stdafx.h"
#include "IStatisticsStorage.h"

using namespace Math::Statistics;

/* static */ IStatisticsStorage* IStatisticsStorage::statisticsStorage = NULL;
/* static */ IStatisticsStorage& IStatisticsStorage::GetStatisticsStorage()
{
	if (IStatisticsStorage::statisticsStorage == NULL)
	{
		IStatisticsStorage::statisticsStorage = new IStatisticsStorage();
	}
	return *IStatisticsStorage::statisticsStorage;
}

IStatisticsStorage::IStatisticsStorage()
{
}

IStatisticsStorage::~IStatisticsStorage(void)
{
	for (std::map<const char*, ClassStats*>::iterator classStatsItr = m_classStatistics.begin(); classStatsItr != m_classStatistics.end(); ++classStatsItr)
	{
		delete classStatsItr->second;
	}
}

ClassStats& IStatisticsStorage::GetClassStats(const char* className)
{
	std::pair<std::map<const char*, ClassStats*>::iterator, bool /* false if element already existed */> insertRes = m_classStatistics.insert(std::pair<const char*, ClassStats*>(className, new ClassStats(className)));
	return *insertRes.first->second;
}