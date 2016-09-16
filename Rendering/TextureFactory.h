#ifndef __RENDERING_TEXTURE_FACTORY_H__
#define __RENDERING_TEXTURE_FACTORY_H__

#include "Rendering.h"
#include "Texture.h"

#include "Utility\ILogger.h"

#include <map>

namespace Rendering
{
	/// <summary>
	/// <code>Texture</code> factory.
	/// </summary>
	class TextureFactory
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>The shader factory constructor.</summary>
		/// <param name="texturesDirectory"> The textures directory. </param>
		RENDERING_API TextureFactory(const std::string& texturesDirectory);

		/// <summary>The destructor.</summary>
		RENDERING_API ~TextureFactory(void);

		/// <summary> Texture factory copy constructor. </summary>
		TextureFactory(const TextureFactory& textureFactory) = delete;
		/// <summary> Texture factory move constructor. </summary>
		TextureFactory(TextureFactory&& textureFactory) = delete;
		/// <summary> Texture factory copy assignment operator. </summary>
		TextureFactory& operator=(TextureFactory& textureFactory) = delete;
		/// <summary> Texture factory move assignment operator. </summary>
		TextureFactory& operator=(TextureFactory&& textureFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API const Texture* CreateTexture(int textureID, const std::string& textureFileName);
		RENDERING_API const Texture* CreateCubeTexture(int textureID, const std::string& cubeTextureDirectory);
		RENDERING_API const Particles::ParticleTexture* CreateParticleTexture(int textureID, const std::string& textureFileName, int rowsCount, bool isAdditive);
		RENDERING_API inline const Texture* GetTexture(int textureID) const
		{
			auto textureItr = m_textureType2TextureMap.find(textureID);
			if (textureItr == m_textureType2TextureMap.end())
			{
				auto particleTextureItr = m_textureType2ParticleTextureMap.find(textureID);
				if (particleTextureItr == m_textureType2ParticleTextureMap.end())
				{
					ERROR_LOG_RENDERING("No texture has been created for the specified ID (", textureID, ").");
				}
				return &particleTextureItr->second;
			}
			return &textureItr->second;
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
		std::map<int, Texture> m_textureType2TextureMap;

		/// <summary>
		/// The map storing all particle textures by their unique ID.
		/// </summary>
		std::map<int, Particles::ParticleTexture> m_textureType2ParticleTextureMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TextureFactory */

} /* end namespace Rendering */

#endif /* __RENDERING_TEXTURE_FACTORY_H__ */