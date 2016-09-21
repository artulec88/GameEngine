#include "stdafx.h"
#include "MappedValues.h"
#include "Utility\ILogger.h"

/* static */ const Math::Real Rendering::MappedValues::DEFAULT_VALUE = REAL_ZERO;
/* static */ const Math::Vector2D Rendering::MappedValues::DEFAULT_VECTOR2D(REAL_ZERO, REAL_ZERO);
/* static */ const Math::Vector3D Rendering::MappedValues::DEFAULT_VECTOR3D(REAL_ZERO, REAL_ZERO, REAL_ZERO);
/* static */ const Math::Vector4D Rendering::MappedValues::DEFAULT_VECTOR4D(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO);

Rendering::MappedValues::MappedValues()
{
	DEBUG_LOG_RENDERING("Mapped values container created.");
}

Rendering::MappedValues::~MappedValues()
{
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Rendering::MappedValues::InitializeTweakBar(TwBar* tweakBar, const char* groupName)
{
	if (realMap.empty() && vec3DMap.empty())
	{
		WARNING_LOG_RENDERING("The group \"", groupName, "\" does not have any parameters set in the maps");
		return;
	}
	char temp[256];
	char defFloatStr[256];
	char defVec3DStr[256];
	//_snprintf_s(defFloatStr, 256, 255, " step=0.01 precision=2 group='Floats' ", groupName);
	//_snprintf_s(defVec3DStr, 256, 255, " group='Vectors' ", groupName);
	_snprintf_s(defFloatStr, 256, 255, " step=0.01 precision=2 group='Floats' ");
	_snprintf_s(defVec3DStr, 256, 255, " group='Vectors' ");

	for (StrToRealMap::iterator realItr = realMap.begin(); realItr != realMap.end(); ++realItr)
	{
		_snprintf_s(temp, 256, 255, "%s\\%s", groupName, (realItr->first).c_str());
		TwAddVarRW(tweakBar, temp, TW_TYPE_FLOAT, &(realItr->second), defFloatStr);
	}
	for (StrToVec3DMap::iterator vec3DItr = vec3DMap.begin(); vec3DItr != vec3DMap.end(); ++vec3DItr)
	{
		_snprintf_s(temp, 256, 255, "%s\\%s", groupName, (vec3DItr->first).c_str());
		TwAddVarRW(tweakBar, temp, vector3DType, &(vec3DItr->second), defVec3DStr);
	}
	const char* tweakBarName = TwGetBarName(tweakBar);
	char defStr[256];
	if (!realMap.empty())
	{
		_snprintf_s(defStr, 256, 255, " %s/Floats group=%s ", tweakBarName, groupName);
		DEBUG_LOG_RENDERING("defStr = \"", defStr, "\"");
		TwDefine(defStr);
	}
	if (!vec3DMap.empty())
	{
		_snprintf_s(defStr, 256, 255, " %s/Vectors group=%s ", tweakBarName, groupName);
		DEBUG_LOG_RENDERING("defStr = \"", defStr, "\"");
		TwDefine(defStr);
	}
}
#endif