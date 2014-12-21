#include "StdAfx.h"
#include "BaseLight.h"
#include "CoreEngine.h"
#include "Renderer.h"
#include "Shader.h"
#include "Utility\ILogger.h"

using namespace Rendering;

BaseLight::BaseLight(const Color& color /* = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO) */, Math::Real intensity /* = REAL_ZERO */) :
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

ShadowCameraTransform BaseLight::CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot)
{
	ShadowCameraTransform shadowCameraTransform;
	shadowCameraTransform.pos = GetTransform().GetTransformedPos();
	shadowCameraTransform.rot = GetTransform().GetTransformedRot();
	return shadowCameraTransform;
}

void BaseLight::InitializeTweakBar(TwBar* lightsBar)
{
	TwAddVarRW(lightsBar, "lightPos", vector3DType, &GetTransform().GetPos(), " label='Pos' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightRot", TW_TYPE_QUAT4F, &GetTransform().GetRot(), " label='Rot' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightShadowInfo", shadowInfoType, &(*GetShadowInfo()), " label='Shadow info' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightColor", TW_TYPE_COLOR4F, &color, " label='Color' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightIntensity", TW_TYPE_REAL, &intensity, " label='Intensity' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightEnabled", TW_TYPE_BOOLCPP, &isEnabled, " label='Enabled' group='Base lights' ");
}