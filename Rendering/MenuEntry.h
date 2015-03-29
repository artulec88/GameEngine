#ifndef __MENU_ENTRY_H__
#define __MENU_ENTRY_H__

#include "Rendering.h"
#include <string>
#include <vector>

namespace Rendering
{

class MenuEntry
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API MenuEntry();
	RENDERING_API MenuEntry(const std::string& text);
	RENDERING_API virtual ~MenuEntry(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	std::string GetText() const { return m_text; }
	void SetParent(MenuEntry* parent) { m_parentMenuEntry = parent; }
	RENDERING_API void AddChildren(MenuEntry* child);
	int GetChildrenCount() const;
	std::string GetChildrenText(int index) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	MenuEntry* m_parentMenuEntry;
	std::vector<MenuEntry*> m_childrenMenuEntries;
	int m_selectedMenuEntryIndex;
	std::string m_text;
/* ==================== Non-static member variables end ==================== */
}; /* end class MenuEntry */

} /* end namespace Rendering */

#endif /* __MENU_ENTRY_H__ */