#include "stdafx.h"
#include "TextureFactory.h"
#include "TextureIDs.h"

#include <Utility\IConfig.h>
#include "Utility\FileManager.h"

Rendering::TextureFactory::TextureFactory(const std::string& texturesDirectory) :
	m_texturesDirectory(texturesDirectory)
	// TODO: If we initialize the map this way then Texture's copy ctrs will be used (instead of move ctrs). Google that!
	//m_textureType2TextureMap({ { TextureTypes::DEFAULT, std::move(Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultTexture", "defaultTexture"))) } })
{
	// This way we make sure that Texture's move ctrs are used instead of copy ctrs.
	//m_textureType2TextureMap.insert(make_pair(TextureTypes::DEFAULT, Rendering::Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultNormalMap", "defaultNormalMap.jpg"))));
	m_textureType2TextureMap.insert(std::make_pair(TextureIDs::DEFAULT_TEXTURE, Rendering::Texture(m_texturesDirectory +
		GET_CONFIG_VALUE_STR_RENDERING("defaultTexture", "defaultTexture.png"))));
	m_textureType2TextureMap.insert(std::make_pair(TextureIDs::DEFAULT_NORMAL_MAP, Rendering::Texture(m_texturesDirectory +
		GET_CONFIG_VALUE_STR_RENDERING("defaultNormalMap", "defaultNormalMap.jpg"))));
	m_textureType2TextureMap.insert(std::make_pair(TextureIDs::DEFAULT_DISPLACEMENT_MAP, Rendering::Texture(m_texturesDirectory +
		GET_CONFIG_VALUE_STR_RENDERING("defaultDisplacementMap", "defaultDisplacementMap.jpg"))));
}


Rendering::TextureFactory::~TextureFactory()
{
}

const Rendering::Texture* Rendering::TextureFactory::CreateTexture(int textureID, const std::string& textureFileName)
{
	INFO_LOG_RENDERING("Creating texture \"", textureFileName, "\" for ID ", textureID);
	std::pair<std::map<int, Texture>::iterator, bool> texturePair =
		m_textureType2TextureMap.insert(std::make_pair(textureID, Texture(m_texturesDirectory + textureFileName)));
	CHECK_CONDITION_RENDERING(texturePair.second, Utility::Logging::WARNING, "Texture \"", textureFileName, "\" has already been created.");
	DEBUG_LOG_RENDERING("Texture \"", textureFileName, "\" has been created for ID ", textureID);
	return &texturePair.first->second;
}

