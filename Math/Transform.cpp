#include "StdAfx.h"
#include "Transform.h"

math::Transform::Transform(const Vector3D& pos /* = Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO) */,
	const Quaternion& rotation /* = math::NO_ROTATION_QUATERNION */,
	Real scale /* = REAL_ONE */) :
	Transform(pos.x, pos.y, pos.z, rotation, scale)
{
}

math::Transform::Transform(Real posX, Real posY, Real posZ, const Quaternion& rot /* = math::NO_ROTATION_QUATERNION */, Real scale /* = REAL_ONE */) :
	m_pos(posX, posY, posZ),
	m_rotation(rot),
	m_scale(scale),
	m_parentTransform(nullptr),
	m_transformation(),
	m_parentTransformation(),
	m_isChanged(true)
{
}

//math::Transform::Transform(const Transform& transform) :
//	m_pos(transform.m_pos),
//	m_rotation(transform.m_rotation),
//	m_scale(transform.m_scale),
//	m_parentTransform(transform.m_parentTransform),
//	m_transformation(transform.m_transformation),
//	m_parentTransformation(transform.m_parentTransformation),
//	m_isChanged(true)
//{
//}

//math::Transform::Transform(Transform&& transform) :
//	m_pos(std::move(transform.m_pos)),
//	m_rotation(std::move(transform.m_rotation)),
//	m_scale(std::move(transform.m_scale)),
//	m_parentTransform(std::move(transform.m_parentTransform)),
//	m_transformation(std::move(transform.m_transformation)),
//	m_parentTransformation(std::move(transform.m_parentTransformation)),
//	m_isChanged(true)
//{
//	//transform.m_parentTransform = nullptr;
//}

//math::Transform& math::Transform::operator=(const Transform& transform)
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

//math::Transform& math::Transform::operator=(Transform&& transform)
//{
//	m_pos = std::move(transform.m_pos);
//	m_rotation = std::move(transform.m_rotation);
//	m_scale = std::move(transform.m_scale);
//	m_parentTransform = std::move(transform.m_parentTransform);
//	m_transformation = std::move(transform.m_transformation);
//	m_parentTransformation = std::move(transform.m_parentTransformation);
//	m_isChanged = true;
//
//	transform.m_parentTransform = nullptr;
//
//	return *this;
//}

math::Transform::~Transform()
{
	//DEBUG_LOG_MATH("Transform is being destroyed");
}

//math::Transform& math::Transform::operator=(const Transform& transform)
//{
//	m_pos = transform.m_pos;
//	m_rotation = transform.m_rotation;
//	m_scale = transform.m_scale;
//	m_parentTransform = transform.m_parentTransform;
//	m_transformation = transform.m_transformation;
//	m_parentTransformation = transform.m_parentTransformation;
//	m_isChanged = transform.m_isChanged;
//	return *this;
//}

//math::Transform& math::Transform::operator=(Transform&& transform)
//{
//	m_pos = std::move(transform.m_pos);
//	m_rotation = std::move(transform.m_rotation);
//	m_scale = std::move(transform.m_scale);
//	m_parentTransform = std::move(transform.m_parentTransform);
//	m_transformation = std::move(transform.m_transformation);
//	m_parentTransformation = std::move(transform.m_parentTransformation);
//	m_isChanged = std::move(transform.m_isChanged);
//	return *this;
//}

//bool math::Transform::IsHierarchyChanged() const
//{
//	// TODO: Check this function
//	if ((parentTransform == nullptr) || isChanged)
//	{
//		return isChanged;
//	}
//
//	// parentTransform != nullptr && isChanged == false
//	return parentTransform->IsHierarchyChanged();
//}

// TODO: Just temporary. Remove in the future
int isChangedCount = 0;
int isNotChangedCount = 0;

math::Matrix4D math::Transform::GetTransformation() const
{
	// TODO: Fix this function
	if (m_isChanged)
	{
		isChangedCount++; // TODO: just temporary. Remove in the future
		//if ((isChangedCount < 4) || (isNotChangedCount < 10) || (isChangedCount % 10000 == 0))
		//{
		//	DEBUG_LOG_MATH("IsChangedCount = ", isChangedCount, ";\t IsNotChangedCount = ", isNotChangedCount);
		//}

		//Matrix4D scaleMatrix(m_scale);

		// TODO: Include multiplying by scale matrix only if scale is not 1.0.
		// TODO: Cache somewhere the rotation matrix of the quaternion.
		m_transformation = Matrix4D(m_pos) * m_rotation.ToRotationMatrix() * Matrix4D(m_scale); // FIXME: Check matrix multiplication
		//m_transformation *= m_scale;
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

	if (m_parentTransform == nullptr)
	{
		return m_transformation;
	}
	CalculateParentTransformation();
	//parentTransformation = parentTransform->GetTransformation();
	return m_parentTransformation * m_transformation; // FIXME: Check matrix multiplication
}

//math::Matrix4D math::Transform::GetProjectedTransformation(const Camera& camera) const
//{
//	Matrix4D transformationMatrix = GetTransformation();
//
//	Matrix4D result = camera.GetViewProjection() * transformationMatrix;
//
//	return result;
//}

void math::Transform::Rotate(const Vector3D& axis, const Angle& angle)
{
	Rotate(Quaternion(axis, angle));
	m_isChanged = true;
}

void math::Transform::Rotate(const Quaternion& rot)
{
	//DEBUG_LOG_MATH("Rotating the transformation by the quaternion ", rot);
	m_rotation = (rot * m_rotation).Normalized(); // FIXME: Check quaternion multiplication
	m_isChanged = true;
}

void math::Transform::LookAt(const Vector3D& point, const Vector3D& up)
{
	m_rotation = GetLookAtRotation(point, up);
	m_isChanged = true;
}

math::Quaternion math::Transform::GetLookAtRotation(const Vector3D& point, const Vector3D& up) const
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

void math::Transform::SetParent(Transform* t)
{
	m_parentTransform = t;
	m_isChanged = true;
}


bool math::Transform::operator==(const Transform& transform) const
{
	return GetTransformation() == transform.GetTransformation();
}

bool math::Transform::operator!=(const Transform& transform) const
{
	return !operator==(transform);
}