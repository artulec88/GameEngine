#include "stdafx.h"
#include "MappedValues.h"
#include "Utility\ILogger.h"

using namespace Rendering;

#ifdef ANT_TWEAK_BAR_ENABLED
	void MappedValues::InitializeTweakBar(TwBar* tweakBar, const char* groupName)
	{
		if (realMap.empty() && vec3DMap.empty())
		{
			LOG(Utility::Warning, LOGPLACE, "The group \"%s\" does not have any parameters set in the maps", groupName);
			return;
		}
		char defFloatStr[256];
		char defVec3DStr[256];
		_snprintf_s(defFloatStr, 256, 255, " step=0.01 precision=2 group='Floats' ", groupName);
		_snprintf_s(defVec3DStr, 256, 255, " group='Vectors' ", groupName);

		for (StrToRealMap::iterator realItr = realMap.begin(); realItr != realMap.end(); ++realItr)
		{
			TwAddVarRW(tweakBar, (realItr->first).c_str(), TW_TYPE_FLOAT, &(realItr->second), defFloatStr);
		}
		for (StrToVec3DMap::iterator vec3DItr = vec3DMap.begin(); vec3DItr != vec3DMap.end(); ++vec3DItr)
		{
			TwAddVarRW(tweakBar, (vec3DItr->first).c_str(), vector3DType, &(vec3DItr->second), defVec3DStr);
		}
		const char* tweakBarName = TwGetBarName(tweakBar);
		char defStr[256];
		if (realMap.empty())
		{
			_snprintf_s(defStr, 256, 255, " %s/Vectors group=%s ", tweakBarName, groupName, groupName);
		}
		else if (vec3DMap.empty())
		{
			_snprintf_s(defStr, 256, 255, " %s/Floats group=%s ", tweakBarName, groupName, groupName);
		}
		else
		{
			_snprintf_s(defStr, 256, 255, " %s/Floats group=%s \n %s/Vectors group=%s ", tweakBarName, groupName, groupName, tweakBarName, groupName, groupName);
		}
		TwDefine(defStr);
	}
#endif