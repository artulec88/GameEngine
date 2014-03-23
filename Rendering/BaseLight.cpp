#include "StdAfx.h"
#include "BaseLight.h"

using namespace Rendering;

BaseLight::BaseLight(void) :
	color(0.0, 0.0, 0.0),
	intensity(0.0)
{
}

BaseLight::BaseLight(const Math::Vector3D& color /* = Math::Vector3D(0.0, 0.0, 0.0) */, Math::Real intensity /* = 0.0 */) :
	color(color),
	intensity(intensity)
{
}

BaseLight::~BaseLight(void)
{
}

Math::Vector3D BaseLight::GetColor() const
{
	return this->color;
}

void BaseLight::SetColor(const Math::Vector3D& color)
{
	this->color = color;
}

Math::Real BaseLight::GetIntensity() const
{
	return this->intensity;
}

void BaseLight::SetIntensity(Math::Real intensity)
{
	this->intensity = intensity;
}
