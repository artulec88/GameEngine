#pragma once

#include "Rendering.h"
#include "Camera.h"

#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"
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

	Math::Vector3D& GetPos() { return translation; } //TODO: Add const keyword
	Math::Quaternion& GetRot() { return rotation; }
	Math::Vector3D& GetScale() { return scale; }
	const Math::Vector3D& GetPos() const { return translation; }
	const Math::Quaternion& GetRot() const { return rotation; }
	const Math::Vector3D& GetScale() const { return scale; }
	//inline Vector3f& GetScale() { return m_scale; }

	void SetTranslation(const Math::Vector3D& pos);
	void SetTranslation(Math::Real x, Math::Real y, Math::Real z);
	void SetRotation(const Math::Quaternion& rot);
	//void SetRotation(Math::Real x, Math::Real y, Math::Real z, Math::Real w);
	void SetScale(const Math::Vector3D& scaleVec);

	inline void SetParent(Transform* t) { parentTransform = t; }
	/**
	 * @brief returns true if the transformation itself or any parent transformation is changed
	 */
	//bool IsHierarchyChanged() const;
protected:
private:
	Math::Vector3D translation;
	Math::Quaternion rotation;
	Math::Vector3D scale;
	
	Transform* parentTransform;
	mutable Math::Matrix4D transformation;
	mutable bool isChanged;
}; /* end class Transform */

} /* end namespace Rendering */