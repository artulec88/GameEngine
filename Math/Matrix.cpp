#include "StdAfx.h"
#include "Matrix.h"
#include "FloatingPoint.h"

#include "Utility\Utility.h"

#include <sstream>

using namespace Math;
using namespace Utility;

#ifdef CALCULATE_MATH_STATS
/* static */ Statistics::ClassStats& Matrix4D::s_classStats(STATS_STORAGE.GetClassStats("Matrix4DStatic"));
#endif

/* static */ const Matrix4D Matrix4D::IDENTITY_MATRIX;

/* static */ int Matrix4D::Signum(int i, int j)
{
	return ((i + j) % 2) ? -1 : 1;
}

Matrix4D::Matrix4D()
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	m[0][0] = REAL_ONE;		m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;	m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = REAL_ONE;		m[1][2] = REAL_ZERO;	m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = REAL_ONE;		m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = REAL_ZERO;	m[3][3] = REAL_ONE;
	STOP_PROFILING;
}

Matrix4D::Matrix4D(Real scale)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	SetScaleMatrix(scale, scale, scale);
	STOP_PROFILING;
}

Matrix4D::Matrix4D(Real posX, Real posY, Real posZ)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	m[0][0] = REAL_ONE;		m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;	m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = REAL_ONE;		m[1][2] = REAL_ZERO;	m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = REAL_ONE;		m[2][3] = REAL_ZERO;
	m[3][0] = posX;			m[3][1] = posY;			m[3][2] = posZ;			m[3][3] = REAL_ONE;
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Vector3D& pos)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	m[0][0] = REAL_ONE;		m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;	m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = REAL_ONE;		m[1][2] = REAL_ZERO;	m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = REAL_ONE;		m[2][3] = REAL_ZERO;
	m[3][0] = pos.GetX();	m[3][1] = pos.GetY();	m[3][2] = pos.GetZ();	m[3][3] = REAL_ONE;
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Angle& angleX, const Angle& angleY)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	Real xSin = angleX.Sin();
	Real xCos = angleX.Cos();
	Real ySin = angleY.Sin();
	Real yCos = angleY.Cos();

	m[0][0] = yCos;			m[0][1] = REAL_ZERO;	m[0][2] = ySin;			m[0][3] = REAL_ZERO;
	m[1][0] = -xSin * ySin;	m[1][1] = xCos;			m[1][2] = xSin * yCos;	m[1][3] = REAL_ZERO;
	m[2][0] = -xCos * ySin;	m[2][1] = -xSin;		m[2][2] = xCos * yCos;	m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = REAL_ZERO;	m[3][3] = REAL_ONE;

	CHECK_CONDITION(rot == Matrix4D(angleX, angleY, Angle(REAL_ZERO)), Utility::Error, "Two RotationEuler functions give different results.");
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Angle& angleX, const Angle& angleY, const Angle& angleZ)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING; // TODO: As there are two methods with the same name "RotationEuler" their stats will be stored in the same place (which is not good). Fix it!
	/* ==================== SOLUTION #1 begin ==================== */
	//Matrix4D rotX, rotY, rotZ; // rotation around X, Y and Z axis respectively

	//Real xSin = angleX.Sin();
	//Real xCos = angleX.Cos();
	//Real ySin = angleY.Sin();
	//Real yCos = angleY.Cos();
	//Real zSin = angleZ.Sin();
	//Real zCos = angleZ.Cos();

	//rotX.m[0][0] = REAL_ONE;	rotX.m[0][1] = REAL_ZERO;	rotX.m[0][2] = REAL_ZERO;	rotX.m[0][3] = REAL_ZERO;
	//rotX.m[1][0] = REAL_ZERO;	rotX.m[1][1] = xCos;		rotX.m[1][2] = xSin;		rotX.m[1][3] = REAL_ZERO;
	//rotX.m[2][0] = REAL_ZERO;	rotX.m[2][1] = -xSin;		rotX.m[2][2] = xCos;		rotX.m[2][3] = REAL_ZERO;
	//rotX.m[3][0] = REAL_ZERO;	rotX.m[3][1] = REAL_ZERO;	rotX.m[3][2] = REAL_ZERO;	rotX.m[3][3] = REAL_ONE;

	//rotY.m[0][0] = yCos;		rotY.m[0][1] = REAL_ZERO;	rotY.m[0][2] = ySin;		rotY.m[0][3] = REAL_ZERO;
	//rotY.m[1][0] = REAL_ZERO;	rotY.m[1][1] = REAL_ONE;	rotY.m[1][2] = REAL_ZERO;	rotY.m[1][3] = REAL_ZERO;
	//rotY.m[2][0] = -ySin;		rotY.m[2][1] = REAL_ZERO;	rotY.m[2][2] = yCos;		rotY.m[2][3] = REAL_ZERO;
	//rotY.m[3][0] = REAL_ZERO;	rotY.m[3][1] = REAL_ZERO;	rotY.m[3][2] = REAL_ZERO;	rotY.m[3][3] = REAL_ONE;

	//rotZ.m[0][0] = zCos;		rotZ.m[0][1] = zSin;		rotZ.m[0][2] = REAL_ZERO;	rotZ.m[0][3] = REAL_ZERO;
	//rotZ.m[1][0] = -zSin;		rotZ.m[1][1] = zCos;		rotZ.m[1][2] = REAL_ZERO;	rotZ.m[1][3] = REAL_ZERO;
	//rotZ.m[2][0] = REAL_ZERO;	rotZ.m[2][1] = REAL_ZERO;	rotZ.m[2][2] = REAL_ONE;	rotZ.m[2][3] = REAL_ZERO;
	//rotZ.m[3][0] = REAL_ZERO;	rotZ.m[3][1] = REAL_ZERO;	rotZ.m[3][2] = REAL_ZERO;	rotZ.m[3][3] = REAL_ONE;

	//return rotZ * rotY * rotX;
	/* ==================== SOLUTION #1 end ==================== */

	/* ==================== SOLUTION #2 begin ==================== */
	Real xSin = angleX.Sin();
	Real xCos = angleX.Cos();
	Real ySin = angleY.Sin();
	Real yCos = angleY.Cos();
	Real zSin = angleZ.Sin();
	Real zCos = angleZ.Cos();

	m[0][0] = yCos * zCos;							m[0][1] = yCos * zSin;							m[0][2] = ySin;			m[0][3] = REAL_ZERO;
	m[1][0] = -xSin * ySin * zCos - xCos * zSin;	m[1][1] = -xSin * ySin * zSin + xCos * zCos;	m[1][2] = xSin * yCos;	m[1][3] = REAL_ZERO;
	m[2][0] = -xCos * ySin * zCos + xSin * zSin;	m[2][1] = -xCos * ySin * zSin - xSin * zCos;	m[2][2] = xCos * yCos;	m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;							m[3][1] = REAL_ZERO;							m[3][2] = REAL_ZERO;	m[3][3] = REAL_ONE;

	//Matrix4D matrixToCompare = rotZ * rotY * rotX;
	//if (rot != matrixToCompare)
	//{
	//	ERROR_LOG("Incorrect euler rotation calculation. Rot =\n%s\nInstead it should be equal to\n%s", rot.ToString().c_str(), matrixToCompare.ToString().c_str());
	//}
	STOP_PROFILING;
	/* ==================== SOLUTION #2 end ==================== */
}

