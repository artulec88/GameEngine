#include "StdAfx.h"
#include "MenuEntry.h"
#include "Utility\IConfig.h"
#include <sstream>

using namespace Rendering;

/* static */ const Math::Vector3D MenuEntry::NOT_SELECTED_MENU_ENTRY_TEXT_COLOR(REAL_ONE, REAL_ZERO, REAL_ZERO);
/* static */ const Math::Vector3D MenuEntry::SELECTED_MENU_ENTRY_TEXT_COLOR(REAL_ONE, REAL_ONE, REAL_ONE);
/* static */ const Math::Vector3D& MenuEntry::GetNotSelectedMenuEntryTextColor()
{
	return MenuEntry::NOT_SELECTED_MENU_ENTRY_TEXT_COLOR;
}
/* static */ const Math::Vector3D& MenuEntry::GetSelectedMenuEntryTextColor()
{
	return MenuEntry::SELECTED_MENU_ENTRY_TEXT_COLOR;
}

MenuEntry::MenuEntry(void) :
	m_action(Actions::UNDEFINED),
	m_text(),
	m_parentMenuEntry(NULL),
	m_childrenMenuEntries(),
	m_selectedMenuEntryIndex(0)
{
}

MenuEntry::MenuEntry(Actions::ActionID actionID, const std::string& text) :
	m_action(actionID),
	m_text(text),
	m_parentMenuEntry(NULL),
	m_childrenMenuEntries(),
	m_selectedMenuEntryIndex(0)
{
}


MenuEntry::~MenuEntry(void)
{
	for (std::vector<MenuEntry*>::iterator childrenMenuEntryItr = m_childrenMenuEntries.begin(); childrenMenuEntryItr != m_childrenMenuEntries.end(); ++childrenMenuEntryItr)
	{
		delete (*childrenMenuEntryItr);
	}
	m_childrenMenuEntries.clear();
}

void MenuEntry::AddChildren(MenuEntry* child)
{
	child->SetParent(this);
	m_childrenMenuEntries.push_back(child);
}

int MenuEntry::GetChildrenCount() const
{
	return static_cast<int>(m_childrenMenuEntries.size());
}

std::string MenuEntry::GetChildrenText(int index) const
{
	std::stringstream ss("");
	ss << m_childrenMenuEntries.at(index)->GetText();
	return ss.str();
}

void MenuEntry::SelectPrevChildMenuEntry()
{
	--m_selectedMenuEntryIndex;
	if (m_selectedMenuEntryIndex < 0)
	{
		m_selectedMenuEntryIndex = GetChildrenCount() - 1;
	}
}

void MenuEntry::SelectNextChildMenuEntry()
{
	++m_selectedMenuEntryIndex;
	if (m_selectedMenuEntryIndex >= GetChildrenCount())
	{
		m_selectedMenuEntryIndex = 0;
	}
}

void MenuEntry::SelectChildMenuEntry(int index)
{
	if ((index < 0) || (index >= GetChildrenCount()))
	{
		LOG(Utility::Warning, LOGPLACE, "Incorrect child menu entry selected. Given index equals %d while it must be in range [0; %d)", index, GetChildrenCount());
	}
	m_selectedMenuEntryIndex = index % GetChildrenCount();
}

MenuEntry* MenuEntry::GetParent()
{
	return m_parentMenuEntry;
}

MenuEntry* MenuEntry::GetSelectedChild()
{
	return m_childrenMenuEntries.at(m_selectedMenuEntryIndex);
}

bool MenuEntry::DoesSelectedChildHaveChildren() const
{
	return m_childrenMenuEntries.at(m_selectedMenuEntryIndex)->HasChildren();
}