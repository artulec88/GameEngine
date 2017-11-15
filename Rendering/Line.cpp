#include "stdafx.h"
#include "Line.h"


Rendering::Text::Line::Line(math::Real spaceWidth, math::Real fontSize, math::Real maxLength) :
	m_spaceSize(spaceWidth * fontSize),
	m_maxLength(maxLength),
	m_currentLineLength(REAL_ZERO)
{
}


Rendering::Text::Line::~Line()
{
}

bool Rendering::Text::Line::AttemptToAddWord(const Word& word)
{
	math::Real additionalLength = word.GetWordWidth();
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