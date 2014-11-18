#pragma once

#include "Rendering.h"
#include "Camera.h"

#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"

namespace Rendering
{

class RENDERING_API Transform
{
public:
	Transform();
	Transform(const Math::Vector3D& pos, const Math::Quaternion& rot, const Math::Vector3D& scale);
	~Transform();

	Math::Matrix4D GetTransformation() const;
	//Math::Matrix4D GetProjectedTransformation(const Camera& camera) const;

	//Math::Vector3D& GetPos() { return pos; } //TODO: Add const keyword
	//Math::Quaternion& GetRot() { return rotation; }
	//Math::Vector3D& GetScale() { return scale; }
	const Math::Vector3D& GetPos() const { return pos; }
	const Math::Quaternion& GetRot() const { return rotation; }
	const Math::Vector3D& GetScale() const { return scale; }

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

	void SetTranslation(const Math::Vector3D& pos);
	void SetTranslation(Math::Real x, Math::Real y, Math::Real z);
	void SetRotation(const Math::Quaternion& rot);
	//void SetRotation(Math::Real x, Math::Real y, Math::Real z, Math::Real w);
	void SetScale(const Math::Vector3D& scaleVec);

	void Rotate(const Math::Vector3D& axis, const Math::Angle& angle);

	void LookAt(const Math::Vector3D& point, const Math::Vector3D& up);
	Math::Quaternion GetLookAtDirection(const Math::Vector3D& point, const Math::Vector3D& up) const;

	void SetParent(Transform* t) { parentTransform = t; }
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
	Math::Vector3D scale;
	
	Transform* parentTransform;
	mutable Math::Matrix4D transformation;
	mutable Math::Matrix4D parentTransformation;
	mutable bool isChanged;
}; /* end class Transform */

} /* end namespace Rendering */