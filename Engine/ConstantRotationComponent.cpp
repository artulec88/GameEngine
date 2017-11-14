#include "stdafx.h"
#include "ConstantRotationComponent.h"


engine::ConstantRotationComponent::ConstantRotationComponent(const Math::Vector3D& rotationAxis, const Math::Angle& rotationSpeedAngle) :
	m_rotationAxis(rotationAxis),
	m_rotationSpeedAngle(rotationSpeedAngle)
{
}


engine::ConstantRotationComponent::~ConstantRotationComponent()
{
}

void engine::ConstantRotationComponent::Update(Math::Real elapsedTime)
{
	GetTransform().Rotate(Math::Quaternion(m_rotationAxis, m_rotationSpeedAngle * elapsedTime));
}