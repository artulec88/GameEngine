#include "StdAfx.h"
#include "DirectionalLight.h"

using namespace Rendering;

DirectionalLight::DirectionalLight(const Math::Vector3D& color /* = Math::Vector3D(0.0, 0.0, 0.0) */, Math::Real intensity /* = 0.0 */,
		const Math::Vector3D& direction /* = Math::Vector3D(0.0, 0.0, 0.0) */) :
	BaseLight(color, intensity),
	direction(direction.Normalized())
{
}


DirectionalLight::~DirectionalLight(void)
{
}

Math::Vector3D DirectionalLight::GetDirection() const
{
	return this->direction;
}

void DirectionalLight::SetDirection(const Math::Vector3D& direction)
{
	this->direction = direction;
	this->direction.Normalize(); // just in case
}