#include "StdAfx.h"
#include "BaseLight.h"
//#include "CoreEngine.h"
#include "Renderer.h"
#include "Shader.h"
#include "Utility\ILogger.h"

Rendering::Lighting::BaseLight::BaseLight(Math::Transform& transform, const Rendering::Color& color, Math::Real intensity) :
	m_transform(transform),
	m_color(color),
	m_intensity(intensity),
	m_shader(NULL),
	m_terrainShader(NULL),
	m_noShadowShader(NULL),
	m_noShadowTerrainShader(NULL),
	m_shadowInfo(NULL),
	m_isEnabled(true),
	m_isShadowingEnabled(true)
{
}

Rendering::Lighting::BaseLight::~BaseLight(void)
{
	// TODO: delete shader if it's not referenced by any other object
	// TODO: Think how to deallocate resources.
	SAFE_DELETE(m_shader);
	SAFE_DELETE(m_terrainShader);
	SAFE_DELETE(m_shadowInfo);
	SAFE_DELETE(m_noShadowShader);
	SAFE_DELETE(m_noShadowTerrainShader);
}

void Rendering::Lighting::BaseLight::SetShader(Rendering::Shader* shader)
{
	// TODO: delete shader if it's not referenced by any other object
	//SAFE_DELETE(this->shader);
	if (m_shader != NULL)
	{
		WARNING_LOG("Setting new shader for the light seems dubious.");
		SAFE_DELETE(m_shader);
	}
	m_shader = shader;
}

void Rendering::Lighting::BaseLight::SetTerrainShader(Rendering::Shader* terrainShader)
{
	// TODO: delete shader if it's not referenced by any other object
	//SAFE_DELETE(m_terrainShader);
	if (m_terrainShader != NULL)
	{
		WARNING_LOG("Setting new terrain shader for the light seems dubious.");
		SAFE_DELETE(m_terrainShader);
	}
	m_terrainShader = terrainShader;
}

void Rendering::Lighting::BaseLight::SetNoShadowShader(Rendering::Shader* noShadowShader)
{
	// TODO: delete shader if it's not referenced by any other object
	//SAFE_DELETE(this->shader);
	if (m_noShadowShader != NULL)
	{
		WARNING_LOG("Setting new \"no-shadow\" shader for the light seems dubious.");
		SAFE_DELETE(m_noShadowShader);
	}
	m_noShadowShader = noShadowShader;
}

void Rendering::Lighting::BaseLight::SetNoShadowTerrainShader(Rendering::Shader* noShadowTerrainShader)
{
	// TODO: delete shader if it's not referenced by any other object
	//SAFE_DELETE(m_terrainShader);
	if (m_noShadowTerrainShader != NULL)
	{
		WARNING_LOG("Setting new \"no-shadow\" terrain shader for the light seems dubious.");
		SAFE_DELETE(m_noShadowTerrainShader);
	}
	m_noShadowTerrainShader = noShadowTerrainShader;
}

void Rendering::Lighting::BaseLight::SetShadowInfo(Rendering::ShadowInfo* shadowInfo)
{
	//SAFE_DELETE(this->shadowInfo);
	if (m_shadowInfo != NULL)
	{
		WARNING_LOG("Setting new shadow info for the light seems dubious.");
		SAFE_DELETE(m_shadowInfo);
	}
	m_shadowInfo = shadowInfo;
}

Rendering::ShadowCameraTransform Rendering::Lighting::BaseLight::CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot)
{
	return Rendering::ShadowCameraTransform(GetTransform().GetTransformedPos(), GetTransform().GetTransformedRot());
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Rendering::Lighting::BaseLight::InitializeTweakBar(TwBar* lightsBar)
{
	TwAddVarRW(lightsBar, "lightPos", vector3DType, &GetTransform().GetPos(), " label='Pos' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightRot", TW_TYPE_QUAT4F, &GetTransform().GetRot(), " label='Rot' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightShadowInfo", shadowInfoType, &(*GetShadowInfo()), " label='Shadow info' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightColor", TW_TYPE_COLOR4F, &m_color, " label='Color' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightIntensity", TW_TYPE_REAL, &m_intensity, " label='Intensity' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightEnabled", TW_TYPE_BOOLCPP, &m_isEnabled, " label='Enabled' group='Base lights' ");
}
#endif
/* ==================== end class BaseLight ==================== */

