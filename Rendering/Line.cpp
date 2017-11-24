#include "stdafx.h"
#include "Line.h"

bool rendering::text::Line::AttemptToAddWord(const Word& word)
{
	const auto additionalLength = word.GetWordWidth() + m_words.empty() ? REAL_ZERO : m_spaceSize;
	if (m_currentLineLength + additionalLength > m_maxLength)
	{
		return false;
	}
	m_words.push_back(word);
	m_currentLineLength += additionalLength;
	return true;
}