#include "stdafx.h"
#include "PlayerComponent.h"


Rendering::PlayerComponent::PlayerComponent(Math::Real speed) :
	GameComponent(),
	m_speed(speed)
{
}


Rendering::PlayerComponent::~PlayerComponent()
{
}

void Rendering::PlayerComponent::Update(Math::Real deltaTime)
{
}

void Rendering::PlayerComponent::KeyEvent(int key, int scancode, int action, int mods)
{
}

void Rendering::PlayerComponent::MouseButtonEvent(int button, int action, int mods)
{
}

void Rendering::PlayerComponent::MousePosEvent(double xPos, double yPos)
{
}