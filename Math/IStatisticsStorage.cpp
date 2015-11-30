#include "stdafx.h"
#include "IStatisticsStorage.h"
#include <sstream>
#include <fstream>

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
	//CRITICAL_LOG("Creating new statistics storage");
}

IStatisticsStorage::~IStatisticsStorage(void)
{
	//DEBUG_LOG("IStatisticsStorage destructor");
	for (std::map<const char*, ClassStats*>::iterator classStatsItr = m_classStatistics.begin(); classStatsItr != m_classStatistics.end(); ++classStatsItr)
	{
		delete classStatsItr->second;
	}
}

ClassStats& IStatisticsStorage::GetClassStats(const char* className)
{
	std::map<const char*, ClassStats*>::const_iterator classStatsItr = m_classStatistics.find(className);
	if (classStatsItr == m_classStatistics.end())
	{
		std::pair<std::map<const char*, ClassStats*>::iterator, bool /* false if element already existed */> insertRes = m_classStatistics.insert(std::pair<const char*, ClassStats*>(className, new ClassStats(className)));
		CHECK_CONDITION(insertRes.second, Utility::Error, "Inserted new element in the statistics storage when there has already been the one to use.");
		return *insertRes.first->second;
	}
	return *classStatsItr->second;
}

void IStatisticsStorage::PrintSimpleReport() const
{
	std::stringstream ss("");
	for (std::map<const char*, ClassStats*>::const_iterator classStatsItr = m_classStatistics.begin(); classStatsItr != m_classStatistics.end(); ++classStatsItr)
	{
		ss << classStatsItr->first << " (" << classStatsItr->second->GetTotalNumberOfSamples() << "); ";
	}
	INFO_LOG("Simple report = \"%s\"", ss.str().c_str());
}

void IStatisticsStorage::PrintReport(const Utility::Timing::TimeSpan& timeSpan) const
{
	// Elapsed time should specify how much time has passed since the start of the application until the shutdown.
	Utility::ILogger::GetLogger().AddFile("ApplicationStats.txt");
	INFO_LOG("Total elapsed time: %s", timeSpan.ToString().c_str());
	std::fstream appStatsFile;
	appStatsFile.open("..\\Docs\\AppStats.dat", std::ios::out);
	appStatsFile << "\"Class name\"\t\"Total time\"\t\"Total time excluding nested calls\"\n";
	for (std::map<const char*, ClassStats*>::const_iterator classStatsItr = m_classStatistics.begin(); classStatsItr != m_classStatistics.end(); ++classStatsItr)
	{
		if (!classStatsItr->second->IsEmpty())
		{
			classStatsItr->second->PrintReport(timeSpan, appStatsFile);
		}
	}
	appStatsFile.close();
}