#include "stdafx.h"
#include "GuiImageControl.h"


Rendering::Controls::GuiImageControl::GuiImageControl(Texture* texture, const Math::Vector2D& screenPosition, const Math::Angle& screenRotationAngle, const Math::Vector2D& scale) :
	GuiControl(screenPosition, screenRotationAngle, scale)
{
}


Rendering::Controls::GuiImageControl::~GuiImageControl()
{
}

void Rendering::Controls::GuiImageControl::Draw(const Shader* guiControlShader, const Renderer& renderer) const
{
}

void Rendering::Controls::GuiImageControl::ApplyColorEffect(const Math::Effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyOutlineColorEffect(const Math::Effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyOffsetEffect(const Math::Effects::Effect<Math::Vector2D>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyCharacterWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyCharacterEdgeTransitionWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyBorderWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyBorderEdgeTransitionWidthEffect(const Math::Effects::Effect<Math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}