#include "StdAfx.h"
#include "Matrix.h"
#include "FloatingPoint.h"
#include <sstream>

// TODO: Link Utility library to the Math library

using namespace Math;

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
	
	SLOW_ASSERT(this == mat);
}

Matrix4D::~Matrix4D()
{
}

/* static */ Matrix4D Matrix4D::Identity()
{
	Matrix4D matrix;
	
	matrix.m[0][0] = 1.0;	matrix.m[0][1] = 0.0;	matrix.m[0][2] = 0.0;	matrix.m[0][3] = 0.0;
	matrix.m[1][0] = 0.0;	matrix.m[1][1] = 1.0;	matrix.m[1][2] = 0.0;	matrix.m[1][3] = 0.0;
	matrix.m[2][0] = 0.0;	matrix.m[2][1] = 0.0;	matrix.m[2][2] = 1.0;	matrix.m[2][3] = 0.0;
	matrix.m[3][0] = 0.0;	matrix.m[3][1] = 0.0;	matrix.m[3][2] = 0.0;	matrix.m[3][3] = 1.0;
	
	SLOW_ASSERT(IsIdentity());
	
	return matrix;
}

/* static */ Matrix4D Matrix4D::PerspectiveProjection(Real fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */)
{
	Matrix4D matrix;
	Real f = static_cast<Real>(1.0 / tan(ToRad(fov / 2)));
	Real div = static_cast<Real>(1.0 / (nearPlane - farPlane));

	//matrix.m[0][0] = f / aspect;	matrix.m[0][1] = 0.0;	matrix.m[0][2] = 0.0;							matrix.m[0][3] = 0.0;
	//matrix.m[1][0] = 0.0;			matrix.m[1][1] = f;		matrix.m[1][2] = 0.0;							matrix.m[1][3] = 0.0;
	//matrix.m[2][0] = 0.0;			matrix.m[2][1] = 0.0;	matrix.m[2][2] = (farPlane + nearPlane) * div;	matrix.m[2][3] = 2.0 * farPlane * nearPlane * div;
	//matrix.m[3][0] = 0.0;			matrix.m[3][1] = 0.0;	matrix.m[3][2] = -1.0;							matrix.m[3][3] = 0.0;

	/* IMPLEMENTATION FROM https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 begin */
	matrix.m[0][0] = f / aspect;	matrix.m[0][1] = 0.0;	matrix.m[0][2] = 0.0;							matrix.m[0][3] = 0.0;
	matrix.m[1][0] = 0.0;			matrix.m[1][1] = f;		matrix.m[1][2] = 0.0;							matrix.m[1][3] = 0.0;
	matrix.m[2][0] = 0.0;			matrix.m[2][1] = 0.0;	matrix.m[2][2] = (-farPlane - nearPlane) * div;	matrix.m[2][3] = static_cast<Real>(2.0) * farPlane * nearPlane * div;
	matrix.m[3][0] = 0.0;			matrix.m[3][1] = 0.0;	matrix.m[3][2] = 1.0;							matrix.m[3][3] = 0.0;
	/* IMPLEMENTATION FROM https://www.youtube.com/watch?v=cgaixZEaDCg&list=PLEETnX-uPtBXP_B2yupUKlflXBznWIlL5 end */

	return matrix;
}

/* static */ Matrix4D Matrix4D::OrtographicProjection(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane)
{
	Matrix4D matrix;

	Real width = right - left;
	Real height = top - bottom;
	Real depth = farPlane - nearPlane;
	
	matrix.m[0][0] = 2.0 / width;	matrix.m[0][1] = 0.0;			matrix.m[0][2] = 0.0;			matrix.m[0][3] = -(right + left) / width;
	matrix.m[1][0] = 0.0;			matrix.m[1][1] = 2.0 / height;	matrix.m[1][2] = 0.0;			matrix.m[1][3] = -(top + bottom) / height;
	matrix.m[2][0] = 0.0;			matrix.m[2][1] = 0.0;			matrix.m[2][2] = -2.0 / depth;	matrix.m[2][3] = -(farPlane + nearPlane) / depth;
	matrix.m[3][0] = 0.0;			matrix.m[3][1] = 0.0;			matrix.m[3][2] = 0.0;			matrix.m[3][3] = 1.0;
	
	return matrix;
}

/* static */ Matrix4D Matrix4D::Translation(Real x, Real y, Real z)
{
	Matrix4D matrix;
	matrix.m[0][0] = 1.0;	matrix.m[0][1] = 0.0;	matrix.m[0][2] = 0.0;	matrix.m[0][3] = x;
	matrix.m[1][0] = 0.0;	matrix.m[1][1] = 1.0;	matrix.m[1][2] = 0.0;	matrix.m[1][3] = y;
	matrix.m[2][0] = 0.0;	matrix.m[2][1] = 0.0;	matrix.m[2][2] = 1.0;	matrix.m[2][3] = z;
	matrix.m[3][0] = 0.0;	matrix.m[3][1] = 0.0;	matrix.m[3][2] = 0.0;	matrix.m[3][3] = 1.0;
	return matrix;
}