Matrix4D::Matrix4D(const Vector3D& forward, const Vector3D& up, const Vector3D& right)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	SetRotationFromVectors(forward, up, right);
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Vector3D& forward, const Vector3D& up)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	Vector3D n = forward.Normalized();
	Vector3D u = up.Normalized().Cross(n);
	Vector3D v = n.Cross(u);
	SetRotationFromVectors(n, v, u);
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Angle& fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	SetPerspectiveProjection(fov, aspect, nearPlane, farPlane);
	STOP_PROFILING;
}

Matrix4D::Matrix4D(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;

	const Real width = right - left;
	const Real height = top - bottom;
	const Real depth = farPlane - nearPlane;
	const Real temp = static_cast<Real>(2);
	
	m[0][0] = temp / width;				m[0][1] = REAL_ZERO;				m[0][2] = REAL_ZERO;						m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;				m[1][1] = temp / height;			m[1][2] = REAL_ZERO;						m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;				m[2][1] = REAL_ZERO;				m[2][2] = -temp / depth;					m[2][3] = REAL_ZERO;
	m[3][0] = -(right + left) / width;	m[3][1] = -(top + bottom) / height;	m[3][2] = -(farPlane + nearPlane) / depth;	m[3][3] = REAL_ONE;
	STOP_PROFILING;
}

