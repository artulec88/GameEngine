#ifndef __RENDERING_CHARACTER_H__
#define __RENDERING_CHARACTER_H__

#include "Rendering.h"
#include "Math/Vector.h"

namespace rendering
{
	namespace text
	{
		/// <summary>
		/// The class representing a single glyph in the font texture.
		/// It holds all the information necessary for the correct text rendering.
		/// </summary>
		class Character
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> The constructor. </summary>
			/// <param name="asciiCode"> The ASCII code of the character. </param>
			/// <param name="textureCoords"> The texture coordinates for the top-left corner of the character in the texture atlas. </param>
			/// <param name="texSize"> The size of the character in the texture atlas. </param>
			/// <param name="offset"> The offset (in X and Z components) from the cursor to the top-left position of the character's quad. </param>
			/// <param name="size"> The size of the character's quad in screen space. </param>
			/// <param name="xAdvance"> The value representing how far in pixels the cursor should advance after adding this character. </param>
			RENDERING_API constexpr Character(int asciiCode, const math::Vector2D& textureCoords, const math::Vector2D& texSize, const math::Vector2D& offset, const math::Vector2D& size, const math::Real xAdvance) :
				m_asciiCode(asciiCode),
				m_textureCoords(textureCoords),
				// TODO: It'd be nicer to simply call texSide + textureCoords, but that is a non-constexpr unfortunately.
				m_maxTextureCoords(math::Vector2D(texSize.x + textureCoords.x, texSize.y + textureCoords.y)),
				m_offset(offset),
				m_size(size),
				m_xAdvance(xAdvance)
			{
			}

			/// <summary> The character destructor. </summary>
			RENDERING_API ~Character() noexcept = default;

			/// <summary> Character copy constructor. </summary>
			/// <param name="character"> The reference to the character to copy construct from. </param>
			RENDERING_API constexpr Character(const Character& character) noexcept = default;

			/// <summary> Character move constructor. </summary>
			/// <param name="character"> The r-value reference to the character to move construct from. </param>
			RENDERING_API constexpr Character(Character&& character) noexcept = default;

			/// <summary> Character copy assignment operator. </summary>
			/// <param name="character"> The reference to the character to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned character. </returns>
			RENDERING_API Character& operator=(const Character& character) noexcept = default;

			/// <summary> Character move assignment operator. </summary>
			/// <param name="character"> The r-value reference to the character to move assign from. </param>
			/// <returns> The reference to the newly move-assigned character. </returns>
			RENDERING_API Character& operator=(Character&& character) noexcept = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/// <summary> Returns the ASCII code of the underlying character. </summary>
			/// <returns> The ASCII code of the underlying character. </returns>
			constexpr int GetAsciiCode() const { return m_asciiCode; }

			/// <summary> Returns the texture coordinates of the character. </summary>
			/// <returns> The texture coordinates of the character. </returns>
			constexpr const math::Vector2D& GetTextureCoords() const { return m_textureCoords; }

			constexpr const math::Vector2D& GetMaxTextureCoords() const { return m_maxTextureCoords; }
			constexpr const math::Vector2D& GetOffset() const { return m_offset; }
			constexpr const math::Vector2D& GetSize() const { return m_size; }
			constexpr math::Real GetXAdvance() const { return m_xAdvance; }
			/* ==================== Non-static member variables end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/// <summary>The ASCII code of the character.</summary>
			int m_asciiCode;
			math::Vector2D m_textureCoords;
			math::Vector2D m_maxTextureCoords;
			math::Vector2D m_offset;
			math::Vector2D m_size;
			math::Real m_xAdvance;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Character */
	} /* end namespace text */
} /* end namespace rendering */

#endif /* __RENDERING_CHARACTER_H__ */