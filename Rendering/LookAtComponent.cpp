#include "StdAfx.h"
#include "LookAtComponent.h"
#include "Transform.h"
#include "Renderer.h"
#include "Utility\Log.h"
#include "Math\Quaternion.h"

using namespace Rendering;

LookAtComponent::LookAtComponent(void) :
	GameComponent(),
	renderer(NULL)
{
}


LookAtComponent::~LookAtComponent(void)
{
}

void LookAtComponent::Update(Math::Real delta)
{
	if (renderer == NULL)
	{
		stdlog(Utility::Warning, LOGPLACE, "LookAtComponent does not have a renderer set up.");
		return;
	}

	//Math::Vector3D currentCameraPos = renderer->GetCurrentCamera().GetTransform().GetPos();
	Math::Vector3D currentCameraPos = renderer->GetCurrentCamera().GetTransform().GetTransformedPos();

	//GetTransform().LookAt(currentCameraPos, Math::Vector3D(0, 1, 0));

	Math::Quaternion newRot = GetTransform().GetLookAtDirection(currentCameraPos, Math::Vector3D(0, 1, 0));
	Math::Quaternion interpolatedRotation = GetTransform().GetRot().Nlerp(newRot, delta * 5, true);
	GetTransform().SetRotation(interpolatedRotation);
	//stdlog(Utility::Debug, LOGPLACE, "LookAtComponent has just set a new rotation");
}

void LookAtComponent::Render(Shader* shader, Renderer* renderer)
{
	if (this->renderer == NULL)
	{
		this->renderer = renderer;
	}
}