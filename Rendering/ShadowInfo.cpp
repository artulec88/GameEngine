#include "StdAfx.h"
#include "ShadowInfo.h"
#include "Utility/ILogger.h"

rendering::ShadowInfo::ShadowInfo(const math::Matrix4D& projection, bool flipFacesEnabled, int shadowMapSizeAsPowerOf2, math::Real shadowSoftness /* = 1.0f */, math::Real lightBleedingReductionFactor /* = 0.2f */, math::Real minVariance /* = 0.00002f */) :
	m_projectionMatrix(projection),
	m_flipFacesEnabled(flipFacesEnabled),
	m_shadowMapSizeAsPowerOf2(shadowMapSizeAsPowerOf2),
	m_shadowSoftness(shadowSoftness),
	m_lightBleedingReductionFactor(lightBleedingReductionFactor),
	m_minVariance(minVariance)
{
}

//Rendering::ShadowInfo::ShadowInfo(math::Matrix4D&& projection, bool flipFacesEnabled, int shadowMapSizeAsPowerOf2, math::Real shadowSoftness /* = 1.0f */, math::Real lightBleedingReductionFactor /* = 0.2f */, math::Real minVariance /* = 0.00002f */) :
//	m_projectionMatrix(std::move(projection)),
//	m_flipFacesEnabled(flipFacesEnabled),
//	m_shadowMapSizeAsPowerOf2(shadowMapSizeAsPowerOf2),
//	m_shadowSoftness(shadowSoftness),
//	m_lightBleedingReductionFactor(lightBleedingReductionFactor),
//	m_minVariance(minVariance)
//{
//}


rendering::ShadowInfo::~ShadowInfo()
{
}
