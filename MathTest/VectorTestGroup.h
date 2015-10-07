#ifndef __MATH_TEST_VECTOR_TEST_GROUP_H__
#define __MATH_TEST_VECTOR_TEST_GROUP_H__

#include "Test.h"
#include "Math\Vector.h"
#include "Math\Angle.h"

namespace MathTest
{

class VectorTestGroup : public TestGroup
{
/* ==================== Constructors and destructors begin ==================== */
public:
	VectorTestGroup(void);
	virtual ~VectorTestGroup(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void StartTests();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class VectorTestGroup */

template <class T>
class VectorTestBase : public Test
{
/* ==================== Constructors and destructors begin ==================== */
protected:
	VectorTestBase(const T& vector) :
		Test(),
		m_vector(vector)
	{
	}
	virtual ~VectorTestBase(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	T m_vector;
/* ==================== Non-static member variables end ==================== */
}; /* end class VectorTestBase */

template <class T>
class VectorTestCompare : public VectorTestBase<T>
{
public:
	VectorTestCompare(const T& vector1, const T& vector2, bool expectedCompareResult) :
		VectorTestBase<T>(vector1),
		m_compareVector(vector2),
		m_expectedCompareResult(expectedCompareResult)
	{
	}
	virtual ~VectorTestCompare()
	{
	}
public:
	virtual void StartTest()
	{
		bool compareResult = (m_vector == m_compareVector);
		CHECK_CONDITION_ALWAYS(compareResult == m_expectedCompareResult, Utility::Error,
			"The vector %s and vector %s comparison result not as expected.",
			m_vector.ToString().c_str(), m_compareVector.ToString().c_str());
	}
protected:
	T m_compareVector;
	bool m_expectedCompareResult;
}; /* end class VectorTestCompare */

//class Vector2DTestLength : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestLength(const Math::Vector2D& vec, Math::Real expectedLength);
//	virtual ~Vector2DTestLength();
//public:
//	virtual void StartTest();
//protected:
//	Math::Real m_expectedLength;
//}; /* end class Vector2DTestLength */
//
//class Vector2DTestAddOperator : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestAddOperator(const Math::Vector2D& vec1, const Math::Vector2D& vec2, const Math::Vector2D& expectedSumVector);
//	virtual ~Vector2DTestAddOperator();
//public:
//	virtual void StartTest();
//protected:
//	Math::Vector2D m_vector2;
//	Math::Vector2D m_expectedSumVector;
//}; /* end class Vector2DTestAddOperator */
//
//class Vector2DTestSubtractOperator : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestSubtractOperator(const Math::Vector2D& vec1, const Math::Vector2D& vec2, const Math::Vector2D& expectedSubtractVector);
//	virtual ~Vector2DTestSubtractOperator();
//public:
//	virtual void StartTest();
//protected:
//	Math::Vector2D m_vector2;
//	Math::Vector2D m_expectedSubtractVector;
//}; /* end class Vector2DTestSubtractOperator */
//
//class Vector2DTestMultiplyOperator : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestMultiplyOperator(const Math::Vector2D& vec1, const Math::Vector2D& vec2, const Math::Vector2D& expectedMultiplyVector);
//	virtual ~Vector2DTestMultiplyOperator();
//public:
//	virtual void StartTest();
//protected:
//	Math::Vector2D m_vector2;
//	Math::Vector2D m_expectedMultiplyVector;
//}; /* end class Vector2DTestMultiplyOperator */
//
//class Vector2DTestDivideOperator : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestDivideOperator(const Math::Vector2D& vec1, const Math::Vector2D& vec2, const Math::Vector2D& expectedDivideVector);
//	virtual ~Vector2DTestDivideOperator();
//public:
//	virtual void StartTest();
//protected:
//	Math::Vector2D m_vector2;
//	Math::Vector2D m_expectedDivideVector;
//}; /* end class Vector2DTestDivideOperator */
//
//class Vector2DTestNormalize : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestNormalize(const Math::Vector2D& vector, const Math::Vector2D& expectedNormalizedVector);
//	virtual ~Vector2DTestNormalize();
//public:
//	virtual void StartTest();
//protected:
//	Math::Vector2D m_expectedNormalizedVector;
//}; /* end class Vector2DTestNormalize */
//
//class Vector2DTestCross : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestCross(const Math::Vector2D& vector1, const Math::Vector2D& vector2, Math::Real expectedCrossResult);
//	virtual ~Vector2DTestCross();
//public:
//	virtual void StartTest();
//protected:
//	Math::Vector2D m_vector2;
//	Math::Real m_expectedCrossResult;
//}; /* end class Vector2DTestCross */
//
//class Vector2DTestDot : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestDot(const Math::Vector2D& vector1, const Math::Vector2D& vector2, Math::Real expectedDotResult);
//	virtual ~Vector2DTestDot();
//public:
//	virtual void StartTest();
//protected:
//	Math::Vector2D m_vector2;
//	Math::Real m_expectedDotResult;
//}; /* end class Vector2DTestDot */
//
//class Vector2DTestRotate : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestRotate(const Math::Vector2D& vec1, const Math::Angle& angle, const Math::Vector2D& expectedRotateVector);
//	virtual ~Vector2DTestRotate();
//public:
//	virtual void StartTest();
//protected:
//	Math::Angle m_angle;
//	Math::Vector2D m_expectedRotateVector;
//}; /* end class Vector2DTestRotate */
//
//class Vector2DTestLerp : public VectorTestBase<Math::Vector2D>
//{
//public:
//	Vector2DTestLerp(const Math::Vector2D& vector1, const Math::Vector2D& vector2, Math::Real lerpFactor, const Math::Vector2D& expectedLerpVector);
//	virtual ~Vector2DTestLerp();
//public:
//	virtual void StartTest();
//protected:
//	Math::Vector2D m_vector2;
//	Math::Real m_lerpFactor;
//	Math::Vector2D m_expectedLerpVector;
//}; /* end class Vector2DTestLerp */

} /* end namespace MathTest */

#endif /* __MATH_TEST_VECTOR_TEST_GROUP_H__ */