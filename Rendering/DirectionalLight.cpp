#include "StdAfx.h"
#include "DirectionalLight.h"
#include "CoreEngine.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"

using namespace Rendering;

/* static */ bool DirectionalLight::directionalLightsEnabled = true;

DirectionalLight::DirectionalLight(const Math::Vector3D& color /* = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO) */, Math::Real intensity /* = REAL_ZERO */,
		int shadowMapSizeAsPowerOf2 /* = 0 */, Math::Real shadowArea /* = 80.0f */, Math::Real shadowSoftness /* = REAL_ONE */,
		Math::Real lightBleedingReductionAmount /* = static_cast<Math::Real>(0.2f) */,
		Math::Real minVariance /* = static_cast<Math::Real>(0.00002f) */ ) :
	BaseLight(color, intensity),
	halfShadowArea(shadowArea / 2.0f)
{
	SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "forward-directional")));
	ASSERT(shader != NULL);
	if (shader == NULL)
	{
		LOG(Utility::Critical, LOGPLACE, "Cannot initialize directional light. Shader is NULL");
		exit(EXIT_FAILURE);
	}

	if (shadowMapSizeAsPowerOf2 != 0) // shadowMapSizeAsPowerOf2 == 0 means the light doesn't cast shadows
	{
		 Math::Matrix4D ortoMatrix = Math::Matrix4D::OrtographicProjection(-halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea);
		SetShadowInfo(new ShadowInfo(ortoMatrix, true, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance));
		ASSERT(shadowInfo != NULL);
		if (shadowInfo == NULL)
		{
			LOG(Utility::Emergency, LOGPLACE, "Cannot initialize directional light. Shadow info is NULL");
		}
	}
}


DirectionalLight::~DirectionalLight(void)
{
}

bool DirectionalLight::IsEnabled() const
{
	if (!directionalLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}

ShadowCameraTransform DirectionalLight::CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot)
{
	// This function in directional light allows the directional light to be casting shadows only in the area around the camera current position
	ShadowCameraTransform shadowCameraTransform;
	shadowCameraTransform.pos = cameraPos + cameraRot.GetForward() * halfShadowArea;
	shadowCameraTransform.rot = GetTransform().GetTransformedRot();

	/**
	 * The reoccurring shimmering is caused by the moving shadow camera by the value less than
	 * the shadow map texel size. If I move by, let's say, half the texel size, render the scene
	 * and generate the shadow map again, the objects aren't going to map to exactly the same texels
	 * in the shadow map. This causes approximation to be calculated slightly differently each frame.
	 * To fix the shimmering effect we have to make sure we only move by the multiple of the texel size.
	 */
	/* ==================== Fixing the shimmering effect begin ==================== */
	Math::Real shadowMapSize = static_cast<Math::Real>(1 << GetShadowInfo()->GetShadowMapSizeAsPowerOf2());
	Math::Real worldSpaceShadowMapTexelSize = (halfShadowArea * 2.0f) / shadowMapSize;
	// Now we transform from the world space into the light space
	Math::Vector3D lightSpaceCameraPos = shadowCameraTransform.pos.Rotate(shadowCameraTransform.rot.Conjugate());

	// Now we need to snap the lightSpaceCameraPos to shadow map texel size increments
	lightSpaceCameraPos.SetX(worldSpaceShadowMapTexelSize * floor(lightSpaceCameraPos.GetX() / worldSpaceShadowMapTexelSize));
	lightSpaceCameraPos.SetY(worldSpaceShadowMapTexelSize * floor(lightSpaceCameraPos.GetY() / worldSpaceShadowMapTexelSize));

	// Now we transform back from the light space into the world space
	shadowCameraTransform.pos = lightSpaceCameraPos.Rotate(shadowCameraTransform.rot);
	/* ==================== Fixing the shimmering effect end ==================== */
	return shadowCameraTransform;
}