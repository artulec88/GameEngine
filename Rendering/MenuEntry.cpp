#include "StdAfx.h"
#include "MenuEntry.h"
#include <sstream>

using namespace Rendering;

MenuEntry::MenuEntry(void) :
	m_parentMenuEntry(NULL),
	m_childrenMenuEntries(),
	m_selectedMenuEntryIndex(0),
	m_text()
{
}

MenuEntry::MenuEntry(const std::string& text) :
	m_parentMenuEntry(NULL),
	m_childrenMenuEntries(),
	m_selectedMenuEntryIndex(0),
	m_text(text)
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