#include "StdAfx.h"
#include "Font.h"
#include "Utility\StringUtility.h"
#include "Utility\ILogger.h"
#include <fstream>
#include <algorithm> // std::find

/* static */ const Math::Real Rendering::Text::Font::LINE_HEIGHT = 0.03f;
/* static */ const int Rendering::Text::Font::SPACE_ASCII_CODE = 32;
/* static */ const int Rendering::Text::Font::PADDING_TOP = 0;
/* static */ const int Rendering::Text::Font::PADDING_LEFT = 1;
/* static */ const int Rendering::Text::Font::PADDING_BOTTOM = 2;
/* static */ const int Rendering::Text::Font::PADDING_RIGHT = 3;
/* static */ const int Rendering::Text::Font::DESIRED_PADDING = 3;

/* static */ const char Rendering::Text::Font::META_DATA_SPLITTER[2] = { ' ', '=' };
/* static */ const char Rendering::Text::Font::NUMBER_SEPARATOR = ',';

Rendering::Text::Font::Font(const std::string& fontTextureAtlasFileName, const std::string& fontMetaDataFileName) :
	m_textureAtlas(fontTextureAtlasFileName, GL_TEXTURE_2D)
{
	m_aspectRatio = static_cast<Math::Real>(1600) / 900; // TODO: Calculate aspect ratio accordingly. Do not use hard-coded size here.
	ReadMetaDataFile("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Fonts\\" + fontMetaDataFileName);
}

Rendering::Text::Font::~Font()
{
}

