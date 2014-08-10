#include "StdAfx.h"
#include "BaseLight.h"
#include "CoreEngine.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\Log.h"

using namespace Rendering;

BaseLight::BaseLight(const Math::Vector3D& color /* = Math::Vector3D(0.0, 0.0, 0.0) */, Math::Real intensity /* = 0.0 */) :
	GameComponent(),
	color(color),
	intensity(intensity),
	shader(NULL),
	shadowInfo(NULL)
{
}

BaseLight::~BaseLight(void)
{
	// TODO: delete shader if it's not referenced by any other object
	// TODO: Think how to deallocate resources.
	if (shader != NULL)
	{
		delete shader;
		shader = NULL;
	}
	if (shadowInfo != NULL)
	{
		delete shadowInfo;
		shadowInfo = NULL;
	}
}

void BaseLight::SetShader(Shader* shader)
{
	// TODO: delete shader if it's not referenced by any other object
	if (this->shader != NULL)
	{
		delete this->shader;
		this->shader = NULL;
	}
	this->shader = shader;
}

void BaseLight::SetShadowInfo(ShadowInfo* shadowInfo)
{
	if (this->shadowInfo != NULL)
	{
		delete this->shadowInfo;
		this->shadowInfo = NULL;
	}
	this->shadowInfo = shadowInfo;
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

void BaseLight::AddToEngine(CoreEngine* coreEngine)
{
	if (coreEngine == NULL)
	{
		stdlog(Utility::Error, LOGPLACE, "Cannot add light to the core engine. Core Engine is NULL");
		return;
	}
	coreEngine->GetRenderer()->AddLight(this);
}