#include "StdAfx.h"
#include "MenuEntry.h"
#include "GameManager.h"

#include "Rendering\GuiButtonControl.h"

#include "Math\IntersectInfo.h"

#include "Utility\IConfig.h"
#include "Utility\ILogger.h"

#include <sstream>

/* ==================== MenuEntry class begin ==================== */
Engine::MenuEntry::MenuEntry(const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize, const Rendering::Texture* iconTexture, const Math::Vector2D& screenPosition,
	Math::Real maxLineLength, const Rendering::Color& textColor, const Rendering::Color& outlineColor, const Math::Vector2D& offset, bool isCentered /* = false */,
	Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */, Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	m_parentMenuEntry(NULL),
	// TODO: Create a GuiControlFactory which will create GUI controls based on the special enum value (e.g.GuiControlType).
	m_guiControl(std::make_unique<Rendering::Controls::GuiButtonControl>(text, font, fontSize, iconTexture, screenPosition, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth))
{
}

Engine::MenuEntry::~MenuEntry()
{
}

void Engine::MenuEntry::Render(Rendering::Renderer* renderer, const Rendering::Shader& guiControlShader) const
{
	renderer->RenderGuiControl(*m_guiControl, guiControlShader);
}

void Engine::MenuEntry::AddChild(MenuEntry* childMenuEntry)
{
	ERROR_LOG_ENGINE("Only composite menu entry can have children menu entries.");
}

void Engine::MenuEntry::ApplyColorEffect(const Math::Effects::Effect<Rendering::Color>& effect)
{
	m_guiControl->ApplyColorEffect(effect);
}

void Engine::MenuEntry::ApplyOutlineColorEffect(const Math::Effects::Effect<Rendering::Color>& effect)
{
	m_guiControl->ApplyOutlineColorEffect(effect);
}

void Engine::MenuEntry::ApplyOffsetEffect(const Math::Effects::Effect<Math::Vector2D>& effect)
{
	m_guiControl->ApplyOffsetEffect(effect);
}

void Engine::MenuEntry::ApplyCharacterWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	m_guiControl->ApplyCharacterWidthEffect(effect);
}

void Engine::MenuEntry::ApplyCharacterEdgeTransitionWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	m_guiControl->ApplyCharacterEdgeTransitionWidthEffect(effect);
}

void Engine::MenuEntry::ApplyBorderWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	m_guiControl->ApplyBorderWidthEffect(effect);
}

void Engine::MenuEntry::ApplyBorderEdgeTransitionWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	m_guiControl->ApplyBorderEdgeTransitionWidthEffect(effect);
}

bool Engine::MenuEntry::DoesMouseHoverOver(Math::Real xPos, Math::Real yPos) const
{
	return GetGuiControl().DoesContainPoint(xPos, yPos).IsIntersecting();
}

void Engine::MenuEntry::SetParent(CompositeMenuEntry* parentMenuEntry)
{
	m_parentMenuEntry = parentMenuEntry;
}
/* ==================== MenuEntry class end ==================== */

/* ==================== CompositeMenuEntry class begin ==================== */
Engine::CompositeMenuEntry::CompositeMenuEntry(const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize, const Rendering::Texture* iconTexture,
	const Math::Vector2D& screenPosition, Math::Real maxLineLength, const Rendering::Color& textColor, const Rendering::Color& outlineColor, const Math::Vector2D& offset,
	bool isCentered /* = false */, Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */,
	Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	MenuEntry(text, font, fontSize, iconTexture, screenPosition, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth),
	m_childrenMenuEntries(),
	m_selectedMenuEntryIndex(0)
{
}


Engine::CompositeMenuEntry::~CompositeMenuEntry(void)
{
}

//void Engine::CompositeMenuEntry::Render(Rendering::Renderer* renderer) const
//{
	//for (std::vector<MenuEntry*>::const_iterator menuEntryItr = m_childrenMenuEntries.begin(); menuEntryItr != m_childrenMenuEntries.end(); ++menuEntryItr)
	//{
	//	(*menuEntryItr)->Render(renderer);
	//}
//}

void Engine::CompositeMenuEntry::RenderAll(Rendering::Renderer* renderer, const Rendering::Shader& guiControlShader) const
{
	for (std::vector<MenuEntry*>::const_iterator menuEntryItr = m_childrenMenuEntries.begin(); menuEntryItr != m_childrenMenuEntries.end(); ++menuEntryItr)
	{
		(*menuEntryItr)->Render(renderer, guiControlShader);
	}
}

size_t Engine::CompositeMenuEntry::GetSelectedChildIndex(Math::Real xPos, Math::Real yPos) const
{
	if (!m_childrenMenuEntries[m_selectedMenuEntryIndex]->DoesMouseHoverOver(xPos, yPos))
	{
		size_t index = 0;
		for (std::vector<MenuEntry*>::const_iterator menuEntryItr = m_childrenMenuEntries.begin(); menuEntryItr != m_childrenMenuEntries.end(); ++menuEntryItr, ++index)
		{
			if ((index != m_selectedMenuEntryIndex) && ((*menuEntryItr)->DoesMouseHoverOver(xPos, yPos)))
			{
				return index;
				break;
			}
		}
	}
	return m_selectedMenuEntryIndex;
}

void Engine::CompositeMenuEntry::AddChild(MenuEntry* menuEntry)
{
	m_childrenMenuEntries.push_back(menuEntry);
	menuEntry->SetParent(this);
}

void Engine::CompositeMenuEntry::Dispatch()
{
	GameManager::GetGameManager()->Input(Engine::Actions::GO_TO_CHILD_MENU_ENTRY);
}

void Engine::CompositeMenuEntry::ApplyColorEffectToAll(const Math::Effects::Effect<Rendering::Color>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyColorEffect(effect);
	}
}

