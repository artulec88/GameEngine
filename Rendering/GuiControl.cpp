#include "stdafx.h"
#include "GuiControl.h"

#include "Utility/ILogger.h"

rendering::controls::GuiControl::GuiControl(const math::Vector2D& screenPosition, const math::Angle& screenRotationAngle, const math::Vector2D& scale) :
	m_screenPosition(screenPosition),
	m_screenRotation(screenRotationAngle),
	m_scale(scale),
	m_transformMatrix(m_screenPosition, m_screenRotation, m_scale),
	m_aabr(screenPosition, screenPosition) // TODO: Not correct if screen rotation angle != 0
{
}


rendering::controls::GuiControl::~GuiControl()
{
}

void rendering::controls::GuiControl::MoveAabr(math::Real bottomLeftOffsetX, math::Real bottomLeftOffsetY, math::Real topRightOffsetX, math::Real topRightOffsetY)
{
	m_aabr.SetBottomLeftPos(math::Vector2D(m_screenPosition.x + bottomLeftOffsetX, m_screenPosition.y + bottomLeftOffsetY));
	m_aabr.SetTopRightPos(math::Vector2D(m_screenPosition.x + topRightOffsetX, m_screenPosition.y + topRightOffsetY));
	DELOCUST_LOG_RENDERING("AABR for the GUI control is:\n\t", m_aabr.GetBottomLeftPos(), "\n\t", m_aabr.GetTopRightPos());
}
