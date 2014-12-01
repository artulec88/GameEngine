#include "StdAfx.h"
#include "DirectionalLight.h"
#include "CoreEngine.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"

using namespace Rendering;

/* static */ bool DirectionalLight::directionalLightsEnabled = true;

DirectionalLight::DirectionalLight(const Math::Vector3D& color /* = Math::Vector3D(0.0, 0.0, 0.0) */, Math::Real intensity /* = 0.0 */) :
	BaseLight(color, intensity)
{
	SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "ForwardDirectional")));
	ShadowInfo* shadowInfo = new ShadowInfo(Math::Matrix4D::OrtographicProjection(-40, 40, -40, 40, -40, 40), true);
	ASSERT(shadowInfo != NULL);
	SetShadowInfo(shadowInfo);
}


DirectionalLight::~DirectionalLight(void)
{
}

bool DirectionalLight::IsEnabled() const
{
	if (!directionalLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}