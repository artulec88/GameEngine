#ifndef __ENGINE_INPUT_IINPUTABLE_MOUSE_H__
#define __ENGINE_INPUT_IINPUTABLE_MOUSE_H__

#include "Engine.h"

namespace Engine {
	namespace Input
	{
		/// <summary>
		/// Interface for objects which can react to player's mouse input
		/// </summary>
		class IInputableMouse
		{
			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Creates the mouse inputable object. </summary>
			ENGINE_API IInputableMouse()
			{
			}

			/// <summary> Destroys the mouse inputable object. </summary>
			ENGINE_API virtual ~IInputableMouse(void)
			{
			}

			IInputableMouse(const IInputableMouse& iInputableMouse) = delete;
			IInputableMouse(IInputableMouse&& iInputableMouse) = default;
			IInputableMouse& operator=(const IInputableMouse& iInputableMouse) = delete;
			IInputableMouse& operator=(IInputableMouse&& iInputableMouse) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// The function handling the mouse button callback.
			/// </summary>
			/// <param name="button">The mouse button that was pressed or released. See http://www.glfw.org/docs/latest/group__buttons.html for details.</param>
			/// <param name="action">One of GLFW_PRESS or GLFW_RELEASE.</param>
			/// <param name="mods">Bit field describing which modifier keys were held down. See http://www.glfw.org/docs/latest/group__mods.html for details.</param>
			ENGINE_API virtual void MouseButtonEvent(int button, int action, int mods) = 0;

			/// <summary>
			/// The function handling the mouse position callback.
			/// </summary>
			/// <param name="xPos">The new X-coordinate, in screen coordinates, of the cursor.</param>
			/// <param name="yPos">The new Y-coordinate, in screen coordinates, of the cursor.</param>
			ENGINE_API virtual void MousePosEvent(double xPos, double yPos) = 0;

			/// <summary>
			/// The function handling the scroll callback.
			/// </summary>
			/// <param name="xOffset">The scroll offset along the X-axis.</param>
			/// <param name="yOffset">The scroll offset along the Y-axis.</param>
			ENGINE_API virtual void ScrollEvent(double xOffset, double yOffset) = 0;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class IInputableMouse */

	} /* end namespace Input */

} /* end namespace Engine */

#endif /* __ENGINE_INPUT_IINPUTABLE_MOUSE_H__ */