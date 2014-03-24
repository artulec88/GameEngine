#include "StdAfx.h"
#include "Transform.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Math;

//TODO: This will need to be changed later to avoid memory issues!
Camera* Transform::s_camera = NULL; //&Camera();

Real Transform::zFar = 0.0;
Real Transform::zNear = 0.0;
Real Transform::fov = 0.0;
Real Transform::width = 0.0;
Real Transform::height = 0.0;

Transform::Transform() :
	translation(Vector3D(0.0, 0.0 , 0.0)),
	rotation(Vector3D(0.0, 0.0 , 0.0)),
	scale(1.0)
{
	//stdlog(Utility::Debug, LOGPLACE, "Transform is being constructed (1)");
}

Transform::Transform(Vector3D& pos, Vector3D& rot, float scale) :
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
	Matrix4D rotationMatrix;
	Matrix4D scaleMatrix;

	translationMatrix = Matrix4D::Translation(translation.GetX(), translation.GetY(), translation.GetZ());
	//rotationMatrix = Matrix4D::Rotation(rotation.GetX(), rotation.GetY(), rotation.GetZ(), 45.0 /*angleInDegrees*/);
	rotationMatrix = Matrix4D::Rotation(rotation.GetX(), rotation.GetY(), rotation.GetZ());
	scaleMatrix = Matrix4D::Scale(scale, scale, scale);

	/**
	 * Apply SCALING, then ROTATION and TRANSLATION
	 */
	Matrix4D result = translationMatrix * rotationMatrix * scaleMatrix;
	//Matrix4D result = Matrix4D::Identity();
	//Matrix4D result = rotationMatrix;

	return result;
}

Math::Vector3D& Transform::GetPos() //TODO: Add const keyword
{
	return translation;
}

Math::Vector3D& Transform::GetRot() //TODO: Add const keyword
{
	return rotation;
}

Real Transform::GetScale() const
{
	return scale;
}

void Transform::SetTranslation(const Math::Vector3D& pos) { this->translation = pos; }
void Transform::SetTranslation(Math::Real x, Math::Real y, Math::Real z) { this->translation = Vector3D(x, y, z); }
void Transform::SetRotation(const Math::Vector3D& rot) { this->rotation = rot; }
void Transform::SetRotation(Math::Real x, Math::Real y, Math::Real z) { this->rotation = Vector3D(x, y, z); }
void Transform::SetScale(Real scale) { this->scale = scale; }

Matrix4D Transform::GetProjectedTransformation() const
{
	Matrix4D transformationMatrix = GetTransformation();
	Matrix4D projectionMatrix = Matrix4D::Projection(fov, static_cast<Real>(width) / height, zNear, zFar);
	Matrix4D cameraRotation = Matrix4D::InitCamera(s_camera->GetForward(), s_camera->GetUp());
	Matrix4D cameraTranslation = Matrix4D::Translation(-s_camera->GetPos());

	//projectionMatrix = Matrix4D::ProjectionInitPersProjTransform(ToRadians(fov), (float)width/(float)height, zNear, zFar);
	//cameraRotation.InitCameraTransform(s_camera->GetForward(), s_camera->GetUp());
	//cameraTranslation.InitTranslationTransform(Vector3D(-s_camera->GetPos().GetX(), -s_camera->GetPos().GetY(), -s_camera->GetPos().GetZ()));

	Matrix4D result = projectionMatrix * cameraRotation * cameraTranslation * transformationMatrix;

	return result;
}

void Transform::SetProjection(Math::Real fov, Math::Real width, Math::Real height, Math::Real zNear, Math::Real zFar)
{
	Transform::fov = fov;
	Transform::width = width;
	Transform::height = height;
	Transform::zNear = zNear;
	Transform::zFar = zFar;
}