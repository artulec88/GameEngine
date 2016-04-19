#include "stdafx.h"
#include "GuiText.h"
#include <sstream>

Rendering::Text::GuiText::Word::Word(Math::Real fontSize) :
	m_width(REAL_ZERO),
	m_wordFontSize(fontSize)
{
}

Rendering::Text::GuiText::Word::~Word(void)
{
}

std::string Rendering::Text::GuiText::Word::ToString() const
{
	std::stringstream ss("");
	for (std::vector<Character>::const_iterator charItr = m_characters.begin(); charItr != m_characters.end(); ++charItr)
	{
		ss << charItr->GetAsciiCode() << ",";
	}
	return ss.str();
}

Rendering::Text::GuiText::Line::Line(Math::Real spaceWidth, Math::Real fontSize, Math::Real maxLength) :
	m_spaceSize(spaceWidth * fontSize),
	m_maxLength(maxLength),
	m_currentLineLength(REAL_ZERO)
{
}

Rendering::Text::GuiText::Line::~Line(void)
{
}

bool Rendering::Text::GuiText::Line::AttemptToAddWord(const Word& word)
{
	Math::Real additionalLength = word.GetWordWidth();
	if (!m_words.empty())
	{
		additionalLength += m_spaceSize;
	}
	if (m_currentLineLength + additionalLength > m_maxLength)
	{
		return false;
	}
	m_words.push_back(word);
	m_currentLineLength += additionalLength;
	return true;
}


