#include "Test.h"

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
	m_timer.Start(); // TODO: m_timer.Reset()?
}

void Test::AfterTest()
{
	m_timer.Stop();
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

bool TestGroup::IsTestGroupEnabled() const
{
	return m_isTestGroupEnabled;
}
/* ==================== class TestGroup end ==================== */