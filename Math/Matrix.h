#ifndef __MATH_MATRIX_H__
#define __MATH_MATRIX_H__

#include "Math.h"
#include "Angle.h"
#include "Vector.h"
#ifdef CALCULATE_MATH_STATS
#include "Statistics.h"
#include "IStatisticsStorage.h"
#endif

#include "Utility\ILogger.h"

#include <string>

#define MATRIX_SIZE 4
#define MATRIX_MODE_TWO_DIMENSIONS // if disabled one dimensional array will be used to store the matrix's values.

namespace Math
{

	/// <summary> The class representing the 4x4 matrix. </summary>
	/// <remarks>
	///	The matrix is defined in a column-major ordering.
	/// See http://www.in.tum.de/fileadmin/user_upload/Lehrstuehle/Lehrstuhl_XV/Teaching/SS07/Praktikum/MatricesTips.pdf for details.
	/// http://stackoverflow.com/questions/10718061/should-arrays-be-used-in-c.
	/// </remarks>
	class Matrix4D
	{
		/* ==================== Static variables and functions begin ==================== */
#ifdef CALCULATE_MATH_STATS
	private:
		static Statistics::ClassStats& s_classStats;
#endif
	public:
		MATH_API static const Matrix4D IDENTITY_MATRIX;
		static int Signum(int i, int j);
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>Default Matrix4D constructor. It creates an identity matrix.</summary>
		/// <returns>Identity matrix.</returns>
		MATH_API Matrix4D();
		/// <summary>
		/// Creates a 4x4 matrix and assigns a specified value for each of all 16 elements.
		/// </summary>
		MATH_API Matrix4D(Math::Real m00, Math::Real m01, Math::Real m02, Math::Real m03,
			Math::Real m10, Math::Real m11, Math::Real m12, Math::Real m13,
			Math::Real m20, Math::Real m21, Math::Real m22, Math::Real m23,
			Math::Real m30, Math::Real m31, Math::Real m32, Math::Real m33);
		/// <summary> Creates a matrix based on the screen position and the scale. </summary>
		/// <param name='screenPosition'> The position on the screen </param>
		/// <param name='scale'> The scale </param>
		/// <remarks> The function is used for the gui textures. </remarks>
		MATH_API Matrix4D(const Vector2D& screenPosition, const Vector2D& scale);
		/// <summary>Creates scale matrix based on the specified parameter.</summary>
		/// <param name='scale'>The scale in all dimensions: X, Y and Z.</param>
		/// <returns>Scale matrix.</returns>
		MATH_API explicit Matrix4D(Real scale);
		/// <summary>Creates translation matrix based on the specified parameters.</summary>
		/// <param name='posX'>The X coordinate of the translation.</param>
		/// <param name='posY'>The Y coordinate of the translation.</param>
		/// <param name='posZ'>The Z coordinate of the translation.</param>
		/// <returns>Translation matrix.</returns>
		MATH_API Matrix4D(Real posX, Real posY, Real posZ);
		/// <summary>Creates translation matrix based on the specified parameter.</summary>
		/// <param name='pos'>The 3D translation vector.</param>
		/// <returns>Translation matrix.</returns>
		MATH_API explicit Matrix4D(const Vector3D& pos);
		/// <summary>Creates rotation matrix based on the specified parameters.</summary>
		/// <param name='angleX'>The rotation angle around X axis.</param>
		/// <param name='angleY'>The rotation angle around Y axis.</param>
		/// <returns>Rotation matrix.</returns>
		MATH_API Matrix4D(const Angle& angleX, const Angle& angleY);
		/// <summary>Creates rotation matrix based on the specified parameters.</summary>
		/// <param name='angleX'>The rotation angle around X axis.</param>
		/// <param name='angleY'>The rotation angle around Y axis.</param>
		/// <param name='angleZ'>The rotation angle around Z axis.</param>
		/// <returns>Rotation matrix.</returns>
		/// <remarks> See http://planning.cs.uiuc.edu/node102.html </remarks>
		MATH_API Matrix4D(const Angle& angleX, const Angle& angleY, const Angle& angleZ);
		/// <summary>Creates rotation matrix based on the specified parameters.</summary>
		/// <param name='forward'>The forward vector.</param>
		/// <param name='up'>The up vector.</param>
		/// <returns>Rotation matrix.</returns>
		MATH_API Matrix4D(const Vector3D& forward, const Vector3D& up);
		/// <summary>Creates rotation matrix based on the specified parameters.</summary>
		/// <param name='forward'>The forward vector.</param>
		/// <param name='up'>The up vector.</param>
		/// <param name='right'>The right vector.</param>
		/// <returns>Rotation matrix.</returns>
		MATH_API Matrix4D(const Vector3D& forward, const Vector3D& up, const Vector3D& right);
		/// <summary>Creates perspective projection matrix based on the specified parameters.</summary>
		/// <param name='fov'>Field of view</param>
		/// <param name='aspect'>Aspect ratio</param>
		/// <param name='nearPlane'>Near plane</param>
		/// <param name='farPlane'>Far plane</param>
		/// <returns>Perspective projection matrix.</returns>
		MATH_API Matrix4D(const Angle& fov, Real aspect, Real nearPlane, Real farPlane);
		/// <summary>Creates ortographic projection matrix based on the specified parameters.</summary>
		/// <param name='left'>The left coordinate for the vertical clipping planes.</param>
		/// <param name='right'>The right coordinate for the vertical clipping planes.</param>
		/// <param name='bottom'>The bottom coordinate for the horizontal clipping planes.</param>
		/// <param name='top'>The top coordinate for the horizontal clipping planes.</param>
		/// <param name='nearPlane'>The distance to the nearer depth clipping planes. The value is negative if the plane is to be behind the viewer.</param>
		/// <param name='farPlane'>The distance to the farther depth clipping planes. The value is negative if the plane is to be behind the viewer.</param>
		/// <returns>Ortographic projection matrix.</returns>
		MATH_API Matrix4D(Real left, Real right, Real bottom, Real top, Real nearPlane, Real farPlane);
		/// <summary>A simple matrix copy-constructor.</summary>
		/// <param name='mat'>A matrix which is to be copied.</param>
		/// <returns>A deep copy of the given matrix.</returns>
		MATH_API Matrix4D(const Matrix4D& mat);

