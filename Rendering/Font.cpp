#include "StdAfx.h"
#include "Font.h"
#include "Utility\StringUtility.h"
#include "Utility\ILogger.h"
#include <fstream>
#include <algorithm> // std::find

Rendering::Text::Font::Font(const Shader* fontShader, const std::string& fontTextureAtlasFileName, const std::string& fontMetaDataFileName) :
	m_shader(fontShader),
	m_textureAtlas(fontTextureAtlasFileName),
	m_metaDataFileName(fontMetaDataFileName)
{
	DELOCUST_LOG_RENDERING("Creating font: (textureAtlas = ", fontTextureAtlasFileName, "; metaData = ", fontMetaDataFileName, ").");
	m_aspectRatio = static_cast<Math::Real>(1600) / 900; // TODO: Calculate aspect ratio accordingly. Do not use hard-coded size here.
	//m_aspectRatio = static_cast<Math::Real>(900) / 1600; // TODO: Calculate aspect ratio accordingly. Do not use hard-coded size here.
	ReadMetaDataFile(fontMetaDataFileName);
	DEBUG_LOG_RENDERING("Font: (textureAtlas = ", fontTextureAtlasFileName, "; metaData = ", fontMetaDataFileName, ") is now created.");
}

Rendering::Text::Font::~Font()
{
	//SAFE_DELETE(m_textureAtlas);
}

Rendering::Text::Font::Font(Font&& font) :
	m_shader(std::move(font.m_shader)),
	m_textureAtlas(std::move(font.m_textureAtlas)),
	m_metaDataFileName(std::move(font.m_metaDataFileName)),
	m_aspectRatio(std::move(font.m_aspectRatio)),
	m_padding(std::move(font.m_padding)),
	m_verticalPerPixelSize(std::move(font.m_verticalPerPixelSize)),
	m_horizontalPerPixelSize(std::move(font.m_horizontalPerPixelSize)),
	m_spaceWidth(std::move(font.m_spaceWidth)),
	m_metaData(std::move(font.m_metaData))
{
}

void Rendering::Text::Font::ReadMetaDataFile(const std::string& fontMetaDataFileName)
{
	// TODO: Implement font meta data file parsing
	std::ifstream file(fontMetaDataFileName);
	CHECK_CONDITION_RETURN_VOID_RENDERING(file.is_open(), Utility::Logging::ERR, "Could not open font meta data file \"", fontMetaDataFileName, "\"");

	int imageWidth;
	std::string line;
	while (!file.eof())
	{
		CHECK_CONDITION_EXIT_ALWAYS_RENDERING(!file.bad() && !file.eof() && !file.fail(), Utility::Logging::EMERGENCY,
			"Error occured in the stream while reading the font meta data file \"", fontMetaDataFileName, "\"");
		std::getline(file, line);
		CHECK_CONDITION_EXIT_ALWAYS_RENDERING(!file.bad(), Utility::Logging::EMERGENCY,
			"Error occured in the stream while reading the font meta data file \"", fontMetaDataFileName, "\"");
		if (!line.empty())
		{
			std::vector<std::string> tokens;
			Utility::StringUtility::CutToTokens(line, tokens, META_DATA_SPLITTER, 2);
			if (tokens[0].compare("info") == 0)
			{
				tokens.erase(tokens.begin()); // Removing the first token
				// Padding calculation begin
				std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "padding");
				CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
					"No \"padding\" attribute found in the font meta data file \"", fontMetaDataFileName, "\"");
				const std::string padding = *(tokenItr + 1);
				std::vector<std::string> subTokens;
				Utility::StringUtility::CutToTokens(padding, subTokens, NUMBER_SEPARATOR);
				CHECK_CONDITION_RENDERING(subTokens.size() == 4, Utility::Logging::ERR, "The \"padding\" attribute must define 4 values instead of ", subTokens.size());
				for (size_t index = 0; index < subTokens.size(); ++index)
				{
					m_padding[index] = Utility::StringUtility::ToInt(subTokens[index]);
				}
				DEBUG_LOG_RENDERING("Padding left = ", m_padding[PADDING_LEFT_INDEX], "; top = ", m_padding[PADDING_TOP_INDEX],
					"; right = ", m_padding[PADDING_RIGHT_INDEX], "; bottom = ", m_padding[PADDING_BOTTOM_INDEX]);
				// Padding calculation end
			}
			else if (tokens[0].compare("common") == 0)
			{
				tokens.erase(tokens.begin()); // Removing the first token
				// Line sizes calculation begin
				std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "lineHeight");
				CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
					"No \"lineHeight\" attribute found in the font meta data file \"", fontMetaDataFileName, "\"");
				int lineHeightPixels = Utility::StringUtility::ToInt(*(tokenItr + 1)) - m_padding[PADDING_TOP_INDEX] - m_padding[PADDING_BOTTOM_INDEX];
				m_verticalPerPixelSize = LINE_HEIGHT / lineHeightPixels;
				m_horizontalPerPixelSize = m_verticalPerPixelSize / m_aspectRatio;
				DEBUG_LOG_RENDERING("Per pixel sizes: horizontal = ", m_horizontalPerPixelSize, ", vertical = ", m_verticalPerPixelSize);
				// Line sizes calculation end

				tokenItr = std::find(tokens.begin(), tokens.end(), "scaleW");
				CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
					"No \"scaleW\" attribute found in the font meta data file \"", fontMetaDataFileName, "\"");
				imageWidth = Utility::StringUtility::ToInt(*(tokenItr + 1));
				DEBUG_LOG_RENDERING("Image width = ", imageWidth);
			}
			else if (tokens[0].compare("char") == 0)
			{
				tokens.erase(tokens.begin()); // Removing the first token
				AddCharacter(tokens, imageWidth); // Creating a character
			}
		}
	}
	file.close();
}

