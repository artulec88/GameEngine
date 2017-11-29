#ifndef __ENGINE_INPUT_IINPUTABLE_MOUSE_H__
#define __ENGINE_INPUT_IINPUTABLE_MOUSE_H__

#include "Engine.h"

namespace engine {
	namespace input
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
			ENGINE_API virtual ~IInputableMouse()
			{
			}

			/// <summary> Mouse inputable copy constructor. </summary>
			/// <param name="inputableMouse"> The reference to inputable mouse to copy construct from. </param>
			IInputableMouse(const IInputableMouse& inputableMouse) = delete;

			/// <summary> Mouse inputable move constructor. </summary>
			/// <param name="inputableMouse"> The r-value reference to inputable mouse to move construct from. </param>
			IInputableMouse(IInputableMouse&& inputableMouse) = default;

			/// <summary> Mouse inputable copy assignment operator. </summary>
			/// <param name="inputableMouse"> The reference to inputable mouse to copy assign from. </param>
			/// <returns> The reference to newly copy-assigned mouse inputable object. </returns>
			IInputableMouse& operator=(const IInputableMouse& inputableMouse) = delete;

			/// <summary> Mouse inputable move assignment operator. </summary>
			/// <param name="inputableMouse"> The r-value reference to inputable mouse to move assign from. </param>
			/// <returns> The reference to newly move-assigned mouse inputable object. </returns>
			IInputableMouse& operator=(IInputableMouse&& inputableMouse) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
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

	} /* end namespace input */

} /* end namespace engine */

#endif /* __ENGINE_INPUT_IINPUTABLE_MOUSE_H__ */