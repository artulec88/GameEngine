#include "StdAfx.h"
#include "ShadowInfo.h"
#include "Utility\ILogger.h"

using namespace Rendering;
using namespace Utility;


ShadowInfo::ShadowInfo(const Math::Matrix4D& projection, Math::Real bias, bool flipFacesEnabled) :
	projectionMatrix(projection),
	bias(bias),
	flipFacesEnabled(flipFacesEnabled)
{
}


ShadowInfo::~ShadowInfo()
{
}
