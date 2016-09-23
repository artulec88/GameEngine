#include "StdAfx.h"
#include "LookAtComponent.h"
#include "Utility\ILogger.h"
#include "Math\Quaternion.h"

Engine::LookAtComponent::LookAtComponent(GameNode* gameNode) :
	GameComponent(),
	IUpdateable(),
	m_entityToLookAtTransform(gameNode->GetTransform())
{
}


Engine::LookAtComponent::~LookAtComponent(void)
{
}

void Engine::LookAtComponent::Update(Math::Real delta)
{
	//Math::Vector3D currentCameraPos = renderer->GetCurrentCamera().GetTransform().GetPos();
	Math::Vector3D currentEntityToLookAtPos = m_entityToLookAtTransform.GetTransformedPos();

	//GetTransform().LookAt(currentCameraPos, Math::Vector3D(0, 1, 0));

	//INFO_LOG_ENGINE("Current camera position = ", currentCameraPos.ToString());
	//Math::Quaternion newRot = GetTransform().GetLookAtRotation(currentCameraPos, Math::Vector3D(0, 1, 0));
	//Math::Quaternion interpolatedRotation = GetTransform().GetRot().Slerp(newRot, delta * 5, true);
	//GetTransform().SetRot(interpolatedRotation);
	//GetTransform().Rotate(interpolatedRotation);
	//GetTransform().SetRot(newRot);

	//GetTransform().SetRot((GetTransform().GetRot() + Math::Quaternion(REAL_ZERO, 0.0001f, REAL_ZERO, REAL_ZERO)).Normalized());

	//DEBUG_LOG_ENGINE("LookAtComponent has just set a new rotation");
}