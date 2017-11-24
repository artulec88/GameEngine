#ifndef __RENDERING_GUI_IMAGE_CONTROL_H__
#define __RENDERING_GUI_IMAGE_CONTROL_H__

#include "Rendering.h"
#include "GuiControl.h"
#include "Texture.h"

namespace rendering
{
	namespace controls
	{
		class GuiImageControl : public GuiControl
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new GUI image control.
			/// </summary>
			/// <param name="texture"> The texture that will be applied on the GUI image mesh. </param>
			/// <param name="screenPosition">
			/// The position on the screen where the top left corner of the GUI image should be rendered.
			/// The top left corner of the screen is (0, 0) and the bottom right is (1, 1).
			/// </param>
			/// <param name="screenRotationAngle"> The angle the GUI control will be rotated by. </param>
			/// <param name="scale"> The scale of this GUI image control. </param>
			RENDERING_API GuiImageControl(Texture* texture, const math::Vector2D& screenPosition, const math::Angle& screenRotationAngle, const math::Vector2D& scale);
			
			/// <summary> GUI image control destructor. </summary>
			RENDERING_API virtual ~GuiImageControl();

			/// <summary> GUI image control copy constructor. </summary>
			/// <param name="guiImageControl"> The reference to GUI image control to copy construct from. </param>
			GuiImageControl(const GuiImageControl& guiImageControl) = delete;

			/// <summary> GUI image control move constructor. </summary>
			/// <param name="guiImageControl"> The r-value reference to GUI image control to move construct from. </param>
			GuiImageControl(GuiImageControl&& guiImageControl) = delete;

			/// <summary> GUI image control copy assignment operator. </summary>
			/// <param name="guiImageControl"> The reference to GUI image control to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned GUI image control. </returns>
			GuiImageControl& operator=(const GuiImageControl& guiImageControl) = delete;

			/// <summary> GUI image control move assignment operator. </summary>
			/// <param name="guiImageControl"> The r-value reference to GUI image control to move assign from. </param>
			/// <returns> The reference to the newly move-assigned GUI image control. </returns>
			GuiImageControl& operator=(GuiImageControl&& guiImageControl) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Draws the GUI image control on the screen using the specified rendering engine and shader.
			/// </summary>
			/// <param name="guiControlShader">The shader to be used when drawing the GUI image.</param>
			/// <param name="renderer">The rendering engine to be used when drawing the GUI image.</param>
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
		}; /* end class GuiImageControl */
	} /* end namespace controls */
} /* end namespace rendering */

#endif // __RENDERING_GUI_IMAGE_CONTROL_H__
