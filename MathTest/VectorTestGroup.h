#ifndef __MATH_TEST_VECTOR_TEST_GROUP_H__
#define __MATH_TEST_VECTOR_TEST_GROUP_H__

#include "Test.h"

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
protected:
	virtual void StartTests();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
};

} /* end namespace MathTest */