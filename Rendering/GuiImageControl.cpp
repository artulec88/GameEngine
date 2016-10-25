#include "stdafx.h"
#include "GuiImageControl.h"
#include "Renderer.h"
#include "MeshIDs.h"

Rendering::Controls::GuiImageControl::GuiImageControl(Texture* texture, const Math::Vector2D& screenPosition, const Math::Angle& screenRotationAngle, const Math::Vector2D& scale) :
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