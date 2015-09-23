#pragma once

#include "Math.h"
#include "Angle.h"
#include "Vector.h"
#ifdef CALCULATE_MATH_STATS
#include "Statistics.h"
#include "IStatisticsStorage.h"
#endif

//#include "Utility\ISerializable.h"
#include "Utility\ILogger.h"

#include <string>

namespace Math
{

#define MATRIX_SIZE 4

class MATH_API Matrix4D// : public Utility::ISerializable
{
/* ==================== Static variables and functions begin ==================== */
#ifdef CALCULATE_MATH_STATS
private:
	static Statistics::ClassStats& s_classStats;
#endif
public:
	static const Matrix4D IDENTITY_MATRIX;
	static int Signum(int i, int j) const;
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Matrix4D();
	/// <summary>Creates scale matrix based on the specified parameter.</summary>
	/// <param name='scale'>The scale in all dimensions: X, Y and Z.</param>
	/// <returns>Scale matrix.</returns>
	Matrix4D(Real scale);
	/// <summary>Creates translation matrix based on the specified parameters.</summary>
	/// <param name='posX'>The X coordinate of the translation.</param>
	/// <param name='posY'>The Y coordinate of the translation.</param>
	/// <param name='posZ'>The Z coordinate of the translation.</param>
	/// <returns>Translation matrix.</returns>
	Matrix4D(Real posX, Real posY, Real posZ);
	/// <summary>Creates translation matrix based on the specified parameter.</summary>
	/// <param name='pos'>The 3D translation vector.</param>
	/// <returns>Translation matrix.</returns>
	Matrix4D(const Vector3D& pos);
	/// <summary>Creates rotation matrix based on the specified parameters.</summary>
	/// <param name='angleX'>The rotation angle around X axis.</param>
	/// <param name='angleY'>The rotation angle around Y axis.</param>
	/// <returns>Rotation matrix.</returns>
	Matrix4D(const Angle& angleX, const Angle& angleY);
	/// <summary>Creates rotation matrix based on the specified parameters.</summary>
	/// <param name='angleX'>The rotation angle around X axis.</param>
	/// <param name='angleY'>The rotation angle around Y axis.</param>
	/// <param name='angleZ'>The rotation angle around Z axis.</param>
	/// <returns>Rotation matrix.</returns>
	Matrix4D(const Angle& angleX, const Angle& angleY, const Angle& angleZ);
	/// <summary>Creates rotation matrix based on the specified parameters.</summary>
	/// <param name='forward'>The forward vector.</param>
	/// <param name='up'>The up vector.</param>
	/// <returns>Rotation matrix.</returns>
	Matrix4D(const Vector3D& forward, const Vector3D& up);
	/// <summary>Creates rotation matrix based on the specified parameters.</summary>
	/// <param name='forward'>The forward vector.</param>
	/// <param name='up'>The up vector.</param>
	/// <param name='right'>The right vector.</param>
	/// <returns>Rotation matrix.</returns>
	Matrix4D(const Vector3D& forward, const Vector3D& up, const Vector3D& right);
	/// <summary>Creates perspective projection matrix based on the specified parameters.</summary>
	/// <param name='fov'>Field of view</param>
	/// <param name='aspect'>Aspect ratio</param>
	/// <param name='nearPlane'>Near plane</param>
	/// <param name='farPlane'>Far plane</param>
	/// <returns>Perspective projection matrix.</returns>
	Matrix4D(const Angle& fov, Real aspect, Real nearPlane, Real farPlane);
	/// <summary>Creates ortographic projection matrix based on the specified parameters.</summary>
	/// <param name='left'>The left coordinate for the vertical clipping planes.</param>
	/// <param name='right'>The right coordinate for the vertical clipping planes.</param>
	/// <param name='bottom'>The bottom coordinate for the horizontal clipping planes.</param>
	/// <param name='top'>The top coordinate for the horizontal clipping planes.</param>
	/// <param name='nearPlane'>The distance to the nearer depth clipping planes. The value is negative if the plane is to be behind the viewer.</param>
	/// <param name='farPlane'>The distance to the farther depth clipping planes. The value is negative if the plane is to be behind the viewer.</param>
	/// <returns>Ortographic projection matrix.</returns>
	Matrix4D(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane);
	Matrix4D(const Matrix4D& mat);
	~Matrix4D();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline void SetElement(int i, int j, Real value);
	inline Real GetElement (int i, int j) const;
	
	Matrix4D operator*(const Matrix4D& m) const;
	Vector3D operator*(const Vector3D& vec) const;
	bool operator==(const Matrix4D& m) const;
	bool operator!=(const Matrix4D& m) const;
	Matrix4D& operator=(const Matrix4D& mat);

	inline const Math::Real* operator[](int index) const;
	inline Math::Real* operator[](int index);

	void SetScaleMatrix(Real scaleX, Real scaleY, Real scaleZ);
	void SetPerspectiveProjection(const Angle& fov, Real aspect, Real nearPlane, Real farPlane);

	Vector3D Transform(const Vector3D& vec); // Write tests
	
	Matrix4D Transposition() const;
	Matrix4D Inversion() const; // TODO: Write tests
	Real Det(int p, int q) const;
	
	/**
	 * Checks whether the matrix is an identity matrix or not.
	 * 
	 * @brief Used only during tests
	 */
	bool IsIdentity() const;

	std::string ToString() const;
private:
	void SetRotationFromVectors(const Vector3D& forward, const Vector3D& up, const Vector3D& right);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Real m[MATRIX_SIZE][MATRIX_SIZE];
#ifdef CALCULATE_MATH_STATS
	mutable Statistics::ClassStats& m_classStats;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class Matrix */

inline Real Matrix4D::GetElement (int i, int j) const
{
	CHECK_CONDITION_EXIT((i >= 0) && (i < MATRIX_SIZE), Utility::Error, "Incorrect row index given (%d)", i);
	CHECK_CONDITION_EXIT((j >= 0) && (j < MATRIX_SIZE), Utility::Error, "Incorrect column index given (%d)", j);
	return m[i][j];
}

inline void Matrix4D::SetElement(int i, int j, Real value)
{
	CHECK_CONDITION_EXIT((i >= 0) && (i < MATRIX_SIZE), Utility::Error, "Incorrect row index given (%d)", i);
	CHECK_CONDITION_EXIT((j >= 0) && (j < MATRIX_SIZE), Utility::Error, "Incorrect column index given (%d)", j);
	m[i][j] = value;
}

inline const Math::Real* Matrix4D::operator[](int index) const
{
	CHECK_CONDITION_EXIT((index >= 0) && (index < MATRIX_SIZE), Utility::Error, "Incorrect row index given (%d)", index);
	return m[index];
}
inline Math::Real* Matrix4D::operator[](int index)
{
	CHECK_CONDITION_EXIT((index >= 0) && (index < MATRIX_SIZE), Utility::Error, "Incorrect row index given (%d)", index);
	return m[index];
}

} /* end namespace Math */
