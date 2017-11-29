#ifndef __ENGINE_MENU_ENTRY_H__
#define __ENGINE_MENU_ENTRY_H__

#include "Engine.h"
#include "ActionConstants.h"
#include "IRenderable.h"

#include "Rendering/Renderer.h"
#include "Rendering/Font.h"
#include "Rendering/GuiControl.h"

#include "Math/Vector.h"
#include "Math/Effect.h"

#include <string>
#include <vector>

namespace engine
{
	// TODO: See if the composite pattern could be applied here. See:
	// https://github.com/bethrobson/Head-First-Design-Patterns/tree/master/src/headfirst/designpatterns/composite, or
	// http://blog.lukaszewski.it/2013/11/19/design-patterns-composite/.

	class CompositeMenuEntry;

	/// <summary>
	/// The base class for both:
	/// * standard menu entries (that may or may not contain children menu entries), e.g. "Start", "Quit" or "Options".
	/// * special menu enries used in "Options"-related menus. They have no children, but they have values, e.g. "Shadows enabled ... Yes/No".
	/// </summary>
	class MenuEntry : public IRenderable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API MenuEntry(const std::string& text, const rendering::text::Font* font, math::Real fontSize, const rendering::Texture* iconTexture, const math::Vector2D& screenPosition,
			const math::Angle& screenRotation, const math::Vector2D& scale, math::Real maxLineLength, const rendering::Color& textColor, const rendering::Color& outlineColor, const math::Vector2D& offset, bool isCentered = false,
			math::Real characterWidth = 0.5f, math::Real characterEdgeTransitionWidth = 0.1f, math::Real borderWidth = 0.4f, math::Real borderEdgeTransitionWidth = 0.1f);
		ENGINE_API virtual ~MenuEntry();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Render(int guiControlShaderID, rendering::Renderer* renderer) const override;
		ENGINE_API virtual void AddChild(MenuEntry* menuEntry);
		ENGINE_API virtual void Dispatch() = 0;
		ENGINE_API virtual MenuEntry* GoTo() { return this; }
		ENGINE_API virtual void ApplyColorEffect(const math::effects::Effect<rendering::Color>& effect);
		ENGINE_API virtual void ApplyOutlineColorEffect(const math::effects::Effect<rendering::Color>& effect);
		ENGINE_API virtual void ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect);
		ENGINE_API virtual void ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect);
		ENGINE_API virtual void ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect);
		ENGINE_API virtual void ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect);
		ENGINE_API virtual void ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect);

		ENGINE_API bool DoesMouseHoverOver(math::Real xPos, math::Real yPos) const;
		ENGINE_API bool HasParent() const { return m_parentMenuEntry != nullptr; }
		ENGINE_API CompositeMenuEntry* GetParent() const { return m_parentMenuEntry; }
		void SetParent(CompositeMenuEntry* parentMenuEntry);
	private:
		const rendering::controls::GuiControl& GetGuiControl() const { return *m_guiControl; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		CompositeMenuEntry* m_parentMenuEntry;
		std::unique_ptr<rendering::controls::GuiControl> m_guiControl;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MenuEntry */

	/// <summary>
	/// The composite menu entry in the game engine.
	/// </summary>
	class CompositeMenuEntry : public MenuEntry
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API CompositeMenuEntry(const std::string& text, const rendering::text::Font* font, math::Real fontSize, const rendering::Texture* iconTexture,
			const math::Vector2D& screenPosition, const math::Angle& screenRotation, const math::Vector2D& scale, math::Real maxLineLength, const rendering::Color& textColor, const rendering::Color& outlineColor, const math::Vector2D& offset,
			bool isCentered = false, math::Real characterWidth = 0.5f, math::Real characterEdgeTransitionWidth = 0.1f, math::Real borderWidth = 0.4f, math::Real borderEdgeTransitionWidth = 0.1f);
		ENGINE_API virtual ~CompositeMenuEntry();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		//ENGINE_API void Render(int guiControlShaderID, Rendering::Renderer* renderer) const override;
		ENGINE_API void AddChild(MenuEntry* menuEntry) override;
		ENGINE_API void Dispatch() override;
		ENGINE_API MenuEntry* GoTo() override { return m_childrenMenuEntries[m_selectedMenuEntryIndex]; }
		ENGINE_API void ApplyColorEffectToAll(const math::effects::Effect<rendering::Color>& effect);
		ENGINE_API void ApplyOutlineColorEffectToAll(const math::effects::Effect<rendering::Color>& effect);
		ENGINE_API void ApplyOffsetEffectToAll(const math::effects::Effect<math::Vector2D>& effect);
		ENGINE_API void ApplyCharacterWidthEffectToAll(const math::effects::Effect<math::Real>& effect);
		ENGINE_API void ApplyCharacterEdgeTransitionWidthEffectToAll(const math::effects::Effect<math::Real>& effect);
		ENGINE_API void ApplyBorderWidthEffectToAll(const math::effects::Effect<math::Real>& effect);
		ENGINE_API void ApplyBorderEdgeTransitionWidthEffectToAll(const math::effects::Effect<math::Real>& effect);

		ENGINE_API void RenderAll(int guiControlShaderID, rendering::Renderer* renderer) const;
		ENGINE_API size_t GetSelectedChildIndex() const { return m_selectedMenuEntryIndex; }
		ENGINE_API size_t GetSelectedChildIndex(math::Real xPos, math::Real yPos) const;
		ENGINE_API MenuEntry* SelectPrevChild();
		ENGINE_API MenuEntry* SelectNextChild();
		ENGINE_API MenuEntry* SelectChild(size_t index);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		std::vector<MenuEntry*> m_childrenMenuEntries;
		size_t m_selectedMenuEntryIndex;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CompositeMenuEntry */

	class ActionMenuEntry : public MenuEntry
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ActionMenuEntry(engine::actions::Action actionID, const std::string& text, const rendering::text::Font* font, math::Real fontSize, const rendering::Texture* iconTexture,
			const math::Vector2D& screenPosition, const math::Angle& screenRotation, const math::Vector2D& scale, math::Real maxLineLength, const rendering::Color& textColor, const rendering::Color& outlineColor, const math::Vector2D& offset,
			bool isCentered = false, math::Real characterWidth = 0.5f, math::Real characterEdgeTransitionWidth = 0.1f, math::Real borderWidth = 0.4f, math::Real borderEdgeTransitionWidth = 0.1f);
		ENGINE_API virtual ~ActionMenuEntry();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		//ENGINE_API virtual void Render(Rendering::Renderer* renderer, const Rendering::Shader& guiControlShader) const;
		ENGINE_API void Dispatch() override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		actions::Action m_actionID;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ActionMenuEntry */

	/// <summary>
	/// The menu entry in the game engine that has a "value" which can be modified by the user, e.g. by clicking on it.
	/// It is useful mainly in the "Options"-type menus where different settings are presented to the user and can be modified / enabled / disabled.
	/// </summary>
	class ValueMenuEntry : public MenuEntry
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ValueMenuEntry(const std::string& text, const rendering::text::Font* font, math::Real fontSize, const rendering::Texture* iconTexture,
			const math::Vector2D& screenPosition, const math::Angle& screenRotation, const math::Vector2D& scale, math::Real maxLineLength, const rendering::Color& textColor, const rendering::Color& outlineColor, const math::Vector2D& offset,
			bool isCentered = false, math::Real characterWidth = 0.5f, math::Real characterEdgeTransitionWidth = 0.1f, math::Real borderWidth = 0.4f, math::Real borderEdgeTransitionWidth = 0.1f);
		ENGINE_API virtual ~ValueMenuEntry();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void Render(int guiControlShaderID, rendering::Renderer* renderer) const override;
		ENGINE_API void Dispatch() override;

		//template<class _Ty, class... _Types> inline typename enable_if<!is_array<_Ty>::value, unique_ptr<_Ty> >::type make_unique(_Types&&... _Args)
		//{	// make a unique_ptr
		//	return (unique_ptr<_Ty>(new _Ty(_STD forward<_Types>(_Args)...)));
		//}

		//template <class T, typenames... Args>
		//ENGINE_API void AddGuiControl()
		//{
		//}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		std::vector<std::string> m_values;
		std::vector<std::unique_ptr<rendering::controls::GuiControl>> m_guiControls;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ValueMenuEntry */

} /* end namespace engine */

#endif /* __ENGINE_MENU_ENTRY_H__ */