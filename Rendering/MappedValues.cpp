#include "stdafx.h"
#include "MappedValues.h"
#include "AntTweakBarTypes.h"

#include "Utility/ILogger.h"

rendering::MappedValues::MappedValues()
{
	DELOCUST_LOG_RENDERING("Mapped values container created.");
}

rendering::MappedValues::~MappedValues()
{
}

#ifdef ANT_TWEAK_BAR_ENABLED
void rendering::MappedValues::InitializeTweakBar(TwBar* tweakBar, const char* groupName)
{
	if (m_realMap.empty() && m_vec3Map.empty())
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

	for (auto realItr = m_realMap.begin(); realItr != m_realMap.end(); ++realItr)
	{
		_snprintf_s(temp, 256, 255, "%s\\%s", groupName, realItr->first.c_str());
		TwAddVarRW(tweakBar, temp, TW_TYPE_FLOAT, &realItr->second, defFloatStr);
	}
	for (auto vec3DItr = m_vec3Map.begin(); vec3DItr != m_vec3Map.end(); ++vec3DItr)
	{
		_snprintf_s(temp, 256, 255, "%s\\%s", groupName, vec3DItr->first.c_str());
		TwAddVarRW(tweakBar, temp, vector3DType, &vec3DItr->second, defVec3DStr);
	}
	const auto tweakBarName = TwGetBarName(tweakBar);
	char defStr[256];
	if (!m_realMap.empty())
	{
		_snprintf_s(defStr, 256, 255, " %s/Floats group=%s ", tweakBarName, groupName);
		DEBUG_LOG_RENDERING("defStr = \"", defStr, "\"");
		TwDefine(defStr);
	}
	if (!m_vec3Map.empty())
	{
		_snprintf_s(defStr, 256, 255, " %s/Vectors group=%s ", tweakBarName, groupName);
		DEBUG_LOG_RENDERING("defStr = \"", defStr, "\"");
		TwDefine(defStr);
	}
}
#endif