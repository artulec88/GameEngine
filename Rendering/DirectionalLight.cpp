#include "StdAfx.h"
#include "DirectionalLight.h"
//#include "CoreEngine.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility/IConfig.h"

rendering::lighting::DirectionalLight::DirectionalLight(const math::Transform& transform, const Color& color, math::Real intensity, int shaderID,
	int terrainShaderID, int noShadowShaderID, int noShadowTerrainShaderID,
	bool shadowInfoFlipFacesEnabled, int shadowInfoShadowMapSizeAsPowerOf2, math::Real shadowInfoShadowSoftness,
	math::Real shadowInfoLightBleedingReductionFactor, math::Real shadowInfoMinVariance, math::Real halfShadowArea) :
	BaseLight(transform, color, intensity, shaderID, terrainShaderID, noShadowShaderID, noShadowTerrainShaderID, (shadowInfoShadowMapSizeAsPowerOf2 != 0)),
	m_halfShadowArea(halfShadowArea)
{
	if (IsShadowingEnabled())
	{
		SetShadowInfo(math::Matrix4D(-m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea),
			true, shadowInfoShadowMapSizeAsPowerOf2, shadowInfoShadowSoftness, shadowInfoLightBleedingReductionFactor, shadowInfoMinVariance);
	}
}

rendering::lighting::DirectionalLight::~DirectionalLight()
{
}

rendering::ShadowCameraTransform rendering::lighting::DirectionalLight::CalcShadowCameraTransform(const math::Vector3D& cameraPos, const math::Quaternion& cameraRot) const
{
	//return BaseLight::CalcShadowCameraTransform(cameraPos, cameraRot);

	// This function in directional light allows the directional light to be casting shadows only in the area around the camera current position
	ShadowCameraTransform shadowCameraTransform(cameraPos + cameraRot.GetForward() * m_halfShadowArea, GetTransform().GetTransformedRot());

	/**
	 * The reoccurring shimmering is caused by the moving shadow camera by the value less than
	 * the shadow map texel size. If I move by, let's say, half the texel size, render the scene
	 * and generate the shadow map again, the objects aren't going to map to exactly the same texels
	 * in the shadow map. This causes approximation to be calculated slightly differently each frame.
	 * To fix the shimmering effect we have to make sure we only move by the multiple of the texel size.
	 */
	/* ==================== Fixing the shimmering effect begin ==================== */
	const auto shadowMapSize = static_cast<math::Real>(1 << GetShadowInfo()->GetShadowMapSizeAsPowerOf2());
	const auto worldSpaceShadowMapTexelSize = (m_halfShadowArea * 2.0f) / shadowMapSize;
	// Now we transform from the world space into the light space
	auto lightSpaceCameraPos(shadowCameraTransform.m_pos.Rotate(shadowCameraTransform.m_rot.Conjugate()));

	// Now we need to snap the lightSpaceCameraPos to shadow map texel size increments
	lightSpaceCameraPos.x = worldSpaceShadowMapTexelSize * math::Floor(lightSpaceCameraPos.x / worldSpaceShadowMapTexelSize);
	lightSpaceCameraPos.y = worldSpaceShadowMapTexelSize * math::Floor(lightSpaceCameraPos.y / worldSpaceShadowMapTexelSize);

	// Now we transform back from the light space into the world space
	shadowCameraTransform.m_pos = lightSpaceCameraPos.Rotate(shadowCameraTransform.m_rot);
	/* ==================== Fixing the shimmering effect end ==================== */
	return shadowCameraTransform;
}
