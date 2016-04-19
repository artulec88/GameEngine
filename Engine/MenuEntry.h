#ifndef __ENGINE_MENU_ENTRY_H__
#define __ENGINE_MENU_ENTRY_H__

#include "Engine.h"
#include "GameCommand.h"
#include "Math\Vector.h"
#include "Math\AABR.h"
#include "Rendering\Font.h"
#include "Rendering\GuiText.h"
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
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ENGINE_API MenuEntry(const GameCommand& gameCommand, const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize, const Math::Vector2D& screenPosition,
		Math::Real maxLineLength, Rendering::Effects::ColorEffect* textEffectColor, const Math::Vector2D& offset, const Math::Vector3D& outlineColor,
		bool isCentered = false, Math::Real characterWidth = 0.5f, Math::Real characterEdgeTransitionWidth = 0.1f, Math::Real borderWidth = 0.4f,
		Math::Real borderEdgeTransitionWidth = 0.1f);
	ENGINE_API virtual ~MenuEntry(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	ENGINE_API Rendering::Text::GuiText& GetGuiText() { return m_guiText; }
	ENGINE_API const Rendering::Text::GuiText& GetGuiText() const { return m_guiText; }
	ENGINE_API Rendering::Text::GuiText& GetChildGuiText(int index) { return m_childrenMenuEntries[index]->GetGuiText(); }
	ENGINE_API const Rendering::Text::GuiText& GetChildGuiText(int index) const { return m_childrenMenuEntries[index]->GetGuiText(); }
	void SetParent(MenuEntry* parent) { m_parentMenuEntry = parent; }
	ENGINE_API int GetChildrenCount() const;
	ENGINE_API bool DoesMouseHoverOverChild(int index, Math::Real xPos, Math::Real yPos) const;
	int GetSelectedMenuEntryIndex() const { return m_selectedMenuEntryIndex; }
	bool IsChildMenuEntrySelected(int index) const { return m_selectedMenuEntryIndex == index; }
	ENGINE_API void SelectChildMenuEntry(int index, Rendering::Effects::ColorEffect* newColorEffectForPreviouslySelectedEntry,
		Rendering::Effects::ColorEffect* newColorEffectForCurrentSelectedEntry, bool wrapping = true);

	ENGINE_API void ExecuteCommand() const { m_gameCommand.Execute(); }
	ENGINE_API void AddChildren(MenuEntry* child);
	ENGINE_API bool HasParent() const { return m_parentMenuEntry != NULL; }
	ENGINE_API bool HasChildren() const { return !m_childrenMenuEntries.empty(); }
	ENGINE_API bool DoesMouseHoverOver(Math::Real xPos, Math::Real yPos) const;
	ENGINE_API bool DoesSelectedChildHaveChildren() const;
	ENGINE_API MenuEntry* GetParent() const;
	ENGINE_API MenuEntry* GetSelectedChild() const;

	// TODO: Update menu entry. For example, make the currently selected menu entry's GUI text outline color blinking.
private:
	void SetColorEffect(Rendering::Effects::ColorEffect* textEffectColor) { m_guiText.SetColorEffect(textEffectColor); }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const GameCommand& m_gameCommand;
	Rendering::Text::GuiText m_guiText;
	MenuEntry* m_parentMenuEntry;
	std::vector<MenuEntry*> m_childrenMenuEntries;
	int m_selectedMenuEntryIndex;
/* ==================== Non-static member variables end ==================== */
}; /* end class MenuEntry */

} /* end namespace Engine */

#endif /* __ENGINE_MENU_ENTRY_H__ */