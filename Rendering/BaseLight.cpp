#include "StdAfx.h"
#include "BaseLight.h"
#include "CoreEngine.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\ILogger.h"

using namespace Rendering;

BaseLight::BaseLight(const Math::Vector3D& color /* = Math::Vector3D(0.0, 0.0, 0.0) */, Math::Real intensity /* = 0.0 */) :
	GameComponent(),
	color(color),
	intensity(intensity),
	shader(NULL),
	shadowInfo(NULL),
	isEnabled(true)
{
}

BaseLight::~BaseLight(void)
{
	// TODO: delete shader if it's not referenced by any other object
	// TODO: Think how to deallocate resources.
	SAFE_DELETE(shader);
	SAFE_DELETE(shadowInfo);
}

void BaseLight::SetShader(Shader* shader)
{
	// TODO: delete shader if it's not referenced by any other object
	//SAFE_DELETE(this->shader);
	if (this->shader != NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Setting new shader for the light seems dubious.");
		delete this->shader;
		this->shader = NULL;
	}
	this->shader = shader;
}

void BaseLight::SetShadowInfo(ShadowInfo* shadowInfo)
{
	//SAFE_DELETE(this->shadowInfo);
	if (this->shadowInfo != NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Setting new shadow info for the light seems dubious.");
		delete this->shadowInfo;
		this->shadowInfo = NULL;
	}
	this->shadowInfo = shadowInfo;
}

void BaseLight::AddToEngine(CoreEngine* coreEngine)
{
	if (coreEngine == NULL)
	{
		LOG(Utility::Critical, LOGPLACE, "Cannot add light to the core engine. Core Engine is NULL");
		return;
	}
	coreEngine->GetRenderer()->AddLight(this);
}