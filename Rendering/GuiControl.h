#ifndef __RENDERING_GUI_CONTROL_H__
#define __RENDERING_GUI_CONTROL_H__

#include "Rendering.h"
#include "Mesh.h"
#include "Shader.h"

#include "Math\AABR.h"
#include "Math\Vector.h"
#include "Math\IntersectInfo.h"
#include "Math\Effect.h"

namespace Rendering
{
	class Renderer;

	namespace Controls
	{
		//enum ControlType
		//{
		//	TEXT = 0,
		//	CHECKBOX,
		//	COMBOBOX,
		//	BUTTON,
		//	MESSAGEBOX
		//}; /* end enum ControlType */

		/// <summary>
		/// The GUI control
		/// </summary>
		class GuiControl
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Basic GUI control constructor.
			/// </summary>
			/// <param name="screenPosition">
			/// The position on the screen where the top left corner of this GUI control should be rendered.
			/// The top left corner of the screen is (0, 0) and the bottom right is (1, 1).
			/// </param>
			RENDERING_API GuiControl(const Math::Vector2D& screenPosition);

			/// <summary>
			/// Destroys the GUI control.
			/// </summary>
			RENDERING_API virtual ~GuiControl(void);
			GuiControl(GuiControl& guiControl) = delete;
			void operator=(GuiControl& guiControl) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Returns information about whether the specified position is inside the GUI control or not.
			/// </summary>
			/// <returns>
			/// The special <code>IntersectInfo</code> object storing information about whether the specified position is inside the GUI control or not.
			/// </returns>
			RENDERING_API Math::IntersectInfo DoesContainPoint(Math::Real x, Math::Real y) const
			{
				//const Math::IntersectInfo intersectInfo = m_aabr.DoesContainPoint(x, y);
				//CRITICAL_LOG_RENDERING("GUI text \"", m_text, "\" intersect info with point (", x, "; ", y, ") = ", intersectInfo.GetDistance());
				//return intersectInfo;
				return m_aabr.DoesContainPoint(x, y);
			}

			/// <summary>
			/// The position of the top-left corner of the text in screen-space.
			/// (0, 0) is the top-left corner of the screen, while (1, 1) is the bottom-right corner.
			/// </summary>
			/// <returns>
			/// The screen-space position of the top-left corner of this GUI text.
			/// </returns>
			RENDERING_API const Math::Vector2D& GetScreenPosition() const { return m_screenPosition; }

			/// <summary>
			/// Draws the GUI control on the screen using the specified rendering engine.
			/// </summary>
			/// <param name="guiControlShader">The shader to be used when drawing the GUI control.</param>
			/// <param name="renderer">The rendering engine to be used when drawing the GUI control.</param>
			RENDERING_API virtual void Draw(const Shader& guiControlShader, const Renderer& renderer) const = 0;

			RENDERING_API virtual void ApplyColorEffect(const Math::Effects::Effect<Color>& effect) = 0;
			RENDERING_API virtual void ApplyOutlineColorEffect(const Math::Effects::Effect<Color>& effect) = 0;
			RENDERING_API virtual void ApplyOffsetEffect(const Math::Effects::Effect<Math::Vector2D>& effect) = 0;
		protected:
			void MoveAABR(Math::Real bottomLeftOffsetX, Math::Real bottomLeftOffsetY, Math::Real topRightOffsetX, Math::Real topRightOffsetY);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Vector2D m_screenPosition;
			Math::AABR m_aabr;
		protected:
			std::unique_ptr<Mesh> m_mesh;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class GuiControl */
	} /* end namespace Controls */
} /* end namespace Rendering */

#endif // __RENDERING_GUI_CONTROL_H__
