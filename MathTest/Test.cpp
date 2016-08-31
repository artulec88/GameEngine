#include "Test.h"
#include "Def.h"
#include "Utility\ILogger.h"

using namespace MathTest;

/* ==================== class Test begin ==================== */
Test::Test(void)
{
}


Test::~Test(void)
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
	//m_timer.Start(); // TODO: m_timer.Reset()?
}

void Test::AfterTimeTest()
{
	//m_timer.Stop();
}

bool Test::IsTestEnabled() const
{
	return m_isTestEnabled;
}

/* ==================== class Test end ==================== */


/* ==================== class TestGroup begin ==================== */
TestGroup::TestGroup() :
	m_timer(),
	m_isTestGroupEnabled(true)
{
}

TestGroup::~TestGroup()
{
	for (std::vector<Test*>::iterator testItr = m_tests.begin(); testItr != m_tests.end(); ++testItr)
	{
		SAFE_DELETE(*testItr);
	}
	m_tests.clear();
	m_timer.Stop();
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
}

void TestGroup::StartTimeTests()
{
	for (std::vector<Test*>::iterator testItr = m_tests.begin(); testItr != m_tests.end(); ++testItr)
	{
		(*testItr)->BeforeTimeTest();
		(*testItr)->StartTimeTest();
		(*testItr)->AfterTimeTest();
	}
}

bool TestGroup::IsTestGroupEnabled() const
{
	return m_isTestGroupEnabled;
}
/* ==================== class TestGroup end ==================== */