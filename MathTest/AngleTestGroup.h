#ifndef __MATH_TEST_ANGLE_TEST_GROUP_H__
#define __MATH_TEST_ANGLE_TEST_GROUP_H__

#include "Test.h"
#include "Math\Angle.h"

namespace MathTest
{

	class AngleTestGroup : public TestGroup
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestGroup(const std::string& angleTestGroupName, int testTimeIterationsCount);
		virtual ~AngleTestGroup(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestGroup */

	class AngleTestBase : public Test
	{
		/* ==================== Constructors and destructors begin ==================== */
	protected:
		AngleTestBase(const Math::Angle& angle);
		virtual ~AngleTestBase(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Angle m_angle;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestBase */

	class AngleTestTrigonometry : public AngleTestBase
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestTrigonometry(const Math::Angle& angle, Math::Real expectedTrigonometryOperatorValue);
		virtual ~AngleTestTrigonometry();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Real m_expectedTrigonometryOperatorValue;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestTrigonometry */

	class AngleTestSin : public AngleTestTrigonometry
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestSin(const Math::Angle& angle, Math::Real expectedSinValue);
		virtual ~AngleTestSin();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestSin */

	class AngleTestCos : public AngleTestTrigonometry
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestCos(const Math::Angle& angle, Math::Real expectedCosValue);
		virtual ~AngleTestCos();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestCos */

	class AngleTestTan : public AngleTestTrigonometry
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestTan(const Math::Angle& angle, Math::Real expectedTanValue);
		virtual ~AngleTestTan();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestTan */


	class AngleTestCompare : public AngleTestBase
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestCompare(const Math::Angle& angle1, const Math::Angle& angle2, bool expectedCompareResult, bool expectedLessResult, bool expectedGreaterResult);
		virtual ~AngleTestCompare();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Angle m_compareAngle;
		bool m_expectedCompareResult;
		bool m_expectedLessResult;
		bool m_expectedGreaterResult;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestCompare */

	template <class T>
	class AngleTestOperator : public AngleTestBase
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		//AngleTestOperator(const Math::Angle& angle1, const Math::Angle& angle2, Math::Real value, Math::Angle& expectedAnglesSum, const Math::Angle& expectedAnglesDiff,
		//	const Math::Angle& expectedAngle1TimesValue, const Math::Angle& expectedAngle1OverValue, const Math::Angle& expectedAngle2TimesValue, const Math::Angle& expectedAngle2OverValue);
		AngleTestOperator(const Math::Angle& angle1, const T& value, const Math::Angle& expectedResult) :
			AngleTestBase(angle1),
			m_value(value),
			m_expectedResult(expectedResult)
		{
		}
		virtual ~AngleTestOperator()
		{
		}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const T m_value;
		const Math::Angle m_expectedResult;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestOperator */

	class AngleTestSum : public AngleTestOperator<Math::Angle>
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestSum(const Math::Angle& angle1, const Math::Angle& angle2, const Math::Angle& expectedAnglesSum);
		virtual ~AngleTestSum();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestSum */

	class AngleTestDiff : public AngleTestOperator<Math::Angle>
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestDiff(const Math::Angle& angle1, const Math::Angle& angle2, const Math::Angle& expectedAnglesDiff);
		virtual ~AngleTestDiff();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestDiff */

	class AngleTestMultiplyByValue : public AngleTestOperator<Math::Real>
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestMultiplyByValue(const Math::Angle& angle1, Math::Real multValue, const Math::Angle& expectedMultResult);
		virtual ~AngleTestMultiplyByValue();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestMultiplyByValue */

	class AngleTestDivideByValue : public AngleTestOperator<Math::Real>
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestDivideByValue(const Math::Angle& angle1, Math::Real divValue, const Math::Angle& expectedDivResult);
		virtual ~AngleTestDivideByValue();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestDivideByValue */

} /* end namespace MathTest */

#endif /* __MATH_TEST_ANGLE_TEST_GROUP_H__ */