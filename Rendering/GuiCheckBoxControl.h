#ifndef __RENDERING_GUI_CHECK_BOX_CONTROL_H__
#define __RENDERING_GUI_CHECK_BOX_CONTROL_H__

#include "Rendering.h"
#include "GuiControl.h"
#include "Font.h"
#include "Shader.h"
#include "Texture.h"

#include "Math/Vector.h"

namespace rendering
{
	namespace controls
	{
		class GuiCheckBoxControl : public GuiControl
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new GUI check box control.
			/// </summary>
			/// <param name="texture"> The texture that will be applied to the checkbox mesh. </param>
			/// <param name="screenPosition">
			/// The position on the screen where the top left corner of the text should be rendered.
			/// The top left corner of the screen is (0, 0) and the bottom right is (1, 1).
			/// </param>
			/// <param name="screenRotationAngle"> The angle the GUI control will be rotated by. </param>
			/// <param name="scale"> The scale of the GUI check box control. </param>
			RENDERING_API GuiCheckBoxControl(Texture* texture, const math::Vector2D& screenPosition, const math::Angle& screenRotationAngle, const math::Vector2D& scale);
			
			/// <summary> GUI check box control destructor. </summary>
			RENDERING_API virtual ~GuiCheckBoxControl();

			/// <summary> GUI check box control copy constructor. </summary>
			/// <param name="guiCheckBoxControl"> The reference to GUI check box control to copy construct from. </param>
			GuiCheckBoxControl(const GuiCheckBoxControl& guiCheckBoxControl) = delete;

			/// <summary> GUI check box control move constructor. </summary>
			/// <param name="guiCheckBoxControl"> The r-value reference to GUI check box control to move construct from. </param>
			GuiCheckBoxControl(GuiCheckBoxControl&& guiCheckBoxControl) = delete;

			/// <summary> GUI check box control copy assignment operator. </summary>
			/// <param name="guiCheckBoxControl"> The reference to GUI check box control to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned GUI check box control. </returns>
			GuiCheckBoxControl& operator=(const GuiCheckBoxControl& guiCheckBoxControl) = delete;

			/// <summary> GUI check box control move assignment operator. </summary>
			/// <param name="guiCheckBoxControl"> The reference to GUI check box control to move assign from. </param>
			/// <returns> The reference to the newly move-assigned GUI check box control. </returns>
			GuiCheckBoxControl& operator=(GuiCheckBoxControl&& guiCheckBoxControl) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Draws the GUI check box on the screen using the specified rendering engine and shader.
			/// </summary>
			/// <param name="guiControlShader">The shader to be used when drawing the GUI check box.</param>
			/// <param name="renderer">The rendering engine to be used when drawing the GUI check box.</param>
			RENDERING_API void Draw(const Shader* guiControlShader, const Renderer& renderer) const override;

			RENDERING_API void ApplyColorEffect(const math::effects::Effect<Color>& effect) override;
			RENDERING_API void ApplyOutlineColorEffect(const math::effects::Effect<Color>& effect) override;
			RENDERING_API void ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect) override;
			RENDERING_API void ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			RENDERING_API void ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			RENDERING_API void ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			RENDERING_API void ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Texture* m_texture;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class GuiCheckBoxControl */
	} /* end namespace controls */
} /* end namespace rendering */

#endif // __RENDERING_GUI_CHECK_BOX_CONTROL_H__
