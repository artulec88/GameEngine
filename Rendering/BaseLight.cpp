#include "StdAfx.h"
#include "BaseLight.h"
//#include "CoreEngine.h"
#include "Renderer.h"
#include "Shader.h"
#include "Utility\ILogger.h"

Rendering::Lighting::BaseLight::BaseLight(const Shader& shader, const Shader& terrainShader,
	const Shader& noShadowShader, const Shader& noShadowTerrainShader) :
	m_transform(),
	m_color(Color(ColorNames::WHITE)),
	m_intensity(REAL_ZERO),
	m_shader(shader),
	m_terrainShader(terrainShader),
	m_noShadowShader(noShadowShader),
	m_noShadowTerrainShader(noShadowTerrainShader),
	m_shadowInfo(nullptr),
	m_isEnabled(false),
	m_isShadowingEnabled(false)
{
}

Rendering::Lighting::BaseLight::~BaseLight(void)
{
}

void Rendering::Lighting::BaseLight::SetShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled, int shadowMapSizeAsPowerOf2,
	Math::Real shadowSoftness /* = REAL_ONE */, Math::Real lightBleedingReductionAmount /* = static_cast<Math::Real>(0.2f) */,
	Math::Real minVariance /* = static_cast<Math::Real>(0.00002f) */)
{
	if (m_shadowInfo != NULL)
	{
		WARNING_LOG_RENDERING("Setting new shadow info for the light seems dubious.");
	}
	m_shadowInfo = std::make_unique<ShadowInfo>(projection, flipFacesEnabled, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance);
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
	//TwAddVarRW(lightsBar, "lightShadowInfo", shadowInfoType, &(*GetShadowInfo()), " label='Shadow info' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightColor", TW_TYPE_COLOR4F, &m_color, " label='Color' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightIntensity", TW_TYPE_REAL, &m_intensity, " label='Intensity' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightEnabled", TW_TYPE_BOOLCPP, &m_isEnabled, " label='Enabled' group='Base lights' ");
}
#endif
/* ==================== end class BaseLight ==================== */

