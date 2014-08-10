#include "StdAfx.h"
#include "ShadowInfo.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;


ShadowInfo::ShadowInfo(const Math::Matrix4D& projection) :
	projectionMatrix(projection)
{
}


ShadowInfo::~ShadowInfo()
{
}
