#include "StdAfx.h"
#include "MenuEntry.h"
#include "GameManager.h"

#include "Rendering/GuiButtonControl.h"

#include "Math/IntersectInfo.h"

#include "Utility/IConfig.h"
#include "Utility/ILogger.h"

/* ==================== MenuEntry class begin ==================== */
engine::MenuEntry::MenuEntry(const std::string& text, const Rendering::Text::Font* font, math::Real fontSize, const Rendering::Texture* iconTexture, const math::Vector2D& screenPosition,
	const math::Angle& screenRotation, const math::Vector2D& scale, math::Real maxLineLength, const Rendering::Color& textColor, const Rendering::Color& outlineColor, const math::Vector2D& offset, bool isCentered /* = false */,
	math::Real characterWidth /* = 0.5f */, math::Real characterEdgeTransitionWidth /* = 0.1f */, math::Real borderWidth /* = 0.4f */, math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	m_parentMenuEntry(nullptr),
	// TODO: Create a GuiControlFactory which will create GUI controls based on the special enum value (e.g.GuiControlType).
	m_guiControl(std::make_unique<Rendering::Controls::GuiButtonControl>(text, font, fontSize, iconTexture, screenPosition, screenRotation, scale, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth))
{
}

engine::MenuEntry::~MenuEntry()
{
}

void engine::MenuEntry::Render(int guiControlShaderID, Rendering::Renderer* renderer) const
{
	renderer->RenderGuiControl(*m_guiControl, guiControlShaderID);
}

void engine::MenuEntry::AddChild(MenuEntry* childMenuEntry)
{
	ERROR_LOG_ENGINE("Only composite menu entry can have children menu entries.");
}

void engine::MenuEntry::ApplyColorEffect(const math::effects::Effect<Rendering::Color>& effect)
{
	m_guiControl->ApplyColorEffect(effect);
}

void engine::MenuEntry::ApplyOutlineColorEffect(const math::effects::Effect<Rendering::Color>& effect)
{
	m_guiControl->ApplyOutlineColorEffect(effect);
}

void engine::MenuEntry::ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect)
{
	m_guiControl->ApplyOffsetEffect(effect);
}

void engine::MenuEntry::ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	m_guiControl->ApplyCharacterWidthEffect(effect);
}

void engine::MenuEntry::ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	m_guiControl->ApplyCharacterEdgeTransitionWidthEffect(effect);
}

void engine::MenuEntry::ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	m_guiControl->ApplyBorderWidthEffect(effect);
}

void engine::MenuEntry::ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	m_guiControl->ApplyBorderEdgeTransitionWidthEffect(effect);
}

bool engine::MenuEntry::DoesMouseHoverOver(math::Real xPos, math::Real yPos) const
{
	return GetGuiControl().DoesContainPoint(xPos, yPos).IsIntersecting();
}

void engine::MenuEntry::SetParent(CompositeMenuEntry* parentMenuEntry)
{
	m_parentMenuEntry = parentMenuEntry;
}
/* ==================== MenuEntry class end ==================== */

/* ==================== CompositeMenuEntry class begin ==================== */
engine::CompositeMenuEntry::CompositeMenuEntry(const std::string& text, const Rendering::Text::Font* font, math::Real fontSize, const Rendering::Texture* iconTexture,
	const math::Vector2D& screenPosition, const math::Angle& screenRotation, const math::Vector2D& scale, math::Real maxLineLength, const Rendering::Color& textColor, const Rendering::Color& outlineColor,
	const math::Vector2D& offset, bool isCentered /* = false */, math::Real characterWidth /* = 0.5f */, math::Real characterEdgeTransitionWidth /* = 0.1f */, math::Real borderWidth /* = 0.4f */,
	math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	MenuEntry(text, font, fontSize, iconTexture, screenPosition, screenRotation, scale, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth),
	m_childrenMenuEntries(),
	m_selectedMenuEntryIndex(0)
{
}


engine::CompositeMenuEntry::~CompositeMenuEntry()
{
}

//void Engine::CompositeMenuEntry::Render(Rendering::Renderer* renderer) const
//{
	//for (std::vector<MenuEntry*>::const_iterator menuEntryItr = m_childrenMenuEntries.begin(); menuEntryItr != m_childrenMenuEntries.end(); ++menuEntryItr)
	//{
	//	(*menuEntryItr)->Render(renderer);
	//}
//}

void engine::CompositeMenuEntry::RenderAll(int guiControlShaderID, Rendering::Renderer* renderer) const
{
	for (std::vector<MenuEntry*>::const_iterator menuEntryItr = m_childrenMenuEntries.begin(); menuEntryItr != m_childrenMenuEntries.end(); ++menuEntryItr)
	{
		(*menuEntryItr)->Render(guiControlShaderID, renderer);
	}
}

// TODO: I guess it would be better for the getSelectedChildIndex to simply return the m_selectedMenuEntryIndex instead of calculating the current menu entry.
size_t engine::CompositeMenuEntry::GetSelectedChildIndex(math::Real xPos, math::Real yPos) const
{
	if (!m_childrenMenuEntries[m_selectedMenuEntryIndex]->DoesMouseHoverOver(xPos, yPos))
	{
		size_t index = 0;
		for (std::vector<MenuEntry*>::const_iterator menuEntryItr = m_childrenMenuEntries.begin(); menuEntryItr != m_childrenMenuEntries.end(); ++menuEntryItr, ++index)
		{
			if ((index != m_selectedMenuEntryIndex) && ((*menuEntryItr)->DoesMouseHoverOver(xPos, yPos)))
			{
				return index;
			}
		}
	}
	return m_selectedMenuEntryIndex;
}

void engine::CompositeMenuEntry::AddChild(MenuEntry* menuEntry)
{
	m_childrenMenuEntries.push_back(menuEntry);
	menuEntry->SetParent(this);
}

void engine::CompositeMenuEntry::Dispatch()
{
	GameManager::GetGameManager()->Input(engine::Actions::GO_TO_CHILD_MENU_ENTRY);
}

void engine::CompositeMenuEntry::ApplyColorEffectToAll(const math::effects::Effect<Rendering::Color>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyColorEffect(effect);
	}
}

