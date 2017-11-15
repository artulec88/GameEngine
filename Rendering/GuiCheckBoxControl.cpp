#include "stdafx.h"
#include "GuiCheckBoxControl.h"


Rendering::Controls::GuiCheckBoxControl::GuiCheckBoxControl(Texture* texture, const math::Vector2D& screenPosition, const math::Angle& screenRotationAngle, const math::Vector2D& scale) :
	GuiControl(screenPosition, screenRotationAngle, scale),
	m_texture(texture)
{
}


Rendering::Controls::GuiCheckBoxControl::~GuiCheckBoxControl()
{
}

void Rendering::Controls::GuiCheckBoxControl::Draw(const Shader* guiControlShader, const Renderer& renderer) const
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support rendering yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyColorEffect(const math::effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyOutlineColorEffect(const math::effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiCheckBoxControl::ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}