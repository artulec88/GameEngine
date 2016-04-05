#include "StdAfx.h"
#include "Font.h"
#include "Utility\StringUtility.h"
#include "Utility\ILogger.h"
#include <fstream>
#include <algorithm> // std::find

/* static */ const Math::Real Rendering::Text::Font::LINE_HEIGHT = 0.03f;
/* static */ const int Rendering::Text::Font::SPACE_ASCII_CODE = 32;
/* static */ const int Rendering::Text::Font::PADDING_TOP_INDEX = 0;
/* static */ const int Rendering::Text::Font::PADDING_LEFT_INDEX = 1;
/* static */ const int Rendering::Text::Font::PADDING_BOTTOM_INDEX = 2;
/* static */ const int Rendering::Text::Font::PADDING_RIGHT_INDEX = 3;
/* static */ const int Rendering::Text::Font::DESIRED_PADDING = 3;

/* static */ const char Rendering::Text::Font::META_DATA_SPLITTER[2] = { ' ', '=' };
/* static */ const char Rendering::Text::Font::NUMBER_SEPARATOR = ',';

Rendering::Text::Font::Font(const std::string& fontTextureAtlasFileName, const std::string& fontMetaDataFileName) :
	m_textureAtlas(fontTextureAtlasFileName),
	m_metaDataFileName(fontMetaDataFileName)
{
	//m_textureAtlas = new Texture(fontTextureAtlasFileName);
	m_aspectRatio = static_cast<Math::Real>(1600) / 900; // TODO: Calculate aspect ratio accordingly. Do not use hard-coded size here.
	ReadMetaDataFile("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Fonts\\" + fontMetaDataFileName);
}

Rendering::Text::Font::~Font()
{
	//SAFE_DELETE(m_textureAtlas);
}

void Rendering::Text::Font::ReadMetaDataFile(const std::string& fontMetaDataFileName)
{
	// TODO: Implement font meta data file parsing
	std::ifstream file(fontMetaDataFileName.c_str());
	CHECK_CONDITION_RETURN_VOID(file.is_open(), Utility::Error, "Could not open font meta data file \"%s\"", fontMetaDataFileName.c_str());

	int imageWidth;
	std::string line;
	while (!file.eof())
	{
		CHECK_CONDITION_EXIT(!file.fail(), Utility::Emergency,
			"Error occured in the stream while reading the font meta data file \"%s\"", fontMetaDataFileName.c_str());
		std::getline(file, line);
		//Utility::StringUtility::RightTrim(line);
		std::vector<std::string> tokens;
		Utility::StringUtility::CutToTokens(line, tokens, META_DATA_SPLITTER, 2);

		if (tokens.empty())
		{
			continue;
		}
		if (tokens[0].compare("info") == 0)
		{
			tokens.erase(tokens.begin()); // Removing the first token
			// Padding calculation begin
			std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "padding");
			CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
				"No \"padding\" attribute found in the font meta data file \"%s\"", fontMetaDataFileName);
			const std::string padding = *(tokenItr + 1);
			std::vector<std::string> subTokens;
			Utility::StringUtility::CutToTokens(padding, subTokens, NUMBER_SEPARATOR);
			CHECK_CONDITION(subTokens.size() == 4, Utility::Error, "The \"padding\" attribute must define 4 values instead of %d", subTokens.size());
			for (size_t index = 0; index < subTokens.size(); ++index)
			{
				m_padding[index] = Utility::StringUtility::ToInt(subTokens[index]);
			}
			// Padding calculation end
		}
		else if (tokens[0].compare("common") == 0)
		{
			tokens.erase(tokens.begin()); // Removing the first token
			// Line sizes calculation begin
			std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "lineHeight");
			CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
				"No \"lineHeight\" attribute found in the font meta data file \"%s\"", fontMetaDataFileName);
			const std::string lineHeight = *(tokenItr + 1);
			int lineHeightPixels = Utility::StringUtility::ToInt(lineHeight) - m_padding[PADDING_TOP_INDEX] - m_padding[PADDING_BOTTOM_INDEX];
			m_verticalPerPixelSize = LINE_HEIGHT / lineHeightPixels;
			m_horizontalPerPixelSize = m_verticalPerPixelSize / m_aspectRatio;
			// Line sizes calculation end

			tokenItr = std::find(tokens.begin(), tokens.end(), "scaleW");
			CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
				"No \"scaleW\" attribute found in the font meta data file \"%s\"", fontMetaDataFileName);
			imageWidth = Utility::StringUtility::ToInt(*(tokenItr + 1));
		}
		else if (tokens[0].compare("char") == 0)
		{
			tokens.erase(tokens.begin()); // Removing the first token
			AddCharacter(tokens, imageWidth); // Creating a character
		}
	}
	file.close();
}

