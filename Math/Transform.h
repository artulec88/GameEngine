#ifndef __MATH_TRANSFORM_H__
#define __MATH_TRANSFORM_H__

#include "Math.h"

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

#include <string>

namespace Math
{

	class Transform
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Transform constructor. </summary>
		MATH_API explicit Transform(const Vector3D& pos = Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), const Quaternion& rot = Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), Real scale = REAL_ONE);

		/// <summary> Transform copy constructor. </summary>
		MATH_API Transform(const Transform& transform);
		/// <summary> Transform move constructor. </summary>
		MATH_API Transform(Transform&& transform);

		/// <summary> Transform copy assignment operator. </summary>
		MATH_API Transform& operator=(const Transform& transform);
		/// <summary> Transform move assignment operator. </summary>
		MATH_API Transform& operator=(Transform&& transform);

		MATH_API ~Transform();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API Matrix4D GetTransformation() const;

		Vector3D& GetPos() { return m_pos; }
		const Vector3D& GetPos() const { return m_pos; }
		const Quaternion& GetRot() const { return m_rotation; }
		Quaternion& GetRot() { return m_rotation; }
		Real GetScale() const { return m_scale; }

		void LookAt(const Vector3D& point, const Vector3D& up);
		Quaternion GetLookAtRotation(const Vector3D& point, const Vector3D& up) const;

		Vector3D GetTransformedPos() const
		{
			if (m_parentTransform == NULL)
			{
				return m_pos;
			}
			CalculateParentTransformation();
			return m_parentTransformation.Transform(m_pos);
		}
		Quaternion GetTransformedRot() const
		{
			if (m_parentTransform == NULL)
			{
				return m_rotation;
			}
			return m_parentTransform->GetTransformedRot() * m_rotation; // FIXME: Check quaternion multiplication
		}

		void SetPos(const Vector3D& pos) { m_pos = pos; m_isChanged = true; }
		void SetPos(Real x, Real y, Real z) { m_pos.Set(x, y, z); m_isChanged = true; }
		void SetRot(const Quaternion& rot) { m_rotation = rot; m_isChanged = true; }
		void SetScale(Real scale) { m_scale = scale; m_isChanged = true; }

		MATH_API void SetParent(Transform* t);

		MATH_API void Rotate(const Vector3D& axis, const Angle& angle);
		MATH_API void Rotate(const Quaternion& rot);
		//void SetRotation(Real x, Real y, Real z, Real w);

		void CalculateParentTransformation() const
		{
			CHECK_CONDITION_RETURN_VOID_MATH(m_parentTransform != NULL, Utility::Logging::ERR, "Parent transform is NULL.");
			m_parentTransformation = m_parentTransform->GetTransformation();
		}

		MATH_API std::string ToString() const;

		/**
		 * @brief returns true if the transformation itself or any parent transformation is changed
		 */
		 //bool IsHierarchyChanged() const;
	 /* ==================== Non-static member functions end ==================== */

	 /* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The position.
		/// </summary>
		Vector3D m_pos;

		/// <summary>
		/// The rotation.
		/// </summary>
		Quaternion m_rotation;

		/// <summary>
		/// The scale.
		/// </summary>
		Real m_scale;

		/// <summary>
		/// The parent transform.
		/// </summary>
		Transform* m_parentTransform;
		mutable Matrix4D m_transformation;
		mutable Matrix4D m_parentTransformation;
		mutable bool m_isChanged;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Transform */

} /* end namespace Math */

#endif /* __MATH_TRANSFORM_H__ */