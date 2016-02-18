#include "StdAfx.h"
#include "LookAtComponent.h"
#include "GameNode.h"
#include "Renderer.h"
#include "Utility\ILogger.h"
#include "Math\Quaternion.h"
#include "CoreEngine.h"

using namespace Rendering;

LookAtComponent::LookAtComponent(GameNode* gameNode) :
	GameComponent(),
	m_entityToLookAtTransform(gameNode->GetTransform()),
	renderer(Core::CoreEngine::GetCoreEngine()->GetRenderer())
{
}


LookAtComponent::~LookAtComponent(void)
{
}

void LookAtComponent::Update(Math::Real delta)
{
	CHECK_CONDITION(renderer != NULL, Utility::Critical, "Cannot update lookup component due to renderer object being NULL.");
	//Math::Vector3D currentCameraPos = renderer->GetCurrentCamera().GetTransform().GetPos();
	Math::Vector3D currentEntityToLookAtPos = m_entityToLookAtTransform.GetTransformedPos();

	//GetTransform().LookAt(currentCameraPos, Math::Vector3D(0, 1, 0));

	//INFO_LOG("Current camera position = %s", currentCameraPos.ToString().c_str());
	//Math::Quaternion newRot = GetTransform().GetLookAtRotation(currentCameraPos, Math::Vector3D(0, 1, 0));
	//Math::Quaternion interpolatedRotation = GetTransform().GetRot().Slerp(newRot, delta * 5, true);
	//GetTransform().SetRot(interpolatedRotation);
	//GetTransform().Rotate(interpolatedRotation);
	//GetTransform().SetRot(newRot);

	//GetTransform().SetRot((GetTransform().GetRot() + Math::Quaternion(REAL_ZERO, 0.0001f, REAL_ZERO, REAL_ZERO)).Normalized());

	//DEBUG_LOG("LookAtComponent has just set a new rotation");
}