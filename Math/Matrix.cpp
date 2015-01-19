#include "StdAfx.h"
#include "Matrix.h"
#include "FloatingPoint.h"

#include "Utility\Utility.h"

#include <sstream>

using namespace Math;
using namespace Utility;

Matrix4D::Matrix4D()
{
	// TODO: Load identity matrix
}

Matrix4D::Matrix4D(const Matrix4D& mat)
{
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
}

Matrix4D::~Matrix4D()
{
}

/* static */ Matrix4D Matrix4D::Identity()
{
	Matrix4D matrix;
	
	matrix.m[0][0] = REAL_ONE;	matrix.m[0][1] = REAL_ZERO;	matrix.m[0][2] = REAL_ZERO;	matrix.m[0][3] = REAL_ZERO;
	matrix.m[1][0] = REAL_ZERO;	matrix.m[1][1] = REAL_ONE;	matrix.m[1][2] = REAL_ZERO;	matrix.m[1][3] = REAL_ZERO;
	matrix.m[2][0] = REAL_ZERO;	matrix.m[2][1] = REAL_ZERO;	matrix.m[2][2] = REAL_ONE;	matrix.m[2][3] = REAL_ZERO;
	matrix.m[3][0] = REAL_ZERO;	matrix.m[3][1] = REAL_ZERO;	matrix.m[3][2] = REAL_ZERO;	matrix.m[3][3] = REAL_ONE;
	
	SLOW_ASSERT(matrix.IsIdentity());
	
	return matrix;
}

/* static */ Matrix4D Matrix4D::PerspectiveProjection(const Angle& fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */)
{
	// CHECKED
	Matrix4D matrix;
	Real f = static_cast<Real>(REAL_ONE / tan(fov.GetAngleInRadians() / 2.0));
	Real div = static_cast<Real>(REAL_ONE / (nearPlane - farPlane));

	//matrix.m[0][0] = f / aspect;	matrix.m[0][1] = 0.0;	matrix.m[0][2] = 0.0;							matrix.m[0][3] = 0.0;
	//matrix.m[1][0] = 0.0;			matrix.m[1][1] = f;		matrix.m[1][2] = 0.0;							matrix.m[1][3] = 0.0;
	//matrix.m[2][0] = 0.0;			matrix.m[2][1] = 0.0;	matrix.m[2][2] = (farPlane + nearPlane) * div;	matrix.m[2][3] = 2.0 * farPlane * nearPlane * div;
	//matrix.m[3][0] = 0.0;			matrix.m[3][1] = 0.0;	matrix.m[3][2] = -1.0;							matrix.m[3][3] = 0.0;

	/* IMPLEMENTATION FROM https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 begin */
	matrix.m[0][0] = f / aspect;	matrix.m[0][1] = REAL_ZERO;	matrix.m[0][2] = REAL_ZERO;												matrix.m[0][3] = REAL_ZERO;
	matrix.m[1][0] = REAL_ZERO;		matrix.m[1][1] = f;			matrix.m[1][2] = REAL_ZERO;												matrix.m[1][3] = REAL_ZERO;
	matrix.m[2][0] = REAL_ZERO;		matrix.m[2][1] = REAL_ZERO;	matrix.m[2][2] = (-nearPlane - farPlane) * div;							matrix.m[2][3] = REAL_ONE;
	matrix.m[3][0] = REAL_ZERO;		matrix.m[3][1] = REAL_ZERO;	matrix.m[3][2] = static_cast<Real>(2.0) * farPlane * nearPlane * div;	matrix.m[3][3] = REAL_ZERO;
	/* IMPLEMENTATION FROM https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 end */

	return matrix;
}

/* static */ Matrix4D Matrix4D::OrtographicProjection(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane)
{
	// CHECKED
	Matrix4D matrix;

	Real width = right - left;
	Real height = top - bottom;
	Real depth = farPlane - nearPlane;

	const Real temp = static_cast<Real>(2);
	
	matrix.m[0][0] = temp / width;				matrix.m[0][1] = REAL_ZERO;					matrix.m[0][2] = REAL_ZERO;							matrix.m[0][3] = REAL_ZERO;
	matrix.m[1][0] = REAL_ZERO;					matrix.m[1][1] = temp / height;				matrix.m[1][2] = REAL_ZERO;							matrix.m[1][3] = REAL_ZERO;
	matrix.m[2][0] = REAL_ZERO;					matrix.m[2][1] = REAL_ZERO;					matrix.m[2][2] = -temp / depth;						matrix.m[2][3] = REAL_ZERO;
	matrix.m[3][0] = -(right + left) / width;	matrix.m[3][1] = -(top + bottom) / height;	matrix.m[3][2] = -(farPlane + nearPlane) / depth;	matrix.m[3][3] = REAL_ONE;
	
	return matrix;
}

