#ifndef __RENDERING_GUI_TEXT_H__
#define __RENDERING_GUI_TEXT_H__

#include "Rendering.h"
#include "Font.h"
#include "Mesh.h"

#include "Math\Vector.h"

#include <string>
#include <vector>

namespace Rendering
{
	namespace Text
	{
		class GuiText
		{
			class Word
			{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
			public:
				/// <summary>
				/// Creates a new word.
				/// </summary>
				/// <param name="fontSize"> The size of font of the text this word belongs to. </param>
				Word(Math::Real fontSize);
				~Word(void);
			//private:
			//	Word(Word& word) {};
			//	void operator=(Word& word) {};
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			public:
				void AddCharacter(const Character& character)
				{
					m_characters.push_back(character);
					m_width += character.GetXAdvance() * m_wordFontSize;
				}
				const std::vector<Character>& GetCharacters() const { return m_characters; }
				Math::Real GetWordWidth() const { return m_width; }
				void Reset()
				{
					m_width = REAL_ZERO;
					m_characters.clear();
				}

				std::string ToString() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			private:
				std::vector<Character> m_characters;
				Math::Real m_width;
				Math::Real m_wordFontSize;
			/* ==================== Non-static member variables end ==================== */
			}; /* end class Word */

			class Line
			{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
			public:
				/// <summary>
				/// Creates an empty line.
				/// </summary>
				/// <param name="spaceWidth"> The screen-space width of a space character. </param>
				/// <param name="fontSize"> The size of font being used. </param>
				/// <param name="maxLength"> The screen-space maximum length of a line. </param>
				Line(Math::Real spaceWidth, Math::Real fontSize, Math::Real maxLength);
				~Line(void);
			//private:
			//	Line(Line& line) {};
			//	void operator=(Line& line) {};
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			public:
				/// <summary>
				/// Attempts to add a word to the line. If the line can fit the word in without reaching the maximum
				/// line length then the word is added and the line length is increased.
				/// </summary>
				/// <param name="word"> The word to try to add. </param>
				/// <returns><code>True</code> if the word has successfully been added to the line and <code>false</code> otherwise. </returns>
				bool AttemptToAddWord(const Word& word);
				Math::Real GetMaxLength() const { return m_maxLength; }
				Math::Real GetLineLength() const { return m_currentLineLength; }
				const std::vector<Word>& GetWords() const { return m_words; }
				void Reset()
				{
					m_currentLineLength = REAL_ZERO;
					m_words.clear();
				}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Real m_spaceSize;
				Math::Real m_maxLength;
				std::vector<Word> m_words;
				Math::Real m_currentLineLength;
			/* ==================== Non-static member variables end ==================== */
			}; /* end class Line */
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// Creates a new text, loads the text's quads into a VBO and adds the text to the screen.
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
			/// <param name="isCentered"> Specified whether the text should be centered or not. </param>
			RENDERING_API GuiText(const std::string& text, const Font* font, Math::Real fontSize, const Math::Vector2D& screenPosition, Math::Real maxLineLength, bool isCentered = false);
			RENDERING_API ~GuiText(void);
		//private:
			//GuiText(GuiText& guiText);
			//void operator=(GuiText& guiText);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary> Returns the font used by this GUI text. </summary>
			/// <returns> The font used by this GUI text. </returns>
			RENDERING_API const Font* GetFont() const { return m_font; }

			/// <summary> Returns the color of this GUI text. </summary>
			/// <returns> The color of this GUI text. </returns>
			RENDERING_API const Math::Vector3D& GetColor() const { return m_color; }

			/// <summary> Sets the color of this GUI text. </summary>
			/// <param name="textColor"> The color we want to set for the GUI text. </param>
			RENDERING_API void SetColor(const Math::Vector3D& textColor) { m_color.Set(textColor.GetX(), textColor.GetY(), textColor.GetZ()); }

			/// <summary>
			/// Returns the number of lines of text. This is determined when the text is loaded
			/// based on the length of the text and the specified maximum line length.
			/// </summary>
			/// <returns> The number of lines of text. </returns>
			RENDERING_API int GetLinesCount() const { return m_linesCount; }

			/// <summary>
			/// The position of the top-left corner of the text in screen-space.
			/// (0, 0) is the top-left corner of the screen, while (1, 1) is the bottom-right corner.
			/// </summary>
			/// <returns>
			/// The screen-space position of the top-left corner of this GUI text.
			/// </returns>
			RENDERING_API const Math::Vector2D& GetScreenPosition() const { return m_screenPosition; }

			RENDERING_API bool IsCentered() const { return m_isCentered; }

			RENDERING_API void Draw() const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		protected:
			std::string m_text;
			const Font* m_font;
			Math::Real m_fontSize;
			Math::Vector3D m_color;
			Math::Vector2D m_screenPosition;
			Math::Real m_maxLineLength;
			int m_linesCount;
			bool m_isCentered;
			
			Mesh* m_mesh;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class GuiText */
	}
} /* end namespace Rendering */

#endif // __RENDERING_GUI_TEXT_H__
