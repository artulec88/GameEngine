#ifndef __RENDERING_TEXTURE_FACTORY_H__
#define __RENDERING_TEXTURE_FACTORY_H__

#include "Rendering.h"
#include "Texture.h"

#include "Utility/ILogger.h"

#include <map>

namespace rendering
{
	class TextureBuilder;

	/// <summary>
	/// <code>Texture</code> factory.
	/// </summary>
	class TextureFactory
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>The texture factory constructor.</summary>
		/// <param name="texturesDirectory"> The textures directory where all textures are stored. </param>
		RENDERING_API explicit TextureFactory(const std::string& texturesDirectory);

		/// <summary>The texture factory destructor.</summary>
		RENDERING_API ~TextureFactory();

		/// <summary> Texture factory copy constructor. </summary>
		/// <param name="textureFactory"> The reference of the texture factory to copy construct from. </param>
		TextureFactory(const TextureFactory& textureFactory) = delete;

		/// <summary> Texture factory move constructor. </summary>
		/// <param name="textureFactory"> The r-value reference of the texture factory to move construct from. </param>
		TextureFactory(TextureFactory&& textureFactory) = delete;

		/// <summary> Texture factory copy assignment operator. </summary>
		/// <param name="textureFactory"> The reference of the texture factory to copy assign from. </param>
		/// <returns> The reference of the newly copy-assigned texture factory. </returns>
		TextureFactory& operator=(TextureFactory& textureFactory) = delete;

		/// <summary> Texture factory move assignment operator. </summary>
		/// <param name="textureFactory"> The r-value reference of the texture factory to move assign from. </param>
		/// <returns> The reference of the newly move-assigned texture factory. </returns>
		TextureFactory& operator=(TextureFactory&& textureFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		RENDERING_API const std::string& GetTexturesDirectory() const { return m_texturesDirectory; }
		RENDERING_API const Texture* CreateTexture(int textureId, const TextureBuilder& textureBuilder);
		RENDERING_API const Texture* CreateCubeTexture(int textureId, const std::string& cubeTextureDirectory);
		RENDERING_API const Texture* GetTexture(int textureId) const
		{
			auto textureItr = m_textureType2TextureMap.find(textureId);
			if (textureItr == m_textureType2TextureMap.end())
			{
				ERROR_LOG_RENDERING("No texture has been created for the specified ID (", textureId, ").");
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
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TextureFactory */

} /* end namespace rendering */

#endif /* __RENDERING_TEXTURE_FACTORY_H__ */