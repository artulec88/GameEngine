#include "StdAfx.h"
#include "SpotLight.h"
#include "Shader.h"
#include "Utility/IConfig.h"
#include "Math/Matrix.h"

rendering::lighting::SpotLight::SpotLight(const math::Transform& transform, const Color& color, math::Real intensity, int shaderId,
	int terrainShaderId, int noShadowShaderId, int noShadowTerrainShaderId, const Attenuation& attenuation,
	math::Real shadowInfoProjectionNearPlane, bool shadowInfoFlipFacesEnabled, int shadowInfoShadowMapSizeAsPowerOf2,
	math::Real shadowInfoShadowSoftness, math::Real shadowInfoLightBleedingReductionFactor, math::Real shadowInfoMinVariance,
	const math::Angle& viewAngle) :
	PointLight(transform, color, intensity, shaderId, terrainShaderId, noShadowShaderId, noShadowTerrainShaderId, attenuation),
	m_cutoff((viewAngle / 2).Cos())
{
	SetIsShadowingEnabled(shadowInfoShadowMapSizeAsPowerOf2 != 0);
	if (IsShadowingEnabled())
	{
		SetShadowInfo(math::Matrix4D(viewAngle, REAL_ONE /* because shadow maps are supposed to be squares */, shadowInfoProjectionNearPlane, m_range),
			shadowInfoFlipFacesEnabled, shadowInfoShadowMapSizeAsPowerOf2, shadowInfoShadowSoftness, shadowInfoLightBleedingReductionFactor, shadowInfoMinVariance);
	}
}


rendering::lighting::SpotLight::~SpotLight()
{
}

