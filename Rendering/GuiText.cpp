#include "stdafx.h"
#include "GuiText.h"
#include "Vertex.h"
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
	m_maxLength(maxLength)
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


Rendering::Text::GuiText::GuiText(const std::string& text, const Font* font, Math::Real fontSize, const Math::Vector2D& screenPosition, Math::Real maxLineLength, bool isCentered /* = false */) :
	m_text(text),
	m_font(font),
	m_fontSize(fontSize),
	m_color(REAL_ONE, REAL_ZERO, REAL_ZERO),
	m_screenPosition(screenPosition),
	m_maxLineLength(maxLineLength),
	m_linesCount(0),
	m_isCentered(isCentered),
	m_mesh(NULL)
{
	const char* chars = text.c_str();
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
	std::vector<Vertex2D> vertices;
	for (std::vector<Line>::const_iterator lineItr = lines.begin(); lineItr != lines.end(); ++lineItr)
	{
		if (m_isCentered)
		{
			cursorX = (m_maxLineLength - lineItr->GetLineLength()) / 2.0f;
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

				//vertices.push_back(Vertex2D(Math::Vector2D(properX, properY), characterItr->GetTextureCoords())); // 0
				//vertices.push_back(Vertex2D(Math::Vector2D(properX, properMaxY), Math::Vector2D(characterItr->GetTextureCoords().GetX(), characterItr->GetMaxTextureCoords().GetY()))); // 1
				//vertices.push_back(Vertex2D(Math::Vector2D(properMaxX, properMaxY), characterItr->GetMaxTextureCoords())); // 2
				//vertices.push_back(Vertex2D(Math::Vector2D(properMaxX, properMaxY), characterItr->GetMaxTextureCoords())); // 3
				//vertices.push_back(Vertex2D(Math::Vector2D(properMaxX, properY), Math::Vector2D(characterItr->GetMaxTextureCoords().GetX(), characterItr->GetTextureCoords().GetY()))); // 4
				//vertices.push_back(Vertex2D(Math::Vector2D(properX, properY), characterItr->GetTextureCoords())); // 5

				//vertices.push_back(Vertex2D(Math::Vector2D(properX, properY), characterItr->GetTextureCoords())); // 0
				//vertices.push_back(Vertex2D(Math::Vector2D(properX, properMaxY), Math::Vector2D(characterItr->GetTextureCoords().GetX(), characterItr->GetMaxTextureCoords().GetY()))); // 1
				//vertices.push_back(Vertex2D(Math::Vector2D(properMaxX, properY), Math::Vector2D(characterItr->GetMaxTextureCoords().GetX(), characterItr->GetTextureCoords().GetY()))); // 4
				//vertices.push_back(Vertex2D(Math::Vector2D(properMaxX, properMaxY), characterItr->GetMaxTextureCoords())); // 2
				//vertices.push_back(Vertex2D(Math::Vector2D(properMaxX, properY), Math::Vector2D(characterItr->GetMaxTextureCoords().GetX(), characterItr->GetTextureCoords().GetY()))); // 4
				//vertices.push_back(Vertex2D(Math::Vector2D(properX, properMaxY), Math::Vector2D(characterItr->GetTextureCoords().GetX(), characterItr->GetMaxTextureCoords().GetY()))); // 1

				vertices.push_back(Vertex2D(Math::Vector2D(properX, properY), characterItr->GetTextureCoords())); // 0
				vertices.push_back(Vertex2D(Math::Vector2D(properMaxX, properY), Math::Vector2D(characterItr->GetMaxTextureCoords().GetX(), characterItr->GetTextureCoords().GetY()))); // 4
				vertices.push_back(Vertex2D(Math::Vector2D(properMaxX, properMaxY), characterItr->GetMaxTextureCoords())); // 2
				vertices.push_back(Vertex2D(Math::Vector2D(properMaxX, properMaxY), characterItr->GetMaxTextureCoords())); // 2
				vertices.push_back(Vertex2D(Math::Vector2D(properX, properMaxY), Math::Vector2D(characterItr->GetTextureCoords().GetX(), characterItr->GetMaxTextureCoords().GetY()))); // 1
				vertices.push_back(Vertex2D(Math::Vector2D(properX, properY), characterItr->GetTextureCoords())); // 0

				cursorX += m_font->GetSpaceWidth() * m_fontSize;
			}
			cursorX += m_font->GetSpaceWidth() * m_fontSize;
		}
		cursorX = REAL_ZERO;
		cursorY += Font::LINE_HEIGHT * m_fontSize;
	}
	int tempCounter = 0;
	for (std::vector<Vertex2D>::const_iterator vertexItr = vertices.begin(); vertexItr != vertices.end(); ++vertexItr, ++tempCounter)
	{
		ERROR_LOG("%d) vertexItr.pos = %s; vertexItr.texCoord = %s", tempCounter, vertexItr->m_pos.ToString().c_str(), vertexItr->m_texCoord.ToString().c_str());
	}
	m_mesh = new TextMesh(&vertices[0], vertices.size());
}


Rendering::Text::GuiText::~GuiText()
{
	SAFE_DELETE(m_mesh);
}

void Rendering::Text::GuiText::Draw() const
{
	m_font->Bind();
	m_mesh->Draw();
}