/* static */ Matrix4D Matrix4D::Translation(const Vector3D& vec)
{
	return Translation(vec.GetX(), vec.GetY(), vec.GetZ());
}

/* static */ Matrix4D Matrix4D::Scale(Real x, Real y, Real z)
{
	Matrix4D matrix;
	matrix.m[0][0] = x;		matrix.m[0][1] = 0.0;	matrix.m[0][2] = 0.0;	matrix.m[0][3] = 0.0;
	matrix.m[1][0] = 0.0;	matrix.m[1][1] = y;		matrix.m[1][2] = 0.0;	matrix.m[1][3] = 0.0;
	matrix.m[2][0] = 0.0;	matrix.m[2][1] = 0.0;	matrix.m[2][2] = z;		matrix.m[2][3] = 0.0;
	matrix.m[3][0] = 0.0;	matrix.m[3][1] = 0.0;	matrix.m[3][2] = 0.0;	matrix.m[3][3] = 1.0;

	return matrix;
}

/* static */ Matrix4D Matrix4D::Scale(const Vector3D& vec)
{
	return Scale(vec.GetX(), vec.GetY(), vec.GetZ());
}

/**
 * @see Vector2D::Rotate(Real angle)
 */
/* static */ Matrix4D Matrix4D::Rotation(Real x, Real y, Real z)
{
	Matrix4D rotX, rotY, rotZ; // rotation around X, Y and Z axis respectively

	Real xRad = Math::ToRad(x);
	Real yRad = Math::ToRad(y);
	Real zRad = Math::ToRad(z);

	Real xSin = sin(xRad);
	Real xCos = cos(xRad);
	Real ySin = sin(yRad);
	Real yCos = cos(yRad);
	Real zSin = sin(zRad);
	Real zCos = cos(zRad);

	// TODO: Can't we define rotation using just one matrix and not three?
	// This way we prevent the hard-computing multiplications of three matrices.

	rotX.m[0][0] = 1.0;	rotX.m[0][1] = 0.0;		rotX.m[0][2] = 0.0;		rotX.m[0][3] = 0.0;
	rotX.m[1][0] = 0.0;	rotX.m[1][1] = xCos;	rotX.m[1][2] = -xSin;	rotX.m[1][3] = 0.0;
	rotX.m[2][0] = 0.0;	rotX.m[2][1] = xSin;	rotX.m[2][2] = xCos;	rotX.m[2][3] = 0.0;
	rotX.m[3][0] = 0.0;	rotX.m[3][1] = 0.0;		rotX.m[3][2] = 0.0;		rotX.m[3][3] = 1.0;

	rotY.m[0][0] = yCos;	rotY.m[0][1] = 0.0;	rotY.m[0][2] = -ySin;	rotY.m[0][3] = 0.0;
	rotY.m[1][0] = 0.0;		rotY.m[1][1] = 1.0;	rotY.m[1][2] = 0.0;		rotY.m[1][3] = 0.0;
	rotY.m[2][0] = ySin;	rotY.m[2][1] = 0.0;	rotY.m[2][2] = yCos;	rotY.m[2][3] = 0.0;
	rotY.m[3][0] = 0.0;		rotY.m[3][1] = 0.0;	rotY.m[3][2] = 0.0;		rotY.m[3][3] = 1.0;

	rotZ.m[0][0] = zCos;	rotZ.m[0][1] = -zSin;	rotZ.m[0][2] = 0.0;	rotZ.m[0][3] = 0.0;
	rotZ.m[1][0] = zSin;	rotZ.m[1][1] = zCos;	rotZ.m[1][2] = 0.0;	rotZ.m[1][3] = 0.0;
	rotZ.m[2][0] = 0.0;		rotZ.m[2][1] = 0.0;		rotZ.m[2][2] = 1.0;	rotZ.m[2][3] = 0.0;
	rotZ.m[3][0] = 0.0;		rotZ.m[3][1] = 0.0;		rotZ.m[3][2] = 0.0;	rotZ.m[3][3] = 1.0;

	return rotZ * rotY * rotX;
}

// TODO: Define last parameter as Angle instance
/* static */ Matrix4D Matrix4D::Rotation(Real x, Real y, Real z, Real angleInDegrees)
{
	Real vecNorm = sqrt(x * x + y * y + z * z);
	x /= vecNorm;
	y /= vecNorm;
	z /= vecNorm;
				
	Real angleInRadians = Math::ToRad(angleInDegrees);

	Matrix4D matrix;
	Real c = cos(angleInRadians);
	Real s = sin(angleInRadians);
	Real omc = 1 - c;
	Real xs = x * s;
	Real ys = y * s;
	Real zs = z * s;
	Real xy = x*y;
	Real xz = x*z;
	Real yz = y*z;

	matrix.m[0][0] = x*x*omc+c;	matrix.m[0][1] = xy*omc-zs;	matrix.m[0][2] = xz*omc+ys;	matrix.m[0][3] = 0.0;
	matrix.m[1][0] = xy*omc+zs;	matrix.m[1][1] = y*y*omc+c;	matrix.m[1][2] = yz*omc-xs;	matrix.m[1][3] = 0.0;
	matrix.m[2][0] = xz*omc-ys;	matrix.m[2][1] = yz*omc+xs;	matrix.m[2][2] = z*z*omc+c;	matrix.m[2][3] = 0.0;
	matrix.m[3][0] = 0.0;		matrix.m[3][1] = 0.0;		matrix.m[3][2] = 0.0;		matrix.m[3][3] = 1.0;
	
	return matrix;
}

