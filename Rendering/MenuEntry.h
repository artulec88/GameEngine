#ifndef __MENU_ENTRY_H__
#define __MENU_ENTRY_H__

#include "Rendering.h"
#include "Math\Vector.h"
#include <string>
#include <vector>

namespace Rendering
{

class MenuEntry
{
/* ==================== Static variables and functions begin ==================== */
private:
	static const Math::Vector3D NOT_SELECTED_MENU_ENTRY_TEXT_COLOR;
	static const Math::Vector3D SELECTED_MENU_ENTRY_TEXT_COLOR;
public:
	static void InitializeMenuColors();
	static const Math::Vector3D& GetNotSelectedMenuEntryTextColor();
	static const Math::Vector3D& GetSelectedMenuEntryTextColor();
/* ==================== Static variables and functions end ==================== */

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
	int GetChildrenCount() const;
	std::string GetChildrenText(int index) const;
	int GetSelectedMenuEntryIndex() const { return m_selectedMenuEntryIndex; }
	bool IsChildMenuEntrySelected(int index) const { return m_selectedMenuEntryIndex == index; }
	void SelectChildMenuEntry(int index);

	RENDERING_API void AddChildren(MenuEntry* child);
	RENDERING_API bool HasParent() const { return m_parentMenuEntry != NULL; }
	RENDERING_API bool HasChildren() const { return !m_childrenMenuEntries.empty(); }
	RENDERING_API bool DoesSelectedChildHaveChildren() const;
	RENDERING_API void SelectPrevChildMenuEntry();
	RENDERING_API void SelectNextChildMenuEntry();
	RENDERING_API MenuEntry* GetParent();
	RENDERING_API MenuEntry* GetSelectedChild();
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