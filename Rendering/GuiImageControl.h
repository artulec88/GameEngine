#ifndef __RENDERING_GUI_IMAGE_CONTROL_H__
#define __RENDERING_GUI_IMAGE_CONTROL_H__

#include "Rendering.h"
#include "GuiControl.h"
#include "Texture.h"

namespace Rendering
{
	namespace Controls
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
			RENDERING_API virtual ~GuiImageControl(void);

			/// <summary> GUI image control copy constructor. </summary>
			GuiImageControl(const GuiImageControl& guiImageControl) = delete;

			/// <summary> GUI image control move constructor. </summary>
			GuiImageControl(GuiImageControl&& guiImageControl) = delete;

			/// <summary> GUI image control copy assignment operator. </summary>
			GuiImageControl& operator=(const GuiImageControl& guiImageControl) = delete;

			/// <summary> GUI image control move assignment operator. </summary>
			GuiImageControl& operator=(GuiImageControl&& guiImageControl) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Draws the GUI image control on the screen using the specified rendering engine and shader.
			/// </summary>
			/// <param name="guiControlShader">The shader to be used when drawing the GUI image.</param>
			/// <param name="renderer">The rendering engine to be used when drawing the GUI image.</param>
			RENDERING_API virtual void Draw(const Shader* guiControlShader, const Renderer& renderer) const override;

			RENDERING_API virtual void ApplyColorEffect(const math::effects::Effect<Color>& effect) override;
			RENDERING_API virtual void ApplyOutlineColorEffect(const math::effects::Effect<Color>& effect) override;
			RENDERING_API virtual void ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect) override;
			RENDERING_API virtual void ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			RENDERING_API virtual void ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			RENDERING_API virtual void ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			RENDERING_API virtual void ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect) override;
		private:
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Texture* m_texture;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class GuiImageControl */
	} /* end namespace Controls */
} /* end namespace Rendering */

#endif // __RENDERING_GUI_IMAGE_CONTROL_H__
