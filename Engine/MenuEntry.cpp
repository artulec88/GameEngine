#include "StdAfx.h"
#include "MenuEntry.h"
#include "GameManager.h"

#include "Rendering\GuiText.h"

#include "Math\IntersectInfo.h"

#include "Utility\IConfig.h"
#include "Utility\ILogger.h"

#include <sstream>

/* ==================== MenuEntry class begin ==================== */
Engine::MenuEntry::MenuEntry(const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize, const Math::Vector2D& screenPosition,
	Math::Real maxLineLength, const Math::Vector3D& textColor, const Math::Vector3D& outlineColor, const Math::Vector2D& offset, bool isCentered /* = false */,
	Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */, Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	m_parentMenuEntry(NULL),
	m_guiControl(std::make_unique<Rendering::Controls::GuiTextControl>(text, font, fontSize, screenPosition, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth))
{
}

Engine::MenuEntry::~MenuEntry()
{
}

void Engine::MenuEntry::Render(Rendering::Renderer* renderer) const
{
	renderer->RenderGuiControl(*m_guiControl);
}

void Engine::MenuEntry::AddChild(MenuEntry* childMenuEntry)
{
	ERROR_LOG_ENGINE("Only composite menu entry can have children menu entries.");
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
Engine::CompositeMenuEntry::CompositeMenuEntry(const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize,
	const Math::Vector2D& screenPosition, Math::Real maxLineLength, const Math::Vector3D& textColor, const Math::Vector3D& outlineColor, const Math::Vector2D& offset,
	bool isCentered /* = false */, Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */,
	Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	MenuEntry(text, font, fontSize, screenPosition, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth),
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

void Engine::CompositeMenuEntry::RenderAll(Rendering::Renderer* renderer) const
{
	for (std::vector<MenuEntry*>::const_iterator menuEntryItr = m_childrenMenuEntries.begin(); menuEntryItr != m_childrenMenuEntries.end(); ++menuEntryItr)
	{
		(*menuEntryItr)->Render(renderer);
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

Engine::MenuEntry* Engine::CompositeMenuEntry::Execute()
{
	return m_childrenMenuEntries[m_selectedMenuEntryIndex];
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
		if (index < 0)
		{
			m_selectedMenuEntryIndex = m_childrenMenuEntries.size() - 1;
		}
		else if (index >= m_childrenMenuEntries.size())
		{
			m_selectedMenuEntryIndex = 0;
		}
		else
		{
			m_selectedMenuEntryIndex = index;
		}
	}
	return m_childrenMenuEntries[m_selectedMenuEntryIndex];
}
/* ==================== CompositeMenuEntry class end ==================== */

/* ==================== ActionMenuEntry class begin ==================== */
Engine::ActionMenuEntry::ActionMenuEntry(const GameCommand& gameCommand, const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize,
	const Math::Vector2D& screenPosition, Math::Real maxLineLength, const Math::Vector3D& textColor, const Math::Vector3D& outlineColor, const Math::Vector2D& offset,
	bool isCentered /* = false */, Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */,
	Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	MenuEntry(text, font, fontSize, screenPosition, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth),
	m_gameCommand(gameCommand)
{
}


Engine::ActionMenuEntry::~ActionMenuEntry(void)
{
}

//void Engine::ActionMenuEntry::Render(Rendering::Renderer* renderer) const
//{
//}

Engine::MenuEntry* Engine::ActionMenuEntry::Execute()
{
	m_gameCommand.Execute(GameManager::GetGameManager());
	return this;
}
/* ==================== ActionMenuEntry class end ==================== */

/* ==================== ValueMenuEntry class begin ==================== */
Engine::ValueMenuEntry::ValueMenuEntry(const std::string& text, const Rendering::Text::Font* font, Math::Real fontSize,
	const Math::Vector2D& screenPosition, Math::Real maxLineLength, const Math::Vector3D& textColor, const Math::Vector3D& outlineColor, const Math::Vector2D& offset,
	bool isCentered /* = false */, Math::Real characterWidth /* = 0.5f */, Math::Real characterEdgeTransitionWidth /* = 0.1f */, Math::Real borderWidth /* = 0.4f */,
	Math::Real borderEdgeTransitionWidth /* = 0.1f */) :
	MenuEntry(text, font, fontSize, screenPosition, maxLineLength, textColor, outlineColor, offset, isCentered, characterWidth, characterEdgeTransitionWidth, borderWidth, borderEdgeTransitionWidth)
{
}


Engine::ValueMenuEntry::~ValueMenuEntry(void)
{
}

void Engine::ValueMenuEntry::Render(Rendering::Renderer* renderer) const
{
	MenuEntry::Render(renderer); // base class rendering
	// TODO: Rendering other GUI controls.
}

Engine::MenuEntry* Engine::ValueMenuEntry::Execute()
{
	WARNING_LOG_ENGINE("Still to be implemented.");
	return this;
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
//		"Cannot find child menu entry AABR. The given index (%d) is not within range [0;%d)", index, GetChildrenCount());
//	return m_childrenMenuEntries[index]->GetGuiControl().DoesContainPoint(xPos, yPos).IsIntersecting();
//}
//
//bool Engine::MenuEntry::DoesMouseHoverOver(Math::Real xPos, Math::Real yPos) const
//{
//	//Math::IntersectInfo intersectInfo = m_aabr.DoesContainPoint(xPos, yPos);
//	//DEBUG_LOG_ENGINE("DoesMouseHoverOver(xPos = %.2f, yPos = %.2f) = %.3f", xPos, yPos, intersectInfo.GetDistance());
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
//		"Incorrect child menu entry selected. Given index equals %d while it must be in range [0; %d)", m_selectedMenuEntryIndex, GetChildrenCount());
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
//		"Cannot return currently selected child menu entry. The index (%d) is not within range [0;%d)", m_selectedMenuEntryIndex, GetChildrenCount());
//	return m_childrenMenuEntries[m_selectedMenuEntryIndex];
//}
//
//bool Engine::MenuEntry::DoesSelectedChildHaveChildren() const
//{
//	CHECK_CONDITION_RETURN_ENGINE(m_selectedMenuEntryIndex >= 0 && m_selectedMenuEntryIndex < GetChildrenCount(), false, Utility::ERR,
//		"Cannot determine whether currently selected child menu entry has children. The index (%d) is not within range [0;%d)", m_selectedMenuEntryIndex, GetChildrenCount());
//	return m_childrenMenuEntries[m_selectedMenuEntryIndex]->HasChildren();
//}