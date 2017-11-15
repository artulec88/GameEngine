#ifndef __MATH_TEST_ANGLE_TEST_GROUP_H__
#define __MATH_TEST_ANGLE_TEST_GROUP_H__

#include "Test.h"
#include "Math/Angle.h"

namespace math_test
{

	class AngleTestGroup : public TestGroup
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestGroup(const std::string& angleTestGroupName, int testTimeIterationsCount);
		virtual ~AngleTestGroup();
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
		explicit AngleTestBase(const math::Angle& angle);
		virtual ~AngleTestBase();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Angle m_angle;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestBase */

	class AngleTestTrigonometry : public AngleTestBase
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestTrigonometry(const math::Angle& angle, math::Real expectedTrigonometryOperatorValue);
		virtual ~AngleTestTrigonometry();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Real m_expectedTrigonometryOperatorValue;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestTrigonometry */

	class AngleTestSin : public AngleTestTrigonometry
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestSin(const math::Angle& angle, math::Real expectedSinValue);
		virtual ~AngleTestSin();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestSin */

	class AngleTestCos : public AngleTestTrigonometry
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestCos(const math::Angle& angle, math::Real expectedCosValue);
		virtual ~AngleTestCos();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestCos */

	class AngleTestTan : public AngleTestTrigonometry
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestTan(const math::Angle& angle, math::Real expectedTanValue);
		virtual ~AngleTestTan();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
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
		AngleTestCompare(const math::Angle& angle1, const math::Angle& angle2, bool expectedCompareResult, bool expectedLessResult, bool expectedGreaterResult);
		virtual ~AngleTestCompare();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Angle m_compareAngle;
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
		//AngleTestOperator(const math::Angle& angle1, const math::Angle& angle2, math::Real value, math::Angle& expectedAnglesSum, const math::Angle& expectedAnglesDiff,
		//	const math::Angle& expectedAngle1TimesValue, const math::Angle& expectedAngle1OverValue, const math::Angle& expectedAngle2TimesValue, const math::Angle& expectedAngle2OverValue);
		AngleTestOperator(const math::Angle& angle1, const T& value, const math::Angle& expectedResult) :
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
		const math::Angle m_expectedResult;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestOperator */

	class AngleTestSum : public AngleTestOperator<math::Angle>
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestSum(const math::Angle& angle1, const math::Angle& angle2, const math::Angle& expectedAnglesSum);
		virtual ~AngleTestSum();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestSum */

	class AngleTestDiff : public AngleTestOperator<math::Angle>
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestDiff(const math::Angle& angle1, const math::Angle& angle2, const math::Angle& expectedAnglesDiff);
		virtual ~AngleTestDiff();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestDiff */

	class AngleTestMultiplyByValue : public AngleTestOperator<math::Real>
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestMultiplyByValue(const math::Angle& angle1, math::Real multValue, const math::Angle& expectedMultResult);
		virtual ~AngleTestMultiplyByValue();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestMultiplyByValue */

	class AngleTestDivideByValue : public AngleTestOperator<math::Real>
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestDivideByValue(const math::Angle& angle1, math::Real divValue, const math::Angle& expectedDivResult);
		virtual ~AngleTestDivideByValue();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestDivideByValue */

} /* end namespace MathTest */

#endif /* __MATH_TEST_ANGLE_TEST_GROUP_H__ */