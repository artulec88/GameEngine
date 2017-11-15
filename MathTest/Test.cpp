#include "Test.h"
#include "Def.h"
#include "Utility/ILogger.h"

using namespace math_test;

/* ==================== class Test begin ==================== */
Test::Test() :
	m_timer(),
	m_isTestEnabled(true)
{
}


Test::~Test()
{
}

void Test::BeforeTest()
{
}

void Test::AfterTest()
{
}

void Test::BeforeTimeTest()
{
	m_timer.Start(); // TODO: m_timer.Reset()?
}

utility::timing::TimeSpan Test::AfterTimeTest()
{
	m_timer.Stop();
	return m_timer.GetTimeSpan();
}

bool Test::IsTestEnabled() const
{
	return m_isTestEnabled;
}

/* ==================== class Test end ==================== */


/* ==================== class TestGroup begin ==================== */
TestGroup::TestGroup(const std::string& testGroupName, int testTimeIterationsCount) :
	m_testGroupName(testGroupName),
	m_testTimeIterationsCount(testTimeIterationsCount),
	m_isTestGroupEnabled(true)
{
}

TestGroup::~TestGroup()
{
	if (!m_testTimeSpans.empty())
	{
		math::Real testsTotalTimeSpan = 0.0f;
		math::Real minTimeSpan = REAL_MAX;
		math::Real maxTimeSpan = 0.0f;
		for (auto testTimeSpanItr = m_testTimeSpans.begin(); testTimeSpanItr != m_testTimeSpans.end(); ++testTimeSpanItr)
		{
			testsTotalTimeSpan += *testTimeSpanItr;
			if (*testTimeSpanItr < minTimeSpan)
			{
				minTimeSpan = *testTimeSpanItr;
			}
			if (*testTimeSpanItr > maxTimeSpan)
			{
				maxTimeSpan = *testTimeSpanItr;
			}
		}
		INFO_LOG_MATH_TEST("Test group \"", m_testGroupName + "\" summary:");
		INFO_LOG_MATH_TEST("Minimum time: ", minTimeSpan, " [ns]");
		INFO_LOG_MATH_TEST("Maximum time: ", maxTimeSpan, " [ns]");
		INFO_LOG_MATH_TEST("Average time: ", testsTotalTimeSpan / m_testTimeSpans.size(), " [ns]");
	}

	for (std::vector<TestGroup*>::iterator testGroupItr = m_testGroups.begin(); testGroupItr != m_testGroups.end(); ++testGroupItr)
	{
		SAFE_DELETE(*testGroupItr);
	}
	for (std::vector<Test*>::iterator testItr = m_tests.begin(); testItr != m_tests.end(); ++testItr)
	{
		SAFE_DELETE(*testItr);
	}
	m_testGroups.clear();
	m_tests.clear();
}

void TestGroup::AddTestGroup(TestGroup* testGroup)
{
	m_testGroups.push_back(testGroup);
}

void TestGroup::AddTest(Test* test)
{
	m_tests.push_back(test);
}

void TestGroup::StartTests()
{
	for (std::vector<Test*>::iterator testItr = m_tests.begin(); testItr != m_tests.end(); ++testItr)
	{
		(*testItr)->BeforeTest();
		(*testItr)->StartTest();
		(*testItr)->AfterTest();
	}
	for (std::vector<TestGroup*>::iterator testGroupItr = m_testGroups.begin(); testGroupItr != m_testGroups.end(); ++testGroupItr)
	{
		(*testGroupItr)->StartTests();
	}
}

void TestGroup::StartTimeTests()
{
	for (std::vector<Test*>::iterator testItr = m_tests.begin(); testItr != m_tests.end(); ++testItr)
	{
		(*testItr)->BeforeTimeTest();
		(*testItr)->StartTimeTest(m_testTimeIterationsCount);
		utility::timing::TimeSpan timeSpan = (*testItr)->AfterTimeTest();
		m_testTimeSpans.push_back(static_cast<math::Real>(timeSpan.GetValue()) / m_testTimeIterationsCount);
	}
	for (std::vector<TestGroup*>::iterator testGroupItr = m_testGroups.begin(); testGroupItr != m_testGroups.end(); ++testGroupItr)
	{
		(*testGroupItr)->StartTimeTests();
	}
}

bool TestGroup::IsTestGroupEnabled() const
{
	return m_isTestGroupEnabled;
}
/* ==================== class TestGroup end ==================== */