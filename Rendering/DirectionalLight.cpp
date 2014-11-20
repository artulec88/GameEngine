#include "StdAfx.h"
#include "DirectionalLight.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"

using namespace Rendering;

DirectionalLight::DirectionalLight(const Math::Vector3D& color /* = Math::Vector3D(0.0, 0.0, 0.0) */, Math::Real intensity /* = 0.0 */) :
	BaseLight(color, intensity)
{
	SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "ForwardDirectional")));
	ShadowInfo* shadowInfo = new ShadowInfo(Math::Matrix4D::OrtographicProjection(-40, 40, -40, 40, -40, 40));
	ASSERT(shadowInfo != NULL);
	SetShadowInfo(shadowInfo);
}


DirectionalLight::~DirectionalLight(void)
{
}