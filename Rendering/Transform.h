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
public:
	Transform(const Math::Vector3D& pos = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), const Math::Quaternion& rot = Math::Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), Math::Real scale = REAL_ONE);
	~Transform();

	Math::Matrix4D GetTransformation() const;
	//Math::Matrix4D GetProjectedTransformation(const CameraBase& camera) const;

#ifdef ANT_TWEAK_BAR_ENABLED
	Math::Vector3D& GetPos() { return pos; }
	Math::Quaternion& GetRot() { return rotation; }
#endif
	//Math::Vector3D& GetScale() { return scale; }
	const Math::Vector3D& GetPos() const { return pos; }
	const Math::Quaternion& GetRot() const { return rotation; }
	Math::Real GetScale() const { return scale; }

	void LookAt(const Math::Vector3D& point, const Math::Vector3D& up);
	Math::Quaternion GetLookAtRotation(const Math::Vector3D& point, const Math::Vector3D& up) const;

	Math::Vector3D GetTransformedPos() const
	{
		if (parentTransform == NULL)
		{
			return pos;
		}
		CalculateParentTransformation();
		return parentTransformation.Transform(pos);
	}
	Math::Quaternion GetTransformedRot() const
	{
		if (parentTransform == NULL)
		{
			return rotation;
		}
		return parentTransform->GetTransformedRot() * rotation;
	}

	void SetPos(const Math::Vector3D& pos) { this->pos = pos; isChanged = true; }
	void SetPos(Math::Real x, Math::Real y, Math::Real z) { this->pos = Math::Vector3D(x, y, z); isChanged = true; }
	void SetRot(const Math::Quaternion& rot) { this->rotation = rot; isChanged = true; }
	void SetScale(Math::Real scale) { this->scale = scale; isChanged = true; }

	void SetParent(Transform* t) { parentTransform = t; }

	void Rotate(const Math::Vector3D& axis, const Math::Angle& angle);
	void Rotate(const Math::Quaternion& rot);
	//void SetRotation(Math::Real x, Math::Real y, Math::Real z, Math::Real w);

	void CalculateParentTransformation() const
	{
		ASSERT(parentTransform != NULL);
		if (parentTransform == NULL)
		{
			LOG(Utility::Error, LOGPLACE, "Parent transform is NULL");
			return;
		}
		parentTransformation = parentTransform->GetTransformation();
	}
	/**
	 * @brief returns true if the transformation itself or any parent transformation is changed
	 */
	//bool IsHierarchyChanged() const;
protected:
private:
	Math::Vector3D pos;
	Math::Quaternion rotation;
	Math::Real scale;
	
	Transform* parentTransform;
	mutable Math::Matrix4D transformation;
	mutable Math::Matrix4D parentTransformation;
	mutable bool isChanged;
}; /* end class Transform */

} /* end namespace Rendering */