#include "StdAfx.h"
#include "PointLight.h"
#include "ForwardPointShader.h"
#include "Utility\Config.h"

using namespace Rendering;
using namespace Utility;

PointLight::PointLight(const Math::Vector3D& color /*= Math::Vector3D(0.0, 0.0, 0.0)*/, Math::Real intensity /*= 0.0*/,
		const Attenuation& attenuation /*= Attenuation(0.0, 0.0, 0.0)*/) :
	BaseLight(ForwardPointShader::GetInstance(), color, intensity),
	attenuation(attenuation)
{
	CalculateRange();
}


PointLight::~PointLight(void)
{
}

void PointLight::CalculateRange()
{
	Math::Real a = this->attenuation.GetExponent();
	Math::Real b = this->attenuation.GetLinear();

	Math::Real maxColorElement = (color.GetX() > color.GetY()) ? color.GetX() : color.GetY();
	if (color.GetZ() > maxColorElement)
	{
		maxColorElement = color.GetZ();
	}
	const int colorDepth = GET_CONFIG_VALUE("ColorDepth", "ColorDepthDefault", 256);
	Math::Real c = this->attenuation.GetConstant() - colorDepth * intensity * maxColorElement;

	this->range = (-b + sqrt(b*b - 4*a*c)) / (2*a);
}