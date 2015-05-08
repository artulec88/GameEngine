#include "StdAfx.h"
#include "BaseLight.h"
#include "CoreEngine.h"
#include "Renderer.h"
#include "Shader.h"
#include "Utility\ILogger.h"

using namespace Rendering;

BaseLight::BaseLight(const Color& color /* = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE) */, Math::Real intensity /* = REAL_ZERO */) :
	GameComponent(),
	m_color(color),
	m_intensity(intensity),
	m_shader(NULL),
	m_terrainShader(NULL),
	m_shadowInfo(NULL),
	m_isEnabled(true),
	m_isShadowingEnabled(true)
{
}

BaseLight::~BaseLight(void)
{
	// TODO: delete shader if it's not referenced by any other object
	// TODO: Think how to deallocate resources.
	SAFE_DELETE(m_shader);
	SAFE_DELETE(m_terrainShader);
	SAFE_DELETE(m_shadowInfo);
}

void BaseLight::SetShader(Shader* shader)
{
	// TODO: delete shader if it's not referenced by any other object
	//SAFE_DELETE(this->shader);
	if (m_shader != NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Setting new shader for the light seems dubious.");
		SAFE_DELETE(m_shader);
	}
	m_shader = shader;
}

void BaseLight::SetTerrainShader(Shader* terrainShader)
{
	// TODO: delete shader if it's not referenced by any other object
	//SAFE_DELETE(m_terrainShader);
	if (m_terrainShader != NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Setting new terrain shader for the light seems dubious.");
		SAFE_DELETE(m_terrainShader);
	}
	m_terrainShader = terrainShader;
}

void BaseLight::SetShadowInfo(ShadowInfo* shadowInfo)
{
	//SAFE_DELETE(this->shadowInfo);
	if (m_shadowInfo != NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Setting new shadow info for the light seems dubious.");
		SAFE_DELETE(m_shadowInfo);
	}
	m_shadowInfo = shadowInfo;
}

ShadowCameraTransform BaseLight::CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot)
{
	ShadowCameraTransform shadowCameraTransform;
	shadowCameraTransform.pos = GetTransform().GetTransformedPos();
	shadowCameraTransform.rot = GetTransform().GetTransformedRot();
	return shadowCameraTransform;
}

#ifdef ANT_TWEAK_BAR_ENABLED
void BaseLight::InitializeTweakBar(TwBar* lightsBar)
{
	TwAddVarRW(lightsBar, "lightPos", vector3DType, &GetTransform().GetPos(), " label='Pos' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightRot", TW_TYPE_QUAT4F, &GetTransform().GetRot(), " label='Rot' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightShadowInfo", shadowInfoType, &(*GetShadowInfo()), " label='Shadow info' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightColor", TW_TYPE_COLOR4F, &m_color, " label='Color' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightIntensity", TW_TYPE_REAL, &m_intensity, " label='Intensity' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightEnabled", TW_TYPE_BOOLCPP, &m_isEnabled, " label='Enabled' group='Base lights' ");
}
#endif