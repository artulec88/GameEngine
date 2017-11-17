#ifndef __RENDERING_GUI_BUTTON_CONTROL_H__
#define __RENDERING_GUI_BUTTON_CONTROL_H__

#include "Rendering.h"
#include "GuiControl.h"
#include "Font.h"
#include "Mesh.h"
#include "Color.h"
#include "Shader.h"
#include "Texture.h"

#include "Math/Vector.h"
#include "Math\AABR.h"
#include "Math\Effect.h"

#include <string>
#include <vector>

namespace Rendering
{
	namespace Controls
	{
		class GuiButtonControl : public GuiControl
		{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new GUI button control, loads the text's quads into a VBO and adds the text to the screen.
			/// </summary>
			/// <param name="text"> The text. </param>
			/// <param name="font">
			/// The font that this text should use. If <code>nullptr</code> is specified then no text will be rendered.
			/// </param>
			/// <param name="fontSize"> The font size of the text. </param>
			/// <param name="iconTexture">
			/// The texture for the GUI button icon. If <code>nullptr</code> is specified then no icon will be rendered.
			/// </param>
			/// <param name="screenPosition">
			/// The position on the screen where the top left corner of the text should be rendered.
			/// The top left corner of the screen is (0, 0) and the bottom right is (1, 1).
			/// </param>
			/// <param name="screenRotationAngle"> The rotation angle of the GUI button control. </param>
			/// <param name="scale"> The scale of the GUI button control. </param>
			/// <param name="maxLineLength">
			/// Basically, the width of the virtual page in terms of screen width
			/// (1 is full screen width, 0.5 is half the width of the screen etc.).
			/// Text cannot go off the edge of the page, so if the text is longer
			/// than this length it will go onto the next line.
			/// When text is centered it is centered into the middle of the line,
			/// based on this line length value.
			/// </param>
			/// <param name="offset"> The offset for the second font texture atlas sampling call. </param>
			/// <param name="outlineColor"> The outline color. </param>
			/// <param name="isCentered"> Specified whether the text should be centered or not. </param>
			/// <param name="characterWidth">
			/// The width of the single character in the text. It has nothing to do with the font size.
			/// It simply defines how "fat" the single character should be.
			/// </param>
			/// <param name="characterEdgeTransitionWidth">
			/// Together with <paramref name="characterWidth"/> it defines what values of distances should be handled for anti-aliasing of the characters.
			/// For distances between <paramref name="characterWidth"/> and the sum <paramref name="characterWidth"/> + <paramref name="characterEdgeTransitionWidth"/>
			/// there will be a smooth transition between the character and the background.
			/// </param>
			/// <param name="borderWidth"> The border width. </param>
			/// <param name="borderEdgeTransitionWidth"> The border edge transition width. </param>
			RENDERING_API GuiButtonControl(const std::string& text, const Text::Font* font, math::Real fontSize, const Texture* iconTexture, const math::Vector2D& screenPosition,
				const math::Angle& screenRotationAngle, const math::Vector2D& scale, math::Real maxLineLength, const Color& textColor, const Color& outlineColor, const math::Vector2D& offset, bool isCentered = false,
				math::Real characterWidth = 0.5f, math::Real characterEdgeTransitionWidth = 0.1f, math::Real borderWidth = 0.4f, math::Real borderEdgeTransitionWidth = 0.1f);

			/// <summary> GUI button control destructor. </summary>
			RENDERING_API virtual ~GuiButtonControl(void);
			
			/// <summary> GUI button control copy constructor. </summary>
			GuiButtonControl(const GuiButtonControl& guiButtonControl) = delete;

			/// <summary> GUI button control move constructor. </summary>
			GuiButtonControl(GuiButtonControl&& guiButtonControl) = delete;

			/// <summary> GUI button control copy assignment operator. </summary>
			GuiButtonControl& operator=(const GuiButtonControl& guiButtonControl) = delete;

