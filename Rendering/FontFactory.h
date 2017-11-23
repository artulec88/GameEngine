#ifndef __RENDERING_TEXT_FONT_FACTORY_H__
#define __RENDERING_TEXT_FONT_FACTORY_H__

#include "Rendering.h"
#include "Font.h"
#include "Shader.h"

namespace rendering
{
	namespace text
	{
		namespace FontIDs
		{
			enum FontID
			{
				CANDARA = 0,
				SEGOE,
				CAMBRIA,
				
				COUNT
			}; /* end enum FontID */
		} /* end namespace FontIDs */

		/// <summary>
		/// Font factory.
		/// </summary>
		class FontFactory
		{
			/* ==================== Static variables begin ==================== */
			/* ==================== Static variables end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Font factory constructor. </summary>
			/// <param name="textShader"> The shader that will be used for rendering text. </param>
			/// <param name="texturesDirectory"> The directory where all font textures are stored. </param>
			/// <param name="fontsDirectory"> The directory where meta data for all fonts are stored. </param>
			RENDERING_API FontFactory(const Shader* textShader, const std::string& texturesDirectory, const std::string& fontsDirectory);

			/// <summary> Font factory destructor. </summary>
			RENDERING_API ~FontFactory(void);

			/// <summary> Font factory copy constructor. </summary>
			/// <param name="fontFactory"> The font factory to copy construct from. </param>
			FontFactory(const FontFactory& fontFactory) = delete;

			/// <summary> Font factory move constructor. </summary>
			/// <param name="fontFactory"> The font factory to move construct from. </param>
			FontFactory(FontFactory&& fontFactory) = delete;

			/// <summary> Font factory copy assignment operator. </summary>
			/// <param name="fontFactory"> The font factory to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned font factory. </returns>
			FontFactory& operator=(const FontFactory& fontFactory) = delete;

			/// <summary> Font factory move assignment operator. </summary>
			/// <param name="fontFactory"> The font factory to move assign from. </param>
			/// <returns> The reference to the newly move-assigned font factory. </returns>
			FontFactory& operator=(FontFactory&& fontFactory) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API const Font* CreateFont(int fontID, const std::string& fontTextureFileName, const std::string& fontMetaDataFileName);
			RENDERING_API const Font* GetFont(int fontID) const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			const Shader* m_textShader;
			std::map<int, Font> m_fontType2FontMap;
			const std::string& m_texturesDirectory;
			const std::string& m_fontsDirectory;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class FontFactory */
	} /* end namespace text */
} /* end namespace rendering */

#endif /* __RENDERING_TEXT_FONT_FACTORY_H__ */