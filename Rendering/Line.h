#ifndef __RENDERING_LINE_H__
#define __RENDERING_LINE_H__

#include "Rendering.h"
#include "Word.h"

namespace Rendering
{
	namespace Text
	{
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
			void Clear()
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
	} /* end namespace Text */
} /* end namespace Rendering */

#endif /* __RENDERING_LINE_H__ */
