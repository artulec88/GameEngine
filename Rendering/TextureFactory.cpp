#include "stdafx.h"
#include "TextureFactory.h"
#include "TextureIDs.h"
#include "TextureBuilder.h"

#include "Utility/IConfig.h"
#include "Utility/FileManager.h"
#include "Engine/ParticlesSystem.h"

rendering::TextureFactory::TextureFactory(const std::string& texturesDirectory) :
	m_texturesDirectory(texturesDirectory)
	// TODO: If we initialize the map this way then Texture's copy ctrs will be used (instead of move ctrs). Google that!
	//m_textureType2TextureMap({ { TextureTypes::DEFAULT, std::move(Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultTexture", "defaultTexture"))) } })
{
	// This way we make sure that Texture's move ctrs are used instead of copy ctrs.
	//m_textureType2TextureMap.insert(make_pair(TextureTypes::DEFAULT, Rendering::Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultNormalMap", "defaultNormalMap.jpg"))));
	m_textureType2TextureMap.insert(std::make_pair(texture_ids::DEFAULT_TEXTURE, Texture(m_texturesDirectory +
		GET_CONFIG_VALUE_STR_RENDERING("defaultTexture", "defaultTexture.png"))));
	m_textureType2TextureMap.insert(std::make_pair(texture_ids::DEFAULT_NORMAL_MAP, Texture(m_texturesDirectory +
		GET_CONFIG_VALUE_STR_RENDERING("defaultNormalMap", "defaultNormalMap.jpg"))));
	m_textureType2TextureMap.insert(std::make_pair(texture_ids::DEFAULT_DISPLACEMENT_MAP, Texture(m_texturesDirectory +
		GET_CONFIG_VALUE_STR_RENDERING("defaultDisplacementMap", "defaultDisplacementMap.jpg"))));
}


rendering::TextureFactory::~TextureFactory()
{
}

const rendering::Texture* rendering::TextureFactory::CreateTexture(int textureId, const TextureBuilder& textureBuilder)
{
	INFO_LOG_RENDERING("Creating texture \"", textureBuilder.GetFileName(), "\" for ID ", textureId);
	const auto texturePair = m_textureType2TextureMap.insert(std::make_pair(textureId, Texture(m_texturesDirectory + textureBuilder.GetFileName(), textureBuilder.GetTarget(),
		textureBuilder.GetFilter(), textureBuilder.GetInternalFormat(), textureBuilder.GetFormat(), textureBuilder.GetWrapping(), textureBuilder.GetAttachment())));
	CHECK_CONDITION_RENDERING(texturePair.second, Utility::Logging::WARNING, "Texture \"", textureFileName, "\" has already been created.");
	DEBUG_LOG_RENDERING("Texture \"", textureBuilder.GetFileName(), "\" has been created for ID ", textureId);
	return &texturePair.first->second;
}

const rendering::Texture* rendering::TextureFactory::CreateCubeTexture(int textureId, const std::string& cubeTextureDirectory)
{
	INFO_LOG_RENDERING("Creating cube texture \"", cubeTextureDirectory, "\" for ID ", textureId);
	constexpr char* directoryPathSeparator = "\\"; // for Windows it's "\", but for Unix it's "/"
	constexpr char* expectedPosXFaceFilename = "right";
	constexpr char* expectedNegXFaceFilename = "left";
	constexpr char* expectedPosYFaceFilename = "up";
	constexpr char* expectedNegYFaceFilename = "down";
	constexpr char* expectedPosZFaceFilename = "front";
	constexpr char* expectedNegZFaceFilename = "back";

	utility::FileManager fileManager;
	const auto cubeTextureFullDir = m_texturesDirectory + cubeTextureDirectory;
	auto filenames = fileManager.ListAllFilesInDirectory(cubeTextureFullDir);
	auto cubeMapPosXFaceFileFound = false; auto cubeMapPosXFaceFileName = cubeTextureFullDir + directoryPathSeparator;
	auto cubeMapNegXFaceFileFound = false; auto cubeMapNegXFaceFileName = cubeTextureFullDir + directoryPathSeparator;
	auto cubeMapPosYFaceFileFound = false; auto cubeMapPosYFaceFileName = cubeTextureFullDir + directoryPathSeparator;
	auto cubeMapNegYFaceFileFound = false; auto cubeMapNegYFaceFileName = cubeTextureFullDir + directoryPathSeparator;
	auto cubeMapPosZFaceFileFound = false; auto cubeMapPosZFaceFileName = cubeTextureFullDir + directoryPathSeparator;
	auto cubeMapNegZFaceFileFound = false; auto cubeMapNegZFaceFileName = cubeTextureFullDir + directoryPathSeparator;
	for (std::vector<std::string>::const_iterator filenameItr = filenames.begin(); filenameItr != filenames.end(); ++filenameItr)
	{
		if (!cubeMapPosXFaceFileFound && (*filenameItr).find(expectedPosXFaceFilename) != std::string::npos)
		{
			cubeMapPosXFaceFileFound = true;
			cubeMapPosXFaceFileName += *filenameItr;
		}
		if (!cubeMapNegXFaceFileFound && (*filenameItr).find(expectedNegXFaceFilename) != std::string::npos)
		{
			cubeMapNegXFaceFileFound = true;
			cubeMapNegXFaceFileName += *filenameItr;
		}
		if (!cubeMapPosYFaceFileFound && (*filenameItr).find(expectedPosYFaceFilename) != std::string::npos)
		{
			cubeMapPosYFaceFileFound = true;
			cubeMapPosYFaceFileName += *filenameItr;
		}
		if (!cubeMapNegYFaceFileFound && (*filenameItr).find(expectedNegYFaceFilename) != std::string::npos)
		{
			cubeMapNegYFaceFileFound = true;
			cubeMapNegYFaceFileName += *filenameItr;
		}
		if (!cubeMapPosZFaceFileFound && (*filenameItr).find(expectedPosZFaceFilename) != std::string::npos)
		{
			cubeMapPosZFaceFileFound = true;
			cubeMapPosZFaceFileName += *filenameItr;
		}
		if (!cubeMapNegZFaceFileFound && (*filenameItr).find(expectedNegZFaceFilename) != std::string::npos)
		{
			cubeMapNegZFaceFileFound = true;
			cubeMapNegZFaceFileName += *filenameItr;
		}
	}
	CHECK_CONDITION_EXIT_RENDERING(cubeMapPosXFaceFileFound, Utility::Logging::ERR, "Cannot locate the right face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapNegXFaceFileFound, Utility::Logging::ERR, "Cannot locate the left face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapPosYFaceFileFound, Utility::Logging::ERR, "Cannot locate the up face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapNegYFaceFileFound, Utility::Logging::ERR, "Cannot locate the down face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapPosZFaceFileFound, Utility::Logging::ERR, "Cannot locate the front face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT_RENDERING(cubeMapNegZFaceFileFound, Utility::Logging::ERR, "Cannot locate the back face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting

	const auto texturePair = m_textureType2TextureMap.insert(std::make_pair(textureId, Texture(cubeMapPosXFaceFileName, cubeMapNegXFaceFileName, cubeMapPosYFaceFileName, cubeMapNegYFaceFileName, cubeMapPosZFaceFileName, cubeMapNegZFaceFileName)));
	CHECK_CONDITION_RENDERING(texturePair.second, Utility::Logging::WARNING, "Cube texture \"", cubeTextureDirectory, "\" has already been created.");
	DEBUG_LOG_RENDERING("Cube texture \"", cubeTextureDirectory, "\" has been created for ID ", textureId);
	return &texturePair.first->second;
}