#include "StdAfx.h"
#include "ShadowInfo.h"
#include "Utility\ILogger.h"

using namespace Rendering;
using namespace Utility;

ShadowInfo::ShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled, int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness /* = 1.0f */, Math::Real lightBleedingReductionAmount /* = 0.2f */, Math::Real minVariance /* = 0.00002f */) :
	m_projectionMatrix(projection),
	m_flipFacesEnabled(flipFacesEnabled),
	m_shadowMapSizeAsPowerOf2(shadowMapSizeAsPowerOf2),
	m_shadowSoftness(shadowSoftness),
	m_lightBleedingReductionAmount(lightBleedingReductionAmount),
	m_minVariance(minVariance)
{
}


ShadowInfo::~ShadowInfo()
{
}
