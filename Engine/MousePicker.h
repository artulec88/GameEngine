#ifndef __ENGINE_MOUSE_PICKER_H__
#define __ENGINE_MOUSE_PICKER_H__

#include "Engine.h"

#include "Math\Vector.h"

namespace Engine
{

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
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		Math::Vector3D m_currentRay;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class MousePicker */

} /* end namespace Engine */

#endif // __ENGINE_MOUSE_PICKER_H__
