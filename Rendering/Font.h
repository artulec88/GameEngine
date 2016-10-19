#ifndef __RENDERING_TEXT_FONT_H__
#define __RENDERING_TEXT_FONT_H__

#include "Rendering.h"
#include "Color.h"
#include "Texture.h"
#include "Shader.h"
#include "Character.h"
#include "Math\Math.h"
#include <map>

namespace Rendering
{
	namespace Text
	{
		/// <summary>
		/// The representation of the font in the rendering library.
		/// </summary>
		class Font
		{
			/* ==================== Static variables begin ==================== */
		public:
			static constexpr Math::Real LINE_HEIGHT = 0.03f;
			static constexpr int SPACE_ASCII_CODE = 32;
		private:
			static constexpr int PADDING_TOP_INDEX = 0;
			static constexpr int PADDING_LEFT_INDEX = 1;
			static constexpr int PADDING_BOTTOM_INDEX = 2;
			static constexpr int PADDING_RIGHT_INDEX = 3;
			static constexpr int DESIRED_PADDING = 8;
			/// <summary> The number of sides (left, top, right, bottom) for the padding. </summary>
			static constexpr int PADDING_SIDES_COUNT = 4;

			static constexpr char META_DATA_SPLITTER[2] = { ' ', '=' };
			static constexpr char NUMBER_SEPARATOR = ',';
			/* ==================== Static variables end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Font constructor. </summary>
			/// <param name="fontShader"> The shader that will be used the font when rendering text. </param>
			/// <param name="fontTextureAtlasFileName"> The file name of the font texture atlas containing the graphical representation for all characters. </param>
			/// <param name="fontMetaDataFileName"> The file name of the font meta data containing the information about each character. </param>
			Font(const Shader* fontShader, const std::string& fontTextureAtlasFileName, const std::string& fontMetaDataFileName);
			/// <summary> Font destructor. </summary>
			~Font(void);
			/// <summary> Font copy constructor. </summary>
			Font(const Font& font) = delete;
			/// <summary> Font move constructor. </summary>
			Font(Font&& font);
			/// <summary> Font copy assignment operator. </summary>
			Font& operator=(const Font& font) = delete;
			/// <summary> Font move assignment operator. </summary>
			Font& operator=(Font&& font) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			const Character& GetCharacter(int asciiCode) const;
			Math::Real GetSpaceWidth() const { return m_spaceWidth; }
			const std::string& GetMetaDataFileName() const { return m_metaDataFileName; }
			void Bind(const Math::Vector2D& translation, const Math::Vector2D& offset, const Color& color, const Color& outlineColor,
				Math::Real characterWidth, Math::Real characterEdgeTransitionWidth, Math::Real borderWidth, Math::Real borderEdgeTransitionWidth) const;
		private:
			void ReadMetaDataFile(const std::string& fontMetaDataFileName);
			void AddCharacter(std::vector<std::string>& tokens, int imageWidth);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			const Shader* m_shader;
			Texture m_textureAtlas;
			const std::string m_metaDataFileName;

			Math::Real m_aspectRatio;

			std::array<int, PADDING_SIDES_COUNT> m_padding;
			Math::Real m_verticalPerPixelSize;
			Math::Real m_horizontalPerPixelSize;

			Math::Real m_spaceWidth;

			std::map<int, Character> m_metaData;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Font */

		/// <summary>
		/// The <code>Font</code> objects comparator.
		/// </summary>
		/// <remarks>
		/// It is more efficient to pass a function-object in a function call than pass function pointers.
		/// That is why, instead of creating a Compare(const Font& other) method in the <code>Font</code> class itself and pass the pointer to this function whenever it is needed,
		/// it is better to create a <code>FontComparator</code> object and pass its temporary instance. The reason is that function-objects are almost always inlined by the compiler.
		/// See https://en.wikibooks.org/wiki/Optimizing_C%2B%2B/Writing_efficient_code/Performance_improving_features#Function-objects.
		/// </remarks>
		struct FontComparator
		{
			bool operator() (const Font& font1, const Font& font2) const
			{
				return font1.GetMetaDataFileName().compare(font2.GetMetaDataFileName()) < 0;
			}
		};
	} /* end namespace Text */
} /* end namespace Rendering */

#endif /* __RENDERING_TEXT_FONT_H__ */