#include "stdafx.h"
#include "MousePicker.h"
#include "CoreEngine.h"
#include "Utility\ILogger.h"

Engine::MousePicker::MousePicker() :
	m_currentRay(REAL_ZERO, REAL_ZERO, REAL_ZERO)
{
}


Engine::MousePicker::~MousePicker()
{
}

void Engine::MousePicker::CalculateCurrentRay(double xPos, double yPos, const Math::Matrix4D& projectionMatrix, const Math::Matrix4D& viewMatrix)
{
	//WARNING_LOG("This function has not been tested yet. Use it with caution.");
	//Math::Real xPosReal = 800.0f;
	//Math::Real yPosReal = 450.0f;

	// Transforming into 3D normalised device space
	Math::Real normalisedDeviceSpaceX = (2.0f * xPos / CoreEngine::GetCoreEngine()->GetWindowWidth()) - 1.0f;
	Math::Real normalisedDeviceSpaceY = 1.0f - (2.0f * yPos / CoreEngine::GetCoreEngine()->GetWindowHeight());
	Math::Real normalisedDeviceSpaceZ = 1.0f; // we don't actually need Z component yet, but whatever...
	Math::Vector3D normalisedDeviceSpaceRay(normalisedDeviceSpaceX, normalisedDeviceSpaceY, normalisedDeviceSpaceZ);
	CRITICAL_LOG("Normalised device space mouse position for viewport coordinates (%.2f, %.2f) = %s", xPos, yPos, normalisedDeviceSpaceRay.ToString().c_str());

	// Transforming into homogeneous clip coordinates
	Math::Vector4D homogeneousClipCoordinatesRay(normalisedDeviceSpaceX, normalisedDeviceSpaceY, -1.0f, 1.0f);
	DEBUG_LOG("Homogeneous clip space mouse position = %s", homogeneousClipCoordinatesRay.ToString().c_str());

	// Transforming into 4D eye space
	Math::Vector4D eyeSpaceRay(projectionMatrix.Inversion() * homogeneousClipCoordinatesRay);
	// Now we manually set the Z, W components to mean "forwards (Z=-1), and not a point (W=0)".
	eyeSpaceRay.SetZ(-1.0f);
	eyeSpaceRay.SetW(0.0f);
	DEBUG_LOG("Eye space ray = %s", eyeSpaceRay.ToString().c_str());

	// Transforming into 4D world coordinates
	Math::Vector3D worldSpaceRay((viewMatrix.Inversion() * eyeSpaceRay).GetXYZ());
	worldSpaceRay.Normalize();
	CRITICAL_LOG("World space ray for position (%.2f, %.2f) = %s", xPos, yPos, worldSpaceRay.ToString().c_str());
}