#include "StdAfx.h"
#include "PointLight.h"
#include "ForwardPointShader.h"

using namespace Rendering;

PointLight::PointLight(const Math::Vector3D& color /*= Math::Vector3D(0.0, 0.0, 0.0)*/, Math::Real intensity /*= 0.0*/,
		const Attenuation& attenuation /*= Attenuation(0.0, 0.0, 0.0)*/,
		const Math::Vector3D& position /*= Math::Vector3D(0.0, 0.0, 0.0)*/,
		Math::Real range /* = 1.0 */) :
	BaseLight(ForwardPointShader::GetInstance(), color, intensity),
	attenuation(attenuation),
	position(position),
	range(range)
{
}


PointLight::~PointLight(void)
{
}

void PointLight::SetPosition(const Math::Vector3D& position)
{
	this->position = position;
}

void PointLight::SetRange(Math::Real range)
{
	this->range = range;
}
