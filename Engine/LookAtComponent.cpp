#include "StdAfx.h"
#include "LookAtComponent.h"
#include "Utility/ILogger.h"
#include "Math/Quaternion.h"

engine::LookAtComponent::LookAtComponent(GameNode* gameNode) :
	GameComponent(),
	IUpdateable(),
	m_entityToLookAtTransform(gameNode->GetTransform())
{
}


engine::LookAtComponent::~LookAtComponent()
{
}

void engine::LookAtComponent::Update(math::Real delta)
{
	//math::Vector3D currentCameraPos = renderer->GetCurrentCamera().GetTransform().GetPos();
	const auto currentEntityToLookAtPos = m_entityToLookAtTransform.GetTransformedPos();

	//GetTransform().LookAt(currentCameraPos, math::Vector3D(0, 1, 0));

	//INFO_LOG_ENGINE("Current camera position = ", currentCameraPos.ToString());
	//math::Quaternion newRot = GetTransform().GetLookAtRotation(currentCameraPos, math::Vector3D(0, 1, 0));
	//math::Quaternion interpolatedRotation = GetTransform().GetRot().Slerp(newRot, delta * 5, true);
	//GetTransform().SetRot(interpolatedRotation);
	//GetTransform().Rotate(interpolatedRotation);
	//GetTransform().SetRot(newRot);

	//GetTransform().SetRot((GetTransform().GetRot() + math::Quaternion(REAL_ZERO, 0.0001f, REAL_ZERO, REAL_ZERO)).Normalized());

	//DEBUG_LOG_ENGINE("LookAtComponent has just set a new rotation");
}