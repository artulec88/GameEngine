#ifndef __MATH_TEST_VECTOR_TEST_GROUP_H__
#define __MATH_TEST_VECTOR_TEST_GROUP_H__

#include "Test.h"
#include "Math\Vector.h"

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

class Vector2DTestBase : public Test
{
/* ==================== Constructors and destructors begin ==================== */
protected:
	Vector2DTestBase(const Math::Vector2D& vector);
	virtual ~Vector2DTestBase(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Math::Vector2D m_vector;
/* ==================== Non-static member variables end ==================== */
}; /* end class Vector2DTestBase */

class Vector2DTestCompare : public Vector2DTestBase
{
public:
	Vector2DTestCompare(const Math::Vector2D& vec1, const Math::Vector2D& vec2, bool expectedCompareResult);
	virtual ~Vector2DTestCompare();
public:
	virtual void StartTest();
protected:
	Math::Vector2D m_compareVector;
	bool m_expectedCompareResult;
}; /* end class Vector2DTestCompare */

class Vector2DTestLength : public Vector2DTestBase
{
public:
	Vector2DTestLength(const Math::Vector2D& vec, Math::Real expectedLength);
	virtual ~Vector2DTestLength();
public:
	virtual void StartTest();
protected:
	Math::Real m_expectedLength;
}; /* end class Vector2DTestLength */

} /* end namespace MathTest */

#endif /* __MATH_TEST_VECTOR_TEST_GROUP_H__ */