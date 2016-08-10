#ifndef __ENGINE_TEXTURE_FACTORY_H__
#define __ENGINE_TEXTURE_FACTORY_H__

#include "Engine.h"
#include "Rendering\Texture.h"

#include "Utility\ILogger.h"

#include <map>

namespace Engine
{
	class TextureFactory
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>The shader factory constructor.</summary>
		/// <param name="texturesDirectory"> The textures directory. </param>
		TextureFactory(const std::string& texturesDirectory);

		/// <summary>The destructor.</summary>
		~TextureFactory(void);

		TextureFactory(const TextureFactory& textureFactory) = delete; // Copy constructor
		TextureFactory(TextureFactory&& textureFactory) = delete; // Move constructor
		void operator=(TextureFactory& textureFactory) = delete; // Copy assignment operator
		void operator=(TextureFactory&& textureFactory) = delete; // Move assignment operator
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		Rendering::Texture* CreateTexture(int textureID, const std::string& textureFileName);
		ENGINE_API inline const Rendering::Texture& GetTexture(int textureID) const
		{
			std::map<int, Rendering::Texture>::const_iterator textureItr = m_textureType2TextureMap.find(textureID);
			if (textureItr == m_textureType2TextureMap.end())
			{
				ERROR_LOG_RENDERING("No texture has been created for the specified ID (", textureID, ").");
			}
			return textureItr->second;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The system directory where all textures are stored.
		/// </summary>
		const std::string& m_texturesDirectory;

		/// <summary>
		/// The map storing all textures by their unique ID.
		/// </summary>
		std::map<int, Rendering::Texture> m_textureType2TextureMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TextureFactory */

} /* end namespace Engine */

#endif /* __ENGINE_TEXTURE_FACTORY_H__ */