#include "StdAfx.h"
#include "SpotLight.h"

using namespace Rendering;
using namespace Math;

SpotLight::SpotLight(const Vector3D& color /*= Vector3D(0.0, 0.0, 0.0)*/,
		Real intensity /*= 0.0*/,
		const Attenuation& attenuation /*= Attenuation(0.0, 0.0, 0.0)*/,
		const Vector3D& position /*= Vector3D(0.0, 0.0, 0.0)*/,
		Real range /*= 1.0*/,
		const Vector3D& direction /*= Vector3D(0.0, 0.0, 0.0)*/,
		Real cutoff /*= 0.0*/) :
	PointLight(color, intensity, attenuation, position, range),
	direction(direction.Normalized()),
	cutoff(cutoff)
{
}


SpotLight::~SpotLight(void)
{
}

void SpotLight::SetDirection(const Math::Vector3D& direction)
{
	this->direction = direction;

	//ASSERT(this->direction.IsNormalized());
	this->direction.Normalize();
}

void SpotLight::SetCutoff(Math::Real cutoff)
{
	this->cutoff = cutoff;
}