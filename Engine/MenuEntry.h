#ifndef __ENGINE_MENU_ENTRY_H__
#define __ENGINE_MENU_ENTRY_H__

#include "Engine.h"
#include "GameCommand.h"
#include "Math\Vector.h"
#include "Math\AABR.h"
#include <string>
#include <vector>

namespace Engine
{

// TODO: Consider moving this namespace to another file (e.g. Actions.h)
namespace MenuActions
{
	/**
	 * TODO: Improve this enum. Maybe it would be better to define MenuActionID as a class to
	 * allow storing additional parameters for a given action.
	 */
	enum MenuActionID
	{
		QUIT = 0,
		START_RESUME,
		PAUSE,
		OTHER,
		UNDEFINED
	};
} /* end namespace MenuActions */

class MenuEntry
{
/* ==================== Static variables and functions begin ==================== */
private:
	static const Math::Vector4D NOT_SELECTED_MENU_ENTRY_TEXT_COLOR;
	static const Math::Vector4D SELECTED_MENU_ENTRY_TEXT_COLOR;
public:
	//static void InitializeMenuColors();
	ENGINE_API static const Math::Vector4D& GetNotSelectedMenuEntryTextColor();
	ENGINE_API static const Math::Vector4D& GetSelectedMenuEntryTextColor();
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ENGINE_API MenuEntry(const std::string& text, const Math::Vector2D& screenPosition, const GameCommand& gameCommand, Math::Real fontSize = 32.0f);
	ENGINE_API virtual ~MenuEntry(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	std::string GetText() const { return m_text; }
	const Math::Vector2D& GetScreenPosition() const { return m_screenPosition; }
	const Math::AABR& GetAABR() const { return m_aabr; }
	Math::Real GetFontSize() const { return m_fontSize; }
	void SetParent(MenuEntry* parent) { m_parentMenuEntry = parent; }
	ENGINE_API int GetChildrenCount() const;
	ENGINE_API std::string GetChildText(int index) const;
	ENGINE_API const Math::Vector2D& GetChildScreenPosition(int index) const;
	ENGINE_API Math::Real GetChildFontSize(int index) const;
	ENGINE_API bool DoesMouseHoverOverChild(int index, Math::Real xPos, Math::Real yPos) const;
	int GetSelectedMenuEntryIndex() const { return m_selectedMenuEntryIndex; }
	bool IsChildMenuEntrySelected(int index) const { return m_selectedMenuEntryIndex == index; }
	ENGINE_API void SelectChildMenuEntry(int index);

	ENGINE_API void ExecuteCommand() const { m_gameCommand.Execute(); }
	ENGINE_API void AddChildren(MenuEntry* child);
	ENGINE_API bool HasParent() const { return m_parentMenuEntry != NULL; }
	ENGINE_API bool HasChildren() const { return !m_childrenMenuEntries.empty(); }
	ENGINE_API bool DoesMouseHoverOver(Math::Real xPos, Math::Real yPos) const;
	ENGINE_API bool DoesSelectedChildHaveChildren() const;
	ENGINE_API void SelectPrevChildMenuEntry();
	ENGINE_API void SelectNextChildMenuEntry();
	ENGINE_API MenuEntry* GetParent() const;
	ENGINE_API MenuEntry* GetSelectedChild() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	std::string m_text;
	Math::Vector2D m_screenPosition;
	const GameCommand& m_gameCommand;
	/// <summary> Menu entry bounding rectangle. </summary>
	Math::AABR m_aabr;
	Math::Real m_fontSize;
	MenuEntry* m_parentMenuEntry;
	std::vector<MenuEntry*> m_childrenMenuEntries;
	int m_selectedMenuEntryIndex;
/* ==================== Non-static member variables end ==================== */
}; /* end class MenuEntry */

} /* end namespace Engine */

#endif /* __ENGINE_MENU_ENTRY_H__ */