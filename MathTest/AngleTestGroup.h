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
		AngleTestGroup(void);
		virtual ~AngleTestGroup(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTests();
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


	class AngleTestCompare : public AngleTestBase
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestCompare(const Math::Angle& angle1, const Math::Angle& angle2, bool expectedCompareResult, bool expectedLessResult, bool expectedGreaterResult);
		virtual ~AngleTestCompare();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Angle m_compareAngle;
		bool m_expectedCompareResult;
		bool m_expectedLessResult;
		bool m_expectedGreaterResult;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestCompare */

	class AngleTestTrigonometry : public AngleTestBase
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		AngleTestTrigonometry(const Math::Angle& angle1, Math::Real expectedSinValue, Math::Real expectedCosValue, Math::Real expectedTanValue);
		virtual ~AngleTestTrigonometry();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void StartTest();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Real m_expectedSinValue;
		Math::Real m_expectedCosValue;
		Math::Real m_expectedTanValue;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AngleTestTrigonometry */

} /* end namespace MathTest */

#endif /* __MATH_TEST_ANGLE_TEST_GROUP_H__ */