void engine::CompositeMenuEntry::ApplyOutlineColorEffectToAll(const math::effects::Effect<Rendering::Color>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyOutlineColorEffect(effect);
	}
}

void engine::CompositeMenuEntry::ApplyOffsetEffectToAll(const math::effects::Effect<math::Vector2D>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyOffsetEffect(effect);
	}
}

void engine::CompositeMenuEntry::ApplyCharacterWidthEffectToAll(const math::effects::Effect<math::Real>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyCharacterWidthEffect(effect);
	}
}

void engine::CompositeMenuEntry::ApplyCharacterEdgeTransitionWidthEffectToAll(const math::effects::Effect<math::Real>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyCharacterEdgeTransitionWidthEffect(effect);
	}
}

void engine::CompositeMenuEntry::ApplyBorderWidthEffectToAll(const math::effects::Effect<math::Real>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyBorderWidthEffect(effect);
	}
}

void engine::CompositeMenuEntry::ApplyBorderEdgeTransitionWidthEffectToAll(const math::effects::Effect<math::Real>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyBorderEdgeTransitionWidthEffect(effect);
	}
}

engine::MenuEntry* engine::CompositeMenuEntry::SelectPrevChild()
{
	return SelectChild(m_selectedMenuEntryIndex - 1);
}

engine::MenuEntry* engine::CompositeMenuEntry::SelectNextChild()
{
	return SelectChild(m_selectedMenuEntryIndex + 1);
}

engine::MenuEntry* engine::CompositeMenuEntry::SelectChild(size_t index)
{
	// TODO: Range check
	if (index != m_selectedMenuEntryIndex)
	{
		m_selectedMenuEntryIndex = math::Clamp(index, static_cast<size_t>(0), m_childrenMenuEntries.size());
	}
	return m_childrenMenuEntries[m_selectedMenuEntryIndex];
}
/* ==================== CompositeMenuEntry class end ==================== */

