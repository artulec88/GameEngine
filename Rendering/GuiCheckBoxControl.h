#ifndef __RENDERING_GUI_CHECK_BOX_CONTROL_H__
#define __RENDERING_GUI_CHECK_BOX_CONTROL_H__

#include "Rendering.h"
#include "GuiControl.h"
#include "Font.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include "Math\Vector.h"
#include "Math\AABR.h"

#include <string>
#include <vector>

namespace Rendering
{
	namespace Controls
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
			RENDERING_API virtual ~GuiCheckBoxControl(void);

			/// <summary> GUI check box control copy constructor. </summary>
			GuiCheckBoxControl(const GuiCheckBoxControl& guiCheckBoxControl) = delete;

			/// <summary> GUI check box control move constructor. </summary>
			GuiCheckBoxControl(GuiCheckBoxControl&& guiCheckBoxControl) = delete;

			/// <summary> GUI check box control copy assignment operator. </summary>
			GuiCheckBoxControl& operator=(const GuiCheckBoxControl& guiCheckBoxControl) = delete;

			/// <summary> GUI check box control move assignment operator. </summary>
			GuiCheckBoxControl& operator=(GuiCheckBoxControl&& guiCheckBoxControl) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Draws the GUI check box on the screen using the specified rendering engine and shader.
			/// </summary>
			/// <param name="guiControlShader">The shader to be used when drawing the GUI check box.</param>
			/// <param name="renderer">The rendering engine to be used when drawing the GUI check box.</param>
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
		}; /* end class GuiCheckBoxControl */
	} /* end namespace Controls */
} /* end namespace Rendering */

#endif // __RENDERING_GUI_CHECK_BOX_CONTROL_H__