/* static */ Matrix4D Matrix4D::Translation(Real x, Real y, Real z)
{
	Matrix4D matrix;
	matrix.m[0][0] = REAL_ONE;	matrix.m[0][1] = REAL_ZERO;	matrix.m[0][2] = REAL_ZERO;	matrix.m[0][3] = REAL_ZERO;
	matrix.m[1][0] = REAL_ZERO;	matrix.m[1][1] = REAL_ONE;	matrix.m[1][2] = REAL_ZERO;	matrix.m[1][3] = REAL_ZERO;
	matrix.m[2][0] = REAL_ZERO;	matrix.m[2][1] = REAL_ZERO;	matrix.m[2][2] = REAL_ONE;	matrix.m[2][3] = REAL_ZERO;
	matrix.m[3][0] = x;			matrix.m[3][1] = y;			matrix.m[3][2] = z;			matrix.m[3][3] = REAL_ONE;
	return matrix;
}

/* static */ Matrix4D Matrix4D::Translation(const Vector3D& vec)
{
	return Translation(vec.GetX(), vec.GetY(), vec.GetZ());
}

/* static */ Matrix4D Matrix4D::Scale(Real x, Real y, Real z)
{
	Matrix4D matrix;
	matrix.m[0][0] = x;			matrix.m[0][1] = REAL_ZERO;	matrix.m[0][2] = REAL_ZERO;	matrix.m[0][3] = REAL_ZERO;
	matrix.m[1][0] = REAL_ZERO;	matrix.m[1][1] = y;			matrix.m[1][2] = REAL_ZERO;	matrix.m[1][3] = REAL_ZERO;
	matrix.m[2][0] = REAL_ZERO;	matrix.m[2][1] = REAL_ZERO;	matrix.m[2][2] = z;			matrix.m[2][3] = REAL_ZERO;
	matrix.m[3][0] = REAL_ZERO;	matrix.m[3][1] = REAL_ZERO;	matrix.m[3][2] = REAL_ZERO;	matrix.m[3][3] = REAL_ONE;

	return matrix;
}

/* static */ Matrix4D Matrix4D::Scale(const Vector3D& vec)
{
	return Scale(vec.GetX(), vec.GetY(), vec.GetZ());
}

/**
 * @see Vector2D::Rotate(Real angle)
 */
/* static */ Matrix4D Matrix4D::RotationEuler(const Angle& angleX, const Angle& angleY, const Angle& angleZ)
{
	// CHECKED
	Matrix4D rotX, rotY, rotZ; // rotation around X, Y and Z axis respectively

	Real xRad = angleX.GetAngleInRadians();
	Real yRad = angleY.GetAngleInRadians();
	Real zRad = angleZ.GetAngleInRadians();

	Real xSin = sin(xRad);
	Real xCos = cos(xRad);
	Real ySin = sin(yRad);
	Real yCos = cos(yRad);
	Real zSin = sin(zRad);
	Real zCos = cos(zRad);

	// TODO: Can't we define rotation using just one matrix and not three?
	// This way we prevent the hard-computing multiplications of three matrices.

	rotX.m[0][0] = REAL_ONE;	rotX.m[0][1] = REAL_ZERO;	rotX.m[0][2] = REAL_ZERO;	rotX.m[0][3] = REAL_ZERO;
	rotX.m[1][0] = REAL_ZERO;	rotX.m[1][1] = xCos;		rotX.m[1][2] = xSin;		rotX.m[1][3] = REAL_ZERO;
	rotX.m[2][0] = REAL_ZERO;	rotX.m[2][1] = -xSin;		rotX.m[2][2] = xCos;		rotX.m[2][3] = REAL_ZERO;
	rotX.m[3][0] = REAL_ZERO;	rotX.m[3][1] = REAL_ZERO;	rotX.m[3][2] = REAL_ZERO;	rotX.m[3][3] = REAL_ONE;

	rotY.m[0][0] = yCos;		rotY.m[0][1] = REAL_ZERO;	rotY.m[0][2] = ySin;		rotY.m[0][3] = REAL_ZERO;
	rotY.m[1][0] = REAL_ZERO;	rotY.m[1][1] = REAL_ONE;	rotY.m[1][2] = REAL_ZERO;	rotY.m[1][3] = REAL_ZERO;
	rotY.m[2][0] = -ySin;		rotY.m[2][1] = REAL_ZERO;	rotY.m[2][2] = yCos;		rotY.m[2][3] = REAL_ZERO;
	rotY.m[3][0] = REAL_ZERO;	rotY.m[3][1] = REAL_ZERO;	rotY.m[3][2] = REAL_ZERO;	rotY.m[3][3] = REAL_ONE;

	rotZ.m[0][0] = zCos;		rotZ.m[0][1] = zSin;		rotZ.m[0][2] = REAL_ZERO;	rotZ.m[0][3] = REAL_ZERO;
	rotZ.m[1][0] = -zSin;		rotZ.m[1][1] = zCos;		rotZ.m[1][2] = REAL_ZERO;	rotZ.m[1][3] = REAL_ZERO;
	rotZ.m[2][0] = REAL_ZERO;	rotZ.m[2][1] = REAL_ZERO;	rotZ.m[2][2] = REAL_ONE;	rotZ.m[2][3] = REAL_ZERO;
	rotZ.m[3][0] = REAL_ZERO;	rotZ.m[3][1] = REAL_ZERO;	rotZ.m[3][2] = REAL_ZERO;	rotZ.m[3][3] = REAL_ONE;

	//return rotX * rotY * rotZ;
	return rotZ * rotY * rotX;
}

