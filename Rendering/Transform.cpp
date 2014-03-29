#include "StdAfx.h"
#include "Transform.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Math;

Transform::Transform() :
	translation(Vector3D(0.0, 0.0 , 0.0)),
	rotation(Quaternion(0.0, 0.0 , 0.0, 1.0)),
	scale(Vector3D(1.0, 1.0, 1.0))
{
	//stdlog(Utility::Debug, LOGPLACE, "Transform is being constructed (1)");
}

Transform::Transform(const Vector3D& pos, const Quaternion& rot, const Vector3D& scale) :
	translation(pos),
	rotation(rot),
	scale(scale)
{
	//stdlog(Utility::Debug, LOGPLACE, "Transform is being constructed (2)");
}

Transform::~Transform()
{
	//stdlog(Utility::Debug, LOGPLACE, "Transform is being destructed");
}

Matrix4D Transform::GetTransformation() const
{
	Matrix4D translationMatrix;
	//Matrix4D rotationMatrix;
	Matrix4D scaleMatrix;

	translationMatrix = Matrix4D::Translation(translation.GetX(), translation.GetY(), translation.GetZ());
	//rotationMatrix = Matrix4D::Rotation(rotation.GetX(), rotation.GetY(), rotation.GetZ(), 45.0 /*angleInDegrees*/);
	//rotationMatrix = Matrix4D::Rotation(rotation.GetX(), rotation.GetY(), rotation.GetZ());
	scaleMatrix = Matrix4D::Scale(scale.GetX(), scale.GetY(), scale.GetZ());

	/**
	 * Apply SCALING, then ROTATION and TRANSLATION
	 */
	Matrix4D result = translationMatrix * rotation.ToRotationMatrix() * scaleMatrix;
	//Matrix4D result = Matrix4D::Identity();
	//Matrix4D result = rotationMatrix;

	return result;
}

Vector3D& Transform::GetPos() //TODO: Add const keyword
{
	return translation;
}

Quaternion& Transform::GetRot() //TODO: Add const keyword
{
	return rotation;
}

Vector3D& Transform::GetScale()
{
	return scale;
}

void Transform::SetTranslation(const Vector3D& pos)
{
	this->translation = pos;
}

void Transform::SetTranslation(Real x, Real y, Real z)
{
	this->translation = Vector3D(x, y, z);
}

void Transform::SetRotation(const Quaternion& rot)
{
	this->rotation = rot;
}

//void Transform::SetRotation(Real x, Real y, Real z, Real w) { this->rotation = Quaternion(x, y, z, w); }
void Transform::SetScale(const Vector3D& scaleVec) { this->scale = scaleVec; }

//Matrix4D Transform::GetProjectedTransformation(const Camera& camera) const
//{
//	Matrix4D transformationMatrix = GetTransformation();
//
//	Matrix4D result = camera.GetViewProjection() * transformationMatrix;
//
//	return result;
//}