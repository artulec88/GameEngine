#ifndef __RENDERING_LINE_H__
#define __RENDERING_LINE_H__

#include "Rendering.h"
#include "Word.h"

namespace rendering
{
	namespace text
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
			Line(math::Real spaceWidth, math::Real fontSize, math::Real maxLength) noexcept :
				m_spaceSize(spaceWidth * fontSize),
				m_maxLength(maxLength),
				m_currentLineLength(REAL_ZERO)
			{
			}
			
			~Line() noexcept = default;

			/// <summary> Line copy constructor. </summary>
			/// <param name="line">The reference to line to copy construct from. </param>
			Line(const Line& line) = default;

			/// <summary> Line move constructor. </summary>
			/// <param name="line">The r-value reference to line to move construct from. </param>
			Line(Line&& line) = default;

			/// <summary> Line copy assignment operator. </summary>
			/// <param name="line">The reference to line to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned line. </returns>
			Line& operator=(const Line& line) = default;

			/// <summary> Line move assignment operator. </summary>
			/// <param name="line">The r-value reference to line to move assign from. </param>
			/// <returns> The reference to the newly move-assigned line. </returns>
			Line& operator=(Line&& line) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Attempts to add a word to the line. If the line can fit the word in without reaching the maximum
			/// line length then the word is added and the line length is increased.
			/// </summary>
			/// <param name="word"> The word to try to add. </param>
			/// <returns><code>True</code> if the word has successfully been added to the line and <code>false</code> otherwise. </returns>
			bool AttemptToAddWord(const Word& word);
			math::Real GetMaxLength() const { return m_maxLength; }
			math::Real GetLineLength() const { return m_currentLineLength; }
			const std::vector<Word>& GetWords() const { return m_words; }
			void Clear()
			{
				m_currentLineLength = REAL_ZERO;
				m_words.clear();
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			math::Real m_spaceSize;
			math::Real m_maxLength;
			std::vector<Word> m_words;
			math::Real m_currentLineLength;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Line */
	} /* end namespace text */
} /* end namespace rendering */

#endif /* __RENDERING_LINE_H__ */
