#include "stdafx.h"
#include "GuiCheckBoxControl.h"


Rendering::Controls::GuiCheckBoxControl::GuiCheckBoxControl(Texture* texture, const Math::Vector2D& screenPosition, const Math::Angle& screenRotationAngle) :
	GuiControl(screenPosition, screenRotationAngle),
	m_texture(texture)
{
}


Rendering::Controls::GuiCheckBoxControl::~GuiCheckBoxControl()
{
}

void Rendering::Controls::GuiCheckBoxControl::Draw(Rendering::Renderer* renderer) const
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support rendering yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyColorEffect(const Math::Effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyOutlineColorEffect(const Math::Effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyOffsetEffect(const Math::Effects::Effect<Math::Vector2D>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyCharacterWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyCharacterEdgeTransitionWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyBorderWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyBorderEdgeTransitionWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}