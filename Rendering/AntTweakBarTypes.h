#ifndef __RENDERING_ANT_TWEAK_BAR_TYPES_H__
#define __RENDERING_ANT_TWEAK_BAR_TYPES_H__

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
		RENDERING_API static void InitializeTweakBarTypes();
	};

	/// <summary> The fog fall-off type. </summary>
	extern TwType fogFallOffType;

	/// <summary> The fog calculation strategy. </summary>
	extern TwType fogCalculationType;
	RENDERING_API extern TwType angleType;
	extern TwType vector3DType;
	extern TwType matrix4DRowType;
	extern TwType matrix4DType;
	extern TwType shadowInfoType;
}

#endif /* ANT_TWEAK_BAR_ENABLED */

#endif /* __RENDERING_ANT_TWEAK_BAR_TYPES_H__ */