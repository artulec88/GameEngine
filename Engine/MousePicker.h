#ifndef __ENGINE_MOUSE_PICKER_H__
#define __ENGINE_MOUSE_PICKER_H__

#include "Engine.h"

#include "Math/Vector.h"
#include "Math\Matrix.h"

namespace engine
{
	/// <summary>
	/// The mouse picker class. It casts a ray from the camera's position in the specified direction.
	/// It is a basic object to use when determining which entity in the scene or which menu entry the user wants to select.
	/// </summary>
	/// <remarks>
	/// The class was based on the following article http://antongerdelan.net/opengl/raycasting.html.
	/// Only one instance of this class is needed by the engine, so we should only create one instance of this object.
	/// </remarks>
	class MousePicker
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API MousePicker();
		ENGINE_API ~MousePicker(void);
	private:
		MousePicker(MousePicker& mousePicker);
		void operator=(MousePicker& mousePicker);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary>
		/// </summary>
		/// <param name="xPos">The X component of the 2D viewport coordinates.</param>
		/// <param name="yPos">The Y component of the 2D viewport coordinates.</param>
		/// <param name="projectionMatrix">The projection matrix of the camera.</param>
		/// <param name="viewMatrix">The view matrix of the camera.</param>
		ENGINE_API void CalculateCurrentRay(double xPos, double yPos, const math::Matrix4D& projectionMatrix, const math::Matrix4D& viewMatrix);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		math::Vector3D m_currentRay;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class MousePicker */

} /* end namespace Engine */

#endif // __ENGINE_MOUSE_PICKER_H__
