#ifndef __RENDERING_GUI_CONTROL_H__
#define __RENDERING_GUI_CONTROL_H__

#include "Rendering.h"
#include "Shader.h"

#include "Math/AABR.h"
#include "Math/Vector.h"
#include "Math/IntersectInfo.h"
#include "Math/Effect.h"

namespace rendering
{
	class Renderer;

	namespace controls
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
		/// The GUI control.
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
			/// <param name="screenRotationAngle">
			/// The rotation on the screen of the GUI control.
			/// </param>
			/// <param name="scale">
			/// The scale of the GUI control.
			/// </param>
			RENDERING_API GuiControl(const math::Vector2D& screenPosition, const math::Angle& screenRotationAngle, const math::Vector2D& scale);

			/// <summary>
			/// Destroys the GUI control.
			/// </summary>
			RENDERING_API virtual ~GuiControl();

			/// <summary> GUI control copy constructor. </summary>
			/// <param name="guiControl"> The reference to the GUI control to copy construct from. </param>
			GuiControl(const GuiControl& guiControl) = delete;

			/// <summary> GUI control move constructor. </summary>
			/// <param name="guiControl"> The r-value reference to the GUI control to move construct from. </param>
			GuiControl(GuiControl&& guiControl) = delete;

			/// <summary> GUI control copy assignment operator. </summary>
			/// <param name="guiControl"> The reference to the GUI control to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned GUI control. </returns>
			GuiControl& operator=(const GuiControl& guiControl) = delete;

			/// <summary> GUI control move assignment operator. </summary>
			/// <param name="guiControl"> The r-value reference to the GUI control to move assign from. </param>
			/// <returns> The reference to the newly move-assigned GUI control. </returns>
			GuiControl& operator=(GuiControl&& guiControl) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Returns information about whether the specified position is inside the GUI control or not.
			/// </summary>
			/// <returns>
			/// The special <code>IntersectInfo</code> object storing information about whether the specified position is inside the GUI control or not.
			/// </returns>
			RENDERING_API math::IntersectInfo DoesContainPoint(math::Real x, math::Real y) const
			{
				//const math::IntersectInfo intersectInfo = m_aabr.DoesContainPoint(x, y);
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
			RENDERING_API const math::Vector2D& GetScreenPosition() const { return m_screenPosition; }

			/// <summary>
			/// The rotation of the GUI control.
			/// </summary>
			/// <returns>
			/// The angle representing the rotation of the GUI control.
			/// </returns>
			RENDERING_API const math::Angle& GetScreenRotation() const { return m_screenRotation; }

			/// <summary>
			/// The scale of the GUI control.
			/// </summary>
			/// <returns>
			/// The scale of the GUI control.
			/// </returns>
			RENDERING_API const math::Vector2D& GetScale() const { return m_scale; }

			/// <summary>
			/// The transformation matrix of this GUI control.
			/// </summary>
			/// <returns>
			/// The transformation matrix representing the scale, rotation and translation operations of the GUI control.
			/// </returns>
			RENDERING_API const math::Matrix4D& GetTransformMatrix() const { return m_transformMatrix; }

			/// <summary>
			/// Draws the GUI control on the screen using the specified rendering engine.
			/// </summary>
			/// <param name="guiControlShader">The shader to be used when drawing the GUI control.</param>
			/// <param name="renderer">The rendering engine to be used when drawing the GUI control.</param>
			RENDERING_API virtual void Draw(const Shader* guiControlShader, const Renderer& renderer) const = 0;

			RENDERING_API virtual void ApplyColorEffect(const math::effects::Effect<Color>& effect) = 0;
			RENDERING_API virtual void ApplyOutlineColorEffect(const math::effects::Effect<Color>& effect) = 0;
			RENDERING_API virtual void ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect) = 0;
			RENDERING_API virtual void ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect) = 0;
			RENDERING_API virtual void ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect) = 0;
			RENDERING_API virtual void ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect) = 0;
			RENDERING_API virtual void ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect) = 0;
		protected:
			void MoveAabr(math::Real bottomLeftOffsetX, math::Real bottomLeftOffsetY, math::Real topRightOffsetX, math::Real topRightOffsetY);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			math::Vector2D m_screenPosition;
			math::Angle m_screenRotation;
			math::Vector2D m_scale;
			math::Matrix4D m_transformMatrix;
			math::Aabr m_aabr;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class GuiControl */
	} /* end namespace controls */
} /* end namespace rendering */

#endif // __RENDERING_GUI_CONTROL_H__
