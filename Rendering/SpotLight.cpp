#include "StdAfx.h"
#include "SpotLight.h"
#include "Shader.h"
#include "Utility\IConfig.h"

using namespace Rendering;
using namespace Math;

/* static */ bool SpotLight::spotLightsEnabled = true;

SpotLight::SpotLight(const Vector3D& color /*= Vector3D(0.0, 0.0, 0.0)*/,
		Real intensity /*= 0.0*/,
		const Attenuation& attenuation /*= Attenuation() */,
		Real cutoff /*= 0.0*/) :
	PointLight(color, intensity, attenuation),
	cutoff(cutoff)
{
	SetShader(new Shader(GET_CONFIG_VALUE_STR("spotLightShader", "forward-spot")));
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

bool SpotLight::IsEnabled() const
{
	if (!spotLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}