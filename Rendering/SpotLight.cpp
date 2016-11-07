#include "StdAfx.h"
#include "SpotLight.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"
#include "Math\Matrix.h"

Rendering::Lighting::SpotLight::SpotLight(const Math::Transform& transform, const Color& color, Math::Real intensity) :
	PointLight(transform, color, intensity),
	m_cutoff(REAL_ZERO)
{
}


Rendering::Lighting::SpotLight::~SpotLight(void)
{
}

void Rendering::Lighting::SpotLight::SetShadowInfo(const Math::Angle& viewAngle, int shadowMapSizeAsPowerOf2, Math::Real projectionNearPlane,
	Math::Real shadowSoftness, Math::Real lightBleedingReductionAmount, Math::Real minVariance)
{
	m_isShadowingEnabled = (shadowMapSizeAsPowerOf2 != 0); // shadowMapSizeAsPowerOf2 == 0 means the light doesn't cast shadows at all
	m_cutoff = (viewAngle / 2).Cos();
	if (m_isShadowingEnabled)
	{
		BaseLight::SetShadowInfo(Math::Matrix4D(viewAngle, REAL_ONE /* because shadow maps are supposed to be squares */, projectionNearPlane, m_range),
			false, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance);
		CHECK_CONDITION_EXIT_RENDERING(m_shadowInfo != NULL, Utility::Logging::CRITICAL, "Cannot initialize spot light. Shadow info is NULL.");
	}
}
