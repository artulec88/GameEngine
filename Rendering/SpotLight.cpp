#include "StdAfx.h"
#include "SpotLight.h"
#include "Shader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"
#include "Math\Matrix.h"

using namespace Rendering::Lighting;
using namespace Math;

/* static */ bool SpotLight::spotLightsEnabled = true;

SpotLight::SpotLight(const Rendering::Color& color /*= Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE)*/,
		Real intensity /*= REAL_ZERO */,
		const Rendering::Attenuation& attenuation /*= Attenuation(REAL_ZERO, REAL_ZERO, REAL_ONE) */,
		const Angle& viewAngle /*= Angle(170.0f) */,
		int shadowMapSizeAsPowerOf2 /* = 0 */,
		Math::Real shadowSoftness /* = REAL_ONE */,
		Math::Real lightBleedingReductionAmount /* = static_cast<Math::Real>(0.2f) */,
		Math::Real minVariance /* = static_cast<Math::Real>(0.00002f) ) */) :
	PointLight(color, intensity, attenuation),
	m_cutoff((viewAngle / 2).Cos())
{
	m_isShadowingEnabled = (shadowMapSizeAsPowerOf2 != 0); // shadowMapSizeAsPowerOf2 == 0 means the light doesn't cast shadows at all
	if (m_isShadowingEnabled)
	{
		Matrix4D projectionMatrix = Matrix4D::PerspectiveProjection(viewAngle, REAL_ONE /* because shadow maps are supposed to be squares */, 0.1f /* TODO: Don't use hard-coded values */, m_range);
		SetShadowInfo(new ShadowInfo(projectionMatrix, false, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance));
		CHECK_CONDITION_EXIT(m_shadowInfo != NULL, Utility::Critical, "Cannot initialize spot light. Shadow info is NULL.");
	}
}


SpotLight::~SpotLight(void)
{
}

//void SpotLight::SetDirection(const Math::Vector3D& direction)
//{
//	this->direction = direction;
//
//	//ASSERT(this->direction.IsNormalized());
//	this->direction.Normalize();
//}

bool SpotLight::IsEnabled() const
{
	if (!spotLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}