#ifndef __MATH_TEST_TEST_GROUP_H__
#define __MATH_TEST_TEST_GROUP_H__

#include "Utility\Time.h"
#include <vector>

namespace MathTest
{

class TestGroup
{
/* ==================== Constructors and destructors begin ==================== */
public:
	TestGroup(void);
	virtual ~TestGroup(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
protected:
	void AddTest(const Test& test);
	virtual void StartTests() = 0;
	bool IsTestGroupEnabled() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	std::vector<Test> m_tests;
	Utility::Timing::Timer m_timer;
	bool m_isTestGroupEnabled;
/* ==================== Non-static member variables end ==================== */
}; /* end class TestGroup */

class Test
{
/* ==================== Constructors and destructors begin ==================== */
public:
	Test(void);
	virtual ~Test(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
protected:
	void StartTest();
	bool IsTestEnabled() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Utility::Timing::Timer m_timer;
	bool m_isTestEnabled;
/* ==================== Non-static member variables end ==================== */
}; /* end class Test */

} /* end namespace MathTest */