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
	m_aspectRatio = static_cast<math::Real>(1600) / 900; // TODO: Calculate aspect ratio accordingly. Do not use hard-coded size here.
	//m_aspectRatio = static_cast<math::Real>(900) / 1600; // TODO: Calculate aspect ratio accordingly. Do not use hard-coded size here.
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
		CHECK_CONDITION_EXIT_ALWAYS_RENDERING(!file.bad() && !file.eof() && !file.fail(), utility::logging::EMERGENCY,
			"Error occured in the stream while reading the font meta data file \"", fontMetaDataFileName, "\"");
		std::getline(file, line);
		CHECK_CONDITION_EXIT_ALWAYS_RENDERING(!file.bad(), utility::logging::EMERGENCY,
			"Error occured in the stream while reading the font meta data file \"", fontMetaDataFileName, "\"");
		if (!line.empty())
		{
			std::vector<std::string> tokens;
			utility::string_utility::CutToTokens(line, tokens, META_DATA_SPLITTER, 2);
			if (tokens[0].compare("info") == 0)
			{
				tokens.erase(tokens.begin()); // Removing the first token
				// Padding calculation begin
				std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "padding");
				CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
					"No \"padding\" attribute found in the font meta data file \"", fontMetaDataFileName, "\"");
				const std::string padding = *(tokenItr + 1);
				std::vector<std::string> subTokens;
				utility::string_utility::CutToTokens(padding, subTokens, NUMBER_SEPARATOR);
				CHECK_CONDITION_RENDERING(subTokens.size() == 4, Utility::Logging::ERR, "The \"padding\" attribute must define 4 values instead of ", subTokens.size());
				for (size_t index = 0; index < subTokens.size(); ++index)
				{
					m_padding[index] = utility::string_utility::ToInt(subTokens[index]);
				}
				DELOCUST_LOG_RENDERING("Padding left = ", m_padding[PADDING_LEFT_INDEX], "; top = ", m_padding[PADDING_TOP_INDEX],
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
				int lineHeightPixels = utility::string_utility::ToInt(*(tokenItr + 1)) - m_padding[PADDING_TOP_INDEX] - m_padding[PADDING_BOTTOM_INDEX];
				m_verticalPerPixelSize = LINE_HEIGHT / lineHeightPixels;
				m_horizontalPerPixelSize = m_verticalPerPixelSize / m_aspectRatio;
				DELOCUST_LOG_RENDERING("Per pixel sizes: horizontal = ", m_horizontalPerPixelSize, ", vertical = ", m_verticalPerPixelSize);
				// Line sizes calculation end

				tokenItr = std::find(tokens.begin(), tokens.end(), "scaleW");
				CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
					"No \"scaleW\" attribute found in the font meta data file \"", fontMetaDataFileName, "\"");
				imageWidth = utility::string_utility::ToInt(*(tokenItr + 1));
				DELOCUST_LOG_RENDERING("Image width = ", imageWidth);
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
	int id = utility::string_utility::ToInt(*(tokenItr + 1));

	tokenItr = std::find(tokens.begin(), tokens.end(), "xadvance");
	CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
		"No \"xadvance\" attribute found in the font meta data file");
	const int metaDataXAdvance = utility::string_utility::ToInt(*(tokenItr + 1));
	math::Real xAdvance = static_cast<math::Real>(metaDataXAdvance - m_padding[PADDING_LEFT_INDEX] - m_padding[PADDING_RIGHT_INDEX]) * m_horizontalPerPixelSize;

	if (id == SPACE_ASCII_CODE)
	{
		m_spaceWidth = xAdvance;
	}
	else
	{
		tokenItr = std::find(tokens.begin(), tokens.end(), "x");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"x\" attribute found in the font meta data file");
		math::Real xTextureCoord = static_cast<math::Real>(utility::string_utility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_LEFT_INDEX] - DESIRED_PADDING) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "y");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"y\" attribute found in the font meta data file");
		math::Real yTextureCoord = static_cast<math::Real>(utility::string_utility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_TOP_INDEX] - DESIRED_PADDING) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "width");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"width\" attribute found in the font meta data file");
		int width = utility::string_utility::ToInt(*(tokenItr + 1)) - m_padding[PADDING_LEFT_INDEX] - m_padding[PADDING_RIGHT_INDEX] + (2 * DESIRED_PADDING);

		tokenItr = std::find(tokens.begin(), tokens.end(), "height");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"height\" attribute found in the font meta data file");
		int height = utility::string_utility::ToInt(*(tokenItr + 1)) - m_padding[PADDING_TOP_INDEX] - m_padding[PADDING_BOTTOM_INDEX] + (2 * DESIRED_PADDING);
		math::Real quadWidth = width * m_horizontalPerPixelSize;
		math::Real quadHeight = height * m_verticalPerPixelSize;
		math::Real xTextureSize = static_cast<math::Real>(width) / imageWidth;
		math::Real yTextureSize = static_cast<math::Real>(height) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "xoffset");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"xoffset\" attribute found in the font meta data file");
		math::Real xOffset = static_cast<math::Real>(utility::string_utility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_LEFT_INDEX] - DESIRED_PADDING) * m_horizontalPerPixelSize;

		tokenItr = std::find(tokens.begin(), tokens.end(), "yoffset");
		CHECK_CONDITION_RENDERING(tokenItr != tokens.end(), Utility::Logging::ERR,
			"No \"yoffset\" attribute found in the font meta data file");
		math::Real yOffset = (utility::string_utility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_TOP_INDEX] - DESIRED_PADDING) * m_verticalPerPixelSize;
		DELOCUST_LOG_RENDERING("New character ID = ", id, " ='", id, "')\n\ttexCoord = ", math::Vector2D(xTextureCoord, yTextureCoord), ",\n\ttexSize = ",
			math::Vector2D(xTextureSize, yTextureSize), ",\n\toffset = ", math::Vector2D(xOffset, yOffset), ",\n\tquadSize = ",
			math::Vector2D(quadWidth, quadHeight), ",\n\txAdvance = ", xAdvance);
		m_metaData.insert(std::pair<int, Character>(id, Character(id, math::Vector2D(xTextureCoord, yTextureCoord), math::Vector2D(xTextureSize, yTextureSize), math::Vector2D(xOffset, yOffset), math::Vector2D(quadWidth, quadHeight), xAdvance)));
		//m_metaData[id] = Character(id, math::Vector2D(xTextureCoord, yTextureCoord), math::Vector2D(xTextureSize, yTextureSize), math::Vector2D(xOffset, yOffset), math::Vector2D(quadWidth, quadHeight), xAdvance);
	}
}

const Rendering::Text::Character& Rendering::Text::Font::GetCharacter(int asciiCode) const
{
	std::map<int, Character>::const_iterator characterItr = m_metaData.find(asciiCode);
	CHECK_CONDITION_RENDERING(characterItr != m_metaData.end(), Utility::Logging::ERR,
		"The ascii code (", asciiCode, ") hasn't been found in the font meta data");
	return characterItr->second;
}

void Rendering::Text::Font::Bind(const math::Vector2D& translation, const math::Vector2D& offset, const Color& color, const Color& outlineColor,
	math::Real characterWidth, math::Real characterEdgeTransitionWidth, math::Real borderWidth, math::Real borderEdgeTransitionWidth) const
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