#include "StdAfx.h"
#include "DirectionalLight.h"
#include "Shader.h"
#include "Utility\Config.h"

using namespace Rendering;

DirectionalLight::DirectionalLight(const Math::Vector3D& color /* = Math::Vector3D(0.0, 0.0, 0.0) */, Math::Real intensity /* = 0.0 */) :
	BaseLight(color, intensity)
{
	SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "directionalLightShader_default", "ForwardDirectional")));
}


DirectionalLight::~DirectionalLight(void)
{
}