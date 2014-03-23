#pragma once

#include "Shader.h"
#include "DirectionalLight.h"
#include "Math\Vector.h"
#include <string>

namespace Rendering
{

class PhongShader : public Shader
{
private:
	static Math::Vector3D ambientLight;
	static DirectionalLight directionalLight;
public:
	static Math::Vector3D GetAmbientLight();
	static void SetAmbientLight(const Math::Vector3D& ambientLight);
	static DirectionalLight GetDirectionalLight();
	static void SetDirectionalLight(const DirectionalLight& directionalLight);

public:
	PhongShader(void);
	virtual ~PhongShader(void);

public:
	void UpdateUniforms(const Math::Matrix4D& worldMatrix, const Math::Matrix4D& projectedMatrix, const Material& material); // derived from Shader class
	virtual void SetUniformLight(const std::string& uniform, const DirectionalLight& directionalLight);
}; /* end class BasicShader */

} /* end namespace Rendering */