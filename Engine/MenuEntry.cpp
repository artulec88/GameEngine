#include "StdAfx.h"
#include "MenuEntry.h"
#include "Math\IntersectInfo.h"
#include "Utility\IConfig.h"
#include <sstream>

Engine::MenuEntry::MenuEntry(const GameCommand& gameCommand, const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize, const Math::Vector2D& screenPosition,
	Math::Real maxLineLength, Rendering::Effects::ColorEffect* textEffectColor, const Math::Vector2D& offset, const Math::Vector3D& outlineColor,
	bool isCentered /* = false */, Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */,
	Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	m_gameCommand(gameCommand),
	m_guiText(text, font, fontSize, screenPosition, maxLineLength, textEffectColor, offset, outlineColor, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth),
	//m_aabr(Math::Vector2D(screenPosition.GetX(), screenPosition.GetY() + fontSize), Math::Vector2D(screenPosition.GetX() + (text.size() - 1) * fontSize, screenPosition.GetY())),
	//m_fontSize(fontSize),
	m_parentMenuEntry(NULL),
	m_childrenMenuEntries(),
	m_selectedMenuEntryIndex(0)
{
	DELOCUST_LOG("MenuEntry \"%s\" constructor", m_guiText.GetText().c_str());
	//DELOCUST_LOG("AABR for menu entry \"%s\" is [%s; %s]", m_text.c_str(), m_aabr.GetBottomLeftPos().ToString().c_str(), m_aabr.GetTopRightPos().ToString().c_str());
}


Engine::MenuEntry::~MenuEntry(void)
{
	DELOCUST_LOG("MenuEntry \"%s\" destructor", m_guiText.GetText().c_str());
	for (std::vector<MenuEntry*>::iterator childrenMenuEntryItr = m_childrenMenuEntries.begin(); childrenMenuEntryItr != m_childrenMenuEntries.end(); ++childrenMenuEntryItr)
	{
		SAFE_DELETE(*childrenMenuEntryItr);
	}
	m_childrenMenuEntries.clear();
	// TODO: Removing m_gameCommand
}

void Engine::MenuEntry::AddChildren(MenuEntry* child)
{
	child->SetParent(this);
	m_childrenMenuEntries.push_back(child);
}

int Engine::MenuEntry::GetChildrenCount() const
{
	return static_cast<int>(m_childrenMenuEntries.size());
}

bool Engine::MenuEntry::DoesMouseHoverOverChild(int index, Math::Real xPos, Math::Real yPos) const
{
	CHECK_CONDITION_RETURN(index >= 0 && index < GetChildrenCount(), "Incorrect index", Utility::Error,
		"Cannot find child menu entry AABR. The given index (%d) is not within range [0;%d)", index, GetChildrenCount());
	return m_childrenMenuEntries[index]->GetGuiText().DoesContainPoint(xPos, yPos).IsIntersecting();
}

bool Engine::MenuEntry::DoesMouseHoverOver(Math::Real xPos, Math::Real yPos) const
{
	//Math::IntersectInfo intersectInfo = m_aabr.DoesContainPoint(xPos, yPos);
	//DEBUG_LOG("DoesMouseHoverOver(xPos = %.2f, yPos = %.2f) = %.3f", xPos, yPos, intersectInfo.GetDistance());
	//return intersectInfo.IsIntersecting();

	return GetGuiText().DoesContainPoint(xPos, yPos).IsIntersecting();
}

void Engine::MenuEntry::SelectChildMenuEntry(int index, Rendering::Effects::ColorEffect* newColorEffectForPreviouslySelectedEntry,
	Rendering::Effects::ColorEffect* newColorEffectForCurrentSelectedEntry, bool wrapping /* = true */)
{
	int previouslySelectedMenuEntryIndex = m_selectedMenuEntryIndex;
	if (index < 0)
	{
		if (wrapping) { m_selectedMenuEntryIndex = GetChildrenCount() - 1; } else { return; }
	}
	else if (index >= GetChildrenCount())
	{
		if (wrapping) { m_selectedMenuEntryIndex = 0; } else { return; }
	}
	else
	{
		m_selectedMenuEntryIndex = index;
	}
	CHECK_CONDITION(m_selectedMenuEntryIndex >= 0 && m_selectedMenuEntryIndex < GetChildrenCount(), Utility::Error,
		"Incorrect child menu entry selected. Given index equals %d while it must be in range [0; %d)", m_selectedMenuEntryIndex, GetChildrenCount());
	m_childrenMenuEntries[previouslySelectedMenuEntryIndex]->SetColorEffect(newColorEffectForPreviouslySelectedEntry);
	m_childrenMenuEntries[m_selectedMenuEntryIndex]->SetColorEffect(newColorEffectForCurrentSelectedEntry);
}

Engine::MenuEntry* Engine::MenuEntry::GetParent() const
{
	return m_parentMenuEntry;
}

Engine::MenuEntry* Engine::MenuEntry::GetSelectedChild() const
{
	CHECK_CONDITION_RETURN(m_selectedMenuEntryIndex >= 0 && m_selectedMenuEntryIndex < GetChildrenCount(), NULL, Utility::Error,
		"Cannot return currently selected child menu entry. The index (%d) is not within range [0;%d)", m_selectedMenuEntryIndex, GetChildrenCount());
	return m_childrenMenuEntries[m_selectedMenuEntryIndex];
}

bool Engine::MenuEntry::DoesSelectedChildHaveChildren() const
{
	CHECK_CONDITION_RETURN(m_selectedMenuEntryIndex >= 0 && m_selectedMenuEntryIndex < GetChildrenCount(), false, Utility::Error,
		"Cannot determine whether currently selected child menu entry has children. The index (%d) is not within range [0;%d)", m_selectedMenuEntryIndex, GetChildrenCount());
	return m_childrenMenuEntries[m_selectedMenuEntryIndex]->HasChildren();
}