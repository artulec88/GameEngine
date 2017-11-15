#include "stdafx.h"
#include "GuiImageControl.h"
#include "Renderer.h"
#include "MeshIDs.h"

Rendering::Controls::GuiImageControl::GuiImageControl(Texture* texture, const math::Vector2D& screenPosition, const math::Angle& screenRotationAngle, const math::Vector2D& scale) :
	GuiControl(screenPosition, screenRotationAngle, scale),
	m_texture(texture)
{
}


Rendering::Controls::GuiImageControl::~GuiImageControl()
{
	//SAFE_DELETE(m_texture);
}

void Rendering::Controls::GuiImageControl::Draw(const Shader* guiControlShader, const Renderer& renderer) const
{
	guiControlShader->Bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	m_texture->Bind(0);
	guiControlShader->SetUniformMatrix("guiTransformationMatrix", GetTransformMatrix());
	guiControlShader->SetUniformi("guiTexture", 0);
	renderer.GetMesh(MeshIDs::DEBUG)->Draw();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Rendering::Controls::GuiImageControl::ApplyColorEffect(const math::effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyOutlineColorEffect(const math::effects::Effect<Color>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyOffsetEffect(const math::effects::Effect<math::Vector2D>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyCharacterWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyCharacterEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyBorderWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}

void Rendering::Controls::GuiImageControl::ApplyBorderEdgeTransitionWidthEffect(const math::effects::Effect<math::Real>& effect)
{
	ERROR_LOG_RENDERING("GuiImage control doesn't support applying effects yet.");
}