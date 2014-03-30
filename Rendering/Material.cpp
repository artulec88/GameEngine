#include "StdAfx.h"
#include "Material.h"

using namespace Rendering;

Material::Material(Texture* texture /* = NULL */,
	const Math::Vector3D& color /* = Math::Vector3D(1.0, 1.0, 1.0) */,
	Math::Real specularIntensity /* = 2 */,
	Math::Real specularPower /* = 32 */) :
	texture(texture),
	color(color),
	specularIntensity(specularIntensity),
	specularPower(specularPower)
{
}


Material::~Material(void)
{
	// TODO: Deallocate textures
	if (texture != NULL)
	{
		delete texture;
		texture = NULL;
	}
}

Math::Real Material::GetSpecularIntensity() const
{
	return this->specularIntensity;
}

void Material::SetSpecularIntensity(Math::Real specularIntensity)
{
	this->specularIntensity = specularIntensity;
}

Math::Real Material::GetSpecularPower() const
{
	return this->specularPower;
}

void Material::SetSpecularPower(Math::Real specularPower)
{
	this->specularPower = specularPower;
}