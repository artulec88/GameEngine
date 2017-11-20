#include "stdafx.h"
#include "StatisticsStorage.h"
#include <sstream>
#include <fstream>

/* static */ math::statistics::StatisticsStorage math::statistics::StatisticsStorage::statsStorage;

math::statistics::StatisticsStorage::StatisticsStorage() :
	m_classStatistics(),
	m_timer()
{
	//m_timer.Start();
	//CRITICAL_LOG_MATH("Creating new statistics storage");
}

math::statistics::StatisticsStorage::~StatisticsStorage()
{
	//m_timer.Stop();
	//PrintReport();
	//DEBUG_LOG_MATH("StatisticsStorage destructor");
}

math::statistics::ClassStats& math::statistics::StatisticsStorage::GetClassStats(const std::string& className)
{
	ClassNames2ClassStatsMap::const_iterator classStatsItr = m_classStatistics.find(className);
	if (classStatsItr == m_classStatistics.end())
	{
		const auto insertRes = m_classStatistics.insert(std::pair<std::string, std::unique_ptr<ClassStats>>(className, std::make_unique<ClassStats>(className)));
		CHECK_CONDITION_MATH(insertRes.second, Utility::Logging::ERR, "Inserted new element in the statistics storage when there has already been the one to use.");
		return *insertRes.first->second;
	}
	return *classStatsItr->second;
}

void math::statistics::StatisticsStorage::PrintSimpleReport() const
{
	std::stringstream ss("");
	for (auto classStatsItr = m_classStatistics.begin(); classStatsItr != m_classStatistics.end(); ++classStatsItr)
	{
		ss << classStatsItr->first << " (" << classStatsItr->second->GetTotalNumberOfSamples() << "); ";
	}
	INFO_LOG_MATH("Simple report = \"", ss.str(), "\"");
}

void math::statistics::StatisticsStorage::PrintReport() const
{
	// Elapsed time should specify how much time has passed since the start of the application until the shutdown.
	utility::logging::ILogger::GetLogger("Math").AddStream("ApplicationStats.txt");
	INFO_LOG_MATH("Total elapsed time: ", m_timer.GetDuration(utility::timing::MILLISECOND), " [ms]");
	std::fstream appStatsFile;
	appStatsFile.open("..\\Docs\\AppStats.dat", std::ios::out);
	appStatsFile << "\"Class name\"\t\"Total time\"\t\"Total time excluding nested calls\"\n";
	for (auto classStatsItr = m_classStatistics.begin(); classStatsItr != m_classStatistics.end(); ++classStatsItr)
	{
		if (!classStatsItr->second->IsEmpty())
		{
			classStatsItr->second->PrintReport(m_timer.GetTimeSpan(), appStatsFile);
		}
	}
	appStatsFile.close();
}