		/// <summary>A simple matrix move-constructor.</summary>
		/// <param name='mat'>A matrix which is to be moved.</param>
		MATH_API Matrix4D(Matrix4D&& mat);

		MATH_API Matrix4D& operator=(const Matrix4D& mat); // copy assignment operator
		MATH_API Matrix4D& operator=(Matrix4D&& mat); // move assignment operator

		/// <summary>A simple matrix destructor.</summary>
		MATH_API ~Matrix4D();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API inline void SetElement(int i, int j, Real value);
		MATH_API inline Real GetElement(int i, int j) const;

		MATH_API Matrix4D operator*(const Matrix4D& m) const;
		MATH_API Vector3D operator*(const Vector3D& vec) const;
		MATH_API Vector4D operator*(const Vector4D& vec) const;
		MATH_API bool operator==(const Matrix4D& m) const;
		MATH_API bool operator!=(const Matrix4D& m) const;

#ifdef MATRIX_MODE_TWO_DIMENSIONS
		MATH_API inline const Math::Real* operator[](int index) const;
		MATH_API inline Math::Real* operator[](int index);
#else
		MATH_API inline const Math::Real* At(int index) const;
		MATH_API inline Math::Real operator[](int index) const;
#endif

		MATH_API void SetScaleMatrix(Real scaleX, Real scaleY, Real scaleZ);
		MATH_API void SetPerspectiveProjection(const Angle& fov, Real aspect, Real nearPlane, Real farPlane);

		MATH_API Vector3D Transform(const Vector3D& vec); // TODO: Write tests

		MATH_API Matrix4D Transposition() const;
		MATH_API Matrix4D Inversion() const; // TODO: Write tests
		MATH_API Real Det(int p, int q) const;

		/// <summary>Checks whether the matrix is an identity matrix or not. It should be used only in tests.</summary>
		/// <returns>True if the matrix is an identity matrix. False otherwise.</returns>
		MATH_API bool IsIdentity() const;

		MATH_API std::string ToString() const;
	private:
		void SetRotationFromVectors(const Vector3D& forward, const Vector3D& up, const Vector3D& right);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		// TODO: Consider using a one-dimensional array to store MATRIX_SIZE * MATRIX_SIZE elements.
		// TODO: Read an article http://stackoverflow.com/questions/17259877/1d-or-2d-array-whats-faster.
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		std::array<std::array<Real, MATRIX_SIZE>, MATRIX_SIZE> m_values;
#else
		std::array<Real, MATRIX_SIZE * MATRIX_SIZE> m_values;
#endif

#ifdef CALCULATE_MATH_STATS
		Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Matrix */

	inline Real Matrix4D::GetElement(int i, int j) const
	{
		CHECK_CONDITION_EXIT_MATH((i >= 0) && (i < MATRIX_SIZE), Utility::Logging::ERR, "Incorrect row index given (", i, ")");
		CHECK_CONDITION_EXIT_MATH((j >= 0) && (j < MATRIX_SIZE), Utility::Logging::ERR, "Incorrect column index given (", j, ")");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		return m_values[i][j];
#else
		return m_values[i * MATRIX_SIZE + j];
#endif
	}

	inline void Matrix4D::SetElement(int i, int j, Real value)
	{
		CHECK_CONDITION_EXIT_MATH((i >= 0) && (i < MATRIX_SIZE), Utility::Logging::ERR, "Incorrect row index given (", i, ")");
		CHECK_CONDITION_EXIT_MATH((j >= 0) && (j < MATRIX_SIZE), Utility::Logging::ERR, "Incorrect column index given (", j, ")");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		m_values[i][j] = value;
#else
		m_values[i * MATRIX_SIZE + j] = value;
#endif
	}

#ifdef MATRIX_MODE_TWO_DIMENSIONS
	inline const Math::Real* Matrix4D::operator[](int index) const
	{
		CHECK_CONDITION_EXIT_MATH((index >= 0) && (index < MATRIX_SIZE), Utility::Logging::ERR, "Incorrect row index given (", index, ")");
		return &m_values[index][0];
	}
	inline Math::Real* Matrix4D::operator[](int index)
	{
		CHECK_CONDITION_EXIT_MATH((index >= 0) && (index < MATRIX_SIZE), Utility::Logging::ERR, "Incorrect row index given (", index, ")");
		return &m_values[index][0];
	}
#else
	inline const Math::Real* Matrix4D::At(int index) const
	{
		CHECK_CONDITION_EXIT_MATH((index >= 0) && (index < MATRIX_SIZE), Utility::Logging::ERR, "Incorrect index given (", index, ")");
		return &m_values[index];
	}
	inline Math::Real Matrix4D::operator[](int index) const
	{
		CHECK_CONDITION_EXIT_MATH((index >= 0) && (index < MATRIX_SIZE * MATRIX_SIZE), Utility::Logging::ERR, "Incorrect index given (", index, ")");
		return m_values[index];
	}
#endif

} /* end namespace Math */

#endif /* __MATH_MATRIX_H__ */