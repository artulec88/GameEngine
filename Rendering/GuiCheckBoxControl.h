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
			RENDERING_API GuiCheckBoxControl(Texture* texture, const Math::Vector2D& screenPosition, const Math::Angle& screenRotationAngle, const Math::Vector2D& scale);
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
			RENDERING_API virtual void Draw(Rendering::Renderer* renderer) const;
			RENDERING_API virtual void ApplyColorEffect(const Math::Effects::Effect<Color>& effect);
			RENDERING_API virtual void ApplyOutlineColorEffect(const Math::Effects::Effect<Color>& effect);
			RENDERING_API virtual void ApplyOffsetEffect(const Math::Effects::Effect<Math::Vector2D>& effect);
			RENDERING_API virtual void ApplyCharacterWidthEffect(const Math::Effects::Effect<Math::Real>& effect);
			RENDERING_API virtual void ApplyCharacterEdgeTransitionWidthEffect(const Math::Effects::Effect<Math::Real>& effect);
			RENDERING_API virtual void ApplyBorderWidthEffect(const Math::Effects::Effect<Math::Real>& effect);
			RENDERING_API virtual void ApplyBorderEdgeTransitionWidthEffect(const Math::Effects::Effect<Math::Real>& effect);
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
