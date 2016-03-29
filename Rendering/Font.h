#ifndef __RENDERING_TEXT_FONT_H__
#define __RENDERING_TEXT_FONT_H__

#include "Rendering.h"
#include "Texture.h"
#include "Character.h"
#include "Math\Math.h"
#include <map>

namespace Rendering
{
	namespace Text
	{
		class Font
		{
		/* ==================== Static variables begin ==================== */
		public:
			static const Math::Real LINE_HEIGHT;
			static const int SPACE_ASCII_CODE;
		private:
			static const int PADDING_TOP;
			static const int PADDING_LEFT;
			static const int PADDING_BOTTOM;
			static const int PADDING_RIGHT;
			static const int DESIRED_PADDING;

			static const char META_DATA_SPLITTER[2];
			static const char NUMBER_SEPARATOR;
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API Font(const std::string& fontTextureAtlasFileName, const std::string& fontMetaDataFileName);
			RENDERING_API virtual ~Font(void);
		//private:
		//	Font(const Font& font);
		//	void operator=(const Font& font);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API const Character& GetCharacter(int asciiCode) const;
			RENDERING_API Math::Real GetSpaceWidth() const { return m_spaceWidth; }
			RENDERING_API const std::string& GetMetaDataFileName() const { return m_metaDataFileName; }
			RENDERING_API void Bind() const { m_textureAtlas.Bind(0); }
		private:
			void ReadMetaDataFile(const std::string& fontMetaDataFileName);
			void AddCharacter(std::vector<std::string>& tokens, int imageWidth);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		protected:
			Texture m_textureAtlas;
			const std::string m_metaDataFileName;

			Math::Real m_aspectRatio;

			int m_padding[4];
			Math::Real m_verticalPerPixelSize;
			Math::Real m_horizontalPerPixelSize;

			Math::Real m_spaceWidth;

			std::map<int, Character> m_metaData;
		/* ==================== Non-static member variables end ==================== */
		}; /* end class Font */

		struct FontComparator
		{
			bool operator() (const Font* font1, const Font* font2) const
			{
				return font1->GetMetaDataFileName().compare(font2->GetMetaDataFileName()) < 0;
			}
		};
	} /* end namespace Text */
} /* end namespace Rendering */

#endif /* __RENDERING_TEXT_FONT_H__ */