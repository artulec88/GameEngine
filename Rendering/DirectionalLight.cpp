#include "StdAfx.h"
#include "DirectionalLight.h"
#include "ForwardDirectionalShader.h"

using namespace Rendering;

DirectionalLight::DirectionalLight(const Math::Vector3D& color /* = Math::Vector3D(0.0, 0.0, 0.0) */, Math::Real intensity /* = 0.0 */) :
	BaseLight(ForwardDirectionalShader::GetInstance(), color, intensity)
{
}


DirectionalLight::~DirectionalLight(void)
{
}