#ifndef __ENGINE_IINPUTABLE_KEYBOARD_H__
#define __ENGINE_IINPUTABLE_KEYBOARD_H__

#include "Engine.h"

namespace engine {
	
	namespace input
	{

		/// <summary>
		/// Interface for objects which can react to player's keyboard input.
		/// </summary>
		class IInputableKeyboard
		{
			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Creates the keyboard inputable object. </summary>
			ENGINE_API IInputableKeyboard()
			{
			}

			/// <summary> Destroys the keyboard inputable object. </summary>
			ENGINE_API virtual ~IInputableKeyboard(void)
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary> Collects keyboard key input according to current game state. </summary>
			/// <param name="key"> The key that triggered the event. </param>
			/// <param name="scancode"> The system-specific scancode of the key. </param>
			/// <param name="action"> The action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT). </param>
			/// <param name="mods"> Bit field describing which modifier keys were held down. </param>
			ENGINE_API virtual void KeyEvent(int key, int scancode, int action, int mods) = 0;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class IInputableKeyboard */

	} /* end namespace input */

} /* end namespace engine */

#endif /* __ENGINE_IINPUTABLE_KEYBOARD_H__ */