void Rendering::Text::Font::ReadMetaDataFile(const std::string& fontMetaDataFileName)
{
	// TODO: Implement font meta data file parsing
	std::ifstream file(fontMetaDataFileName.c_str());
	if (!file.is_open())
	{
		ERROR_LOG("Could not open font meta data file \"%s\"", fontMetaDataFileName.c_str());
		return;
	}

	int imageWidth;
	std::string line;
	int tempLineCount = 1;
	while (!file.eof())
	{
		if (file.fail())
		{
			EMERGENCY_LOG("Error occured in the stream while reading the font meta data file \"%s\"", fontMetaDataFileName.c_str());
			exit(EXIT_FAILURE);
		}
		std::getline(file, line);
		//Utility::StringUtility::RightTrim(line);
		DELOCUST_LOG("Font meta data file: line after trimming = \"%s\"", tempLineCount, line.c_str());
		std::vector<std::string> tokens;
		Utility::CutToTokens(line, tokens, META_DATA_SPLITTER, 2);
		//for (std::vector<std::string>::const_iterator tokenItr = tokens.begin(); tokenItr != tokens.end() && (tempLineCount < 8); ++tokenItr)
		//{
		//	EMERGENCY_LOG("tempLineCount = %d) Token = \"%s\"", tempLineCount, tokenItr->c_str());
		//}
		++tempLineCount;

		if (tokens.empty())
		{
			continue;
		}
		if (tokens[0].compare("info") == 0)
		{
			tokens.erase(tokens.begin()); // Removing the first token
			// Padding calculation begin
			std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "padding");
			const std::string padding = *(tokenItr + 1);
			std::vector<std::string> subTokens;
			Utility::CutToTokens(padding, subTokens, NUMBER_SEPARATOR);
			for (size_t index = 0; index < subTokens.size(); ++index)
			{
				m_padding[index] = Utility::StringUtility::ToInt(subTokens[index]);
			}
			const int paddingWidth = m_padding[PADDING_LEFT] + m_padding[PADDING_RIGHT];
			const int paddingHeight = m_padding[PADDING_TOP] + m_padding[PADDING_BOTTOM];
			// Padding calculation end
		}
		else if (tokens[0].compare("common") == 0)
		{
			tokens.erase(tokens.begin()); // Removing the first token
			// Line sizes calculation begin
			std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "lineHeight");
			const std::string lineHeight = *(tokenItr + 1);
			int lineHeightPixels = Utility::StringUtility::ToInt(lineHeight) - m_padding[PADDING_TOP] - m_padding[PADDING_BOTTOM];
			m_verticalPerPixelSize = LINE_HEIGHT / lineHeightPixels;
			m_horizontalPerPixelSize = m_verticalPerPixelSize / m_aspectRatio;
			// Line sizes calculation end

			tokenItr = std::find(tokens.begin(), tokens.end(), "scaleW");
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
	std::vector<std::string>::iterator tokenItr = std::find(tokens.begin(), tokens.end(), "id") + 1;
	int id = Utility::StringUtility::ToInt(*tokenItr);

	tokenItr = std::find(tokens.begin(), tokens.end(), "xadvance") + 1;
	Math::Real xAdvance = (Utility::StringUtility::ToInt(*tokenItr) - m_padding[PADDING_LEFT] - m_padding[PADDING_RIGHT]) * m_horizontalPerPixelSize;

	if (id == SPACE_ASCII_CODE)
	{
		m_spaceWidth = xAdvance;
	}
	else
	{
		tokenItr = std::find(tokens.begin(), tokens.end(), "x") + 1;
		Math::Real xTextureCoord = static_cast<Math::Real>(Utility::StringUtility::ToInt(*tokenItr) + m_padding[PADDING_LEFT] - DESIRED_PADDING) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "y") + 1;
		Math::Real yTextureCoord = static_cast<Math::Real>(Utility::StringUtility::ToInt(*tokenItr) + m_padding[PADDING_TOP] - DESIRED_PADDING) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "width") + 1;
		int width = Utility::StringUtility::ToInt(*tokenItr) - m_padding[PADDING_LEFT] - m_padding[PADDING_RIGHT] + (2.0f * DESIRED_PADDING);

		tokenItr = std::find(tokens.begin(), tokens.end(), "height") + 1;
		int height = Utility::StringUtility::ToInt(*tokenItr) - m_padding[PADDING_TOP] - m_padding[PADDING_BOTTOM] + (2.0f * DESIRED_PADDING);
		Math::Real quadWidth = width * m_horizontalPerPixelSize;
		Math::Real quadHeight = height * m_verticalPerPixelSize;
		Math::Real xTextureSize = static_cast<Math::Real>(width) / imageWidth;
		Math::Real yTextureSize = static_cast<Math::Real>(height) / imageWidth;

		tokenItr = std::find(tokens.begin(), tokens.end(), "xoffset") + 1;
		Math::Real xOffset = (Utility::StringUtility::ToInt(*tokenItr) + m_padding[PADDING_LEFT] - DESIRED_PADDING) * m_horizontalPerPixelSize;

		tokenItr = std::find(tokens.begin(), tokens.end(), "yoffset") + 1;
		Math::Real yOffset = (Utility::StringUtility::ToInt(*tokenItr) + m_padding[PADDING_TOP] - DESIRED_PADDING) * m_verticalPerPixelSize;
		m_metaData.insert(std::pair<int, Character>(id, Character(id, Math::Vector2D(xTextureCoord, yTextureCoord), Math::Vector2D(xTextureSize, yTextureSize), Math::Vector2D(xOffset, yOffset), Math::Vector2D(quadWidth, quadHeight), xAdvance)));
		//m_metaData[id] = Character(id, Math::Vector2D(xTextureCoord, yTextureCoord), Math::Vector2D(xTextureSize, yTextureSize), Math::Vector2D(xOffset, yOffset), Math::Vector2D(quadWidth, quadHeight), xAdvance);
	}
}

const Rendering::Text::Character& Rendering::Text::Font::GetCharacter(int asciiCode) const
{
	std::map<int, Character>::const_iterator characterItr = m_metaData.find(asciiCode);
	if (characterItr == m_metaData.end())
	{
		ERROR_LOG("The ascii code (%d) hasn't been found in the font meta data", asciiCode);
	}
	return characterItr->second;
}