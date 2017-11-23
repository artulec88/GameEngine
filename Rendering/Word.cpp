#include "stdafx.h"
#include "Word.h"
#include <sstream>

rendering::text::Word::Word(math::Real fontSize) :
	m_width(REAL_ZERO),
	m_fontSize(fontSize)
{
}


rendering::text::Word::~Word()
{
}

void rendering::text::Word::AddCharacter(const Character& character)
{
	m_characters.push_back(character);
	m_width += character.GetXAdvance() * m_fontSize;
}

void rendering::text::Word::Clear()
{
	m_width = REAL_ZERO;
	m_characters.clear();
}
