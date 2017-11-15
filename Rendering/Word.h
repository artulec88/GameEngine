#ifndef __RENDERING_WORD_H__
#define __RENDERING_WORD_H__

#include "Rendering.h"
#include "Character.h"

namespace Rendering
{
	namespace Text
	{
		/// <summary>
		/// The internal representation of a single text word.
		/// </summary>
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
			Word(math::Real fontSize);
			~Word(void);
			//private:
			//	Word(Word& word) {};
			//	void operator=(Word& word) {};
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Appends the given character to the current word.
			/// </summary>
			/// <param name="character">The character to be appended to the word.</param>
			void AddCharacter(const Character& character);
			inline const std::vector<Character>& GetCharacters() const { return m_characters; }
			inline math::Real GetWordWidth() const { return m_width; }
			void Clear();

			friend std::ostream& operator<<(std::ostream& out, const Word& word)
			{
				for (auto charItr = word.m_characters.begin(); charItr != word.m_characters.end(); ++charItr)
				{
					out << charItr->GetAsciiCode() << ",";
				}
				return out;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::vector<Character> m_characters;
			math::Real m_width;
			math::Real m_fontSize;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Word */
	} /* end namespace Text */
} /* end namespace Rendering */

#endif /* __RENDERING_WORD_H__ */