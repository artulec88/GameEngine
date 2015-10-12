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


template <class T>
class VectorTestMultiplyOperator : public VectorTestBase<T>
{
public:
	VectorTestMultiplyOperator(const T& vector1, const T& vector2, Math::Real value, const T& expectedMultiplyVector1, const T& expectedMultiplyVector2) :
		VectorTestBase<T>(vector1),
		m_vector2(vector2),
		m_value(value),
		m_expectedMultiplyVector1(expectedMultiplyVector1),
		m_expectedMultiplyVector2(expectedMultiplyVector2)
	{
	}
	virtual ~VectorTestMultiplyOperator()
	{
	}
public:
	virtual void StartTest()
	{
		T multiplyVector1 = m_vector * m_vector2;
		T multiplyVector2 = m_vector * m_value;
		CHECK_CONDITION_ALWAYS(multiplyVector1 == m_expectedMultiplyVector1, Utility::Error,
			"The multiplication of vectors %s and %s is a vector %s. It is different than expected %s",
			m_vector.ToString().c_str(), m_vector2.ToString().c_str(), multiplyVector1.ToString().c_str(),
			m_expectedMultiplyVector1.ToString().c_str());
		CHECK_CONDITION_ALWAYS(multiplyVector2 == m_expectedMultiplyVector2, Utility::Error,
			"The multiplication of vector %s and value %.2f is a vector %s. It is different than expected %s",
			m_vector.ToString().c_str(), m_value, multiplyVector2.ToString().c_str(),
			m_expectedMultiplyVector2.ToString().c_str());

		T tempVector(m_vector);
		m_vector *= m_vector2;
		CHECK_CONDITION_ALWAYS(m_vector == m_expectedMultiplyVector1, Utility::Error,
			"The operator *= returned different result than operator * (%s != %s)",
			m_vector.ToString().c_str(), m_expectedMultiplyVector1.ToString().c_str());
		m_vector = tempVector;
		m_vector *= m_value;
		CHECK_CONDITION_ALWAYS(m_vector == m_expectedMultiplyVector2, Utility::Error,
			"The operator *= returned different result than operator * (%s != %s)",
			m_vector.ToString().c_str(), m_expectedMultiplyVector2.ToString().c_str());
	}
protected:
	T m_vector2;
	Math::Real m_value;
	T m_expectedMultiplyVector1;
	T m_expectedMultiplyVector2;
}; /* end class VectorTestMultiplyOperator */

template <class T>
class VectorTestDivideOperator : public VectorTestBase<T>
{
public:
	VectorTestDivideOperator(const T& vector1, const T& vector2, Math::Real value, const T& expectedDivideVector1, const T& expectedDivideVector2) :
		VectorTestBase<T>(vector1),
		m_vector2(vector2),
		m_value(value),
		m_expectedDivideVector1(expectedDivideVector1),
		m_expectedDivideVector2(expectedDivideVector2)
	{
	}
	virtual ~VectorTestDivideOperator()
	{
	}
public:
	virtual void StartTest()
	{
		T divideVector1 = m_vector / m_vector2;
		T divideVector2 = m_vector / m_value;
		CHECK_CONDITION_ALWAYS(divideVector1 == m_expectedDivideVector1, Utility::Error,
			"The division of vectors %s and %s is a vector %s. It is different than expected %s",
			m_vector.ToString().c_str(), m_vector2.ToString().c_str(), divideVector1.ToString().c_str(),
			m_expectedDivideVector1.ToString().c_str());
		CHECK_CONDITION_ALWAYS(divideVector2 == m_expectedDivideVector2, Utility::Error,
			"The division of vector %s and value %.2f is a vector %s. It is different than expected %s",
			m_vector.ToString().c_str(), m_value, divideVector2.ToString().c_str(),
			m_expectedDivideVector2.ToString().c_str());

		T tempVector(m_vector);
		m_vector /= m_vector2;
		CHECK_CONDITION_ALWAYS(m_vector == m_expectedDivideVector1, Utility::Error,
			"The operator /= returned different result than operator / (%s != %s)",
			m_vector.ToString().c_str(), m_expectedDivideVector1.ToString().c_str());
		m_vector = tempVector;
		m_vector /= m_value;
		CHECK_CONDITION_ALWAYS(m_vector == m_expectedDivideVector2, Utility::Error,
			"The operator /= returned different result than operator / (%s != %s)",
			m_vector.ToString().c_str(), m_expectedDivideVector2.ToString().c_str());
	}
protected:
	T m_vector2;
	Math::Real m_value;
	T m_expectedDivideVector1;
	T m_expectedDivideVector2;
}; /* end class VectorTestDivideOperator */

