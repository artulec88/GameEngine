#include "stdafx.h"
#include "FontFactory.h"
#include "Utility/ILogger.h"

rendering::text::FontFactory::FontFactory(const Shader* textShader, const std::string& texturesDirectory, const std::string& fontsDirectory) :
	m_textShader(textShader),
	m_fontType2FontMap(),
	m_texturesDirectory(texturesDirectory),
	m_fontsDirectory(fontsDirectory)
	// TODO: If we initialize the map this way then Font's copy ctrs will be used (instead of move ctrs). Google that!
	//m_fontsMap({ { font_ids::CAMBRIA, std::move(Font(/*params*/) } )
{
	m_fontType2FontMap.emplace(font_ids::CAMBRIA, Font(m_textShader, m_texturesDirectory + "cambria.png", m_fontsDirectory + "cambria.fnt"));
	m_fontType2FontMap.emplace(font_ids::CANDARA, Font(m_textShader, m_texturesDirectory + "candara.png", m_fontsDirectory + "candara.fnt"));
	m_fontType2FontMap.emplace(font_ids::SEGOE, Font(m_textShader, m_texturesDirectory + "segoe.png", m_fontsDirectory + "segoe.fnt"));
}


rendering::text::FontFactory::~FontFactory()
{
}

const rendering::text::Font* rendering::text::FontFactory::CreateFont(int fontId, const std::string& fontTextureFileName, const std::string& fontMetaDataFileName)
{
	const auto fontPair = m_fontType2FontMap.insert(std::make_pair(fontId, Font(m_textShader, m_texturesDirectory + fontTextureFileName, m_fontsDirectory + fontMetaDataFileName)));
	CHECK_CONDITION_RENDERING(fontPair.second, Utility::Logging::WARNING, "Font \"", fontTextureFileName, "\" has already been created.");
	return &fontPair.first->second;
}

const rendering::text::Font* rendering::text::FontFactory::GetFont(int fontId) const
{
	DEBUG_LOG_RENDERING("Retrieving font for font ID: ", fontId);
	CHECK_CONDITION_EXIT_RENDERING(m_fontType2FontMap.find(fontId) != m_fontType2FontMap.end(), Utility::Logging::EMERGENCY,
		"No font has been created for the specified type of font (", fontId, ").");
	return &m_fontType2FontMap.find(fontId)->second;
}