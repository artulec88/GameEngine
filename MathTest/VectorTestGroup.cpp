#include "VectorTestGroup.h"
#include "Math\Vector.h"
#include "Math\FloatingPoint.h"

using namespace MathTest;

/* ==================== class VectorTestGroup begin ==================== */
VectorTestGroup::VectorTestGroup(void) :
	TestGroup()
{
	NOTICE_LOG("Vector test started");
}


VectorTestGroup::~VectorTestGroup(void)
{
	NOTICE_LOG("Vector test finished");
}

void VectorTestGroup::StartTests()
{
	if (!IsTestGroupEnabled())
	{
		return;
	}
}
/* ==================== class VectorTestGroup end ==================== */


/* ==================== class Vector2DTestBase begin ==================== */
Vector2DTestBase::Vector2DTestBase(const Math::Vector2D& vector) :
	Test(),
	m_vector(vector)
{
}

Vector2DTestBase::~Vector2DTestBase(void)
{
}
/* ==================== class Vector2DTestBase end ==================== */


/* ==================== class Vector2DTestCompare begin ==================== */
Vector2DTestCompare::Vector2DTestCompare(const Math::Vector2D& vector1, const Math::Vector2D& vector2, bool expectedCompareResult) :
	Vector2DTestBase(vector1),
	m_compareVector(vector2),
	m_expectedCompareResult(expectedCompareResult)
{
}

Vector2DTestCompare::~Vector2DTestCompare(void)
{
}

void Vector2DTestCompare::StartTest()
{
	bool compareResult = (m_vector == m_compareVector);
	CHECK_CONDITION_ALWAYS(compareResult == m_expectedCompareResult, Utility::Error,
		"The vector %s and vector %s comparison result not as expected.",
		m_vector.ToString().c_str(), m_compareVector.ToString().c_str());
}
/* ==================== class Vector2DTestBase end ==================== */


/* ==================== class Vector2DTestLength begin ==================== */
Vector2DTestLength::Vector2DTestLength(const Math::Vector2D& vector, Math::Real expectedLength) :
	Vector2DTestBase(vector),
	m_expectedLength(expectedLength)
{
}

Vector2DTestLength::~Vector2DTestLength(void)
{
}

void Vector2DTestLength::StartTest()
{
	Math::Real lengthSquared = m_vector.LengthSquared();
	Math::Real length = m_vector.Length();
	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(length, m_expectedLength), Utility::Error,
		"The vector %s has length %.2f, but was expected to have a length %.2f",
		m_vector.ToString().c_str(), length, m_expectedLength);
	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(lengthSquared, m_expectedLength * m_expectedLength), Utility::Error,
		"The vector %s has length %.2f and squared length %.2f, but expected to have a squared length %.2f",
		m_vector.ToString().c_str(), lengthSquared, m_expectedLength * m_expectedLength);
}
/* ==================== class Vector2DTestLength end ==================== */


/* ==================== class Vector2DTestAddOperator begin ==================== */
Vector2DTestAddOperator::Vector2DTestAddOperator(const Math::Vector2D& vector1, const Math::Vector2D& vector2, const Math::Vector2D& expectedSumVector) :
	Vector2DTestBase(vector1),
	m_vector2(vector2),
	m_expectedSumVector(expectedSumVector)
{
}

Vector2DTestAddOperator::~Vector2DTestAddOperator(void)
{
}

void Vector2DTestAddOperator::StartTest()
{
	Math::Vector2D sumVector = m_vector + m_vector2;
	CHECK_CONDITION_ALWAYS(sumVector == m_expectedSumVector, Utility::Error,
		"The sum of vectors %s and %s is a vector %s. It is different than expected %s",
		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), sumVector.ToString().c_str(),
		m_expectedSumVector.ToString().c_str());

	m_vector += m_vector2;
	CHECK_CONDITION_ALWAYS(m_vector == m_expectedSumVector, Utility::Error,
		"The operator += returned different result than operator + (%s != %s)",
		m_vector.ToString().c_str(), m_expectedSumVector.ToString().c_str());
}
/* ==================== class Vector2DTestAddOperator end ==================== */


