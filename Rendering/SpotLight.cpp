#include "StdAfx.h"
#include "SpotLight.h"
#include "Shader.h"
#include "Utility\IConfig.h"

using namespace Rendering;
using namespace Math;

/* static */ bool SpotLight::spotLightsEnabled = true;

SpotLight::SpotLight(const Vector3D& color /*= Vector3D(0.0, 0.0, 0.0)*/,
		Real intensity /*= 0.0*/,
		const Attenuation& attenuation /*= Attenuation(0.0, 0.0, 0.0)*/,
		Real cutoff /*= 0.0*/) :
	PointLight(new Shader(GET_CONFIG_VALUE_STR("spotLightShader", "ForwardSpot")), color, intensity, attenuation),
	//direction(direction.Normalized()),
	cutoff(cutoff)
{
	//SetShader(ForwardSpotShader::GetInstance());
	//SetShader(ForwardPointShader::GetInstance());
}


SpotLight::~SpotLight(void)
{
}

//void SpotLight::SetDirection(const Math::Vector3D& direction)
//{
//	this->direction = direction;
//
//	//ASSERT(this->direction.IsNormalized());
//	this->direction.Normalize();
//}

void SpotLight::SetCutoff(Math::Real cutoff)
{
	this->cutoff = cutoff;
}

bool SpotLight::IsEnabled() const
{
	if (!spotLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}