#include "stdafx.h"
#include "Word.h"
#include <sstream>

Rendering::Text::Word::Word(Math::Real fontSize) :
	m_width(REAL_ZERO),
	m_fontSize(fontSize)
{
}


Rendering::Text::Word::~Word()
{
}

void Rendering::Text::Word::AddCharacter(const Character& character)
{
	m_characters.push_back(character);
	m_width += character.GetXAdvance() * m_fontSize;
}

void Rendering::Text::Word::Clear()
{
	m_width = REAL_ZERO;
	m_characters.clear();
}

std::string Rendering::Text::Word::ToString() const
{
	std::stringstream ss("");
	for (std::vector<Character>::const_iterator charItr = m_characters.begin(); charItr != m_characters.end(); ++charItr)
	{
		ss << charItr->GetAsciiCode() << ",";
	}
	return ss.str();
}