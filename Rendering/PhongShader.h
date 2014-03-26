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
/* ==================== Static variables begin ==================== */
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
/* ==================== Static variables end ==================== */

/* ==================== Static functions begin ==================== */
public:
	static PhongShader* GetInstance();
	static Math::Vector3D GetAmbientLight();
	static void SetAmbientLight(const Math::Vector3D& ambientLight);
	static DirectionalLight GetDirectionalLight();
	static void SetDirectionalLight(const DirectionalLight& directionalLight);
	static void SetPointLights(PointLight* pointLights, int arraySize);
	static void SetSpotLights(SpotLight* spotLights, int arraySize);
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	virtual ~PhongShader(void);
private:
	PhongShader(void);
	PhongShader(PhongShader const&) {}
	void operator=(PhongShader const&) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer); // derived from Shader class
/* ==================== Non-static member functions end ==================== */
}; /* end class BasicShader */

} /* end namespace Rendering */