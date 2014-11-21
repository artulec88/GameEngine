#include "StdAfx.h"
#include "ShadowInfo.h"
#include "Utility\ILogger.h"

using namespace Rendering;
using namespace Utility;

	Math::Matrix4D projectionMatrix;
	bool flipFacesEnabled;
	Math::Real shadowSoftness;
	Math::Real lightBleedingReductionAmount;
	Math::Real minVariance;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ShadowInfo(const Math::Matrix4D& projection, bool flipFacesEnabled, Math::Real shadowSoftness, Math::Real lightBleedingReductionAmount, Math::Real minVariance = static_cast<Real>(0.00002f) );


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
