#pragma once

#include "Rendering.h"

#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"

namespace Rendering
{

class Camera;

class RENDERING_API Transform
{
/* ==================== Constructors and destructors begin ==================== */
public:
	Transform(const Math::Vector3D& pos = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), const Math::Quaternion& rot = Math::Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), Math::Real scale = REAL_ONE);
	~Transform();
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Math::Matrix4D GetTransformation() const;
	//Math::Matrix4D GetProjectedTransformation(const CameraBase& camera) const;

	const Math::Vector3D& GetPos() const { return m_pos; }
	const Math::Quaternion& GetRot() const { return m_rotation; }
	Math::Real GetScale() const { return m_scale; }

	void LookAt(const Math::Vector3D& point, const Math::Vector3D& up);
	Math::Quaternion GetLookAtRotation(const Math::Vector3D& point, const Math::Vector3D& up) const;

	Math::Vector3D GetTransformedPos() const
	{
		if (m_parentTransform == NULL)
		{
			return m_pos;
		}
		CalculateParentTransformation();
		return parentTransformation.Transform(m_pos);
	}
	Math::Quaternion GetTransformedRot() const
	{
		if (m_parentTransform == NULL)
		{
			return m_rotation;
		}
		return m_parentTransform->GetTransformedRot() * m_rotation;
	}

	void SetPos(const Math::Vector3D& pos) { m_pos = pos; isChanged = true; }
	void SetPos(Math::Real x, Math::Real y, Math::Real z) { m_pos = Math::Vector3D(x, y, z); isChanged = true; }
	void SetRot(const Math::Quaternion& rot) { m_rotation = rot; isChanged = true; }
	void SetScale(Math::Real scale) { m_scale = scale; isChanged = true; }

	void SetParent(Transform* t);

	void Rotate(const Math::Vector3D& axis, const Math::Angle& angle);
	void Rotate(const Math::Quaternion& rot);
	//void SetRotation(Math::Real x, Math::Real y, Math::Real z, Math::Real w);

	void CalculateParentTransformation() const
	{
		CHECK_CONDITION_RETURN_VOID(m_parentTransform != NULL, Utility::Error, "Parent transform is NULL.");
		parentTransformation = m_parentTransform->GetTransformation();
	}

#ifdef ANT_TWEAK_BAR_ENABLED
	Math::Vector3D& GetPos() { return pos; }
	Math::Quaternion& GetRot() { return rotation; }
#endif

	/**
	 * @brief returns true if the transformation itself or any parent transformation is changed
	 */
	//bool IsHierarchyChanged() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Vector3D m_pos;
	Math::Quaternion m_rotation;
	Math::Real m_scale;
	
	Transform* m_parentTransform;
	mutable Math::Matrix4D transformation;
	mutable Math::Matrix4D parentTransformation;
	mutable bool isChanged;
/* ==================== Non-static member variables end ==================== */
}; /* end class Transform */

} /* end namespace Rendering */