#ifndef __RENDERING_WORD_H__
#define __RENDERING_WORD_H__

#include "Rendering.h"
#include "Character.h"

namespace rendering
{
	namespace text
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
			explicit Word(math::Real fontSize);

			/// <summary> Word destructor. </summary>
			~Word();

			/// <summary> Word copy constructor. </summary>
			/// <param name="word"> The reference to word to copy construct from. </param>
			Word(const Word& word) = default;

			/// <summary> Word move constructor. </summary>
			/// <param name="word"> The r-value reference to word to move construct from. </param>
			Word(Word&& word) noexcept = default;

			/// <summary> Word copy assignment operator. </summary>
			/// <param name="word"> The reference to word to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned word. </returns>
			Word& operator=(const Word& word) = delete;

			/// <summary> Word move assignment operator. </summary>
			/// <param name="word"> The r-value reference to word to move assign from. </param>
			/// <returns> The reference to the newly move-assigned word. </returns>
			Word& operator=(Word&& word) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Appends the given character to the current word.
			/// </summary>
			/// <param name="character">The character to be appended to the word.</param>
			void AddCharacter(const Character& character);
			const std::vector<Character>& GetCharacters() const { return m_characters; }
			math::Real GetWordWidth() const { return m_width; }
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
	} /* end namespace text */
} /* end namespace rendering */

#endif /* __RENDERING_WORD_H__ */