///* static */ Matrix4D Matrix4D::Rotation(Real x, Real y, Real z, const Angle& angle)
//{
//	Real vecNorm = sqrt(x * x + y * y + z * z);
//	x /= vecNorm;
//	y /= vecNorm;
//	z /= vecNorm;
//				
//	Real angleInRadians = angle.GetAngleInRadians();
//
//	Matrix4D matrix;
//	Real c = cos(angleInRadians);
//	Real s = sin(angleInRadians);
//	Real omc = 1 - c;
//	Real xs = x * s;
//	Real ys = y * s;
//	Real zs = z * s;
//	Real xy = x*y;
//	Real xz = x*z;
//	Real yz = y*z;
//
//	matrix.m[0][0] = x*x*omc+c;	matrix.m[0][1] = xy*omc-zs;	matrix.m[0][2] = xz*omc+ys;	matrix.m[0][3] = 0.0;
//	matrix.m[1][0] = xy*omc+zs;	matrix.m[1][1] = y*y*omc+c;	matrix.m[1][2] = yz*omc-xs;	matrix.m[1][3] = 0.0;
//	matrix.m[2][0] = xz*omc-ys;	matrix.m[2][1] = yz*omc+xs;	matrix.m[2][2] = z*z*omc+c;	matrix.m[2][3] = 0.0;
//	matrix.m[3][0] = 0.0;		matrix.m[3][1] = 0.0;		matrix.m[3][2] = 0.0;		matrix.m[3][3] = 1.0;
//	
//	return matrix;
//}

///* static */ Matrix4D Matrix4D::Rotation(const Vector3D& vec, const Angle& angle)
//{
//	return Rotation(vec.GetX(), vec.GetY(), vec.GetZ(), angle);
//}

/* static */ Matrix4D Matrix4D::RotationFromVectors(const Vector3D& forward, const Vector3D& up, const Vector3D& right)
{
	Vector3D f = forward;
	Vector3D u = up;
	Vector3D r = right;

	//f.Normalize(); // TODO: Should not be necessary
	//u.Normalize(); // TODO: Should not be necessary
	//r.Normalize(); // TODO: Should not be necessary
	
	//Vector3D right = up;
	//right.Normalize(); // TODO: Should not be necessary
	//right = right.Cross(forw);

	//Vector3D newUp = forw.Cross(right);

	Matrix4D matrix;
	matrix.m[0][0] = r.GetX();	matrix.m[0][1] = u.GetX();	matrix.m[0][2] = f.GetX();	matrix.m[0][3] = REAL_ZERO;
	matrix.m[1][0] = r.GetY();	matrix.m[1][1] = u.GetY();	matrix.m[1][2] = f.GetY();	matrix.m[1][3] = REAL_ZERO;
	matrix.m[2][0] = r.GetZ();	matrix.m[2][1] = u.GetZ();	matrix.m[2][2] = f.GetZ();	matrix.m[2][3] = REAL_ZERO;
	matrix.m[3][0] = REAL_ZERO;	matrix.m[3][1] = REAL_ZERO;	matrix.m[3][2] = REAL_ZERO;	matrix.m[3][3] = REAL_ONE;
	return matrix;
}

