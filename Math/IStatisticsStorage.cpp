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
		//LOG(Utility::Critical, LOGPLACE, "Creating new statistics storage");
		IStatisticsStorage::statisticsStorage = new IStatisticsStorage();
	}
	return *IStatisticsStorage::statisticsStorage;
}

IStatisticsStorage::IStatisticsStorage()
{
	//LOG(Utility::Critical, LOGPLACE, "IStatisticsStorage constructor");
}

IStatisticsStorage::~IStatisticsStorage(void)
{
	LOG(Utility::Critical, LOGPLACE, "IStatisticsStorage destructor");
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
	LOG(Utility::Info, LOGPLACE, "Simple report = \"%s\"", ss.str().c_str());
}

void IStatisticsStorage::PrintReport(Math::Real totalElapsedSeconds /* Create Time object and pass it here instead of Math::Real */) const
{
	// Elapsed time should specify how much time has passed since the start of the application until the shutdown.
	Utility::ILogger::GetLogger().AddFile("ApplicationStats.txt");
	LOG(Utility::Info, LOGPLACE, "Total elapsed time: %.3f [s]", totalElapsedSeconds);
	std::fstream appStatsFile;
	appStatsFile.open("..\\Docs\\AppStats.dat", std::ios::out);
	appStatsFile << "\"Class name\"\t\"Total time\"\t\"Total time excluding nested calls\"\n";
	for (std::map<const char*, ClassStats*>::const_iterator classStatsItr = m_classStatistics.begin(); classStatsItr != m_classStatistics.end(); ++classStatsItr)
	{
		if (!classStatsItr->second->IsEmpty())
		{
			classStatsItr->second->PrintReport(totalElapsedSeconds, appStatsFile);
		}
	}
	appStatsFile.close();
}