#include "StdAfx.h"
#include "BaseLight.h"
#include "Renderer.h"
#include "Shader.h"
#include "Utility\Log.h"

using namespace Rendering;

BaseLight::BaseLight(const Math::Vector3D& color /* = Math::Vector3D(0.0, 0.0, 0.0) */, Math::Real intensity /* = 0.0 */) :
	GameComponent(),
	color(color),
	intensity(intensity),
	shader(NULL)
{
}

BaseLight::~BaseLight(void)
{
	// TODO: Think how to deallocate resources.
	//if (shader == NULL)
	//{
	//	delete shader;
	//}
}

void BaseLight::SetShader(Shader* shader)
{
	if (this->shader != NULL)
	{
		delete this->shader;
		this->shader = NULL;
	}
	this->shader = shader;
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

void BaseLight::AddToRenderingEngine(Renderer* renderer)
{
	if (renderer == NULL)
	{
		stdlog(Utility::Error, LOGPLACE, "Cannot add light to the rendering engine. Renderer is NULL");
		return;
	}
	renderer->AddLight(this);
}