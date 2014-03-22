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
	static Camera& GetCamera() { return *s_camera; }
	static void SetCamera(Camera& camera) { s_camera = &camera; }

	static void SetProjection(Math::Real fov, Math::Real width, Math::Real height, Math::Real zNear, Math::Real zFar);

	Transform();
	Transform(Math::Vector3D& pos, Math::Vector3D& rot, Math::Real scale);

	Math::Matrix4D GetTransformation() const;
	Math::Matrix4D GetProjectedTransformation()const ;

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
	// TODO: Consider changing this field to non-static
	static Camera* s_camera;

	// TODO: Consider changing these fields to non-static
	static float fov;
	static float width;
	static float height;
	static float zNear;
	static float zFar;

	Math::Vector3D translation;
	Math::Vector3D rotation;
	Math::Real scale;
	//Vector3f m_scale;
}; /* end class Transform */

} /* end namespace Rendering */