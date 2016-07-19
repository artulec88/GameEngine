#ifndef __RENDERING_TEXT_FONT_FACTORY_H__
#define __RENDERING_TEXT_FONT_FACTORY_H__

#include "Rendering.h"
#include "Font.h"

namespace Rendering
{
	namespace Text
	{
		namespace FontTypes
		{
			enum FontType
			{
				CANDARA = 0,
				SEGOE,
				CAMBRIA,
				
				COUNT
			}; /* end enum FontType */
		} /* end namespace FontTypes */

		class FontFactory
		{
			/* ==================== Static variables begin ==================== */
			/* ==================== Static variables end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API FontFactory(const std::string& texturesDirectory, const std::string& fontsDirectory);
			RENDERING_API ~FontFactory(void);
			FontFactory(const FontFactory& fontFactory) = delete;
			void operator=(const FontFactory& fontFactory) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API const Font* GetFont(FontTypes::FontType fontType);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			std::map<FontTypes::FontType, Font> m_fontMap;
			const std::string& m_texturesDirectory;
			const std::string& m_fontsDirectory;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class FontFactory */
	} /* end namespace Text */
} /* end namespace Rendering */

#endif /* __RENDERING_TEXT_FONT_FACTORY_H__ */