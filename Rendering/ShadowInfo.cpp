#include "StdAfx.h"
#include "ShadowInfo.h"
#include "Utility\ILogger.h"

using namespace Rendering;
using namespace Utility;

ShadowInfo::ShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled, Math::Real shadowSoftness, Math::Real lightBleedingReductionAmount, Math::Real minVariance /* = static_cast<Real>(0.00002f) */) :
	projectionMatrix(projection),
	flipFacesEnabled(flipFacesEnabled),
	shadowSoftness(shadowSoftness),
	lightBleedingReductionAmount(lightBleedingReductionAmount),
	minVariance(minVariance)
{
}


ShadowInfo::~ShadowInfo()
{
}
