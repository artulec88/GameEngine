#include "stdafx.h"
#include "MousePicker.h"
#include "CoreEngine.h"
#include "Utility/ILogger.h"

engine::MousePicker::MousePicker() :
	m_currentRay(REAL_ZERO, REAL_ZERO, REAL_ZERO)
{
}


engine::MousePicker::~MousePicker()
{
}

void engine::MousePicker::CalculateCurrentRay(double xPos, double yPos, const math::Matrix4D& projectionMatrix, const math::Matrix4D& viewMatrix)
{
	//WARNING_LOG_ENGINE("This function has not been tested yet. Use it with caution.");
	//math::Real xPosReal = 800.0f;
	//math::Real yPosReal = 450.0f;

	// Transforming into 3D normalised device space
	const auto normalisedDeviceSpaceX = (2.0f * xPos / CoreEngine::GetCoreEngine()->GetWindowWidth()) - 1.0f;
	const auto normalisedDeviceSpaceY = 1.0f - (2.0f * yPos / CoreEngine::GetCoreEngine()->GetWindowHeight());
	const auto normalisedDeviceSpaceZ = 1.0f; // we don't actually need Z component yet, but whatever...
	math::Vector3D normalisedDeviceSpaceRay(normalisedDeviceSpaceX, normalisedDeviceSpaceY, normalisedDeviceSpaceZ);
	DEBUG_LOG_ENGINE("Normalised device space mouse position for viewport coordinates (", xPos, ", ", yPos, ") = ", normalisedDeviceSpaceRay);

	// Transforming into homogeneous clip coordinates
	math::Vector4D homogeneousClipCoordinatesRay(normalisedDeviceSpaceX, normalisedDeviceSpaceY, -1.0f, 1.0f);
	DEBUG_LOG_ENGINE("Homogeneous clip space mouse position = ", homogeneousClipCoordinatesRay);

	// Transforming into 4D eye space
	math::Vector4D eyeSpaceRay(projectionMatrix.Inversion() * homogeneousClipCoordinatesRay);
	// Now we manually set the Z, W components to mean "forwards (Z=-1), and not a point (W=0)".
	eyeSpaceRay.z = -1.0f;
	eyeSpaceRay.w = 0.0f;
	DEBUG_LOG_ENGINE("Eye space ray = ", eyeSpaceRay);

	// Transforming into 4D world coordinates
	math::Vector3D worldSpaceRay((viewMatrix.Inversion() * eyeSpaceRay).GetXyz());
	worldSpaceRay.Normalize();
	DEBUG_LOG_ENGINE("World space ray for position (", xPos, ", ", yPos, ") = ", worldSpaceRay);
}