Matrix4D::Matrix4D(const Matrix4D& mat)
#ifdef CALCULATE_MATH_STATS
	: m_classStats(STATS_STORAGE.GetClassStats("Matrix4D"))
#endif
{
	START_PROFILING;
	// TODO: Check which of the three solution is faster
	
	/* ==================== SOLUTION #1 begin ==================== */
	m[0][0] = mat[0][0];	m[0][1] = mat[0][1];	m[0][2] = mat[0][2];	m[0][3] = mat[0][3];
	m[1][0] = mat[1][0];	m[1][1] = mat[1][1];	m[1][2] = mat[1][2];	m[1][3] = mat[1][3];
	m[2][0] = mat[2][0];	m[2][1] = mat[2][1];	m[2][2] = mat[2][2];	m[2][3] = mat[2][3];
	m[3][0] = mat[3][0];	m[3][1] = mat[3][1];	m[3][2] = mat[3][2];	m[3][3] = mat[3][3];
	/* ==================== SOLUTION #1 end ==================== */
	
	/* ==================== SOLUTION #2 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		m[i][j] = mat[i][j];
	//	}
	//}
	/* ==================== SOLUTION #2 end ==================== */
	
	/* ==================== SOLUTION #3 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		this->SetElement(i, j, mat.GetElement(i, j));
	//	}
	//}
	/* ==================== SOLUTION #3 end ==================== */
	
	CHECK_CONDITION((*this) == mat, Utility::Error, "The copy constructor should cause the two matrices to be equal, but they are not.");
	STOP_PROFILING;
}

Matrix4D::~Matrix4D()
{
}

std::string Matrix4D::ToString() const
{
	const std::string INDENTATION_STRING = " ";
	std::stringstream s("");
	s << m[0][0] << INDENTATION_STRING;
	s << m[0][1] << INDENTATION_STRING;
	s << m[0][2] << INDENTATION_STRING;
	s << m[0][3] << INDENTATION_STRING;
	s << std::endl;

	s << m[1][0] << INDENTATION_STRING;
	s << m[1][1] << INDENTATION_STRING;
	s << m[1][2] << INDENTATION_STRING;
	s << m[1][3] << INDENTATION_STRING;
	s << std::endl;

	s << m[2][0] << INDENTATION_STRING;
	s << m[2][1] << INDENTATION_STRING;
	s << m[2][2] << INDENTATION_STRING;
	s << m[2][3] << INDENTATION_STRING;
	s << std::endl;

	s << m[3][0] << INDENTATION_STRING;
	s << m[3][1] << INDENTATION_STRING;
	s << m[3][2] << INDENTATION_STRING;
	s << m[3][3] << INDENTATION_STRING;
	s << std::endl;

	return s.str();
}
	
