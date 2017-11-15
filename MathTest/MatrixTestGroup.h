#ifndef __MATH_TEST_MATRIX_TEST_GROUP_H__
#define __MATH_TEST_MATRIX_TEST_GROUP_H__

#include "Test.h"
#include "Math/Matrix.h"

namespace math_test
{

	class MatrixTestGroup : public TestGroup
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		MatrixTestGroup(const std::string& quaternionTestGroupName, int testTimeIterationsCount);
		virtual ~MatrixTestGroup();
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
		explicit MatrixTestBase(const math::Matrix4D& matrix);
		virtual ~MatrixTestBase();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Matrix4D m_matrix;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MatrixTestBase */

	class MatrixTestCompare : public MatrixTestBase
	{
	public:
		MatrixTestCompare(const math::Matrix4D& matrix1, const math::Matrix4D& matrix2, bool expectedCompareResult);
		virtual ~MatrixTestCompare();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		math::Matrix4D m_compareMatrix;
		bool m_expectedCompareResult;
	}; /* end class MatrixTestCompare */

	class MatrixTestMultiplyByMatrixOperator : public MatrixTestBase
	{
	public:
		MatrixTestMultiplyByMatrixOperator(const math::Matrix4D& matrix1, const math::Matrix4D& matrix2, const math::Matrix4D& expectedMultiplyResultMatrix);
		virtual ~MatrixTestMultiplyByMatrixOperator();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		math::Matrix4D m_matrix2;
		math::Matrix4D m_expectedMultiplyResultMatrix;
	}; /* end class MatrixTestMultiplyByMatrixOperator */

	class MatrixTestMultiplyByVectorOperator : public MatrixTestBase
	{
	public:
		MatrixTestMultiplyByVectorOperator(const math::Matrix4D& matrix1, const math::Vector3D& vector, const math::Vector3D& expectedMultiplyResultVector);
		virtual ~MatrixTestMultiplyByVectorOperator();
	public:
		virtual void StartTest() override;
		virtual void StartTimeTest(unsigned int iterationsCount) override;
	protected:
		math::Vector3D m_vector;
		math::Vector3D m_expectedMultiplyResultVector;
	}; /* end class MatrixTestMultiplyByVectorOperator */

} /* end namespace MathTest */

#endif /* __MATH_TEST_MATRIX_TEST_GROUP_H__ */