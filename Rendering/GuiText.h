#ifndef __RENDERING_GUI_TEXT_H__
#define __RENDERING_GUI_TEXT_H__

#include "Rendering.h"
#include "GuiControl.h"
#include "Font.h"
#include "Mesh.h"
#include "Shader.h"

#include "Math\Vector.h"
#include "Math\AABR.h"

#include <string>
#include <vector>

namespace Rendering
{
	namespace Controls
	{
		class GuiTextControl : public GuiControl
		{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new GUI text control, loads the text's quads into a VBO and adds the text to the screen.
			/// </summary>
			/// <param name="text"> The text. </param>
			/// <param name="font"> The font that this text should use. </param>
			/// <param name="fontSize"> The font size of the text. </param>
			/// <param name="screenPosition">
			/// The position on the screen where the top left corner of the text should be rendered.
			/// The top left corner of the screen is (0, 0) and the bottom right is (1, 1).
			/// </param>
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
			RENDERING_API GuiTextControl(const std::string& text, const Text::Font* font, Math::Real fontSize, const Math::Vector2D& screenPosition, Math::Real maxLineLength,
				const Math::Vector3D& textColor, const Math::Vector3D& outlineColor, const Math::Vector2D& offset, bool isCentered = false,
				Math::Real characterWidth = 0.5f, Math::Real characterEdgeTransitionWidth = 0.1f, Math::Real borderWidth = 0.4f, Math::Real borderEdgeTransitionWidth = 0.1f);
			RENDERING_API virtual ~GuiTextControl(void);
			//private:
			//GuiTextControl(GuiTextControl& guiText);
			//void operator=(GuiTextControl& guiText);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Updates GUI text's graphical effects. Does not change the text itself, nor its size, nor font.
			/// </summary>
			RENDERING_API void Update(Math::Real deltaTime)
			{
				//m_colorEffect->Update(deltaTime);
				//m_outlineColorEffect->Update(deltaTime);
				//m_offsetEffect->Update(deltaTime);
			}

			/// <summary> Returns the font used by this GUI text. </summary>
			/// <returns> The font used by this GUI text. </returns>
			RENDERING_API const Text::Font* GetFont() const { return m_font; }

			RENDERING_API const std::string& GetText() const { return m_text; }
			RENDERING_API void SetText(const std::string& text);

			/// <summary> Returns the color of this GUI text. </summary>
			/// <returns> The color of this GUI text. </returns>
			RENDERING_API const Math::Vector3D& GetColor() const { return m_color; }
			RENDERING_API Math::Vector3D* GetColorPointer() { return &m_color; }

			RENDERING_API const Math::Vector3D& GetOutlineColor() const { return m_outlineColor; }
			RENDERING_API Math::Vector3D* GetOutlineColorPointer() { return &m_outlineColor; }

			RENDERING_API const Math::Vector2D& GetOffset() const { return m_offset; }
			RENDERING_API Math::Vector2D* GetOffsetPointer() { return &m_offset; }
			RENDERING_API Math::Real GetCharacterWidth() const { return m_characterWidth; }
			RENDERING_API Math::Real* GetCharacterWidthPointer() { return &m_characterWidth; }
			RENDERING_API Math::Real GetCharacterEdgeTransitionWidth() const { return m_characterEdgeTransitionWidth; }
			RENDERING_API Math::Real* GetCharacterEdgeTransitionWidthPointer() { return &m_characterEdgeTransitionWidth; }
			RENDERING_API Math::Real GetBorderWidth() const { return m_borderWidth; }
			RENDERING_API Math::Real* GetBorderWidthPointer() { return &m_borderWidth; }
			RENDERING_API Math::Real GetBorderEdgeTransitionWidth() const { return m_borderEdgeTransitionWidth; }
			RENDERING_API Math::Real* GetBorderEdgeTransitionWidthPointer() { return &m_borderEdgeTransitionWidth; }

			RENDERING_API virtual void Draw(const Rendering::Renderer& renderer) const;
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
			std::string m_text;
			const Text::Font* m_font;
			Math::Real m_fontSize;
			Math::Vector3D m_color;
			Math::Vector3D m_outlineColor;
			Math::Vector2D m_offset;
			Math::Real m_maxLineLength;
			int m_linesCount;
			bool m_isCentered;

			Math::Real m_characterWidth;
			Math::Real m_characterEdgeTransitionWidth;
			Math::Real m_borderWidth;
			Math::Real m_borderEdgeTransitionWidth;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class GuiTextControl */
	}
} /* end namespace Rendering */

#endif // __RENDERING_GUI_TEXT_H__