Rendering::Text::GuiText::GuiText(const std::string& text, const Font* font, Math::Real fontSize, const Math::Vector2D& screenPosition, Math::Real maxLineLength,
	Effects::ColorEffect* textColorEffect, const Math::Vector2D& offset, const Math::Vector3D& outlineColor, bool isCentered /* = false */,
	Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */, Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	m_text(text),
	m_font(font),
	m_fontSize(fontSize),
	m_colorEffect(textColorEffect),
	m_screenPosition(screenPosition),
	m_maxLineLength(maxLineLength),
	m_linesCount(0),
	m_isCentered(isCentered),
	m_offset(offset),
	m_outlineColor(outlineColor),
	m_characterWidth(characterWidth),
	m_characterEdgeTransitionWidth(characterEdgeTransitionWidth),
	m_borderWidth(borderWidth),
	m_borderEdgeTransitionWidth(borderEdgeTransitionWidth),
	m_mesh(NULL),
	m_aabr(Math::Vector2D(REAL_ZERO, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO))
{
	if (!text.empty())
	{
		SetText(text);
	}
}


Rendering::Text::GuiText::~GuiText()
{
	SAFE_DELETE(m_mesh);
	//SAFE_DELETE(m_colorEffect);
}

void Rendering::Text::GuiText::SetText(const std::string& text)
{
	std::vector<Line> lines;
	Line currentLine(m_font->GetSpaceWidth(), m_fontSize, m_maxLineLength);
	Word currentWord(m_fontSize);
	bool added;
	for (std::string::const_iterator textItr = text.begin(); textItr != text.end(); ++textItr)
	{
		const int asciiCode = static_cast<int>((*textItr));
		if (asciiCode == Font::SPACE_ASCII_CODE)
		{
			added = currentLine.AttemptToAddWord(currentWord);
			if (!added)
			{
				lines.push_back(currentLine);
				currentLine.Reset();
				currentLine.AttemptToAddWord(currentWord);
			}
			currentWord.Reset();
			continue;
		}
		currentWord.AddCharacter(m_font->GetCharacter(asciiCode));
	}
	// Completing the lines structure
	added = currentLine.AttemptToAddWord(currentWord);
	if (!added)
	{
		lines.push_back(currentLine);
		currentLine.Reset();
		currentLine.AttemptToAddWord(currentWord);
	}
	lines.push_back(currentLine);

	// Creating quad vertices based on the lines vector
	m_linesCount = lines.size();
	Math::Real cursorX = REAL_ZERO;
	Math::Real cursorY = REAL_ZERO;
	std::vector<Math::Vector2D> positions;
	std::vector<Math::Vector2D> textureCoordinates;
	for (std::vector<Line>::const_iterator lineItr = lines.begin(); lineItr != lines.end(); ++lineItr)
	{
		if (m_isCentered)
		{
			cursorX = (m_isCentered) ? (m_maxLineLength - lineItr->GetLineLength()) / 2.0f : REAL_ZERO;
		}
		for (std::vector<Word>::const_iterator wordItr = lineItr->GetWords().begin(); wordItr != lineItr->GetWords().end(); ++wordItr)
		{
			for (std::vector<Character>::const_iterator characterItr = wordItr->GetCharacters().begin(); characterItr != wordItr->GetCharacters().end(); ++characterItr)
			{
				// Adding positions
				const Math::Real x = cursorX + (characterItr->GetOffset().GetX() * m_fontSize);
				const Math::Real y = cursorY + (characterItr->GetOffset().GetY() * m_fontSize);
				const Math::Real maxX = x + (characterItr->GetSize().GetX() * m_fontSize);
				const Math::Real maxY = y + (characterItr->GetSize().GetY() * m_fontSize);
				const Math::Real properX = (2.0f * x) - 1.0f;
				const Math::Real properY = (-2.0f * y) + 1.0f;
				const Math::Real properMaxX = (2.0f * maxX) - 1.0f;
				const Math::Real properMaxY = (-2.0f * maxY) + 1.0f;

				//CRITICAL_LOG("%d)\n\t(x, y) = (%.3f, %.3f);\n\t(maxX,maxY) = (%.3f, %.3f);\n\t(properX, properY) = (%.3f, %.3f);\n\t(properMaxX, properMaxY) = (%.3f, %.3f);",
				//	tempCount++, x, y, maxX, maxY, properX, properY, properMaxX, properMaxY);

				positions.emplace_back(properX, properY); textureCoordinates.push_back(characterItr->GetTextureCoords()); // 0
				positions.emplace_back(properMaxX, properY); textureCoordinates.emplace_back(characterItr->GetMaxTextureCoords().GetX(), characterItr->GetTextureCoords().GetY()); // 4
				positions.emplace_back(properMaxX, properMaxY); textureCoordinates.push_back(characterItr->GetMaxTextureCoords()); // 2
				positions.emplace_back(properMaxX, properMaxY); textureCoordinates.push_back(characterItr->GetMaxTextureCoords()); // 2
				positions.emplace_back(properX, properMaxY); textureCoordinates.emplace_back(characterItr->GetTextureCoords().GetX(), characterItr->GetMaxTextureCoords().GetY()); // 1
				positions.emplace_back(properX, properY); textureCoordinates.push_back(characterItr->GetTextureCoords()); // 0

				cursorX += characterItr->GetXAdvance() * m_fontSize;
			}
			cursorX += m_font->GetSpaceWidth() * m_fontSize;
		}
		cursorY += Font::LINE_HEIGHT * m_fontSize;
	}
	// TODO: Screen width and height are hard-coded here.
	m_aabr.SetBottomLeftPos(Math::Vector2D(m_screenPosition.GetX() * 1600.0f, (m_screenPosition.GetY() + cursorY) * 900.0f));
	m_aabr.SetTopRightPos(Math::Vector2D((m_screenPosition.GetX() + cursorX) * 1600.0f, m_screenPosition.GetY() * 900.0f));
	if (m_mesh == NULL)
	{
		m_mesh = new TextMesh(&positions[0], &textureCoordinates[0], positions.size());
	}
	else
	{
		m_mesh->ReplaceData(&positions[0], &textureCoordinates[0], positions.size());
	}
}

void Rendering::Text::GuiText::Draw() const
{
	m_mesh->Draw();
}