#include "StdAfx.h"
#include "ShadowInfo.h"
#include "Utility\ILogger.h"

using namespace Rendering;
using namespace Utility;

ShadowInfo::ShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled, int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness /* = 1.0f */, Math::Real lightBleedingReductionAmount /* = 0.2f */, Math::Real minVariance /* = 0.00002f */) :
	projectionMatrix(projection),
	flipFacesEnabled(flipFacesEnabled),
	shadowMapSizeAsPowerOf2(shadowMapSizeAsPowerOf2),
	shadowSoftness(shadowSoftness),
	lightBleedingReductionAmount(lightBleedingReductionAmount),
	minVariance(minVariance)
{
}


ShadowInfo::~ShadowInfo()
{
}