Matrix4D Matrix4D::operator*(const Matrix4D& mat) const
{
	START_PROFILING;
	/* ==================== SOLUTION #1 (doesn't work!!!) begin ==================== */
	//Matrix4D matrix;

	//matrix.m[0][0] = m[0][0] * mat.m[0][0] + m[1][0] * mat.m[0][1] + m[2][0] * mat.m[0][2];
	//matrix.m[0][1] = m[0][1] * mat.m[0][0] + m[1][1] * mat.m[0][1] + m[2][1] * mat.m[0][2];
	//matrix.m[0][2] = m[0][2] * mat.m[0][0] + m[1][2] * mat.m[0][1] + m[2][2] * mat.m[0][2];
	//matrix.m[0][3] = REAL_ZERO;

	//matrix.m[1][0] = m[0][0] * mat.m[1][0] + m[1][0] * mat.m[1][1] + m[2][0] * mat.m[1][2];
	//matrix.m[1][1] = m[0][1] * mat.m[1][0] + m[1][1] * mat.m[1][1] + m[2][1] * mat.m[1][2];
	//matrix.m[1][2] = m[0][2] * mat.m[1][0] + m[1][2] * mat.m[1][1] + m[2][2] * mat.m[1][2];
	//matrix.m[1][3] = REAL_ZERO;

	//matrix.m[2][0] = m[0][0] * mat.m[2][0] + m[1][0] * mat.m[2][1] + m[2][0] * mat.m[2][2];
	//matrix.m[2][1] = m[0][1] * mat.m[2][0] + m[1][1] * mat.m[2][1] + m[2][1] * mat.m[2][2];
	//matrix.m[2][2] = m[0][2] * mat.m[2][0] + m[1][2] * mat.m[2][1] + m[2][2] * mat.m[2][2];
	//matrix.m[2][3] = REAL_ZERO;

	//matrix.m[3][0] = m[0][0] * mat.m[3][0] + m[1][0] * mat.m[3][1] + m[2][0] * mat.m[3][2] + m[3][0];
	//matrix.m[3][1] = m[0][1] * mat.m[3][0] + m[1][1] * mat.m[3][1] + m[2][1] * mat.m[3][2] + m[3][1];
	//matrix.m[3][2] = m[0][2] * mat.m[3][0] + m[1][2] * mat.m[3][1] + m[2][2] * mat.m[3][2] + m[3][2];
	//matrix.m[3][3] = REAL_ONE;
	//
	//return matrix;
	/* ==================== SOLUTION #1 (doesn't work!!!) end ==================== */

	/* ==================== SOLUTION #2 begin ==================== */
	Matrix4D matrix;

	matrix.m[0][0] = m[0][0] * mat.m[0][0] + m[1][0] * mat.m[0][1] + m[2][0] * mat.m[0][2] + m[3][0] * mat.m[0][3];
	matrix.m[0][1] = m[0][1] * mat.m[0][0] + m[1][1] * mat.m[0][1] + m[2][1] * mat.m[0][2] + m[3][1] * mat.m[0][3];
	matrix.m[0][2] = m[0][2] * mat.m[0][0] + m[1][2] * mat.m[0][1] + m[2][2] * mat.m[0][2] + m[3][2] * mat.m[0][3];
	matrix.m[0][3] = m[0][3] * mat.m[0][0] + m[1][3] * mat.m[0][1] + m[2][3] * mat.m[0][2] + m[3][3] * mat.m[0][3];

	matrix.m[1][0] = m[0][0] * mat.m[1][0] + m[1][0] * mat.m[1][1] + m[2][0] * mat.m[1][2] + m[3][0] * mat.m[1][3];
	matrix.m[1][1] = m[0][1] * mat.m[1][0] + m[1][1] * mat.m[1][1] + m[2][1] * mat.m[1][2] + m[3][1] * mat.m[1][3];
	matrix.m[1][2] = m[0][2] * mat.m[1][0] + m[1][2] * mat.m[1][1] + m[2][2] * mat.m[1][2] + m[3][2] * mat.m[1][3];
	matrix.m[1][3] = m[0][3] * mat.m[1][0] + m[1][3] * mat.m[1][1] + m[2][3] * mat.m[1][2] + m[3][3] * mat.m[1][3];

	matrix.m[2][0] = m[0][0] * mat.m[2][0] + m[1][0] * mat.m[2][1] + m[2][0] * mat.m[2][2] + m[3][0] * mat.m[2][3];
	matrix.m[2][1] = m[0][1] * mat.m[2][0] + m[1][1] * mat.m[2][1] + m[2][1] * mat.m[2][2] + m[3][1] * mat.m[2][3];
	matrix.m[2][2] = m[0][2] * mat.m[2][0] + m[1][2] * mat.m[2][1] + m[2][2] * mat.m[2][2] + m[3][2] * mat.m[2][3];
	matrix.m[2][3] = m[0][3] * mat.m[2][0] + m[1][3] * mat.m[2][1] + m[2][3] * mat.m[2][2] + m[3][3] * mat.m[2][3];

	matrix.m[3][0] = m[0][0] * mat.m[3][0] + m[1][0] * mat.m[3][1] + m[2][0] * mat.m[3][2] + m[3][0] * mat.m[3][3];
	matrix.m[3][1] = m[0][1] * mat.m[3][0] + m[1][1] * mat.m[3][1] + m[2][1] * mat.m[3][2] + m[3][1] * mat.m[3][3];
	matrix.m[3][2] = m[0][2] * mat.m[3][0] + m[1][2] * mat.m[3][1] + m[2][2] * mat.m[3][2] + m[3][2] * mat.m[3][3];
	matrix.m[3][3] = m[0][3] * mat.m[3][0] + m[1][3] * mat.m[3][1] + m[2][3] * mat.m[3][2] + m[3][3] * mat.m[3][3];
	
	STOP_PROFILING;
	return matrix;
	/* ==================== SOLUTION #2 end ==================== */

	/* ==================== SOLUTION #3 begin ==================== */
	//for (unsigned int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (unsigned int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		//matrix.m[i][j] = REAL_ZERO;
	//		Real sum = REAL_ZERO;
	//		for (unsigned int k = 0; k < MATRIX_SIZE; ++k)
	//		{
	//			sum += m[k][j] * mat.m[i][k];
	//		}
	//		matrix.m[i][j] = sum;
	//	}
	//}
	//return matrix;
	/* ==================== SOLUTION #3 end ==================== */
}

