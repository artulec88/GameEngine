#ifndef __RENDERING_CHARACTER_H__
#define __RENDERING_CHARACTER_H__

#include "Rendering.h"
#include "Math\Vector.h"

namespace Rendering
{
	namespace Text
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
			RENDERING_API Character(int asciiCode, const Math::Vector2D& textureCoords, const Math::Vector2D& texSize, const Math::Vector2D& offset, const Math::Vector2D& size, const Math::Real xAdvance) :
				m_asciiCode(asciiCode),
				m_textureCoords(textureCoords),
				m_maxTextureCoords(texSize + textureCoords),
				m_offset(offset),
				m_size(size),
				m_xAdvance(xAdvance)
			{
			}
			/// <summary> The character destructor. </summary>
			RENDERING_API ~Character(void)
			{
			}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		public:
			int GetAsciiCode() const { return m_asciiCode; };
			const Math::Vector2D& GetTextureCoords() const { return m_textureCoords; };
			const Math::Vector2D& GetMaxTextureCoords() const { return m_maxTextureCoords; }
			const Math::Vector2D& GetOffset() const { return m_offset; }
			const Math::Vector2D& GetSize() const { return m_size; }
			const Math::Real GetXAdvance() const { return m_xAdvance; }
		/* ==================== Non-static member variables end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		private:
			/// <summary>The ASCII code of the character.</summary>
			int m_asciiCode;
			Math::Vector2D m_textureCoords;
			Math::Vector2D m_maxTextureCoords;
			Math::Vector2D m_offset;
			Math::Vector2D m_size;
			Math::Real m_xAdvance;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class Character */
	} /* end namespace Text */
} /* end namespace Rendering */

#endif /* __RENDERING_CHARACTER_H__ */