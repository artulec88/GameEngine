#include "stdafx.h"
#include "GuiCheckBoxControl.h"


Rendering::Controls::GuiCheckBoxControl::GuiCheckBoxControl(Texture* texture, const Math::Vector2D& screenPosition) :
	GuiControl(screenPosition),
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