#include "stdafx.h"
#include "IStatisticsStorage.h"
#include <sstream>
#include <fstream>

/* static */ Math::Statistics::IStatisticsStorage& Math::Statistics::IStatisticsStorage::GetStatisticsStorage()
{
	static IStatisticsStorage statisticsStorage;
	return statisticsStorage;
}

Math::Statistics::IStatisticsStorage::IStatisticsStorage() :
	m_classStatistics(),
	m_timer()
{
	//m_timer.Start();
	//CRITICAL_LOG_MATH("Creating new statistics storage");
}

Math::Statistics::IStatisticsStorage::~IStatisticsStorage(void)
{
	//m_timer.Stop();
	//PrintReport();
	//DEBUG_LOG_MATH("IStatisticsStorage destructor");
}

Math::Statistics::ClassStats& Math::Statistics::IStatisticsStorage::GetClassStats(const std::string& className)
{
	ClassNames2ClassStatsMap::const_iterator classStatsItr = m_classStatistics.find(className);
	if (classStatsItr == m_classStatistics.end())
	{
		std::pair<ClassNames2ClassStatsMap::iterator, bool /* false if element already existed */> insertRes =
			m_classStatistics.insert(std::pair<std::string, std::unique_ptr<ClassStats>>(className, std::make_unique<ClassStats>(className)));
		CHECK_CONDITION_MATH(insertRes.second, Utility::Logging::ERR, "Inserted new element in the statistics storage when there has already been the one to use.");
		return *insertRes.first->second;
	}
	return *classStatsItr->second;
}

void Math::Statistics::IStatisticsStorage::PrintSimpleReport() const
{
	std::stringstream ss("");
	for (ClassNames2ClassStatsMap::const_iterator classStatsItr = m_classStatistics.begin(); classStatsItr != m_classStatistics.end(); ++classStatsItr)
	{
		ss << classStatsItr->first << " (" << classStatsItr->second->GetTotalNumberOfSamples() << "); ";
	}
	INFO_LOG_MATH("Simple report = \"", ss.str(), "\"");
}

void Math::Statistics::IStatisticsStorage::PrintReport() const
{
	// Elapsed time should specify how much time has passed since the start of the application until the shutdown.
	Utility::Logging::ILogger::GetLogger("Math").AddStream("ApplicationStats.txt");
	INFO_LOG_MATH("Total elapsed time: ", m_timer.GetDuration(Utility::Timing::MILLISECOND), " [ms]");
	std::fstream appStatsFile;
	appStatsFile.open("..\\Docs\\AppStats.dat", std::ios::out);
	appStatsFile << "\"Class name\"\t\"Total time\"\t\"Total time excluding nested calls\"\n";
	for (ClassNames2ClassStatsMap::const_iterator classStatsItr = m_classStatistics.begin(); classStatsItr != m_classStatistics.end(); ++classStatsItr)
	{
		if (!classStatsItr->second->IsEmpty())
		{
			classStatsItr->second->PrintReport(m_timer.GetDuration(Utility::Timing::MILLISECOND), appStatsFile);
		}
	}
	appStatsFile.close();
}