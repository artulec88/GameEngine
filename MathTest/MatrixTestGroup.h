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
		MatrixTestGroup(const std::string& quaternionTestGroupName, int testTimeIterationsCount);
		virtual ~MatrixTestGroup(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
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
		virtual void StartTimeTest(unsigned int iterationsCount);
	protected:
		Math::Matrix4D m_compareMatrix;
		bool m_expectedCompareResult;
	}; /* end class MatrixTestCompare */

	class MatrixTestMultiplyByMatrixOperator : public MatrixTestBase
	{
	public:
		MatrixTestMultiplyByMatrixOperator(const Math::Matrix4D& matrix1, const Math::Matrix4D& matrix2, const Math::Matrix4D& expectedMultiplyResultMatrix);
		virtual ~MatrixTestMultiplyByMatrixOperator();
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
	protected:
		Math::Matrix4D m_matrix2;
		Math::Matrix4D m_expectedMultiplyResultMatrix;
	}; /* end class MatrixTestMultiplyByMatrixOperator */

	class MatrixTestMultiplyByVectorOperator : public MatrixTestBase
	{
	public:
		MatrixTestMultiplyByVectorOperator(const Math::Matrix4D& matrix1, const Math::Vector3D& vector, const Math::Vector3D& expectedMultiplyResultVector);
		virtual ~MatrixTestMultiplyByVectorOperator();
	public:
		virtual void StartTest();
		virtual void StartTimeTest(unsigned int iterationsCount);
	protected:
		Math::Vector3D m_vector;
		Math::Vector3D m_expectedMultiplyResultVector;
	}; /* end class MatrixTestMultiplyByVectorOperator */

} /* end namespace MathTest */

#endif /* __MATH_TEST_MATRIX_TEST_GROUP_H__ */