#include "stdafx.h"
#include "TextureFactory.h"

#include <Utility\IConfig.h>

Engine::TextureFactory::TextureFactory(const std::string& texturesDirectory) :
	m_texturesDirectory(texturesDirectory)
	// TODO: If we initialize the map this way then Texture's copy ctrs will be used (instead of move ctrs). Google that!
	//m_textureType2TextureMap({ { TextureTypes::DEFAULT, std::move(Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultTexture", "defaultTexture"))) } })
{
	// This way we make sure that Texture's move ctrs are used instead of copy ctrs.
	//m_textureType2TextureMap.insert(std::make_pair(TextureTypes::DEFAULT, Rendering::Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultNormalMap", "defaultNormalMap.jpg"))));
	//m_textureType2TextureMap.insert(std::make_pair(TextureTypes::DEFAULT_NORMAL_MAP, Rendering::Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultNormalMap", "defaultNormalMap.jpg"))));
	//m_textureType2TextureMap.insert(std::make_pair(TextureTypes::DEFAULT_DISPLACEMENT_MAP, Rendering::Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultDisplacementMap", "defaultDisplacementMap.jpg"))));
}


Engine::TextureFactory::~TextureFactory()
{
}

Rendering::Texture* Engine::TextureFactory::CreateTexture(int textureID, const std::string& textureFileName)
{
	std::pair<std::map<int, Rendering::Texture>::iterator, bool> texturePair =
		m_textureType2TextureMap.insert(std::make_pair(textureID, Rendering::Texture(textureFileName)));
	CHECK_CONDITION_ENGINE(texturePair.second, Utility::Logging::WARNING, "Texture \"", textureFileName, "\" has already been created.");
	return &texturePair.first->second;
}