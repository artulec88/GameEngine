#include "stdafx.h"
#include "MappedValues.h"

using namespace Rendering;

#ifdef ANT_TWEAK_BAR_ENABLED
	void MappedValues::InitializeTweakBar(TwBar* tweakBar, const char* groupName)
	{
		for (StrToRealMap::iterator realItr = realMap.begin(); realItr != realMap.end(); ++realItr)
		{
			TwAddVarRW(tweakBar, (realItr->first).c_str(), TW_TYPE_FLOAT, &(realItr->second), " step=0.01 precision=2 group='Floats' ");
		}
		for (StrToVec3DMap::iterator vec3DItr = vec3DMap.begin(); vec3DItr != vec3DMap.end(); ++vec3DItr)
		{
			TwAddVarRW(tweakBar, (vec3DItr->first).c_str(), vector3DType, &(vec3DItr->second), " step=0.01 precision=2 group='Vectors' ");
		}
		const char* tweakBarName = TwGetBarName(tweakBar);
		char defStr[256];
		_snprintf(defStr, 255, " %s/Floats group=%s \n %s/Vectors group=%s ", tweakBarName, groupName, tweakBarName, groupName);
		TwDefine(defStr);
	}
#endif