#include "StdAfx.h"
#include "MenuEntry.h"
#include "Math\IntersectInfo.h"
#include "Utility\IConfig.h"
#include <sstream>

/* static */ const Math::Vector4D Engine::MenuEntry::NOT_SELECTED_MENU_ENTRY_TEXT_COLOR(REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ONE);
/* static */ const Math::Vector4D Engine::MenuEntry::SELECTED_MENU_ENTRY_TEXT_COLOR(REAL_ONE, REAL_ONE, REAL_ONE, REAL_ONE);
///* static */ void MenuEntry::InitializeMenuColors()
//{
//}
/* static */ const Math::Vector4D& Engine::MenuEntry::GetNotSelectedMenuEntryTextColor()
{
	return Engine::MenuEntry::NOT_SELECTED_MENU_ENTRY_TEXT_COLOR;
}
/* static */ const Math::Vector4D& Engine::MenuEntry::GetSelectedMenuEntryTextColor()
{
	return Engine::MenuEntry::SELECTED_MENU_ENTRY_TEXT_COLOR;
}

Engine::MenuEntry::MenuEntry(const std::string& text, const Math::Vector2D& screenPosition, const Engine::GameCommand& gameCommand, Math::Real fontSize /* = 32.0f */) :
	m_text(text),
	m_screenPosition(screenPosition),
	m_gameCommand(gameCommand),
	m_aabr(Math::Vector2D(screenPosition.GetX(), screenPosition.GetY() + fontSize), Math::Vector2D(screenPosition.GetX() + (text.size() - 1) * fontSize, screenPosition.GetY())),
	m_fontSize(fontSize),
	m_parentMenuEntry(NULL),
	m_childrenMenuEntries(),
	m_selectedMenuEntryIndex(0)
{
	DELOCUST_LOG("MenuEntry \"%s\" constructor", text.c_str());
	CRITICAL_LOG("AABR for menu entry \"%s\" is [%s; %s]", m_text.c_str(), m_aabr.GetBottomLeftPos().ToString().c_str(), m_aabr.GetTopRightPos().ToString().c_str());
}


Engine::MenuEntry::~MenuEntry(void)
{
	DELOCUST_LOG("MenuEntry \"%s\" destructor", m_text.c_str());
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

std::string Engine::MenuEntry::GetChildText(int index) const
{
	CHECK_CONDITION_RETURN(index >= 0 && index < GetChildrenCount(), "Incorrect index", Utility::Error,
		"Cannot find child menu entry text. The given index (%d) is not within range [0;%d)", index, GetChildrenCount());
	return m_childrenMenuEntries[index]->GetText();
}

const Math::Vector2D& Engine::MenuEntry::GetChildScreenPosition(int index) const
{
	CHECK_CONDITION_RETURN(index >= 0 && index < GetChildrenCount(), "Incorrect index", Utility::Error,
		"Cannot find child menu entry screen position. The given index (%d) is not within range [0;%d)", index, GetChildrenCount());
	return m_childrenMenuEntries[index]->GetScreenPosition();
}

Math::Real Engine::MenuEntry::GetChildFontSize(int index) const
{
	CHECK_CONDITION_RETURN(index >= 0 && index < GetChildrenCount(), "Incorrect index", Utility::Error,
		"Cannot find child menu entry font size. The given index (%d) is not within range [0;%d)", index, GetChildrenCount());
	return m_childrenMenuEntries[index]->GetFontSize();
}

bool Engine::MenuEntry::DoesMouseHoverOverChild(int index, Math::Real xPos, Math::Real yPos) const
{
	CHECK_CONDITION_RETURN(index >= 0 && index < GetChildrenCount(), "Incorrect index", Utility::Error,
		"Cannot find child menu entry AABR. The given index (%d) is not within range [0;%d)", index, GetChildrenCount());
	return m_childrenMenuEntries[index]->GetAABR().DoesContainPoint(xPos, yPos).IsIntersecting();
}

bool Engine::MenuEntry::DoesMouseHoverOver(Math::Real xPos, Math::Real yPos) const
{
	//Math::IntersectInfo intersectInfo = m_aabr.DoesContainPoint(xPos, yPos);
	//CRITICAL_LOG("DoesMouseHoverOver(xPos = %.2f, yPos = %.2f) = %.3f", xPos, yPos, intersectInfo.GetDistance());
	//return intersectInfo.IsIntersecting();

	return m_aabr.DoesContainPoint(xPos, yPos).IsIntersecting();
}

void Engine::MenuEntry::SelectPrevChildMenuEntry()
{
	--m_selectedMenuEntryIndex;
	if (m_selectedMenuEntryIndex < 0)
	{
		m_selectedMenuEntryIndex = GetChildrenCount() - 1;
	}
}

void Engine::MenuEntry::SelectNextChildMenuEntry()
{
	++m_selectedMenuEntryIndex;
	if (m_selectedMenuEntryIndex >= GetChildrenCount())
	{
		m_selectedMenuEntryIndex = 0;
	}
}

void Engine::MenuEntry::SelectChildMenuEntry(int index)
{
	CHECK_CONDITION(index >= 0 && index < GetChildrenCount(), Utility::Warning,
		"Incorrect child menu entry selected. Given index equals %d while it must be in range [0; %d)", index, GetChildrenCount());
	m_selectedMenuEntryIndex = index % GetChildrenCount();
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