/* ==================== class Vector2DTestSubtractOperator begin ==================== */
Vector2DTestSubtractOperator::Vector2DTestSubtractOperator(const Math::Vector2D& vector1, const Math::Vector2D& vector2, const Math::Vector2D& expectedSubtractVector) :
	Vector2DTestBase(vector1),
	m_vector2(vector2),
	m_expectedSubtractVector(expectedSubtractVector)
{
}

Vector2DTestSubtractOperator::~Vector2DTestSubtractOperator(void)
{
}

void Vector2DTestSubtractOperator::StartTest()
{
	Math::Vector2D subtractVector = m_vector - m_vector2;
	CHECK_CONDITION_ALWAYS(subtractVector == m_expectedSubtractVector, Utility::Error,
		"The subtraction of vectors %s and %s is a vector %s. It is different than expected %s",
		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), subtractVector.ToString().c_str(),
		m_expectedSubtractVector.ToString().c_str());

	m_vector -= m_vector2;
	CHECK_CONDITION_ALWAYS(m_vector == m_expectedSubtractVector, Utility::Error,
		"The operator -= returned different result than operator - (%s != %s)",
		m_vector.ToString().c_str(), m_expectedSubtractVector.ToString().c_str());
}
/* ==================== class Vector2DTestSubtractOperator end ==================== */


/* ==================== class Vector2DTestMultiplyOperator begin ==================== */
Vector2DTestMultiplyOperator::Vector2DTestMultiplyOperator(const Math::Vector2D& vector1, const Math::Vector2D& vector2, const Math::Vector2D& expectedMultiplyVector) :
	Vector2DTestBase(vector1),
	m_vector2(vector2),
	m_expectedMultiplyVector(expectedMultiplyVector)
{
}

Vector2DTestMultiplyOperator::~Vector2DTestMultiplyOperator(void)
{
}

void Vector2DTestMultiplyOperator::StartTest()
{
	Math::Vector2D multiplyVector = m_vector * m_vector2;
	CHECK_CONDITION_ALWAYS(multiplyVector == m_expectedMultiplyVector, Utility::Error,
		"The multiplication of vectors %s and %s is a vector %s. It is different than expected %s",
		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), multiplyVector.ToString().c_str(),
		m_expectedMultiplyVector.ToString().c_str());

	m_vector *= m_vector2;
	CHECK_CONDITION_ALWAYS(m_vector == m_expectedMultiplyVector, Utility::Error,
		"The operator *= returned different result than operator * (%s != %s)",
		m_vector.ToString().c_str(), m_expectedMultiplyVector.ToString().c_str());
}
/* ==================== class Vector2DTestMultiplyOperator end ==================== */


/* ==================== class Vector2DTestDivideOperator begin ==================== */
Vector2DTestDivideOperator::Vector2DTestDivideOperator(const Math::Vector2D& vector1, const Math::Vector2D& vector2, const Math::Vector2D& expectedDivideVector) :
	Vector2DTestBase(vector1),
	m_vector2(vector2),
	m_expectedDivideVector(expectedDivideVector)
{
}

Vector2DTestDivideOperator::~Vector2DTestDivideOperator(void)
{
}

void Vector2DTestDivideOperator::StartTest()
{
	Math::Vector2D divideVector = m_vector / m_vector2;
	CHECK_CONDITION_ALWAYS(divideVector == m_expectedDivideVector, Utility::Error,
		"The division of vectors %s and %s is a vector %s. It is different than expected %s",
		m_vector.ToString().c_str(), m_vector2.ToString().c_str(), divideVector.ToString().c_str(),
		m_expectedDivideVector.ToString().c_str());

	m_vector /= m_vector2;
	CHECK_CONDITION_ALWAYS(m_vector == m_expectedDivideVector, Utility::Error,
		"The operator /= returned different result than operator / (%s != %s)",
		m_vector.ToString().c_str(), m_expectedDivideVector.ToString().c_str());
}
/* ==================== class Vector2DTestDivideOperator end ==================== */