/* ==================== ActionMenuEntry class begin ==================== */
engine::ActionMenuEntry::ActionMenuEntry(engine::Actions::Action actionID, const std::string& text, const Rendering::Text::Font* font, math::Real fontSize, const Rendering::Texture* iconTexture,
	const math::Vector2D& screenPosition, const math::Angle& screenRotation, const math::Vector2D& scale, math::Real maxLineLength, const Rendering::Color& textColor, const Rendering::Color& outlineColor, const math::Vector2D& offset,
	bool isCentered /* = false */, math::Real characterWidth /* = 0.5f */, math::Real characterEdgeTransitionWidth /* = 0.1f */, math::Real borderWidth /* = 0.4f */,
	math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	MenuEntry(text, font, fontSize, iconTexture, screenPosition, screenRotation, scale, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth),
	m_actionID(actionID)
{
}


engine::ActionMenuEntry::~ActionMenuEntry()
{
}

//void Engine::ActionMenuEntry::Render(Rendering::Renderer* renderer) const
//{
//}

void engine::ActionMenuEntry::Dispatch()
{
	GameManager::GetGameManager()->Input(m_actionID);
}
/* ==================== ActionMenuEntry class end ==================== */

/* ==================== ValueMenuEntry class begin ==================== */
engine::ValueMenuEntry::ValueMenuEntry(const std::string& text, const Rendering::Text::Font* font, math::Real fontSize, const Rendering::Texture* iconTexture,
	const math::Vector2D& screenPosition, const math::Angle& screenRotation, const math::Vector2D& scale, math::Real maxLineLength, const Rendering::Color& textColor, const Rendering::Color& outlineColor, const math::Vector2D& offset,
	bool isCentered /* = false */, math::Real characterWidth /* = 0.5f */, math::Real characterEdgeTransitionWidth /* = 0.1f */, math::Real borderWidth /* = 0.4f */,
	math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	MenuEntry(text, font, fontSize, iconTexture, screenPosition, screenRotation, scale, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth)
{
}


engine::ValueMenuEntry::~ValueMenuEntry()
{
}

void engine::ValueMenuEntry::Render(int guiControlShaderID, Rendering::Renderer* renderer) const
{
	MenuEntry::Render(guiControlShaderID, renderer); // base class rendering
	// TODO: Rendering other GUI controls.
}

void engine::ValueMenuEntry::Dispatch()
{
	WARNING_LOG_ENGINE("Still to be implemented.");
}
/* ==================== ValueMenuEntry class end ==================== */

//void Engine::MenuEntry::ExecuteCommand() const
//{
//	m_gameCommand.Execute(GameManager::GetGameManager());
//}
//
//void Engine::MenuEntry::AddChild(MenuEntry* child)
//{
//	child->SetParent(this);
//	m_childrenMenuEntries.push_back(child);
//}
//
//int Engine::MenuEntry::GetChildrenCount() const
//{
//	return static_cast<int>(m_childrenMenuEntries.size());
//}
//
//bool Engine::MenuEntry::DoesMouseHoverOverChild(int index, math::Real xPos, math::Real yPos) const
//{
//	CHECK_CONDITION_RETURN_ENGINE(index >= 0 && index < GetChildrenCount(), "Incorrect index", Utility::ERR,
//		"Cannot find child menu entry AABR. The given index (", index, ") is not within range [0;", GetChildrenCount(), ")");
//	return m_childrenMenuEntries[index]->GetGuiControl().DoesContainPoint(xPos, yPos).IsIntersecting();
//}
//
//bool Engine::MenuEntry::DoesMouseHoverOver(math::Real xPos, math::Real yPos) const
//{
//	//math::IntersectInfo intersectInfo = m_aabr.DoesContainPoint(xPos, yPos);
//	//DEBUG_LOG_ENGINE("DoesMouseHoverOver(xPos = ", xPos, ", yPos = ", yPos, ") = ", intersectInfo.GetDistance());
//	//return intersectInfo.IsIntersecting();
//
//	return GetGuiControl().DoesContainPoint(xPos, yPos).IsIntersecting();
//}
//
//void Engine::MenuEntry::SelectChildMenuEntry(int index, bool wrapping /* = true */)
//{
//	int previouslySelectedMenuEntryIndex = m_selectedMenuEntryIndex;
//	if (index < 0)
//	{
//		if (wrapping) { m_selectedMenuEntryIndex = GetChildrenCount() - 1; } else { return; }
//	}
//	else if (index >= GetChildrenCount())
//	{
//		if (wrapping) { m_selectedMenuEntryIndex = 0; } else { return; }
//	}
//	else
//	{
//		m_selectedMenuEntryIndex = index;
//	}
//	CHECK_CONDITION_ENGINE(m_selectedMenuEntryIndex >= 0 && m_selectedMenuEntryIndex < GetChildrenCount(), Utility::ERR,
//		"Incorrect child menu entry selected. Given index equals ", m_selectedMenuEntryIndex, " while it must be in range [0; ", GetChildrenCount(), ")");
//	//m_childrenMenuEntries[previouslySelectedMenuEntryIndex]->SetColorEffect(newColorEffectForPreviouslySelectedEntry);
//	//m_childrenMenuEntries[previouslySelectedMenuEntryIndex]->SetOutlineColorEffect(newOutlineColorEffectForPreviouslySelectedEntry);
//	//m_childrenMenuEntries[previouslySelectedMenuEntryIndex]->SetOffsetEffect(newOffsetEffectForPreviouslySelectedEntry);
//	//m_childrenMenuEntries[m_selectedMenuEntryIndex]->SetColorEffect(newColorEffectForCurrentSelectedEntry);
//	//m_childrenMenuEntries[m_selectedMenuEntryIndex]->SetOutlineColorEffect(newOutlineColorEffectForCurrentSelectedEntry);
//	//m_childrenMenuEntries[m_selectedMenuEntryIndex]->SetOffsetEffect(newOffsetEffectForCurrentSelectedEntry);
//}
//
//Engine::MenuEntry* Engine::MenuEntry::GetParent() const
//{
//	return m_parentMenuEntry;
//}
//
//Engine::MenuEntry* Engine::MenuEntry::GetSelectedChild() const
//{
//	CHECK_CONDITION_RETURN_ENGINE(m_selectedMenuEntryIndex >= 0 && m_selectedMenuEntryIndex < GetChildrenCount(), NULL, Utility::ERR,
//		"Cannot return currently selected child menu entry. The index (", m_selectedMenuEntryIndex, ") is not within range [0;", GetChildrenCount(), ")");
//	return m_childrenMenuEntries[m_selectedMenuEntryIndex];
//}
//
//bool Engine::MenuEntry::DoesSelectedChildHaveChildren() const
//{
//	CHECK_CONDITION_RETURN_ENGINE(m_selectedMenuEntryIndex >= 0 && m_selectedMenuEntryIndex < GetChildrenCount(), false, Utility::ERR,
//		"Cannot determine whether currently selected child menu entry has children. The index (", m_selectedMenuEntryIndex, ") is not within range [0;", GetChildrenCount(), ")");
//	return m_childrenMenuEntries[m_selectedMenuEntryIndex]->HasChildren();
//}