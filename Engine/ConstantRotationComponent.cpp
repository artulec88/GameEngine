#include "stdafx.h"
#include "ConstantRotationComponent.h"


engine::ConstantRotationComponent::ConstantRotationComponent(const math::Vector3D& rotationAxis, const math::Angle& rotationSpeedAngle) :
	m_rotationAxis(rotationAxis),
	m_rotationSpeedAngle(rotationSpeedAngle)
{
}


engine::ConstantRotationComponent::~ConstantRotationComponent()
{
}

void engine::ConstantRotationComponent::Update(math::Real elapsedTime)
{
	GetTransform().Rotate(math::Quaternion(m_rotationAxis, m_rotationSpeedAngle * elapsedTime));
}