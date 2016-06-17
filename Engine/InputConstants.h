#ifndef __ENGINE_INPUT_CONSTANTS_H__
#define __ENGINE_INPUT_CONSTANTS_H__

#include "Engine.h"

namespace Engine
{
	namespace Input
	{
		namespace RawInputKeys
		{
			enum RawInputKey
			{
				KEY_ESCAPE = 0,
				KEY_ENTER,
				KEY_TAB,
				KEY_BACKSPACE,
				KEY_INSERT,
				KEY_DELETE,
				KEY_RIGHT,
				KEY_LEFT,
				KEY_DOWN,
				KEY_UP,
				KEY_PAGE_UP,
				KEY_PAGE_DOWN,
				KEY_HOME,
				KEY_END,
				KEY_CAPS_LOCK,
				KEY_SCROLL_LOCK,
				KEY_NUM_LOCK,
				KEY_PRINT_SCREEN,
				KEY_PAUSE,
				KEY_F1,
				KEY_F2,
				KEY_F3,
				KEY_F4,
				KEY_F5,
				KEY_F6,
				KEY_F7,
				KEY_F8,
				KEY_F9,
				KEY_F10,
				KEY_F11,
				KEY_F12,
				KEY_F13,
				KEY_F14,
				KEY_F15,
				KEY_F16,
				KEY_F17,
				KEY_F18,
				KEY_F19,
				KEY_F20,
				KEY_F21,
				KEY_F22,
				KEY_F23,
				KEY_F24,
				KEY_F25,
				KEY_KP_0,
				KEY_KP_1,
				KEY_KP_2,
				KEY_KP_3,
				KEY_KP_4,
				KEY_KP_5,
				KEY_KP_6,
				KEY_KP_7,
				KEY_KP_8,
				KEY_KP_9,
				KEY_KP_DECIMAL,
				KEY_KP_DIVIDE,
				KEY_KP_MULTIPLY,
				KEY_KP_SUBTRACT,
				KEY_KP_ADD,
				KEY_KP_ENTER,
				KEY_KP_EQUAL,
				KEY_LEFT_SHIFT,
				KEY_LEFT_CONTROL,
				KEY_LEFT_ALT,
				KEY_LEFT_SUPER,
				KEY_RIGHT_SHIFT,
				KEY_RIGHT_CONTROL,
				KEY_RIGHT_ALT,
				KEY_RIGHT_SUPER,
				KEY_Q,
				KEY_W,
				KEY_E,
				KEY_R,
				KEY_T,
				KEY_Y,
				KEY_U,
				KEY_I,
				KEY_O,
				KEY_P,
				KEY_A,
				KEY_S,
				KEY_D,
				KEY_F,
				KEY_G,
				KEY_H,
				KEY_J,
				KEY_K,
				KEY_L,
				KEY_Z,
				KEY_X,
				KEY_C,
				KEY_V,
				KEY_B,
				KEY_N,
				KEY_M,
			}; /* end enum RawInputKey */
		} /* end namespace RawInputKeys */

		namespace RawInputAxes
		{
			enum RawInputAxis
			{
				RAW_INPUT_AXIS_MOUSE_X = 0,
				RAW_INPUT_AXIS_MOUSE_Y
			}; /* end enum RawInputAxis */
		} /* end namespace RawInputAxes */

		namespace Actions
		{
			enum Action
			{
				ACTION_INVALID = 0,
				ACTION_QUIT_GAME,
				ACTION_ONE,
				ACTION_TWO,
				ACTION_THREE,
				ACTION_FOUR,
				ACTION_FIVE,
				ACTION_SIX,
				ACTION_SEVEN
			}; /* end enum Action */
		} /* end namespace Actions */

		namespace States
		{
			enum State
			{
				STATE_INVALID = 0,
				STATE_ONE,
				STATE_TWO,
				STATE_THREE
			}; /* end enum State */
		} /* end namespace States */

		namespace Ranges
		{
			enum Range
			{
				RANGE_INVALID = 0,
				RANGE_ONE,
				RANGE_TWO
			}; /* end enum Range */
		} /* end namespace Ranges */
	} /* end namespace Input */
} /* end namspace Engine */

#endif /* __ENGINE_INPUT_CONSTANTS_H__ */