void Rendering::Text::Font::AddCharacter(std::vector<std::string>& tokens, int imageWidth)
{
	std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "id");
	CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
		"No \"id\" attribute found in the font meta data file");
	int id = Utility::StringUtility::ToInt(*(tokenItr + 1));

	tokenItr = std::find(tokens.begin(), tokens.end(), "xadvance");
	CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
		"No \"xadvance\" attribute found in the font meta data file");
	Math::Real xAdvance = (Utility::StringUtility::ToInt(*(tokenItr + 1)) - m_padding[PADDING_LEFT_INDEX] - m_padding[PADDING_RIGHT_INDEX]) * m_horizontalPerPixelSize;

	if (id == SPACE_ASCII_CODE)
	{
		m_spaceWidth = xAdvance;
	}
	else
	{
		tokenItr = std::find(tokens.begin(), tokens.end(), "x");
		CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
			"No \"x\" attribute found in the font meta data file");
		Math::Real xTextureCoord = static_cast<Math::Real>(Utility::StringUtility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_LEFT_INDEX] - DESIRED_PADDING) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "y");
		CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
			"No \"y\" attribute found in the font meta data file");
		Math::Real yTextureCoord = static_cast<Math::Real>(Utility::StringUtility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_TOP_INDEX] - DESIRED_PADDING) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "width");
		CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
			"No \"width\" attribute found in the font meta data file");
		int width = Utility::StringUtility::ToInt(*(tokenItr + 1)) - m_padding[PADDING_LEFT_INDEX] - m_padding[PADDING_RIGHT_INDEX] + (2.0f * DESIRED_PADDING);

		tokenItr = std::find(tokens.begin(), tokens.end(), "height");
		CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
			"No \"height\" attribute found in the font meta data file");
		int height = Utility::StringUtility::ToInt(*(tokenItr + 1)) - m_padding[PADDING_TOP_INDEX] - m_padding[PADDING_BOTTOM_INDEX] + (2.0f * DESIRED_PADDING);
		Math::Real quadWidth = width * m_horizontalPerPixelSize;
		Math::Real quadHeight = height * m_verticalPerPixelSize;
		Math::Real xTextureSize = static_cast<Math::Real>(width) / imageWidth;
		Math::Real yTextureSize = static_cast<Math::Real>(height) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "xoffset");
		CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
			"No \"xoffset\" attribute found in the font meta data file");
		Math::Real xOffset = (Utility::StringUtility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_LEFT_INDEX] - DESIRED_PADDING) * m_horizontalPerPixelSize;

		tokenItr = std::find(tokens.begin(), tokens.end(), "yoffset");
		CHECK_CONDITION(tokenItr != tokens.end(), Utility::Error,
			"No \"yoffset\" attribute found in the font meta data file");
		Math::Real yOffset = (Utility::StringUtility::ToInt(*(tokenItr + 1)) + m_padding[PADDING_TOP_INDEX] - DESIRED_PADDING) * m_verticalPerPixelSize;
		m_metaData.insert(std::pair<int, Character>(id, Character(id, Math::Vector2D(xTextureCoord, yTextureCoord), Math::Vector2D(xTextureSize, yTextureSize), Math::Vector2D(xOffset, yOffset), Math::Vector2D(quadWidth, quadHeight), xAdvance)));
		//m_metaData[id] = Character(id, Math::Vector2D(xTextureCoord, yTextureCoord), Math::Vector2D(xTextureSize, yTextureSize), Math::Vector2D(xOffset, yOffset), Math::Vector2D(quadWidth, quadHeight), xAdvance);
	}
}

const Rendering::Text::Character& Rendering::Text::Font::GetCharacter(int asciiCode) const
{
	std::map<int, Character>::const_iterator characterItr = m_metaData.find(asciiCode);
	CHECK_CONDITION(characterItr != m_metaData.end(), Utility::Error,
		"The ascii code (%d) hasn't been found in the font meta data", asciiCode);
	return characterItr->second;
}