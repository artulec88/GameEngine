#include "MatrixTestGroup.h"

using namespace MathTest;

/* ==================== class MatrixTestGroup begin ==================== */
MatrixTestGroup::MatrixTestGroup(void) :
	TestGroup()
{
	NOTICE_LOG("Matrix tests started");
}


MatrixTestGroup::~MatrixTestGroup(void)
{
	NOTICE_LOG("Matrix tests finished");
}

void MatrixTestGroup::StartTests()
{
	if (!IsTestGroupEnabled())
	{
		return;
	}
	TestGroup::StartTests();
}
/* ==================== class MatrixTestGroup end ==================== */

/* ==================== class MatrixTestBase begin ==================== */
MatrixTestBase::MatrixTestBase(const Math::Matrix4D& matrix) :
	Test(),
	m_matrix(matrix)
{
}

MatrixTestBase::~MatrixTestBase(void)
{
}
/* ==================== class MatrixTestBase end ==================== */

/* ==================== class MatrixTestCompare begin ==================== */
MatrixTestCompare::MatrixTestCompare(const Math::Matrix4D& matrix1, const Math::Matrix4D& matrix2, bool expectedCompareResult) :
	MatrixTestBase(matrix1),
	m_compareMatrix(matrix2),
	m_expectedCompareResult(expectedCompareResult)
{
}

MatrixTestCompare::~MatrixTestCompare(void)
{
}

void MatrixTestCompare::StartTest()
{
	bool compareResult = (m_matrix == m_compareMatrix);
	CHECK_CONDITION_ALWAYS(compareResult == m_expectedCompareResult, Utility::Error,
		"The matrix %s and matrix %s comparison result is not as expected.",
		m_matrix.ToString().c_str(), m_compareMatrix.ToString().c_str());

	// TODO: Test != operator
}
/* ==================== class MatrixTestCompare end ==================== */


/* ==================== class MatrixTestMultiplyByMatrixOperator begin ==================== */
MatrixTestMultiplyByMatrixOperator::MatrixTestMultiplyByMatrixOperator(const Math::Matrix4D& matrix1, const Math::Matrix4D& matrix2, const Math::Matrix4D& expectedMultiplyResultMatrix) :
	MatrixTestBase(matrix1),
	m_matrix2(matrix2),
	m_expectedMultiplyResultMatrix(expectedMultiplyResultMatrix)
{
}
MatrixTestMultiplyByMatrixOperator::~MatrixTestMultiplyByMatrixOperator()
{
}

void MatrixTestMultiplyByMatrixOperator::StartTest()
{
	Math::Matrix4D multiplyResultMatrix = m_matrix * m_matrix2;
	CHECK_CONDITION_ALWAYS(multiplyResultMatrix == m_expectedMultiplyResultMatrix, Utility::Error,
		"The multiplication of matrices %s and %s equals %s. It is different than expected %s",
		m_matrix.ToString().c_str(), m_matrix2.ToString().c_str(), multiplyResultMatrix.ToString().c_str(),
		m_expectedMultiplyResultMatrix.ToString().c_str());
}
/* ==================== class MatrixTestMultiplyByMatrixOperator end ==================== */


/* ==================== class MatrixTestMultiplyByVectorOperator begin ==================== */
MatrixTestMultiplyByVectorOperator::MatrixTestMultiplyByVectorOperator(const Math::Matrix4D& matrix, const Math::Vector3D& vector, const Math::Vector3D& expectedMultiplyResultVector) :
	MatrixTestBase(matrix),
	m_vector(vector),
	m_expectedMultiplyResultVector(expectedMultiplyResultVector)
{
}
MatrixTestMultiplyByVectorOperator::~MatrixTestMultiplyByVectorOperator()
{
}

void MatrixTestMultiplyByVectorOperator::StartTest()
{
	Math::Vector3D multiplyResultVector = m_matrix * m_vector;
	CHECK_CONDITION_ALWAYS(multiplyResultVector == m_expectedMultiplyResultVector, Utility::Error,
		"The multiplication of matrix:\n%s and vector %s equals %s. It is different than expected %s.",
		m_matrix.ToString().c_str(), m_vector.ToString().c_str(), multiplyResultVector.ToString().c_str(),
		m_expectedMultiplyResultVector.ToString().c_str());
}
/* ==================== class MatrixTestMultiplyByVectorOperator end ==================== */