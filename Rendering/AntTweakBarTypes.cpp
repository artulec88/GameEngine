#include "stdafx.h"
#include "AntTweakBarTypes.h"

#ifdef ANT_TWEAK_BAR_ENABLED
#include <stddef.h>

TwType Rendering::angleUnitType;
TwType Rendering::angleType;
TwType Rendering::vector3DType;
TwType Rendering::matrix4DRowType;
TwType Rendering::matrix4DType;

/* static */ void Rendering::AntTweakBarTypes::InitializeTweakBarTypes()
{
	// array used to describe the Angle::Unit enum values
	TwEnumVal angleUnitEV[] = { { Math::Angle::DEGREE, "Degree" }, { Math::Angle::RADIAN, "Rad" } };
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
}

#endif