void Rendering::Text::Font::AddCharacter(std::vector<std::string>& tokens, int imageWidth)
{
	std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "id");
	CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
		"No \"id\" attribute found in the font meta data file");
	int id = Utility::StringUtility::ToInt(*(tokenItr + 1));

	tokenItr = std::find(tokens.begin(), tokens.end(), "xadvance");
	CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
		"No \"xadvance\" attribute found in the font meta data file");
	const int metaDataXAdvance = Utility::StringUtility::ToInt(*(tokenItr + 1));
	Math::Real xAdvance = static_cast<Math::Real>(metaDataXAdvance - m_padding[PADDING_LEFT_INDEX] - m_padding[PADDING_RIGHT_INDEX]) * m_horizontalPerPixelSize;

	if (id == SPACE_ASCII_CODE)
	{
		m_spaceWidth = xAdvance;
	}
	else
	{
		tokenItr = std::find(tokens.begin(), tokens.end(), "x");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"x\" attribute found in the font meta data file");
		Math::Real xTextureCoord = static_cast<Math::Real>(Utility::StringUtility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_LEFT_INDEX] - DESIRED_PADDING) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "y");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"y\" attribute found in the font meta data file");
		Math::Real yTextureCoord = static_cast<Math::Real>(Utility::StringUtility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_TOP_INDEX] - DESIRED_PADDING) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "width");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"width\" attribute found in the font meta data file");
		int width = Utility::StringUtility::ToInt(*(tokenItr + 1)) - m_padding[PADDING_LEFT_INDEX] - m_padding[PADDING_RIGHT_INDEX] + (2 * DESIRED_PADDING);

		tokenItr = std::find(tokens.begin(), tokens.end(), "height");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"height\" attribute found in the font meta data file");
		int height = Utility::StringUtility::ToInt(*(tokenItr + 1)) - m_padding[PADDING_TOP_INDEX] - m_padding[PADDING_BOTTOM_INDEX] + (2 * DESIRED_PADDING);
		Math::Real quadWidth = width * m_horizontalPerPixelSize;
		Math::Real quadHeight = height * m_verticalPerPixelSize;
		Math::Real xTextureSize = static_cast<Math::Real>(width) / imageWidth;
		Math::Real yTextureSize = static_cast<Math::Real>(height) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "xoffset");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"xoffset\" attribute found in the font meta data file");
		Math::Real xOffset = static_cast<Math::Real>(Utility::StringUtility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_LEFT_INDEX] - DESIRED_PADDING) * m_horizontalPerPixelSize;

		tokenItr = std::find(tokens.begin(), tokens.end(), "yoffset");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"yoffset\" attribute found in the font meta data file");
		Math::Real yOffset = (Utility::StringUtility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_TOP_INDEX] - DESIRED_PADDING) * m_verticalPerPixelSize;
		DEBUG_LOG_RENDERING("New character ID = ", id, " ='", id, "')\n\ttexCoord = ", Math::Vector2D(xTextureCoord, yTextureCoord), ",\n\ttexSize = ",
			Math::Vector2D(xTextureSize, yTextureSize), ",\n\toffset = ", Math::Vector2D(xOffset, yOffset), ",\n\tquadSize = ",
			Math::Vector2D(quadWidth, quadHeight), ",\n\txAdvance = ", xAdvance);
		m_metaData.insert(std::pair<int, Character>(id, Character(id, Math::Vector2D(xTextureCoord, yTextureCoord), Math::Vector2D(xTextureSize, yTextureSize), Math::Vector2D(xOffset, yOffset), Math::Vector2D(quadWidth, quadHeight), xAdvance)));
		//m_metaData[id] = Character(id, Math::Vector2D(xTextureCoord, yTextureCoord), Math::Vector2D(xTextureSize, yTextureSize), Math::Vector2D(xOffset, yOffset), Math::Vector2D(quadWidth, quadHeight), xAdvance);
	}
}

const Rendering::Text::Character& Rendering::Text::Font::GetCharacter(int asciiCode) const
{
	std::map<int, Character>::const_iterator characterItr = m_metaData.find(asciiCode);
	CHECK_CONDITION_RENDERING(characterItr != m_metaData.end(), Utility::Logging::ERR,
		"The ascii code (", asciiCode, ") hasn't been found in the font meta data");
	return characterItr->second;
}

void Rendering::Text::Font::Bind(const Math::Vector2D& translation, const Math::Vector2D& offset, const Color& color, const Color& outlineColor,
	Math::Real characterWidth, Math::Real characterEdgeTransitionWidth, Math::Real borderWidth, Math::Real borderEdgeTransitionWidth) const
{
	m_shader->Bind();
	m_shader->SetUniformVector2D("translation", translation);
	m_shader->SetUniformVector2D("offset", offset);
	m_shader->SetUniformColor("textColor", color);
	m_shader->SetUniformColor("outlineColor", outlineColor);
	m_shader->SetUniformf("characterWidth", characterWidth);
	m_shader->SetUniformf("characterEdgeTransitionDistance", characterEdgeTransitionWidth);
	m_shader->SetUniformf("borderWidth", borderWidth);
	m_shader->SetUniformf("borderEdgeTransitionDistance", borderEdgeTransitionWidth);
	m_shader->SetUniformi("fontAtlas", 0);
	m_textureAtlas.Bind(0);
}