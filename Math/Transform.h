#ifndef __MATH_TRANSFORM_H__
#define __MATH_TRANSFORM_H__

#include "Math.h"

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace math
{

	class Transform
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Transform constructor. </summary>
		/// <param name="pos"> The position to be stored in the transform. </param>
		/// <param name="rotation"> The rotation to be stored in the transform. </param>
		/// <param name="scale"> The scale to be stored in the transform. </param>
		MATH_API explicit Transform(const Vector3D& pos = Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO),
			const Quaternion& rotation = NO_ROTATION_QUATERNION,
			Real scale = REAL_ONE);

		/// <summary> Transform constructor. </summary>
		/// <param name="posX"> The X component of the position to be stored in the transform. </param>
		/// <param name="posY"> The Y component of the position to be stored in the transform. </param>
		/// <param name="posZ"> The Z component of the position to be stored in the transform. </param>
		/// <param name="rotation"> The rotation to be stored in the transform. </param>
		/// <param name="scale"> The scale to be stored in the transform. </param>
		MATH_API Transform(Real posX, Real posY, Real posZ,
			const Quaternion& rotation = NO_ROTATION_QUATERNION,
			Real scale = REAL_ONE);

		/// <summary> Transform destructor. </summary>
		MATH_API ~Transform();

		/// <summary> Transform copy constructor. </summary>
		/// <param name="transform"> The transform to copy construct from. </param>
		MATH_API Transform(const Transform& transform) = default;

		/// <summary> Transform move constructor. </summary>
		/// <param name="transform"> The transform to move construct from. </param>
		MATH_API Transform(Transform&& transform) = default;

		/// <summary> Transform copy assignment operator. </summary>
		/// <param name="transform"> The transform to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned transform. </returns>
		MATH_API Transform& operator=(const Transform& transform) = default;

		/// <summary> Transform move assignment operator. </summary>
		/// <param name="transform"> The transform to move assign from. </param>
		/// <returns> The reference to the newly move-assigned transform. </returns>
		MATH_API Transform& operator=(Transform&& transform) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		MATH_API Matrix4D GetTransformation() const;

		const Vector3D& GetPos() const { return m_pos; }
		const Quaternion& GetRot() const { return m_rotation; }
		//Quaternion& GetRot() { return m_rotation; }
		Real GetScale() const { return m_scale; }
		//Real GetScaleX() const { return m_scaleMatrix.GetElement(0, 0); }
		//Real GetScaleY() const { return m_scaleMatrix.GetElement(1, 1); }
		//Real GetScaleZ() const { return m_scaleMatrix.GetElement(2, 2); }

		void LookAt(const Vector3D& point, const Vector3D& up);
		Quaternion GetLookAtRotation(const Vector3D& point, const Vector3D& up) const;

		Vector3D GetTransformedPos() const
		{
			if (m_parentTransform == nullptr)
			{
				return m_pos;
			}
			CalculateParentTransformation();
			return m_parentTransformation.Transform(m_pos);
			//return m_parentTransform->GetTransformedPos() + m_pos; // TODO: shouldn't it be enough?
		}
		Quaternion GetTransformedRot() const
		{
			if (m_parentTransform == nullptr)
			{
				return m_rotation;
			}
			return m_parentTransform->GetTransformedRot() * m_rotation; // FIXME: Check quaternion multiplication
		}
		//Real GetTransformedScale() const
		//{
		//	WARNING_LOG_MATH("Function not tested. Use with caution.");
		//	if (m_parentTransform == nullptr)
		//	{
		//		return m_scale;
		//	}
		//	return m_parentTransform->GetTransformedScale() * m_scale;
		//}

		void SetPos(const Vector3D& pos)
		{
			m_pos = pos;
			m_isChanged = true;
		}
		void SetPos(Real x, Real y, Real z)
		{
			m_pos.x = x;
			m_pos.y = y;
			m_pos.z = z;
			m_isChanged = true;
		}
		void SetPosX(Real x)
		{
			m_pos.x = x;
			m_isChanged = true;
		}
		void SetPosY(Real y)
		{
			m_pos.y = y;
			m_isChanged = true;
		}
		void SetPosZ(Real z)
		{
			m_pos.z = z;
			m_isChanged = true;
		}
		void IncreasePos(Real x, Real y, Real z)
		{
			m_pos.x += x;
			m_pos.y += y;
			m_pos.z += z;
			m_isChanged = true;
		}
		void IncreasePos(const Vector3D& translation)
		{
			m_pos += translation;
			m_isChanged = true;
		}
		void IncreasePosX(Real x)
		{
			m_pos.x += x;
			m_isChanged = true;
		}
		void IncreasePosY(Real y)
		{
			m_pos.y += y;
			m_isChanged = true;
		}
		void IncreasePosZ(Real z)
		{
			m_pos.z += z;
			m_isChanged = true;
		}
		void SetRot(const Quaternion& rot)
		{
			m_rotation = rot;
			m_isChanged = true;
		}
		void SetScale(Real scale)
		{
			m_scale = scale;
			m_isChanged = true;
		}

		MATH_API void SetParent(Transform* t);

		MATH_API void Rotate(const Vector3D& axis, const Angle& angle);
		MATH_API void Rotate(const Quaternion& rot);
		//void SetRotation(Real x, Real y, Real z, Real w);

		MATH_API inline void CalculateParentTransformation() const;

		/**
		 * @brief returns true if the transformation itself or any parent transformation is changed
		 */
		 //bool IsHierarchyChanged() const;

		MATH_API bool operator==(const Transform& transform) const;
		MATH_API bool operator!=(const Transform& transform) const;

		friend std::ostream& operator<<(std::ostream& out, const Transform& transform)
		{
			out << "Transform: position: " << transform.m_pos << " rotation: " << transform.m_rotation << " scale: " <<
				transform.m_scale << " hasParent: " << (transform.m_parentTransform != nullptr ? "Yes" : "No");
			return out;
		}
	 /* ==================== Non-static member functions end ==================== */

	 /* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary> The position relative to the parent transform. </summary>
		Vector3D m_pos;

		/// <summary> The transformed position. The position relative to the origin. </summary>
		//mutable Vector3D m_transformedPos;

		/// <summary> The rotation relative to the parent transform. </summary>
		Quaternion m_rotation;

		/// <summary> The transformed rotation. The rotation relative to the origin. </summary>
		//mutable Quaternion m_transformedRotation;

		/// <summary> The scale. </summary>
		/// TODO: Instead of storing a Real value, we should store a scale matrix directly.
		Real m_scale;
		//Matrix4D m_scaleMatrix;

		/// <summary>
		/// The parent transform.
		/// </summary>
		Transform* m_parentTransform;
		mutable Matrix4D m_transformation;
		mutable Matrix4D m_parentTransformation;
		mutable bool m_isChanged; // TODO: Compiler warning '7' bytes padding added after 'm_isChanged'.
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Transform */

} /* end namespace math */

#endif /* __MATH_TRANSFORM_H__ */