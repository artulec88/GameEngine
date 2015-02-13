#pragma once

#include "Math\Math.h"
#include <string>
#include <sstream>

namespace Rendering
{

class RENDERING_API Color
{
/* ==================== Constructors and destructors begin ==================== */
public:
	Color(void) : m_red(REAL_ZERO), m_green(REAL_ZERO), m_blue(REAL_ZERO), m_alpha(REAL_ONE) { }
	Color(Math::Real red, Math::Real green, Math::Real blue, Math::Real alpha = REAL_ONE) : m_red(red), m_green(green), m_blue(blue), m_alpha(alpha) { };
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	Math::Real GetRed() const { return m_red; }
	Math::Real GetGreen() const { return m_green; }
	Math::Real GetBlue() const { return m_blue; }
	Math::Real GetAlpha() const { return m_alpha; }

	void SetRed(Math::Real red) { m_red = red; }
	void SetGreen(Math::Real green) { m_green = green; }
	void SetBlue(Math::Real blue) { m_blue = blue; }
	void SetAlpha(Math::Real alpha) { m_alpha = alpha; }

	Color Lerp(const Color& color, Math::Real lerpFactor) const
	{
		return Color(((1 - lerpFactor) * m_red) + (lerpFactor * color.GetRed()), ((1 - lerpFactor) * m_green) + (lerpFactor * color.GetGreen()),
			((1 - lerpFactor) * m_blue) + (lerpFactor * color.GetBlue()), ((1 - lerpFactor) * m_alpha) + (lerpFactor * color.GetAlpha()));
	}
	std::string ToString() const
	{
		std::stringstream ss("");
		ss << "(red=" << m_red << "; green=" << m_green << "; blue=" << m_blue << "; alpha=" << m_alpha << ")";
		return ss.str();
	}
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Real m_red, m_green, m_blue;
	Math::Real m_alpha;
/* ==================== Non-static member variables end ==================== */
}; /* end class Color */

} /* end namespace Rendering */