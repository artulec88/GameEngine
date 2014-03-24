#pragma once

#include "Rendering.h"
#include "Shader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Math\Vector.h"
#include <string>

namespace Rendering
{

class RENDERING_API PhongShader : public Shader
{
private:
	/**
	 * @brief
	 *
	 * TODO: This value should be the same as the value in the fragment shader.
	 */
	static const int MAX_POINT_LIGHTS = 4;
	static const int MAX_SPOT_LIGHTS = 4;
	static int pointLightCount;
	static int spotLightCount;
	static Math::Vector3D ambientLight;
	static DirectionalLight directionalLight;
	static PointLight* pointLights;
	static SpotLight* spotLights;
public:
	static Math::Vector3D GetAmbientLight();
	static void SetAmbientLight(const Math::Vector3D& ambientLight);
	static DirectionalLight GetDirectionalLight();
	static void SetDirectionalLight(const DirectionalLight& directionalLight);
	static void SetPointLights(PointLight* pointLights, int arraySize);
	static void SetSpotLights(SpotLight* spotLights, int arraySize);

public:
	PhongShader(void);
	virtual ~PhongShader(void);

public:
	void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer); // derived from Shader class
}; /* end class BasicShader */

} /* end namespace Rendering */