/* static */ Matrix4D Matrix4D::RotationFromDirection(const Vector3D& forward, const Vector3D& up)
{
	Vector3D n = forward.Normalized();
	Vector3D u = up.Normalized().Cross(n);
	Vector3D v = n.Cross(u);

	return RotationFromVectors(n, v, u);
}

/* static */ int Matrix4D::Signum(int i, int j)
{
	return ((i + j) % 2) ? -1 : 1;
}

std::string Matrix4D::ToString() const
{
	std::stringstream s("");
	s << m[0][0] << " ";
	s << m[0][1] << " ";
	s << m[0][2] << " ";
	s << m[0][3] << " ";
	s << std::endl;

	s << m[1][0] << " ";
	s << m[1][1] << " ";
	s << m[1][2] << " ";
	s << m[1][3] << " ";
	s << std::endl;

	s << m[2][0] << " ";
	s << m[2][1] << " ";
	s << m[2][2] << " ";
	s << m[2][3] << " ";
	s << std::endl;

	s << m[3][0] << " ";
	s << m[3][1] << " ";
	s << m[3][2] << " ";
	s << m[3][3] << " ";
	s << std::endl;

	return s.str();
}
	
Matrix4D Matrix4D::operator*(const Matrix4D& mat) const
{
	Matrix4D matrix;

	//matrix.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
	//matrix.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
	//matrix.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
	//matrix.m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];

	//matrix.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
	//matrix.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
	//matrix.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
	//matrix.m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];
	//		
	//matrix.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
	//matrix.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
	//matrix.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
	//matrix.m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];
	//
	//matrix.m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
	//matrix.m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
	//matrix.m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
	//matrix.m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];

	for (unsigned int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (unsigned int j = 0; j < MATRIX_SIZE; ++j)
		{
			//matrix.m[i][j] = REAL_ZERO;
			Real sum = REAL_ZERO;
			for (unsigned int k = 0; k < MATRIX_SIZE; ++k)
			{
				sum += m[k][j] * mat.m[i][k];
			}
			matrix.m[i][j] = sum;
		}
	}

	return matrix;
}

Vector3D Matrix4D::operator*(const Vector3D& vec) const
{
	Vector3D result;

	Real oneperw = 1.0f / (m[0][3] * vec.GetX() + m[1][3] * vec.GetY() + m[2][3] * vec.GetZ() + m[3][3]);
	result.SetX((m[0][0] * vec.GetX() + m[1][0] * vec.GetY() + m[2][0] * vec.GetZ() + m[3][0]) * oneperw);
	result.SetY((m[0][1] * vec.GetX() + m[1][1] * vec.GetY() + m[2][1] * vec.GetZ() + m[3][1]) * oneperw);
	result.SetZ((m[0][2] * vec.GetX() + m[1][2] * vec.GetY() + m[2][2] * vec.GetZ() + m[3][2]) * oneperw);

	return result;
}

/**
 * @see Matrix4D::IsIdentity
 */
bool Matrix4D::operator==(const Matrix4D& matrix) const
{
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
				return false;
			}
		}
	}
	
	return true;
}

Matrix4D& Matrix4D::operator=(const Matrix4D& mat)
{
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
	
	return *this;
}

Matrix4D Matrix4D::Transposition() const
{
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

	return matrix;
}

Real Matrix4D::Det(int p, int q) const
{
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
	
	return result;
}

Matrix4D Matrix4D::Inversion() const
{
	Real det = 0;
	for (int j = 0; j < MATRIX_SIZE; ++j)
	{
		det += Signum(3, j) * m[3][j] * Det(3, j);
	}

	if (det == 0)
	{
		return Identity();
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

	return result;
}

/**
 * @see Matrix4D::operator ==
 */
bool Matrix4D::IsIdentity() const
{
	/**
	 * TODO: This function is rather slow. Maybe before creating FloatingPoint objects compare
	 * the numbers using simple tools, like epsilon?
	 * Additionaly, maybe consider creating a static function in the Math library for comparing numbers?
	 */
	const Real zero = 0.0f;
	const Real unit = 1.0f;
	
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			if (i == j)
			{
				if (! AlmostEqual(unit, m[i][j]))
				{
					return false;
				}
			}
			else /* i != j */
			{
				if (! AlmostEqual(zero, m[i][j]))
				{
					return false;
				}
			}
		}
	}
	
	return true;
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