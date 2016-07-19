#include "stdafx.h"
#include "FontFactory.h"
#include "Utility\ILogger.h"

Rendering::Text::FontFactory::FontFactory(const std::string& texturesDirectory, const std::string& fontsDirectory) :
	m_fontMap(),
	m_texturesDirectory(texturesDirectory),
	m_fontsDirectory(fontsDirectory)
{
}


Rendering::Text::FontFactory::~FontFactory()
{
}

const Rendering::Text::Font* Rendering::Text::FontFactory::GetFont(FontTypes::FontType fontType)
{
	DEBUG_LOG_RENDERING("Retrieving font for font type: ", fontType);
	std::map<FontTypes::FontType, Font>::const_iterator fontItr = m_fontMap.find(fontType);
	if (fontItr == m_fontMap.end())
	{
		std::pair<std::map<FontTypes::FontType, Font>::iterator, bool> itr;
		switch (fontType)
		{
		case FontTypes::CAMBRIA:
			itr = m_fontMap.emplace(std::make_pair(fontType, Font(/*m_texturesDirectory + */"cambria.png", m_fontsDirectory + "cambria.fnt")));
			break;
		case FontTypes::CANDARA:
			itr = m_fontMap.emplace(std::make_pair(fontType, Font(/*m_texturesDirectory + */"candara.png", m_fontsDirectory + "candara.fnt")));
			break;
		case FontTypes::SEGOE:
			itr = m_fontMap.emplace(std::make_pair(fontType, Font(/*m_texturesDirectory + */"segoe.png", m_fontsDirectory + "segoe.fnt")));
			break;
		default:
			EMERGENCY_LOG_RENDERING("The specified font type (", fontType, ") is not supported by the game engine.");
			return &m_fontMap.begin()->second;
		}
		CHECK_CONDITION_RENDERING(itr.second, Utility::Logging::EMERGENCY, "The font type (", fontType, ") has already existed in the font map.");
		return &itr.first->second;
	}
	return &fontItr->second;
}