#ifndef __MATH_TEST_TEST_GROUP_H__
#define __MATH_TEST_TEST_GROUP_H__

#include "Utility/Time.h"

#include "Math/Math.h"

#include <vector>

namespace math_test
{

	class Test
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		Test();
		virtual ~Test();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BeforeTest();
		virtual void AfterTest();
		virtual void StartTest() = 0;
		virtual void BeforeTimeTest();
		virtual Utility::Timing::TimeSpan AfterTimeTest();
		virtual void StartTimeTest(unsigned int iterationsCount) = 0;
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
		TestGroup(const std::string& testGroupName, int testTimeIterationsCount);
		virtual ~TestGroup();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		void AddTestGroup(TestGroup* testGroup);
		void AddTest(Test* test);
		void StartTests();
		void StartTimeTests();
		bool IsTestGroupEnabled() const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const std::string m_testGroupName;
		const int m_testTimeIterationsCount;
		std::vector<TestGroup*> m_testGroups;
		std::vector<Test*> m_tests;
		std::vector<Math::Real> m_testTimeSpans;
		bool m_isTestGroupEnabled;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TestGroup */

} /* end namespace MathTest */

#endif /* __MATH_TEST_TEST_GROUP_H__ */