/* static */ Matrix4D Matrix4D::Rotation(const Vector3D& vec, Real angleInDegrees)
{
	return Rotation(vec.GetX(), vec.GetY(), vec.GetZ(), angleInDegrees);
}

/* static */ Matrix4D Matrix4D::Rotation(const Vector3D& forward, const Vector3D& up, const Vector3D& right)
{
	Vector3D f = forward;
	Vector3D u = up;
	Vector3D r = right;

	f.Normalize(); // TODO: Should not be necessary
	u.Normalize(); // TODO: Should not be necessary
	r.Normalize(); // TODO: Should not be necessary
	
	//Vector3D right = up;
	//right.Normalize(); // TODO: Should not be necessary
	//right = right.Cross(forw);

	//Vector3D newUp = forw.Cross(right);

	Matrix4D matrix;
	matrix.m[0][0] = r.GetX();	matrix.m[0][1] = r.GetY();	matrix.m[0][2] = r.GetZ();	matrix.m[0][3] = 0.0;
	matrix.m[1][0] = u.GetX();	matrix.m[1][1] = u.GetY();	matrix.m[1][2] = u.GetZ();	matrix.m[1][3] = 0.0;
	matrix.m[2][0] = f.GetX();	matrix.m[2][1] = f.GetY();	matrix.m[2][2] = f.GetZ();	matrix.m[2][3] = 0.0;
	matrix.m[3][0] = 0.0;		matrix.m[3][1] = 0.0;		matrix.m[3][2] = 0.0;		matrix.m[3][3] = 1.0;
	return matrix;
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

	matrix.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
	matrix.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
	matrix.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
	matrix.m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];

	matrix.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
	matrix.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
	matrix.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
	matrix.m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];
			
	matrix.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
	matrix.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
	matrix.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
	matrix.m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];
			                                                                                                                          
	matrix.m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
	matrix.m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
	matrix.m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
	matrix.m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];

	return matrix;
}

Vector3D Matrix4D::operator*(const Vector3D& vec) const
{
	Vector3D result;

	Real oneperw = static_cast<Real>(1.0) / (m[3][0] * vec.GetX() + m[3][1] * vec.GetY() + m[3][2] * vec.GetZ() + m[3][3]);
	result.SetX((m[0][0] * vec.GetX() + m[0][1] * vec.GetY() + m[0][2] * vec.GetZ() + m[0][3]) * oneperw);
	result.SetY((m[1][0] * vec.GetX() + m[1][1] * vec.GetY() + m[1][2] * vec.GetZ() + m[1][3]) * oneperw);
	result.SetZ((m[2][0] * vec.GetX() + m[2][1] * vec.GetY() + m[2][2] * vec.GetZ() + m[2][3]) * oneperw);

	return result;
}

bool Matrix4D::operator==(const Matrix4D& m) const
{
	// TODO: Fix this function
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
	
	SLOW_ASSERT(this == mat);
	
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
	// check this condition here. Use SLOW_ASSERT
	SLOW_ASSERT(matrix.Transposition() == *this)

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
	SLOW_ASSERT((this * result).IsIdentity());

	return result;
}
	
bool Matrix4D::IsIdentity() const
{
	/**
	 * TODO: This function is rather slow. Maybe before creating FloatingPoint objects compare
	 * the numbers using simple tools, like epsilon?
	 * Additionaly, maybe consider creating a static function in the Math library for comparing numbers?
	 */
	const Real epsilon = static_cast<Real>(0.1);
	const Real zero = static_cast<Real>(0.0);
	const Real unit = static_cast<Real>(1.0);
	const FloatingPoint<Real> zeroValue(zero);
	const FloatingPoint<Real> unitValue(unit);
	
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			if (i == j)
			{
				if (abs(unit - m[i][j]) > epsilon)
				{
					return false;
				}
				FloatingPoint<Real> matrixValue(m[i][j]);
				if (! matrixValue.AlmostEqual(unitValue))
				{
					return false;
				}
			}
			else /* i != j */
			{
				if (abs(zero - m[i][j]) > epsilon)
				{
					return false;
				}
				FloatingPoint<Real> matrixValue(m[i][j]);
				if (! matrixValue.AlmostEqual(zeroValue))
				{
					return false;
				}
			}
		}
	}
	
	return true;
}
