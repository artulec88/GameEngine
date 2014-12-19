#pragma once

#include "Rendering.h"
#ifdef ANT_TWEAK_BAR_ENABLED

#include "AntTweakBar\include\AntTweakBar.h"
#include "Math\Angle.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"

#define TW_TYPE_REAL TW_TYPE_FLOAT // change it to TW_TYPE_DOUBLE if we decide to change Math::Real definition to double

namespace Rendering
{
	// create a new TwType associated to the enum defined by the modeEV array
	class AntTweakBarTypes
	{
	public:
		static void InitializeTweakBarTypes();
	};

	extern TwType angleUnitType;
	extern TwType angleType;
	extern TwType vector3DType;
	extern TwType matrix4DRowType;
	extern TwType matrix4DType;
}

#endif