#pragma once

#include "Rendering.h"
#include "Camera.h"

#include "Math\Vector.h"
#include "Math\Matrix.h"

namespace Rendering
{

class RENDERING_API Transform
{
public:
	Transform();
	Transform(Math::Vector3D& pos, Math::Vector3D& rot, Math::Real scale);
	~Transform();

	Math::Matrix4D GetTransformation() const;
	//Math::Matrix4D GetProjectedTransformation(const Camera& camera) const;

	Math::Vector3D& GetPos(); //TODO: Add const keyword
	Math::Vector3D& GetRot();
	Math::Real GetScale() const;
	//inline Vector3f& GetScale() { return m_scale; }

	void SetTranslation(const Math::Vector3D& pos);
	void SetTranslation(Math::Real x, Math::Real y, Math::Real z);
	void SetRotation(const Math::Vector3D& rot);
	void SetRotation(Math::Real x, Math::Real y, Math::Real z);
	void SetScale(float scale);
protected:
private:
	Math::Vector3D translation;
	Math::Vector3D rotation;
	Math::Real scale;
	//Vector3f m_scale;
}; /* end class Transform */

} /* end namespace Rendering */