Vector3D Matrix4D::operator*(const Vector3D& vec) const
{
	START_PROFILING;
	const Real oneperw = REAL_ONE / (m[0][3] * vec.GetX() + m[1][3] * vec.GetY() + m[2][3] * vec.GetZ() + m[3][3]);
	Real x = (m[0][0] * vec.GetX() + m[1][0] * vec.GetY() + m[2][0] * vec.GetZ() + m[3][0]) * oneperw;
	Real y = (m[0][1] * vec.GetX() + m[1][1] * vec.GetY() + m[2][1] * vec.GetZ() + m[3][1]) * oneperw;
	Real z = (m[0][2] * vec.GetX() + m[1][2] * vec.GetY() + m[2][2] * vec.GetZ() + m[3][2]) * oneperw;
	STOP_PROFILING;
	return Vector3D(x, y, z);
}

/**
 * @see Matrix4D::IsIdentity
 */
bool Matrix4D::operator==(const Matrix4D& matrix) const
{
	START_PROFILING;
	/**
	 * TODO: This function is rather slow. Maybe before creating FloatingPoint objects compare
	 * the numbers using simple tools, like epsilon?
	 * Additionaly, maybe consider creating a static function in the Math library for comparing numbers?
	 */
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			if (! AlmostEqual(matrix.GetElement(i, j), m[i][j]))
			{
				STOP_PROFILING;
				return false;
			}
		}
	}
	STOP_PROFILING;
	return true;
}

bool Matrix4D::operator!=(const Matrix4D& matrix) const
{
	return (!((*this) == matrix));
}

Matrix4D& Matrix4D::operator=(const Matrix4D& mat)
{
	START_PROFILING;
	// TODO: Check which of the three solution is faster
	
	/* ==================== SOLUTION #1 begin ==================== */
	m[0][0] = mat[0][0];	m[0][1] = mat[0][1];	m[0][2] = mat[0][2];	m[0][3] = mat[0][3];
	m[1][0] = mat[1][0];	m[1][1] = mat[1][1];	m[1][2] = mat[1][2];	m[1][3] = mat[1][3];
	m[2][0] = mat[2][0];	m[2][1] = mat[2][1];	m[2][2] = mat[2][2];	m[2][3] = mat[2][3];
	m[3][0] = mat[3][0];	m[3][1] = mat[3][1];	m[3][2] = mat[3][2];	m[3][3] = mat[3][3];
	/* ==================== SOLUTION #1 end ==================== */
	
	/* ==================== SOLUTION #2 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		m[i][j] = mat[i][j];
	//	}
	//}
	/* ==================== SOLUTION #2 end ==================== */
	
	/* ==================== SOLUTION #3 begin ==================== */
	//for (int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		this->SetElement(i, j, mat.GetElement(i, j));
	//	}
	//}
	/* ==================== SOLUTION #3 end ==================== */
	
	SLOW_ASSERT((*this) == mat);
	
	STOP_PROFILING;
	return *this;
}

