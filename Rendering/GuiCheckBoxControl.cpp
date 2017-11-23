#include "stdafx.h"
#include "GuiCheckBoxControl.h"


rendering::controls::GuiCheckBoxControl::GuiCheckBoxControl(Texture* texture, const math::Vector2D& screenPosition, const math::Angle& screenRotationAngle, const math::Vector2D& scale) :
	GuiControl(screenPosition, screenRotationAngle, scale),
	m_texture(texture)
{
}


rendering::controls::GuiCheckBoxControl::~GuiCheckBoxControl()
{
}

void rendering::controls::GuiCheckBoxControl::Draw(const Shader* guiControlShader, const Renderer& renderer) const
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support rendering yet.");
}

void rendering::controls::GuiCheckBoxControl::ApplyColorEffect(const math::effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void rendering::controls::GuiCheckBoxControl::ApplyOutlineColorEffect(const math::effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void rendering::controls::GuiCheckBoxControl::ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void rendering::controls::GuiCheckBoxControl::ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void rendering::controls::GuiCheckBoxControl::ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void rendering::controls::GuiCheckBoxControl::ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}

void rendering::controls::GuiCheckBoxControl::ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiCheckBox control doesn't support applying effects yet.");
}