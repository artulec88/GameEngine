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

