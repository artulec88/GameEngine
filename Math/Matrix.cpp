#include "StdAfx.h"
#include "Matrix.h"
#include <sstream>

using namespace Math;

Matrix4D::Matrix4D()
{
}

Matrix4D::Matrix4D(const Matrix4D& mat)
{
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			this->SetElement(i, j, mat.GetElement(i, j));
		}
	}
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
	
	return matrix;
}

/* static */ Matrix4D Matrix4D::Projection(Real fov /* Field of View */, Real aspect /* Aspect */, Real nearPlane /* Near plane */, Real farPlane /* Far plane */)
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

/* static */ Matrix4D Matrix4D::InitCamera(const Vector3D& forward, const Vector3D& up)
{
	Vector3D forw = forward;
	forw.Normalize(); // TODO: Should not be necessary
	
	Vector3D right = up;
	right.Normalize(); // TODO: Should not be necessary
	right = right.Cross(forw);

	Vector3D newUp = forw.Cross(right);

	Matrix4D matrix;
	matrix.m[0][0] = right.GetX();	matrix.m[0][1] = right.GetY();	matrix.m[0][2] = right.GetZ();	matrix.m[0][3] = 0.0;
	matrix.m[1][0] = newUp.GetX();	matrix.m[1][1] = newUp.GetY();	matrix.m[1][2] = newUp.GetZ();	matrix.m[1][3] = 0.0;
	matrix.m[2][0] = forw.GetX();	matrix.m[2][1] = forw.GetY();	matrix.m[2][2] = forw.GetZ();	matrix.m[2][3] = 0.0;
	matrix.m[3][0] = 0.0;			matrix.m[3][1] = 0.0;			matrix.m[3][2] = 0.0;			matrix.m[3][3] = 1.0;
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

void Matrix4D::SetElement(int i, int j, Real value)
{
#ifdef _DEBUG
	if ((i < 0) || (i > 3))
	{
		fprintf(stderr, "Incorrect row index given (%d)", i);
		exit(-1);
	}
	if ((j < 0) || (j > 3))
	{
		fprintf(stderr, "Incorrect column index given (%d)", j);
		exit(-1);
	}
#endif
	m[i][j] = value;
}

Real Matrix4D::GetElement (int i, int j) const
{
#ifdef _DEBUG
	if ((i < 0) || (i > 3))
	{
		fprintf(stderr, "Incorrect row index given (%d)", i);
		exit(-1);
	}
	if ((j < 0) || (j > 3))
	{
		fprintf(stderr, "Incorrect column index given (%d)", j);
		exit(-1);
	}
#endif
	return m[i][j];
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

	Real oneperw = 1.0 / (m[3][0] * vec.GetX() + m[3][1] * vec.GetY() + m[3][2] * vec.GetZ() + m[3][3]);
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
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			this->SetElement(i, j, mat.GetElement(i, j));
		}
	}

	return *this;
}

const Math::Real* Matrix4D::operator[](int index) const { return m[index]; }
Math::Real* Matrix4D::operator[](int index) { return m[index]; }

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

	return result;
}
	
bool Matrix4D::IsIdentity() const
{
	// TODO: Fix this function
	return true;
}