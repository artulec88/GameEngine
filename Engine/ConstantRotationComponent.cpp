#include "stdafx.h"
#include "ConstantRotationComponent.h"


Engine::ConstantRotationComponent::ConstantRotationComponent(const Math::Vector3D& rotationAxis, const Math::Angle& rotationSpeedAngle) :
	m_rotationAxis(rotationAxis),
	m_rotationSpeedAngle(rotationSpeedAngle)
{
}


Engine::ConstantRotationComponent::~ConstantRotationComponent()
{
}

void Engine::ConstantRotationComponent::Update(Math::Real elapsedTime)
{
	GetTransform().Rotate(Math::Quaternion(m_rotationAxis, m_rotationSpeedAngle * elapsedTime));
}