			/// <summary> GUI button control move assignment operator. </summary>
			GuiButtonControl& operator=(GuiButtonControl&& guiButtonControl) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates GUI text's graphical effects. Does not change the text itself, nor its size, nor font.
			/// </summary>
			RENDERING_API void Update(math::Real deltaTime)
			{
				//m_colorEffect->Update(deltaTime);
				//m_outlineColorEffect->Update(deltaTime);
				//m_offsetEffect->Update(deltaTime);
			}

			//RENDERING_API void acceptColorEffect(const math::effects::Effect<Color>& effect) { effect.Apply(&m_color); }
			//RENDERING_API void acceptOutlineColorEffect(const math::effects::Effect<Color>& effect) { effect.Apply(&m_outlineColor); }
			RENDERING_API void acceptOffsetEffect(const math::effects::Effect<math::Vector2D>& effect) { effect.Apply(&m_offset); }

			/// <summary> Returns the font used by this GUI text. </summary>
			/// <returns> The font used by this GUI text. </returns>
			RENDERING_API const Text::Font* GetFont() const { return m_font; }

			RENDERING_API const std::string& GetText() const { return m_text; }
			RENDERING_API void SetText(const std::string& text);

			/// <summary> Returns the color of this GUI text. </summary>
			/// <returns> The color of this GUI text. </returns>
			RENDERING_API const Color& GetColor() const { return m_color; }

			RENDERING_API const Color& GetOutlineColor() const { return m_outlineColor; }

			RENDERING_API const math::Vector2D& GetOffset() const { return m_offset; }
			RENDERING_API math::Real GetCharacterWidth() const { return m_characterWidth; }
			RENDERING_API math::Real GetCharacterEdgeTransitionWidth() const { return m_characterEdgeTransitionWidth; }
			RENDERING_API math::Real GetBorderWidth() const { return m_borderWidth; }
			RENDERING_API math::Real GetBorderEdgeTransitionWidth() const { return m_borderEdgeTransitionWidth; }

			/// <summary>
			/// Draws the GUI button on the screen using the specified rendering engine and shader.
			/// </summary>
			/// <param name="guiControlShader">The shader to be used when drawing the GUI button.</param>
			/// <param name="renderer">The rendering engine to be used when drawing the GUI button.</param>
			RENDERING_API virtual void Draw(const Shader* guiControlShader, const Renderer& renderer) const override;

			RENDERING_API virtual void ApplyColorEffect(const math::effects::Effect<Color>& effect) override;
			RENDERING_API virtual void ApplyOutlineColorEffect(const math::effects::Effect<Color>& effect) override;
			RENDERING_API virtual void ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect) override;
			RENDERING_API virtual void ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			RENDERING_API virtual void ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			RENDERING_API virtual void ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect) override;
			RENDERING_API virtual void ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect) override;
		private:
			/// <summary>
			/// Returns the number of lines of text. This is determined when the text is loaded
			/// based on the length of the text and the specified maximum line length.
			/// </summary>
			/// <returns> The number of lines of text. </returns>
			inline int GetLinesCount() const { return m_linesCount; }
			/// <summary>
			/// Returns <code>true</code> if the GUI text is centered and <code>false</code> otherwise.
			/// </summary>
			/// <returns> The value <code>true</code> if GUI text is centered and <code>false</code> otherwise. </returns>
			inline bool IsCentered() const { return m_isCentered; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::unique_ptr<Mesh> m_mesh;
			std::string m_text;
			const Text::Font* m_font;
			math::Real m_fontSize;
			const Texture* m_iconTexture;
			const math::Matrix4D m_iconTransformationMatrix;
			Color m_color;
			Color m_outlineColor;
			math::Vector2D m_offset;
			math::Real m_maxLineLength;
			int m_linesCount;
			bool m_isCentered;

			math::Real m_characterWidth;
			math::Real m_characterEdgeTransitionWidth;
			math::Real m_borderWidth;
			math::Real m_borderEdgeTransitionWidth;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class GuiButtonControl */
	} /* end namespace Controls */
} /* end namespace Rendering */

#endif // __RENDERING_GUI_BUTTON_CONTROL_H__
