#pragma once

#include "Shader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Math\Vector.h"
#include <string>

namespace Rendering
{

class PhongShader : public Shader
{
private:
	/**
	 * @brief
	 *
	 * TODO: This value should be the same as the value in the fragment shader.
	 */
	static const int MAX_POINT_LIGHTS = 4;
	static int pointLightCount;
	static Math::Vector3D ambientLight;
	static DirectionalLight directionalLight;
	static PointLight* pointLights;
public:
	static Math::Vector3D GetAmbientLight();
	static void SetAmbientLight(const Math::Vector3D& ambientLight);
	static DirectionalLight GetDirectionalLight();
	static void SetDirectionalLight(const DirectionalLight& directionalLight);
	static void SetPointLights(PointLight* pointLights, int arraySize);

public:
	PhongShader(void);
	virtual ~PhongShader(void);

public:
	void UpdateUniforms(const Math::Matrix4D& worldMatrix, const Math::Matrix4D& projectedMatrix, const Material& material); // derived from Shader class
}; /* end class BasicShader */

} /* end namespace Rendering */