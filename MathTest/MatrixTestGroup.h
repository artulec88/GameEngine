#ifndef __MATH_TEST_MATRIX_TEST_GROUP_H__
#define __MATH_TEST_MATRIX_TEST_GROUP_H__

#include "Test.h"
#include "Math\Matrix.h"

namespace MathTest
{

class MatrixTestGroup : public TestGroup
{
/* ==================== Constructors and destructors begin ==================== */
public:
	MatrixTestGroup(void);
	virtual ~MatrixTestGroup(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void StartTests();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class MatrixTestGroup */

class MatrixTestBase : public Test
{
/* ==================== Constructors and destructors begin ==================== */
protected:
	MatrixTestBase(const Math::Matrix4D& matrix);
	virtual ~MatrixTestBase(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Math::Matrix4D m_matrix;
/* ==================== Non-static member variables end ==================== */
}; /* end class MatrixTestBase */

class MatrixTestCompare : public MatrixTestBase
{
public:
	MatrixTestCompare(const Math::Matrix4D& matrix1, const Math::Matrix4D& matrix2, bool expectedCompareResult);
	virtual ~MatrixTestCompare();
public:
	virtual void StartTest();
protected:
	Math::Matrix4D m_compareMatrix;
	bool m_expectedCompareResult;
}; /* end class MatrixTestCompare */

template <class T> // T may stand for Vector3D or Matrix4D
class MatrixTestMultiplyOperator : public MatrixTestBase
{
public:
	MatrixTestMultiplyOperator(const Math::Matrix4D& matrix1, const T& values, const T& expectedMultiplyResult) :
		MatrixTestBase(matrix1),
		m_values(values),
		m_expectedMultiplyResult(expectedMultiplyResult)
	{
	}
	virtual ~MatrixTestMultiplyOperator()
	{
	}
public:
	virtual void StartTest()
	{
		T multiplyResult = m_matrix * m_values;
		CHECK_CONDITION_ALWAYS(multiplyResult == m_expectedMultiplyResult, Utility::Error,
			"The multiplication of matrix %s and %s equals %s. It is different than expected %s",
			m_matrix.ToString().c_str(), m_values.ToString().c_str(), multiplyResult.ToString().c_str(),
			m_expectedMultiplyResult.ToString().c_str());
	}
protected:
	T m_values;
	T m_expectedMultiplyResult;
}; /* end class MatrixTestMultiplyOperator */

} /* end namespace MathTest */

#endif /* __MATH_TEST_MATRIX_TEST_GROUP_H__ */