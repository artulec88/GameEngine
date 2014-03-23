#include "StdAfx.h"
#include "PointLight.h"

using namespace Rendering;

PointLight::PointLight(const Math::Vector3D& color /*= Math::Vector3D(0.0, 0.0, 0.0)*/, Math::Real intensity /*= 0.0*/,
		const Attenuation& attenuation /*= Attenuation(0.0, 0.0, 0.0)*/,
		const Math::Vector3D& position /*= Math::Vector3D(0.0, 0.0, 0.0)*/) :
	BaseLight(color, intensity),
	attenuation(attenuation),
	position(position)
{
}


PointLight::~PointLight(void)
{
}

void PointLight::SetPosition(const Math::Vector3D& position)
{
	this->position = position;
}
