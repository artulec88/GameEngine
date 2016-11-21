#include "stdafx.h"
#include "GuiControl.h"

#include "Utility\ILogger.h"

Rendering::Controls::GuiControl::GuiControl(const Math::Vector2D& screenPosition, const Math::Angle& screenRotationAngle, const Math::Vector2D& scale) :
	m_screenPosition(screenPosition),
	m_screenRotation(screenRotationAngle),
	m_scale(scale),
	m_transformMatrix(m_screenPosition, m_screenRotation, m_scale),
	m_aabr(screenPosition, screenPosition) // TODO: Not correct if screen rotation angle != 0
{
}


Rendering::Controls::GuiControl::~GuiControl()
{
}

void Rendering::Controls::GuiControl::MoveAABR(Math::Real bottomLeftOffsetX, Math::Real bottomLeftOffsetY, Math::Real topRightOffsetX, Math::Real topRightOffsetY)
{
	m_aabr.SetBottomLeftPos(Math::Vector2D(m_screenPosition.x + bottomLeftOffsetX, (m_screenPosition.y + bottomLeftOffsetY)));
	m_aabr.SetTopRightPos(Math::Vector2D(m_screenPosition.x + topRightOffsetX, m_screenPosition.y + topRightOffsetY));
	DELOCUST_LOG_RENDERING("AABR for the GUI control is:\n\t", m_aabr.GetBottomLeftPos(), "\n\t", m_aabr.GetTopRightPos());
}
