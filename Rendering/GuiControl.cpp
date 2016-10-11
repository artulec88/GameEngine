#include "stdafx.h"
#include "GuiControl.h"

#include "Utility\ILogger.h"

Rendering::Controls::GuiControl::GuiControl(const Math::Vector2D& screenPosition) :
	m_screenPosition(screenPosition),
	m_aabr(screenPosition, screenPosition),
	m_mesh(nullptr)
{
}


Rendering::Controls::GuiControl::~GuiControl()
{
}

void Rendering::Controls::GuiControl::MoveAABR(Math::Real bottomLeftOffsetX, Math::Real bottomLeftOffsetY, Math::Real topRightOffsetX, Math::Real topRightOffsetY)
{
	m_aabr.SetBottomLeftPos(Math::Vector2D(m_screenPosition.GetX() + bottomLeftOffsetX, (m_screenPosition.GetY() + bottomLeftOffsetY)));
	m_aabr.SetTopRightPos(Math::Vector2D(m_screenPosition.GetX() + topRightOffsetX, m_screenPosition.GetY() + topRightOffsetY));
	DELOCUST_LOG_RENDERING("AABR for the GUI control is:\n\t", m_aabr.GetBottomLeftPos(), "\n\t", m_aabr.GetTopRightPos());
}