Matrix4D Matrix4D::Transposition() const
{
	START_PROFILING;
	Matrix4D matrix;

	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			matrix.m[i][j] = m[j][i];
		}
	}

	// TODO: According to wikipedia (http://en.wikipedia.org/wiki/Transpose) (A^T)^T = A, so
	// check this condition here. Use SLOW_ASSERT, but use it wisely just once not to cause a runtime stack overflow
	// (due to recursive calls on all control paths).
	// SLOW_ASSERT(matrix.Transposition() == (*this));
	STOP_PROFILING;
	return matrix;
}

Real Matrix4D::Det(int p, int q) const
{
	START_PROFILING;
	/*
		0: 1 2 3
		1: 0 2 3
		2: 0 1 3
		3: 0 1 2
	*/

	int i[3] = 
	{
		((p == 0) ? 1 : 0),
		((p < 2) ? 2 : 1),
		((p == 3) ? 2 : 3)
	};
	int j[3] = 
	{
		((q == 0) ? 1 : 0),
		((q < 2) ? 2 : 1),
		((q == 3) ? 2 : 3)
	};
	Real result = 0;
	//#define _det3(M, A00, A10, A20, A01, A11, A21, A02, A12, A22)
	for (int k = 0; k < 3; ++k)
	{
		result += m[i[k]][j[0]] * m[i[(k + 1) % 3]][j[1]] * m[i[(k + 2) % 3]][j[2]];
		result -= m[i[k]][j[2]] * m[i[(k + 1) % 3]][j[1]] * m[i[(k + 2) % 3]][j[0]];
	}
	
	// TODO: Consider creating some asserts here.
	STOP_PROFILING;
	return result;
}

Matrix4D Matrix4D::Inversion() const
{
	START_PROFILING;
	Real det = 0;
	for (int j = 0; j < MATRIX_SIZE; ++j)
	{
		det += Signum(3, j) * m[3][j] * Det(3, j);
	}

	if (det == 0)
	{
		STOP_PROFILING;
		return Matrix4D::IDENTITY_MATRIX;
	}

	Matrix4D result;

	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			result.m[j][i] = Signum(i,j) * Det(i,j) / det;
		}
	}
	
	// TODO: Use SLOW_ASSERT(IsIdentity()) for the result of the multiplication M*M^(-1)
	SLOW_ASSERT(((*this) * result).IsIdentity());

	STOP_PROFILING;
	return result;
}

/**
 * @see Matrix4D::operator ==
 */
bool Matrix4D::IsIdentity() const
{
	START_PROFILING;
	/**
	 * TODO: This function is rather slow. Maybe before creating FloatingPoint objects compare
	 * the numbers using simple tools, like epsilon?
	 * Additionaly, maybe consider creating a static function in the Math library for comparing numbers?
	 */
	
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			if (i == j)
			{
				if (! AlmostEqual(REAL_ONE, m[i][j]))
				{
					STOP_PROFILING;
					return false;
				}
			}
			else /* i != j */
			{
				if (! AlmostEqual(REAL_ZERO, m[i][j]))
				{
					STOP_PROFILING;
					return false;
				}
			}
		}
	}
	STOP_PROFILING;	
	return true;
}

void Matrix4D::SetScaleMatrix(Real scaleX, Real scaleY, Real scaleZ)
{
	START_PROFILING;
	m[0][0] = scaleX;		m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;	m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = scaleY;		m[1][2] = REAL_ZERO;	m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = scaleZ;		m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = REAL_ZERO;	m[3][3] = REAL_ONE;
	STOP_PROFILING;
}

