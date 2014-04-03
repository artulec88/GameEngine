#include "StdAfx.h"
#include "Transform.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Math;

Transform::Transform() :
	pos(Vector3D(0.0, 0.0 , 0.0)),
	rotation(Quaternion(0.0, 0.0 , 0.0, 1.0)),
	scale(Vector3D(1.0, 1.0, 1.0)),
	parentTransform(NULL),
	isChanged(true)
{
	//stdlog(Utility::Debug, LOGPLACE, "Transform is being constructed (1)");
}

Transform::Transform(const Vector3D& pos, const Quaternion& rot, const Vector3D& scale) :
	pos(pos),
	rotation(rot),
	scale(scale),
	parentTransform(NULL),
	isChanged(true)
{
	//stdlog(Utility::Debug, LOGPLACE, "Transform is being constructed (2)");
}

Transform::~Transform()
{
	//stdlog(Utility::Debug, LOGPLACE, "Transform is being destructed");
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
		if ((isChangedCount < 4) || (isNotChangedCount < 10) || (isChangedCount % 10000 == 0))
		{
			stdlog(Utility::Debug, LOGPLACE, "IsChangedCount = %d;\t IsNotChangedCount = %d", isChangedCount, isNotChangedCount);
		}

		Matrix4D translationMatrix = Matrix4D::Translation(pos.GetX(), pos.GetY(), pos.GetZ());;
		Matrix4D scaleMatrix = Matrix4D::Scale(scale.GetX(), scale.GetY(), scale.GetZ());

		transformation = translationMatrix * rotation.ToRotationMatrix() * scaleMatrix;
		isChanged = false;
	}
	else /* if (! IsHierarchyChanged()) */
	{
		isNotChangedCount++; // TODO: just temporary. Remove in the future
		if ((isChangedCount < 4) || (isNotChangedCount < 10) || (isNotChangedCount % 10000 == 0))
		{
			stdlog(Utility::Debug, LOGPLACE, "IsChangedCount = %d;\t IsNotChangedCount = %d", isChangedCount, isNotChangedCount);
		}
	}

	/**
	 * Apply SCALING, then ROTATION and TRANSLATION
	 */
	if (parentTransform == NULL)
	{
		return transformation;
	}
	else
	{
		return parentTransform->GetTransformation() * transformation;
	}
}

void Transform::SetTranslation(const Vector3D& pos)
{
	this->pos = pos;
	isChanged = true;
}

void Transform::SetTranslation(Real x, Real y, Real z)
{
	this->pos = Vector3D(x, y, z);
	isChanged = true;
}

void Transform::SetRotation(const Quaternion& rot)
{
	this->rotation = rot;
	isChanged = true;
}

//void Transform::SetRotation(Real x, Real y, Real z, Real w) { this->rotation = Quaternion(x, y, z, w); }
void Transform::SetScale(const Vector3D& scaleVec)
{
	this->scale = scaleVec;
	isChanged = true;
}

//Matrix4D Transform::GetProjectedTransformation(const Camera& camera) const
//{
//	Matrix4D transformationMatrix = GetTransformation();
//
//	Matrix4D result = camera.GetViewProjection() * transformationMatrix;
//
//	return result;
//}