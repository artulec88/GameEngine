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
		bool diffCompareResult = (m_vector != m_compareVector);
		CHECK_CONDITION_ALWAYS(diffCompareResult != m_expectedCompareResult, Utility::Error,
			"The vector %s and vector %s diff operator result not as expected.",
			m_vector.ToString().c_str(), m_compareVector.ToString().c_str());
		CHECK_CONDITION_ALWAYS(diffCompareResult != compareResult, Utility::Error,
			"The operators == and != should always give opposite results.");
	}
protected:
	T m_compareVector;
	bool m_expectedCompareResult;
}; /* end class VectorTestCompare */

template <class T>
class VectorTestLength : public VectorTestBase<T>
{
public:
	VectorTestLength(const T& vector, Math::Real expectedLength) :
		VectorTestBase<T>(vector),
		m_expectedLength(expectedLength)
	{
	}
	virtual ~VectorTestLength()
	{
	}
public:
	virtual void StartTest()
	{
		Math::Real lengthSquared = m_vector.LengthSquared();
		Math::Real length = m_vector.Length();
		CHECK_CONDITION_ALWAYS(Math::AlmostEqual(length, m_expectedLength), Utility::Error,
			"The vector %s has length %.2f, but was expected to have length %.2f",
			m_vector.ToString().c_str(), length, m_expectedLength);
		CHECK_CONDITION_ALWAYS(Math::AlmostEqual(lengthSquared, m_expectedLength * m_expectedLength), Utility::Error,
			"The vector %s has length %.2f and squared length %.2f, but expected to have squared length %.2f",
			m_vector.ToString().c_str(), lengthSquared, m_expectedLength * m_expectedLength);
	}
protected:
	Math::Real m_expectedLength;
}; /* end class VectorTestLength */

template <class T>
class VectorTestAddOperator : public VectorTestBase<T>
{
public:
	VectorTestAddOperator(const T& vector1, const T& vector2, const T& expectedSumVector) :
		VectorTestBase<T>(vector1),
		m_vector2(vector2),
		m_expectedSumVector(expectedSumVector)
	{
	}
	virtual ~VectorTestAddOperator()
	{
	}
public:
	virtual void StartTest()
	{
		T sumVector = m_vector + m_vector2;
		CHECK_CONDITION_ALWAYS(sumVector == m_expectedSumVector, Utility::Error,
			"The sum of vectors %s and %s is a vector %s. It is different than expected %s",
			m_vector.ToString().c_str(), m_vector2.ToString().c_str(), sumVector.ToString().c_str(),
			m_expectedSumVector.ToString().c_str());

		m_vector += m_vector2;
		CHECK_CONDITION_ALWAYS(m_vector == m_expectedSumVector, Utility::Error,
			"The operator += returned different result than operator + (%s != %s)",
			m_vector.ToString().c_str(), m_expectedSumVector.ToString().c_str());
	}
protected:
	T m_vector2;
	T m_expectedSumVector;
}; /* end class VectorTestAddOperator */

template <class T>
class VectorTestSubtractOperator : public VectorTestBase<T>
{
public:
	VectorTestSubtractOperator(const T& vector1, const T& vector2, const T& expectedSubtractVector) :
		VectorTestBase<T>(vector1),
		m_vector2(vector2),
		m_expectedSubtractVector(expectedSubtractVector)
	{
	}
	virtual ~VectorTestSubtractOperator()
	{
	}
public:
	virtual void StartTest()
	{
		T subtractVector = m_vector - m_vector2;
		CHECK_CONDITION_ALWAYS(subtractVector == m_expectedSubtractVector, Utility::Error,
			"The subtraction of vectors %s and %s is a vector %s. It is different than expected %s",
			m_vector.ToString().c_str(), m_vector2.ToString().c_str(), subtractVector.ToString().c_str(),
			m_expectedSubtractVector.ToString().c_str());

		m_vector -= m_vector2;
		CHECK_CONDITION_ALWAYS(m_vector == m_expectedSubtractVector, Utility::Error,
			"The operator -= returned different result than operator - (%s != %s)",
			m_vector.ToString().c_str(), m_expectedSubtractVector.ToString().c_str());
	}
protected:
	T m_vector2;
	T m_expectedSubtractVector;
}; /* end class VectorTestSubtractOperator */


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