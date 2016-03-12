#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Math\Transform.h"
#include "Math\Quaternion.h"

///* static */ const Math::Real Engine::PhysicsComponent::GRAVITY = 0.001f; // TODO: Don't hard-code any value! Ever!

Engine::PhysicsComponent::PhysicsComponent(Physics::PhysicsObject* physicsObject) :
	GameComponent(),
	m_physicsObject(physicsObject)
{
}


Engine::PhysicsComponent::~PhysicsComponent()
{
	SAFE_DELETE(m_physicsObject);
}

void Engine::PhysicsComponent::Update(Math::Real deltaTime)
{
	//m_physicsObject->Integrate(deltaTime);
}

void Engine::PhysicsComponent::KeyEvent(int key, int scancode, int action, int mods)
{
	DEBUG_LOG("key = %d, scancode = %d, action = %d, mods = %d", key, scancode, action, mods);
	// TODO: Increase acceleration / velocity
}
