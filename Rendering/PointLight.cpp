#include "StdAfx.h"
#include "PointLight.h"
//#include "CoreEngine.h"
#include "Shader.h"
#include "Utility\IConfig.h"

using namespace Rendering;
using namespace Utility;

/* static */ bool PointLight::pointLightsEnabled = true;

PointLight::PointLight(const Color& color /* = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO) */, Math::Real intensity /* = REAL_ZERO */,
		const Attenuation& attenuation /* = Attenuation() */) :
	BaseLight(color, intensity),
	attenuation(attenuation)
{
	CalculateRange();
	SetShader(new Shader(GET_CONFIG_VALUE_STR("pointLightShader", "forward-point")));
}

PointLight::~PointLight(void)
{
}

void PointLight::CalculateRange()
{
	Math::Real a = this->attenuation.GetExponent();
	Math::Real b = this->attenuation.GetLinear();

	Math::Real maxColorElement = (color.GetRed() > color.GetGreen()) ? color.GetRed() : color.GetGreen();
	if (color.GetBlue() > maxColorElement)
	{
		maxColorElement = color.GetBlue();
	}
	const int colorDepth = GET_CONFIG_VALUE("ColorDepth", 256);
	Math::Real c = this->attenuation.GetConstant() - colorDepth * intensity * maxColorElement;

	this->range = (-b + sqrt(b*b - 4*a*c)) / (2*a);
}

bool PointLight::IsEnabled() const
{
	if (!pointLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}