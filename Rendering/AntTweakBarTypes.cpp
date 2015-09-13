#include "stdafx.h"
#include "AntTweakBarTypes.h"
#include "ShadowInfo.h"

#ifdef ANT_TWEAK_BAR_ENABLED
#include <stddef.h>

TwType Rendering::fogFallOffType;
TwType Rendering::fogCalculationType;
TwType Rendering::angleUnitType;
TwType Rendering::angleType;
TwType Rendering::vector3DType;
TwType Rendering::matrix4DRowType;
TwType Rendering::matrix4DType;
TwType Rendering::shadowInfoType;

/* static */ void Rendering::AntTweakBarTypes::InitializeTweakBarTypes()
{
	TwEnumVal fogFallOffTypeEV[] = {
		{ Rendering::FogEffect::LINEAR, "Linear" },
		{ Rendering::FogEffect::EXPONENTIAL, "Exponential" },
		{ Rendering::FogEffect::SQUARED_EXPONENTIAL, "Squared exponential" }
	};
	fogFallOffType = TwDefineEnum("FogFallOffType", fogFallOffTypeEV, 3);
	
	TwEnumVal fogCalculationTypeEV[] = {
		{ Rendering::FogEffect::PLANE_BASED, "Plane-based" },
		{ Rendering::FogEffect::RANGE_BASED, "Range-based" }
	};
	fogCalculationType = TwDefineEnum("FogCalculationType", fogCalculationTypeEV, 2);

	// array used to describe the Angle::Unit enum values
	TwEnumVal angleUnitEV[] = { { Math::Unit::DEGREE, "Degree" }, { Math::Unit::RADIAN, "Rad" } };
	angleUnitType = TwDefineEnum("Unit", angleUnitEV, 2);
	TwStructMember angleMembers[] = { { "Angle", TW_TYPE_REAL, 0, " precision=1 "}, { "Unit", angleUnitType, 4, ""} };
	angleType = TwDefineStruct("Angle", angleMembers, 2, sizeof(Math::Angle), NULL, NULL);

	TwStructMember vector3DMembers[3] = {
		{ "X", TW_TYPE_REAL, 0, " precision=2 "},
		{ "Y", TW_TYPE_REAL, 4, " precision=2 "},
		{ "Z", TW_TYPE_REAL, 8, " precision=2 "}
	};
	vector3DType = TwDefineStruct("Vector", vector3DMembers, 3, sizeof(Math::Vector3D), NULL, NULL);

	TwStructMember matrix4DRowMembers [4] = {
		{ "[0]", TW_TYPE_REAL, 0, " precision=2 " },
		{ "[1]", TW_TYPE_REAL, 16, " precision=2 " },
		{ "[2]", TW_TYPE_REAL, 32, " precision=2 " },
		{ "[3]", TW_TYPE_REAL, 48, " precision=2 " },
	};
	matrix4DRowType = TwDefineStruct("Matrix4DRow", matrix4DRowMembers, 4, sizeof(Math::Matrix4D), NULL, NULL);

	TwStructMember matrix4DMembers[4] = {
		{ "[0]", matrix4DRowType, 0, "" },
		{ "[1]", matrix4DRowType, 4, "" },
		{ "[2]", matrix4DRowType, 8, "" },
		{ "[3]", matrix4DRowType, 12, "" }
	};
	matrix4DType = TwDefineStruct("Matrix4D", matrix4DMembers, 4, sizeof(Math::Matrix4D) / 4, NULL, NULL);

	TwStructMember shadowInfoMembers[6] = {
		{ "Projection", matrix4DType, 4, "" },
		{ "Flip faces", TW_TYPE_BOOLCPP, 68, "" },
		{ "Map size", TW_TYPE_INT32, 72, "" },
		{ "Softness", TW_TYPE_REAL, 76, " min=0.0 step=0.01 " },
		{ "Bleeding reduction amount", TW_TYPE_REAL, 80, " min=0.0 step=0.01 " },
		{ "Min variance", TW_TYPE_REAL, 84, " min=0.0 step=0.000002 " }
	};
	shadowInfoType = TwDefineStruct("ShadowInfo", shadowInfoMembers, 6, sizeof(Rendering::ShadowInfo), NULL, NULL);
}

#endif