void Engine::CompositeMenuEntry::ApplyOutlineColorEffectToAll(const Math::Effects::Effect<Rendering::Color>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyOutlineColorEffect(effect);
	}
}

void Engine::CompositeMenuEntry::ApplyOffsetEffectToAll(const Math::Effects::Effect<Math::Vector2D>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyOffsetEffect(effect);
	}
}

void Engine::CompositeMenuEntry::ApplyCharacterWidthEffectToAll(const Math::Effects::Effect<Math::Real>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyCharacterWidthEffect(effect);
	}
}

void Engine::CompositeMenuEntry::ApplyCharacterEdgeTransitionWidthEffectToAll(const Math::Effects::Effect<Math::Real>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyCharacterEdgeTransitionWidthEffect(effect);
	}
}

void Engine::CompositeMenuEntry::ApplyBorderWidthEffectToAll(const Math::Effects::Effect<Math::Real>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyBorderWidthEffect(effect);
	}
}

void Engine::CompositeMenuEntry::ApplyBorderEdgeTransitionWidthEffectToAll(const Math::Effects::Effect<Math::Real>& effect)
{
	for (auto childMenuEntryItr = m_childrenMenuEntries.begin(); childMenuEntryItr != m_childrenMenuEntries.end(); ++childMenuEntryItr)
	{
		(*childMenuEntryItr)->ApplyBorderEdgeTransitionWidthEffect(effect);
	}
}

Engine::MenuEntry* Engine::CompositeMenuEntry::SelectPrevChild()
{
	return SelectChild(m_selectedMenuEntryIndex - 1);
}

Engine::MenuEntry* Engine::CompositeMenuEntry::SelectNextChild()
{
	return SelectChild(m_selectedMenuEntryIndex + 1);
}

Engine::MenuEntry* Engine::CompositeMenuEntry::SelectChild(size_t index)
{
	// TODO: Range check
	if (index != m_selectedMenuEntryIndex)
	{
		m_selectedMenuEntryIndex = Math::Clamp(index, (size_t)0, m_childrenMenuEntries.size());
	}
	return m_childrenMenuEntries[m_selectedMenuEntryIndex];
}
/* ==================== CompositeMenuEntry class end ==================== */

/* ==================== ActionMenuEntry class begin ==================== */
Engine::ActionMenuEntry::ActionMenuEntry(Engine::Actions::Action actionID, const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize, const Rendering::Texture* iconTexture,
	const Math::Vector2D& screenPosition, Math::Real maxLineLength, const Rendering::Color& textColor, const Rendering::Color& outlineColor, const Math::Vector2D& offset,
	bool isCentered /* = false */, Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */,
	Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	MenuEntry(text, font, fontSize, iconTexture, screenPosition, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth),
	m_actionID(actionID)
{
}


Engine::ActionMenuEntry::~ActionMenuEntry(void)
{
}

//void Engine::ActionMenuEntry::Render(Rendering::Renderer* renderer) const
//{
//}

void Engine::ActionMenuEntry::Dispatch()
{
	GameManager::GetGameManager()->Input(m_actionID);
}
/* ==================== ActionMenuEntry class end ==================== */

/* ==================== ValueMenuEntry class begin ==================== */
Engine::ValueMenuEntry::ValueMenuEntry(const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize, const Rendering::Texture* iconTexture,
	const Math::Vector2D& screenPosition, Math::Real maxLineLength, const Rendering::Color& textColor, const Rendering::Color& outlineColor, const Math::Vector2D& offset,
	bool isCentered /* = false */, Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */,
	Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	MenuEntry(text, font, fontSize, iconTexture, screenPosition, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth)
{
}


Engine::ValueMenuEntry::~ValueMenuEntry(void)
{
}

void Engine::ValueMenuEntry::Render(Rendering::Renderer* renderer, const Rendering::Shader& guiControlShader) const
{
	MenuEntry::Render(renderer, guiControlShader); // base class rendering
	// TODO: Rendering other GUI controls.
}

void Engine::ValueMenuEntry::Dispatch()
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
//bool Engine::MenuEntry::DoesMouseHoverOverChild(int index, Math::Real xPos, Math::Real yPos) const
//{
//	CHECK_CONDITION_RETURN_ENGINE(index >= 0 && index < GetChildrenCount(), "Incorrect index", Utility::ERR,
//		"Cannot find child menu entry AABR. The given index (", index, ") is not within range [0;", GetChildrenCount(), ")");
//	return m_childrenMenuEntries[index]->GetGuiControl().DoesContainPoint(xPos, yPos).IsIntersecting();
//}
//
//bool Engine::MenuEntry::DoesMouseHoverOver(Math::Real xPos, Math::Real yPos) const
//{
//	//Math::IntersectInfo intersectInfo = m_aabr.DoesContainPoint(xPos, yPos);
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