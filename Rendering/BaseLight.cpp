#include "StdAfx.h"
#include "BaseLight.h"
#include "Utility/ILogger.h"

Rendering::Lighting::BaseLight::BaseLight(const math::Transform& transform, const Color& color, math::Real intensity, int shaderID,
	int terrainShaderID, int noShadowShaderID, int noShadowTerrainShaderID, bool isShadowingEnabled /* = false */) :
	m_transform(transform),
	m_color(color),
	m_intensity(intensity),
	m_shaderID(shaderID),
	m_terrainShaderID(terrainShaderID),
	m_noShadowShaderID(noShadowShaderID),
	m_noShadowTerrainShaderID(noShadowTerrainShaderID),
	m_shadowInfo(nullptr),
	m_isEnabled(true),
	m_isShadowingEnabled(isShadowingEnabled)
{
}

Rendering::Lighting::BaseLight::~BaseLight(void)
{
}

//Rendering::Lighting::BaseLight::BaseLight(const BaseLight& baseLight) :
//	m_transform(baseLight.m_transform),
//	m_color(baseLight.m_color),
//	m_intensity(baseLight.m_intensity),
//	m_shader(baseLight.m_shader),
//	m_terrainShader(baseLight.m_terrainShader),
//	m_noShadowShader(baseLight.m_noShadowShader),
//	m_noShadowTerrainShader(baseLight.m_noShadowTerrainShader),
//	m_shadowInfo(baseLight.m_shadowInfo),
//	m_isEnabled(baseLight.m_isEnabled),
//	m_isShadowingEnabled(baseLight.m_isShadowingEnabled)
//{
//}

//Rendering::Lighting::BaseLight::BaseLight(BaseLight&& baseLight) :
//	m_transform(std::move(baseLight.m_transform)),
//	m_color(std::move(baseLight.m_color)),
//	m_intensity(std::move(baseLight.m_intensity)),
//	m_shader(std::move(baseLight.m_shader)),
//	m_terrainShader(std::move(baseLight.m_terrainShader)),
//	m_noShadowShader(std::move(baseLight.m_noShadowShader)),
//	m_noShadowTerrainShader(std::move(baseLight.m_noShadowTerrainShader)),
//	m_shadowInfo(std::move(baseLight.m_shadowInfo)),
//	m_isEnabled(std::move(baseLight.m_isEnabled)),
//	m_isShadowingEnabled(std::move(baseLight.m_isShadowingEnabled))
//{
//}

void Rendering::Lighting::BaseLight::SetShadowInfo(const math::Matrix4D& projection, bool flipFacesEnabled, int shadowMapSizeAsPowerOf2,
	math::Real shadowSoftness /* = REAL_ONE */, math::Real lightBleedingReductionAmount /* = static_cast<math::Real>(0.2f) */,
	math::Real minVariance /* = static_cast<math::Real>(0.00002f) */)
{
	CHECK_CONDITION_RENDERING(m_shadowInfo == nullptr, Utility::Logging::WARNING, "Setting new shadow info for the light seems dubious.");
	m_shadowInfo = std::make_unique<ShadowInfo>(projection, flipFacesEnabled, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance);
	CHECK_CONDITION_EXIT_RENDERING(m_shadowInfo != nullptr, Utility::Logging::CRITICAL, "Cannot initialize the light. Shadow info is nullptr.");
}

Rendering::ShadowCameraTransform Rendering::Lighting::BaseLight::CalcShadowCameraTransform(const math::Vector3D& cameraPos, const math::Quaternion& cameraRot) const
{
	return Rendering::ShadowCameraTransform(GetTransform().GetTransformedPos(), GetTransform().GetTransformedRot());
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Rendering::Lighting::BaseLight::InitializeTweakBar(TwBar* lightsBar)
{
	//TwAddVarRW(lightsBar, "lightPos", vector3DType, &GetTransform().GetPos(), " label='Pos' group='Base lights' ");
	//TwAddVarRW(lightsBar, "lightRot", TW_TYPE_QUAT4F, &GetTransform().GetRot(), " label='Rot' group='Base lights' ");
	//TwAddVarRW(lightsBar, "lightShadowInfo", shadowInfoType, &(*GetShadowInfo()), " label='Shadow info' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightColor", TW_TYPE_COLOR4F, &m_color, " label='Color' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightIntensity", TW_TYPE_REAL, &m_intensity, " label='Intensity' group='Base lights' ");
	TwAddVarRW(lightsBar, "lightEnabled", TW_TYPE_BOOLCPP, &m_isEnabled, " label='Enabled' group='Base lights' ");
}
#endif
/* ==================== end class BaseLight ==================== */