void Matrix4D::SetPerspectiveProjection(const Angle& fov, Real aspect, Real nearPlane, Real farPlane)
{
	START_PROFILING;
	const Real f = static_cast<Real>(REAL_ONE / tan(fov.GetAngleInRadians() / 2.0));
	const Real div = static_cast<Real>(REAL_ONE / (nearPlane - farPlane));

	//m[0][0] = f / aspect;	m[0][1] = 0.0;	m[0][2] = 0.0;							m[0][3] = 0.0;
	//m[1][0] = 0.0;		m[1][1] = f;	m[1][2] = 0.0;							m[1][3] = 0.0;
	//m[2][0] = 0.0;		m[2][1] = 0.0;	m[2][2] = (farPlane + nearPlane) * div;	m[2][3] = 2.0 * farPlane * nearPlane * div;
	//m[3][0] = 0.0;		m[3][1] = 0.0;	m[3][2] = -1.0;							m[3][3] = 0.0;

	/* IMPLEMENTATION FROM https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 begin */
	m[0][0] = f / aspect;	m[0][1] = REAL_ZERO;	m[0][2] = REAL_ZERO;											m[0][3] = REAL_ZERO;
	m[1][0] = REAL_ZERO;	m[1][1] = f;			m[1][2] = REAL_ZERO;											m[1][3] = REAL_ZERO;
	m[2][0] = REAL_ZERO;	m[2][1] = REAL_ZERO;	m[2][2] = (-nearPlane - farPlane) * div;						m[2][3] = REAL_ONE;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = static_cast<Real>(2.0) * farPlane * nearPlane * div;	m[3][3] = REAL_ZERO;
	/* IMPLEMENTATION FROM https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 end */

	STOP_PROFILING;
}

Vector3D Matrix4D::Transform(const Vector3D& vec)
{
	return Vector3D(m[0][0] * vec.GetX() + m[1][0] * vec.GetY() + m[2][0] * vec.GetZ() + m[3][0],
					m[0][1] * vec.GetX() + m[1][1] * vec.GetY() + m[2][1] * vec.GetZ() + m[3][1],
					m[0][2] * vec.GetX() + m[1][2] * vec.GetY() + m[2][2] * vec.GetZ() + m[3][2]);

	//Vector3D ret(REAL_ZERO, REAL_ZERO, REAL_ZERO);

	//for (unsigned int i = 0; i < MATRIX_SIZE; ++i)
	//{
	//	for (unsigned int j = 0; j < MATRIX_SIZE; ++j)
	//	{
	//		ret.opera
	//	}
	//}
}

void Matrix4D::SetRotationFromVectors(const Vector3D& forward, const Vector3D& up, const Vector3D& right)
{
	//Vector3D f = forward.Normalized();
	//Vector3D u = up.Normalized();
	//Vector3D r = right.Normalized();
	
	//Vector3D right = up;
	//right.Normalize(); // TODO: Should not be necessary
	//right = right.Cross(forw);

	//Vector3D newUp = forw.Cross(right);
	CHECK_CONDITION(forward.IsNormalized(), Utility::Error, "Cannot correctly perform the rotation. The specified forward vector is not normalized.");
	CHECK_CONDITION(up.IsNormalized(), Utility::Error, "Cannot correctly perform the rotation. The specified up vector is not normalized.");
	CHECK_CONDITION(right.IsNormalized(), Utility::Error, "Cannot correctly perform the rotation. The specified right vector is not normalized.");
	m[0][0] = right.GetX();	m[0][1] = up.GetX();	m[0][2] = forward.GetX();	m[0][3] = REAL_ZERO;
	m[1][0] = right.GetY();	m[1][1] = up.GetY();	m[1][2] = forward.GetY();	m[1][3] = REAL_ZERO;
	m[2][0] = right.GetZ();	m[2][1] = up.GetZ();	m[2][2] = forward.GetZ();	m[2][3] = REAL_ZERO;
	m[3][0] = REAL_ZERO;	m[3][1] = REAL_ZERO;	m[3][2] = REAL_ZERO;		m[3][3] = REAL_ONE;
}