const Rendering::Texture* Rendering::TextureFactory::CreateCubeTexture(int textureID, const std::string& cubeTextureDirectory)
{
	INFO_LOG_RENDERING("Creating cube texture \"", cubeTextureDirectory, "\" for ID ", textureID);
	const std::string DIRECTORY_PATH_SEPARATOR = "\\"; // for Windows it's "\", but for Unix it's "/"
	const std::string EXPECTED_POS_X_FACE_FILENAME = "right";
	const std::string EXPECTED_NEG_X_FACE_FILENAME = "left";
	const std::string EXPECTED_POS_Y_FACE_FILENAME = "up";
	const std::string EXPECTED_NEG_Y_FACE_FILENAME = "down";
	const std::string EXPECTED_POS_Z_FACE_FILENAME = "front";
	const std::string EXPECTED_NEG_Z_FACE_FILENAME = "back";

	utility::FileManager fileManager;
	std::vector<std::string> filenames = fileManager.ListAllFilesInDirectory(m_texturesDirectory + cubeTextureDirectory);
	bool cubeMapPosXFaceFileFound = false; std::string cubeMapPosXFaceFileName = cubeTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegXFaceFileFound = false; std::string cubeMapNegXFaceFileName = cubeTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapPosYFaceFileFound = false; std::string cubeMapPosYFaceFileName = cubeTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegYFaceFileFound = false; std::string cubeMapNegYFaceFileName = cubeTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapPosZFaceFileFound = false; std::string cubeMapPosZFaceFileName = cubeTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegZFaceFileFound = false; std::string cubeMapNegZFaceFileName = cubeTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	for (std::vector<std::string>::const_iterator filenameItr = filenames.begin(); filenameItr != filenames.end(); ++filenameItr)
	{
		if ((!cubeMapPosXFaceFileFound) && ((*filenameItr).find(EXPECTED_POS_X_FACE_FILENAME) != std::string::npos))
		{
			cubeMapPosXFaceFileFound = true;
			cubeMapPosXFaceFileName += (*filenameItr);
		}
		if ((!cubeMapNegXFaceFileFound) && ((*filenameItr).find(EXPECTED_NEG_X_FACE_FILENAME) != std::string::npos))
		{
			cubeMapNegXFaceFileFound = true;
			cubeMapNegXFaceFileName += (*filenameItr);
		}
		if ((!cubeMapPosYFaceFileFound) && ((*filenameItr).find(EXPECTED_POS_Y_FACE_FILENAME) != std::string::npos))
		{
			cubeMapPosYFaceFileFound = true;
			cubeMapPosYFaceFileName += (*filenameItr);
		}
		if ((!cubeMapNegYFaceFileFound) && ((*filenameItr).find(EXPECTED_NEG_Y_FACE_FILENAME) != std::string::npos))
		{
			cubeMapNegYFaceFileFound = true;
			cubeMapNegYFaceFileName += (*filenameItr);
		}
		if ((!cubeMapPosZFaceFileFound) && ((*filenameItr).find(EXPECTED_POS_Z_FACE_FILENAME) != std::string::npos))
		{
			cubeMapPosZFaceFileFound = true;
			cubeMapPosZFaceFileName += (*filenameItr);
		}
		if ((!cubeMapNegZFaceFileFound) && ((*filenameItr).find(EXPECTED_NEG_Z_FACE_FILENAME) != std::string::npos))
		{
			cubeMapNegZFaceFileFound = true;
			cubeMapNegZFaceFileName += (*filenameItr);
		}
	}
	CHECK_CONDITION_EXIT_RENDERING(cubeMapPosXFaceFileFound, Utility::Logging::ERR, "Cannot locate the right face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapNegXFaceFileFound, Utility::Logging::ERR, "Cannot locate the left face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapPosYFaceFileFound, Utility::Logging::ERR, "Cannot locate the up face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapNegYFaceFileFound, Utility::Logging::ERR, "Cannot locate the down face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapPosZFaceFileFound, Utility::Logging::ERR, "Cannot locate the front face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapNegZFaceFileFound, Utility::Logging::ERR, "Cannot locate the back face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting

	std::pair<std::map<int, Texture>::iterator, bool> texturePair =
		m_textureType2TextureMap.insert(std::make_pair(textureID, Texture(cubeMapPosXFaceFileName, cubeMapNegXFaceFileName, cubeMapPosYFaceFileName, cubeMapNegYFaceFileName, cubeMapPosZFaceFileName, cubeMapNegZFaceFileName)));
	CHECK_CONDITION_RENDERING(texturePair.second, Utility::Logging::WARNING, "Cube texture \"", cubeTextureDirectory, "\" has already been created.");
	DEBUG_LOG_RENDERING("Cube texture \"", cubeTextureDirectory, "\" has been created for ID ", textureID);
	return &texturePair.first->second;
}

const Rendering::Particles::ParticleTexture* Rendering::TextureFactory::CreateParticleTexture(int textureID, const std::string& textureFileName, int rowsCount, bool isAdditive)
{
	INFO_LOG_RENDERING("Creating particles texture \"", textureFileName, "\" for ID ", textureID);
	std::pair<std::map<int, Particles::ParticleTexture>::iterator, bool> particleTexturePair =
		m_textureType2ParticleTextureMap.insert(std::make_pair(textureID, Particles::ParticleTexture(m_texturesDirectory + textureFileName, rowsCount, isAdditive)));
	CHECK_CONDITION_RENDERING(particleTexturePair.second, Utility::Logging::WARNING, "Texture \"", textureFileName, "\" has already been created.");
	return &particleTexturePair.first->second;
}