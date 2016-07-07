#ifndef __ENGINE_MENU_ENTRY_H__
#define __ENGINE_MENU_ENTRY_H__

#include "Engine.h"
#include "GameCommand.h"
#include "Math\Vector.h"
#include "Rendering\Font.h"
#include "Rendering\GuiControl.h"
#include "Effect.h"
#include <string>
#include <vector>

namespace Engine
{
	// TODO: See if the composite pattern could be applied here.

	/// <summary>
	/// The basic menu entry in the game engine.
	/// </summary>
	class MenuEntry
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API MenuEntry(const GameCommand& gameCommand, const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize, const Math::Vector2D& screenPosition,
			Math::Real maxLineLength, const Math::Vector3D& textColor, const Math::Vector3D& outlineColor, const Math::Vector2D& offset, bool isCentered = false,
			Math::Real characterWidth = 0.5f, Math::Real characterEdgeTransitionWidth = 0.1f, Math::Real borderWidth = 0.4f, Math::Real borderEdgeTransitionWidth = 0.1f);
		ENGINE_API virtual ~MenuEntry(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API Rendering::Controls::GuiControl& GetGuiControl() { return *m_guiControl; }
		ENGINE_API const Rendering::Controls::GuiControl& GetGuiControl() const { return *m_guiControl; }
		ENGINE_API Rendering::Controls::GuiControl& GetChildGuiControl(int index) { return m_childrenMenuEntries[index]->GetGuiControl(); }
		ENGINE_API const Rendering::Controls::GuiControl& GetChildGuiControl(int index) const { return m_childrenMenuEntries[index]->GetGuiControl(); }
		void SetParent(MenuEntry* parent) { m_parentMenuEntry = parent; }
		ENGINE_API int GetChildrenCount() const;
		ENGINE_API bool DoesMouseHoverOverChild(int index, Math::Real xPos, Math::Real yPos) const;
		int GetSelectedMenuEntryIndex() const { return m_selectedMenuEntryIndex; }
		bool IsChildMenuEntrySelected(int index) const { return m_selectedMenuEntryIndex == index; }
		ENGINE_API void SelectChildMenuEntry(int index, bool wrapping = true);

		ENGINE_API void ExecuteCommand() const;
		ENGINE_API void AddChild(MenuEntry* child);
		ENGINE_API bool HasParent() const { return m_parentMenuEntry != NULL; }
		ENGINE_API bool HasChildren() const { return !m_childrenMenuEntries.empty(); }
		ENGINE_API bool DoesMouseHoverOver(Math::Real xPos, Math::Real yPos) const;
		ENGINE_API bool DoesSelectedChildHaveChildren() const;
		ENGINE_API MenuEntry* GetParent() const;
		ENGINE_API MenuEntry* GetSelectedChild() const;
		ENGINE_API void ApplyColorEffect(Effects::Effect<Math::Vector3D>* colorEffect)
		{
			//colorEffect->SetAttribute(m_guiControl->GetColorPointer());
		}
		ENGINE_API void ApplyOutlineColorEffect(Effects::Effect<Math::Vector3D>* outlineColorEffect)
		{
			//outlineColorEffect->SetAttribute(m_guiControl->GetOutlineColorPointer());
		}
		ENGINE_API void ApplyOffsetEffect(Effects::Effect<Math::Vector2D>* offsetEffect)
		{
			//offsetEffect->SetAttribute(m_guiControl->GetOffsetPointer());
		}
		ENGINE_API void ApplyCharacterWidthEffect(Effects::Effect<Math::Real>* characterWidthEffect)
		{
			//characterWidthEffect->SetAttribute(m_guiControl->GetCharacterWidthPointer());
		}
		ENGINE_API void ApplyCharacterEdgeTransitionWidthEffect(Effects::Effect<Math::Real>* characterEdgeTransitionWidthEffect)
		{
			//characterEdgeTransitionWidthEffect->SetAttribute(m_guiControl->GetCharacterEdgeTransitionWidthPointer());
		}
		ENGINE_API void ApplyBorderWidthEffect(Effects::Effect<Math::Real>* borderWidthEffect)
		{
			//borderWidthEffect->SetAttribute(m_guiControl->GetBorderWidthPointer());
		}
		ENGINE_API void ApplyBorderEdgeTransitionWidthEffect(Effects::Effect<Math::Real>* borderEdgeTransitionWidthEffect)
		{
			//borderEdgeTransitionWidthEffect->SetAttribute(m_guiControl->GetBorderEdgeTransitionWidthPointer());
		}
		// TODO: Update menu entry. For example, make the currently selected menu entry's GUI text outline color blinking.
	private:
		//void SetColorEffect(Rendering::Effects::Effect<Math::Vector3D>* textEffectColor) { m_guiText.SetColorEffect(textEffectColor); }
		//void SetOutlineColorEffect(Rendering::Effects::Effect<Math::Vector3D>* outlineColorEffect) { m_guiText.SetOutlineColorEffect(outlineColorEffect); }
		//void SetOffsetEffect(Rendering::Effects::Effect<Math::Vector2D>* offsetEffect) { m_guiText.SetOffsetEffect(offsetEffect); }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		const GameCommand& m_gameCommand;
		std::unique_ptr<Rendering::Controls::GuiControl> m_guiControl;
		MenuEntry* m_parentMenuEntry;
		std::vector<MenuEntry*> m_childrenMenuEntries;
		int m_selectedMenuEntryIndex;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MenuEntry */


	/// <summary>
	/// The menu entry in the game engine that has a "value" which can be modified by the user, e.g. by clicking on it.
	/// It is useful mainly in the "Options"-type menus where different settings are presented to the user and can be modified / enabled / disabled.
	/// </summary>
	//class MenuEntryWithValue : public MenuEntry
	//{
	//	/* ==================== Static variables and functions begin ==================== */
	//	/* ==================== Static variables and functions end ==================== */

	//	/* ==================== Constructors and destructors begin ==================== */
	//public:
	//	ENGINE_API MenuEntryWithValue(const GameCommand& gameCommand, const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize, const Math::Vector2D& screenPosition,
	//		Math::Real maxLineLength, const Math::Vector3D& textColor, const Math::Vector3D& outlineColor, const Math::Vector2D& offset, bool isCentered = false,
	//		Math::Real characterWidth = 0.5f, Math::Real characterEdgeTransitionWidth = 0.1f, Math::Real borderWidth = 0.4f, Math::Real borderEdgeTransitionWidth = 0.1f);
	//	ENGINE_API virtual ~MenuEntryWithValue(void);
	//	/* ==================== Constructors and destructors end ==================== */

	//	/* ==================== Non-static member functions begin ==================== */
	//	/* ==================== Non-static member functions end ==================== */

	//	/* ==================== Non-static member variables begin ==================== */
	//private:
	//	const GameCommand& m_gameCommand;
	//	std::unique_ptr<Rendering::Controls::GuiControl> m_valueGuiControl;
	//	/* ==================== Non-static member variables end ==================== */
	//}; /* end class MenuEntryWithValue */

} /* end namespace Engine */

#endif /* __ENGINE_MENU_ENTRY_H__ */