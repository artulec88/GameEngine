#ifndef __MATH_TEST_TEST_GROUP_H__
#define __MATH_TEST_TEST_GROUP_H__

#include "Utility\Time.h"
#include <vector>

namespace MathTest
{

	class Test
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		Test(void);
		virtual ~Test(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BeforeTest();
		virtual void AfterTest();
		virtual void StartTest() = 0;
		virtual void BeforeTimeTest();
		virtual void AfterTimeTest();
		virtual void StartTimeTest() = 0;
		bool IsTestEnabled() const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Utility::Timing::Timer m_timer;
		bool m_isTestEnabled;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Test */

	class TestGroup
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		TestGroup(void);
		virtual ~TestGroup(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		void AddTest(Test* test);
		void StartTests();
		void StartTimeTests();
		bool IsTestGroupEnabled() const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		std::vector<Test*> m_tests;
		Utility::Timing::Timer m_timer;
		bool m_isTestGroupEnabled;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TestGroup */

} /* end namespace MathTest */

#endif /* __MATH_TEST_TEST_GROUP_H__ */