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
	Math::Real length = m_vector.Length();
	CHECK_CONDITION_ALWAYS(Math::AlmostEqual(length, m_expectedLength), Utility::Error,
		"The vector %s has length %.2f, but was expected to have a length %.2f",
		m_vector.ToString().c_str(), length, m_expectedLength);
}
/* ==================== class Vector2DTestLength end ==================== */