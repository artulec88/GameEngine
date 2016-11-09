#include "StdAfx.h"
#include "DirectionalLight.h"
//#include "CoreEngine.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"

Rendering::Lighting::DirectionalLight::DirectionalLight(const Math::Transform& transform, const Color& color, Math::Real intensity, const Shader* shader,
	const Shader* terrainShader, const Shader* noShadowShader, const Shader* noShadowTerrainShader,
	bool shadowInfoFlipFacesEnabled, int shadowInfoShadowMapSizeAsPowerOf2, Math::Real shadowInfoShadowSoftness,
	Math::Real shadowInfoLightBleedingReductionFactor, Math::Real shadowInfoMinVariance, Math::Real halfShadowArea) :
	BaseLight(transform, color, intensity, shader, terrainShader, noShadowShader, noShadowTerrainShader, (shadowInfoShadowMapSizeAsPowerOf2 != 0)),
	m_halfShadowArea(halfShadowArea)
{
	if (IsShadowingEnabled())
	{
		SetShadowInfo(Math::Matrix4D(-m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea, -m_halfShadowArea, m_halfShadowArea),
			true, shadowInfoShadowMapSizeAsPowerOf2, shadowInfoShadowSoftness, shadowInfoLightBleedingReductionFactor, shadowInfoMinVariance);
	}
}

Rendering::Lighting::DirectionalLight::~DirectionalLight(void)
{
}

Rendering::ShadowCameraTransform Rendering::Lighting::DirectionalLight::CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot) const
{
	//return BaseLight::CalcShadowCameraTransform(cameraPos, cameraRot);

	// This function in directional light allows the directional light to be casting shadows only in the area around the camera current position
	Rendering::ShadowCameraTransform shadowCameraTransform(cameraPos + cameraRot.GetForward() * m_halfShadowArea, GetTransform().GetTransformedRot());

	/**
	 * The reoccurring shimmering is caused by the moving shadow camera by the value less than
	 * the shadow map texel size. If I move by, let's say, half the texel size, render the scene
	 * and generate the shadow map again, the objects aren't going to map to exactly the same texels
	 * in the shadow map. This causes approximation to be calculated slightly differently each frame.
	 * To fix the shimmering effect we have to make sure we only move by the multiple of the texel size.
	 */
	/* ==================== Fixing the shimmering effect begin ==================== */
	Math::Real shadowMapSize = static_cast<Math::Real>(1 << GetShadowInfo()->GetShadowMapSizeAsPowerOf2());
	Math::Real worldSpaceShadowMapTexelSize = (m_halfShadowArea * 2.0f) / shadowMapSize;
	// Now we transform from the world space into the light space
	Math::Vector3D lightSpaceCameraPos(shadowCameraTransform.m_pos.Rotate(shadowCameraTransform.m_rot.Conjugate()));

	// Now we need to snap the lightSpaceCameraPos to shadow map texel size increments
	lightSpaceCameraPos.SetX(worldSpaceShadowMapTexelSize * floor(lightSpaceCameraPos.GetX() / worldSpaceShadowMapTexelSize));
	lightSpaceCameraPos.SetY(worldSpaceShadowMapTexelSize * floor(lightSpaceCameraPos.GetY() / worldSpaceShadowMapTexelSize));

	// Now we transform back from the light space into the world space
	shadowCameraTransform.m_pos = lightSpaceCameraPos.Rotate(shadowCameraTransform.m_rot);
	/* ==================== Fixing the shimmering effect end ==================== */
	return shadowCameraTransform;
}
