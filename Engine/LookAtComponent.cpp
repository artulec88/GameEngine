#include "StdAfx.h"
#include "LookAtComponent.h"
#include "Utility\ILogger.h"
#include "Math\Quaternion.h"

Engine::LookAtComponent::LookAtComponent(GameNode* gameNode) :
	GameComponent(),
	m_entityToLookAtTransform(gameNode->GetTransform())
{
}


Engine::LookAtComponent::~LookAtComponent(void)
{
}

void Engine::LookAtComponent::Update(Math::Real delta)
{
	CHECK_CONDITION_ENGINE(renderer != NULL, Utility::CRITICAL, "Cannot update lookup component due to renderer object being NULL.");
	//Math::Vector3D currentCameraPos = renderer->GetCurrentCamera().GetTransform().GetPos();
	Math::Vector3D currentEntityToLookAtPos = m_entityToLookAtTransform.GetTransformedPos();

	//GetTransform().LookAt(currentCameraPos, Math::Vector3D(0, 1, 0));

	//INFO_LOG_ENGINE("Current camera position = %s", currentCameraPos.ToString().c_str());
	//Math::Quaternion newRot = GetTransform().GetLookAtRotation(currentCameraPos, Math::Vector3D(0, 1, 0));
	//Math::Quaternion interpolatedRotation = GetTransform().GetRot().Slerp(newRot, delta * 5, true);
	//GetTransform().SetRot(interpolatedRotation);
	//GetTransform().Rotate(interpolatedRotation);
	//GetTransform().SetRot(newRot);

	//GetTransform().SetRot((GetTransform().GetRot() + Math::Quaternion(REAL_ZERO, 0.0001f, REAL_ZERO, REAL_ZERO)).Normalized());

	//DEBUG_LOG_ENGINE("LookAtComponent has just set a new rotation");
}