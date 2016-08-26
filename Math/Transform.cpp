#include "StdAfx.h"
#include "Transform.h"

#include "Utility\ILogger.h"

#include <sstream>

Math::Transform::Transform(const Vector3D& pos /* = Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO) */, const Quaternion& rot /* = Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE) */, Real scale /* = REAL_ONE */) :
	m_pos(pos),
	m_rotation(rot),
	m_scale(scale),
	m_parentTransform(NULL),
	m_transformation(),
	m_parentTransformation(),
	m_isChanged(true)
{
}

Math::Transform::Transform(const Transform& transform) :
	m_pos(transform.m_pos),
	m_rotation(transform.m_rotation),
	m_scale(transform.m_scale),
	m_parentTransform(transform.m_parentTransform),
	m_transformation(transform.m_transformation),
	m_parentTransformation(transform.m_parentTransformation),
	m_isChanged(true)
{
}

Math::Transform::Transform(Transform&& transform) :
	m_pos(std::move(transform.m_pos)),
	m_rotation(std::move(transform.m_rotation)),
	m_scale(std::move(transform.m_scale)),
	m_parentTransform(std::move(transform.m_parentTransform)),
	m_transformation(std::move(transform.m_transformation)),
	m_parentTransformation(std::move(transform.m_parentTransformation)),
	m_isChanged(true)
{
	transform.m_parentTransform = NULL;
}

//Math::Transform& Math::Transform::operator=(const Transform& transform)
//{
//	m_pos = transform.m_pos;
//	m_rotation = transform.m_rotation;
//	m_scale = transform.m_scale;
//	m_parentTransform = transform.m_parentTransform;
//	m_transformation = transform.m_transformation;
//	m_parentTransformation = transform.m_parentTransformation;
//	m_isChanged = true;
//
//	return *this;
//}

//Math::Transform& Math::Transform::operator=(Transform&& transform)
//{
//	m_pos = std::move(transform.m_pos);
//	m_rotation = std::move(transform.m_rotation);
//	m_scale = std::move(transform.m_scale);
//	m_parentTransform = std::move(transform.m_parentTransform);
//	m_transformation = std::move(transform.m_transformation);
//	m_parentTransformation = std::move(transform.m_parentTransformation);
//	m_isChanged = true;
//
//	transform.m_parentTransform = NULL;
//
//	return *this;
//}

Math::Transform::~Transform()
{
	//DEBUG_LOG_MATH("Transform is being destroyed");
}

Math::Transform& Math::Transform::operator=(const Transform& transform)
{
	m_pos = transform.m_pos;
	m_rotation = transform.m_rotation;
	m_scale = transform.m_scale;
	m_parentTransform = transform.m_parentTransform;
	m_transformation = transform.m_transformation;
	m_parentTransformation = transform.m_parentTransformation;
	m_isChanged = transform.m_isChanged;
	return *this;
}

Math::Transform& Math::Transform::operator=(Transform&& transform)
{
	m_pos = std::move(transform.m_pos);
	m_rotation = std::move(transform.m_rotation);
	m_scale = std::move(transform.m_scale);
	m_parentTransform = std::move(transform.m_parentTransform);
	m_transformation = std::move(transform.m_transformation);
	m_parentTransformation = std::move(transform.m_parentTransformation);
	m_isChanged = std::move(transform.m_isChanged);
	return *this;
}

//bool Math::Transform::IsHierarchyChanged() const
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

Math::Matrix4D Math::Transform::GetTransformation() const
{
	// TODO: Fix this function
	if (m_isChanged)
	{
		isChangedCount++; // TODO: just temporary. Remove in the future
		//if ((isChangedCount < 4) || (isNotChangedCount < 10) || (isChangedCount % 10000 == 0))
		//{
		//	DEBUG_LOG_MATH("IsChangedCount = ", isChangedCount, ";\t IsNotChangedCount = ", isNotChangedCount);
		//}

		Matrix4D translationMatrix(m_pos);
		Matrix4D scaleMatrix(m_scale);

		m_transformation = translationMatrix * m_rotation.ToRotationMatrix() * scaleMatrix; // FIXME: Check matrix multiplication
		m_isChanged = false;
	}
	else /* if (! IsHierarchyChanged()) */
	{
		isNotChangedCount++; // TODO: just temporary. Remove in the future
		//if ((isChangedCount < 4) || (isNotChangedCount < 10) || (isNotChangedCount % 10000 == 0))
		//{
		//	DEBUG_LOG_MATH("IsChangedCount = ", isChangedCount, ";\t IsNotChangedCount = ", isNotChangedCount);
		//}
	}

	if (m_parentTransform == NULL)
	{
		return m_transformation;
	}
	else
	{
		CalculateParentTransformation();
		//parentTransformation = parentTransform->GetTransformation();
		return m_parentTransformation * m_transformation; // FIXME: Check matrix multiplication
	}
}

//Math::Matrix4D Math::Transform::GetProjectedTransformation(const Camera& camera) const
//{
//	Matrix4D transformationMatrix = GetTransformation();
//
//	Matrix4D result = camera.GetViewProjection() * transformationMatrix;
//
//	return result;
//}

void Math::Transform::Rotate(const Vector3D& axis, const Angle& angle)
{
	Rotate(Quaternion(axis, angle));
	m_isChanged = true;
}

void Math::Transform::Rotate(const Quaternion& rot)
{
	//DEBUG_LOG_MATH("Rotating the transformation by the quaternion ", rot.ToString());
	m_rotation = (rot * m_rotation).Normalized(); // FIXME: Check quaternion multiplication
	m_isChanged = true;
}

void Math::Transform::LookAt(const Vector3D& point, const Vector3D& up)
{
	m_rotation = GetLookAtRotation(point, up);
	m_isChanged = true;
}

Math::Quaternion Math::Transform::GetLookAtRotation(const Vector3D& point, const Vector3D& up) const
{
	return Quaternion(Matrix4D((point - m_pos).Normalized(), up));
	//Vector3D forward = point - pos;
	//forward.Normalize();
	//
	////Vector3D right = forward.Cross(up);
	//Vector3D right = (up.Normalized());
	//right = right.Cross(forward);
	//right.Normalize();

	//Vector3D u = forward.Cross(right);
	//u.Normalize();

	//Matrix4D rotMatrix(forward, u, right); // rotation from vectors
	//return Quaternion(rotMatrix);
}

void Math::Transform::SetParent(Transform* t)
{
	m_parentTransform = t;
}

std::string Math::Transform::ToString() const
{
	std::stringstream ss("");
	ss << "Pos = " << m_pos.ToString() << "; Rot = " << m_rotation.ToString() << "; Scale = " << m_scale;
	return ss.str();
}