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
	Transform(const Math::Vector3D& pos, const Math::Quaternion& rot, Math::Real scale);
	~Transform();

	Math::Matrix4D GetTransformation() const;
	//Math::Matrix4D GetProjectedTransformation(const Camera& camera) const;

	Math::Vector3D& GetPos(); //TODO: Add const keyword
	Math::Quaternion& GetRot();
	Math::Real GetScale() const;
	//inline Vector3f& GetScale() { return m_scale; }

	void SetTranslation(const Math::Vector3D& pos);
	void SetTranslation(Math::Real x, Math::Real y, Math::Real z);
	void SetRotation(const Math::Quaternion& rot);
	//void SetRotation(Math::Real x, Math::Real y, Math::Real z);
	void SetScale(float scale);
protected:
private:
	Math::Vector3D translation;
	Math::Quaternion rotation;
	Math::Real scale;
	//Vector3f m_scale;
}; /* end class Transform */

} /* end namespace Rendering */