template <class T>
class VectorTestNormalize : public VectorTestBase<T>
{
public:
	VectorTestNormalize(const T& vector, const T& expectedNormalizedVector) :
		VectorTestBase<T>(vector),
		m_expectedNormalizedVector(expectedNormalizedVector)
	{
		Math::Real lengthSquared = m_expectedNormalizedVector.LengthSquared();
		Math::Real length = m_expectedNormalizedVector.Length();
		CHECK_CONDITION_ALWAYS(Math::AlmostEqual(lengthSquared, REAL_ONE), Utility::Error,
			"Given expected normalized vector %s is in fact not normalized.", m_expectedNormalizedVector.ToString().c_str());
		CHECK_CONDITION_ALWAYS(Math::AlmostEqual(length, lengthSquared), Utility::Error,
			"Given expected normalized vector %s gives different results for length and squared length (%.7f and %.7f respectively).",
			m_expectedNormalizedVector.ToString().c_str(), length, lengthSquared);
		CHECK_CONDITION_ALWAYS(m_expectedNormalizedVector.IsNormalized(), Utility::Error,
			"Given expected normalized vector %s is in fact not normalized.",
			m_expectedNormalizedVector.ToString().c_str());
	}
	virtual ~VectorTestNormalize()
	{
	}
public:
	virtual void StartTest()
	{
		T normalizedVector = m_vector.Normalized();
		CHECK_CONDITION_ALWAYS(normalizedVector == m_expectedNormalizedVector, Utility::Error,
			"The vector %s after normalization equals %s. It is different than expected %s.",
			m_vector.ToString().c_str(), normalizedVector.ToString().c_str(),
			m_expectedNormalizedVector.ToString().c_str());
		CHECK_CONDITION_ALWAYS(normalizedVector.IsNormalized(), Utility::Error,
			"Calculated normalized vector %s is in fact not normalized.",
			normalizedVector.ToString().c_str());

		m_vector.Normalize();
		CHECK_CONDITION_ALWAYS(m_vector == m_expectedNormalizedVector, Utility::Error,
			"The vector after normalization %s is different than expected %s",
			m_vector.ToString().c_str(), m_expectedNormalizedVector.ToString().c_str());
		CHECK_CONDITION_ALWAYS(m_vector.IsNormalized(), Utility::Error,
			"Calculated normalized vector %s is in fact not normalized.",
			m_vector.ToString().c_str());
	}
protected:
	T m_expectedNormalizedVector;
}; /* end class VectorTestNormalize */


class Vector2DTestCross : public VectorTestBase<Math::Vector2D>
{
public:
	Vector2DTestCross(const Math::Vector2D& vector1, const Math::Vector2D& vector2, Math::Real expectedCrossResult);
	virtual ~Vector2DTestCross();
public:
	virtual void StartTest();
protected:
	Math::Vector2D m_vector2;
	Math::Real m_expectedCrossResult;
}; /* end class Vector2DTestCross */

class Vector3DTestCross : public VectorTestBase<Math::Vector3D>
{
public:
	Vector3DTestCross(const Math::Vector3D& vector1, const Math::Vector3D& vector2, const Math::Vector3D& expectedCrossResult);
	virtual ~Vector3DTestCross();
public:
	virtual void StartTest();
protected:
	Math::Vector3D m_vector2;
	Math::Vector3D m_expectedCrossResult;
}; /* end class Vector3DTestCross */

template <class T>
class VectorTestDot : public VectorTestBase<T>
{
public:
	VectorTestDot(const T& vector1, const T& vector2, Math::Real expectedDotResult) :
		VectorTestBase<T>(vector1),
		m_vector2(vector2),
		m_expectedDotResult(expectedDotResult)
	{
	}
	virtual ~VectorTestDot()
	{
	}
public:
	virtual void StartTest()
	{
		Math::Real dotResult = m_vector.Dot(m_vector2);
		CHECK_CONDITION_ALWAYS(Math::AlmostEqual(dotResult, m_expectedDotResult), Utility::Error,
			"The dot product of vectors %s and %s equals %.3f. It is different than expected %.3f",
			m_vector.ToString().c_str(), m_vector2.ToString().c_str(), dotResult, m_expectedDotResult);
	}
protected:
	T m_vector2;
	Math::Real m_expectedDotResult;
}; /* end class VectorTestDot */

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