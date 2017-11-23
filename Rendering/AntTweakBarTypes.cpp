#include "stdafx.h"
#include "AntTweakBarTypes.h"
#include "ShadowInfo.h"

#ifdef ANT_TWEAK_BAR_ENABLED

TwType rendering::fogFallOffType;
TwType rendering::fogCalculationType;
TwType rendering::angleType;
TwType rendering::vector3DType;
TwType rendering::matrix4DRowType;
TwType rendering::matrix4DType;
TwType rendering::shadowInfoType;

/* static */ void rendering::AntTweakBarTypes::InitializeTweakBarTypes()
{
	TwEnumVal fogFallOffTypeEV[] = {
		{ fog_effect::LINEAR, "Linear" },
		{ fog_effect::EXPONENTIAL, "Exponential" }
	};
	fogFallOffType = TwDefineEnum("FogFallOffType", fogFallOffTypeEV, 2);
	
	TwEnumVal fogCalculationTypeEV[] = {
		{ fog_effect::PLANE_BASED, "Plane-based" },
		{ fog_effect::RANGE_BASED, "Range-based" }
	};
	fogCalculationType = TwDefineEnum("FogCalculationType", fogCalculationTypeEV, 2);

	TwStructMember angleMembers[] = { { "Angle", TW_TYPE_REAL, 0, " precision=1 "} };
	angleType = TwDefineStruct("Angle", angleMembers, 1, sizeof(math::Angle), nullptr, nullptr);

	TwStructMember vector3DMembers[3] = {
		{ "X", TW_TYPE_REAL, 0, " precision=2 "},
		{ "Y", TW_TYPE_REAL, 4, " precision=2 "},
		{ "Z", TW_TYPE_REAL, 8, " precision=2 "}
	};
	vector3DType = TwDefineStruct("Vector", vector3DMembers, 3, sizeof(math::Vector3D), nullptr, nullptr);

	TwStructMember matrix4DRowMembers [4] = {
		{ "[0]", TW_TYPE_REAL, 0, " precision=2 " },
		{ "[1]", TW_TYPE_REAL, 16, " precision=2 " },
		{ "[2]", TW_TYPE_REAL, 32, " precision=2 " },
		{ "[3]", TW_TYPE_REAL, 48, " precision=2 " },
	};
	matrix4DRowType = TwDefineStruct("Matrix4DRow", matrix4DRowMembers, 4, sizeof(math::Matrix4D), nullptr, nullptr);

	TwStructMember matrix4DMembers[4] = {
		{ "[0]", matrix4DRowType, 0, "" },
		{ "[1]", matrix4DRowType, 4, "" },
		{ "[2]", matrix4DRowType, 8, "" },
		{ "[3]", matrix4DRowType, 12, "" }
	};
	matrix4DType = TwDefineStruct("Matrix4D", matrix4DMembers, 4, sizeof(math::Matrix4D) / 4, nullptr, nullptr);

	TwStructMember shadowInfoMembers[6] = {
		{ "Projection", matrix4DType, 4, "" },
		{ "Flip faces", TW_TYPE_BOOLCPP, 68, "" },
		{ "Map size", TW_TYPE_INT32, 72, "" },
		{ "Softness", TW_TYPE_REAL, 76, " min=0.0 step=0.01 " },
		{ "Bleeding reduction amount", TW_TYPE_REAL, 80, " min=0.0 step=0.01 " },
		{ "Min variance", TW_TYPE_REAL, 84, " min=0.0 step=0.000002 " }
	};
	shadowInfoType = TwDefineStruct("ShadowInfo", shadowInfoMembers, 6, sizeof(ShadowInfo), nullptr, nullptr);
}

#endif