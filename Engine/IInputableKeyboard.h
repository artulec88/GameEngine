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
			ENGINE_API virtual ~IInputableKeyboard()
			{
			}

			/// <summary> Keyboard inputable copy constructor. </summary>
			/// <param name="inputableKeyboard"> The reference to inputable keyboard to copy construct from. </param>
			IInputableKeyboard(const IInputableKeyboard& inputableKeyboard) = delete;

			/// <summary> Keyboard inputable move constructor. </summary>
			/// <param name="inputableKeyboard"> The r-value reference to inputable keyboard to move construct from. </param>
			IInputableKeyboard(IInputableKeyboard&& inputableKeyboard) = delete;

			/// <summary> Keyboard inputable copy assignment operator. </summary>
			/// <param name="inputableKeyboard"> The reference to inputable keyboard to copy assign from. </param>
			/// <returns> The reference to newly copy-assigned keyboard inputable object. </returns>
			IInputableKeyboard& operator=(const IInputableKeyboard& inputableKeyboard) = delete;

			/// <summary> Keyboard inputable move assignment operator. </summary>
			/// <param name="inputableKeyboard"> The r-value reference to inputable keyboard to move assign from. </param>
			/// <returns> The reference to newly move-assigned keyboard inputable object. </returns>
			IInputableKeyboard& operator=(IInputableKeyboard&& inputableKeyboard) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
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