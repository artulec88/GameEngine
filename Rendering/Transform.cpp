#include "StdAfx.h"
#include "Transform.h"
#include "Camera.h"
#include "Utility\ILogger.h"

using namespace Rendering;
using namespace Math;

Transform::Transform(const Vector3D& pos /* = Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO) */, const Quaternion& rot /* = Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE) */, Math::Real scale /* = REAL_ONE */) :
	m_pos(pos),
	m_rotation(rot),
	m_scale(scale),
	m_parentTransform(NULL),
	isChanged(true)
{
}

Transform::~Transform()
{
	//DEBUG_LOG("Transform is being destroyed");
}

//bool Transform::IsHierarchyChanged() const
//{
//	// TODO: Check this function
//	if ((parentTransform == NULL) || isChanged)
//	{
//		return isChanged;
//	}
//
//	// parentTransform != NULL && isChanged == false
//	return parentTransform->IsHierarchyChanged();
//}

// TODO: Just temporary. Remove in the future
int isChangedCount = 0;
int isNotChangedCount = 0;

Matrix4D Transform::GetTransformation() const
{
	// TODO: Fix this function
	if (isChanged)
	{
		isChangedCount++; // TODO: just temporary. Remove in the future
		//if ((isChangedCount < 4) || (isNotChangedCount < 10) || (isChangedCount % 10000 == 0))
		//{
		//	DEBUG_LOG("IsChangedCount = %d;\t IsNotChangedCount = %d", isChangedCount, isNotChangedCount);
		//}

		Matrix4D translationMatrix(m_pos);
		Matrix4D scaleMatrix(m_scale);

		transformation = translationMatrix * m_rotation.ToRotationMatrix() * scaleMatrix;
		isChanged = false;
	}
	else /* if (! IsHierarchyChanged()) */
	{
		isNotChangedCount++; // TODO: just temporary. Remove in the future
		//if ((isChangedCount < 4) || (isNotChangedCount < 10) || (isNotChangedCount % 10000 == 0))
		//{
		//	DEBUG_LOG("IsChangedCount = %d;\t IsNotChangedCount = %d", isChangedCount, isNotChangedCount);
		//}
	}

	/**
	 * Apply TRANSLATION, then ROTATION and SCALING
	 */
	if (m_parentTransform == NULL)
	{
		return transformation;
	}
	else
	{
		CalculateParentTransformation();
		//parentTransformation = parentTransform->GetTransformation();
		return parentTransformation * transformation;
	}
}

//Matrix4D Transform::GetProjectedTransformation(const Camera& camera) const
//{
//	Matrix4D transformationMatrix = GetTransformation();
//
//	Matrix4D result = camera.GetViewProjection() * transformationMatrix;
//
//	return result;
//}

void Transform::Rotate(const Math::Vector3D& axis, const Math::Angle& angle)
{
	Rotate(Quaternion(axis, angle));
	isChanged = true;
}

void Transform::Rotate(const Quaternion& rot)
{
	//DEBUG_LOG("Started...");
	m_rotation = (rot * m_rotation).Normalized();
	isChanged = true;
}

void Transform::LookAt(const Math::Vector3D& point, const Math::Vector3D& up)
{
	m_rotation = GetLookAtRotation(point, up);
	isChanged = true;
}

Math::Quaternion Transform::GetLookAtRotation(const Math::Vector3D& point, const Math::Vector3D& up) const
{
	return Quaternion(Math::Matrix4D((point - m_pos).Normalized(), up));
	//Math::Vector3D forward = point - pos;
	//forward.Normalize();
	//
	////Math::Vector3D right = forward.Cross(up);
	//Math::Vector3D right = (up.Normalized());
	//right = right.Cross(forward);
	//right.Normalize();

	//Math::Vector3D u = forward.Cross(right);
	//u.Normalize();

	//Math::Matrix4D rotMatrix(forward, u, right); // rotation from vectors
	//return Math::Quaternion(rotMatrix);
}

void Transform::SetParent(Transform* t)
{
	m_parentTransform = t;
}