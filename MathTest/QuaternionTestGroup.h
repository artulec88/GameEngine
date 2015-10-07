#ifndef __MATH_TEST_QUATERNION_TEST_GROUP_H__
#define __MATH_TEST_QUATERNION_TEST_GROUP_H__

#include "Test.h"
#include "Math\Quaternion.h"

namespace MathTest
{

class QuaternionTestGroup : public TestGroup
{
/* ==================== Constructors and destructors begin ==================== */
public:
	QuaternionTestGroup(void);
	virtual ~QuaternionTestGroup(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void StartTests();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class QuaternionTestGroup */

class QuaternionTestBase : public Test
{
/* ==================== Constructors and destructors begin ==================== */
protected:
	QuaternionTestBase(const Math::Quaternion& quaternion);
	virtual ~QuaternionTestBase(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Math::Quaternion m_quaternion;
/* ==================== Non-static member variables end ==================== */
}; /* end class QuaternionTestBase */

class QuaternionTestCompare : public QuaternionTestBase
{
public:
	QuaternionTestCompare(const Math::Quaternion& quaternion1, const Math::Quaternion& quaternion2, bool expectedCompareResult);
	virtual ~QuaternionTestCompare();
public:
	virtual void StartTest();
protected:
	Math::Quaternion m_compareQuaternion;
	bool m_expectedCompareResult;
}; /* end class QuaternionTestCompare */

class QuaternionTestLength : public QuaternionTestBase
{
public:
	QuaternionTestLength(const Math::Quaternion& quaternion, Math::Real expectedLength);
	virtual ~QuaternionTestLength();
public:
	virtual void StartTest();
protected:
	Math::Real m_expectedLength;
}; /* end class QuaternionTestLength */

} /* end namespace MathTest */

#endif /* __MATH_TEST_QUATERNION_TEST_GROUP_H__ */