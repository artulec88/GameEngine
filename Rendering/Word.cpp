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
