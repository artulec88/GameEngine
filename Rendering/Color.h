#ifndef __RENDERING_COLOR_H__
#define __RENDERING_COLOR_H__

#include "Rendering.h"
#include "Math\Vector.h"
#include <string>
#include <sstream>

namespace Rendering
{
	namespace ColorNames
	{
		/// <summary>
		/// See http://www.w3schools.com/colors/colors_hex.asp.
		/// </summary>
		enum ColorName
		{
			INDIAN_RED = 0, /// <summary> Red-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=205,92,92 </summary>
			LIGHT_CORAL, /// <summary> Red-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=240,128,128 </summary>
			SALMON, /// <summary> Red-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=250,128,114 </summary>
			DARK_SALMON, /// <summary> Red-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=233,150,122 </summary>
			LIGHT_SALMON, /// <summary> Red-Orange-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,160,122 </summary>
			CRIMSON, /// <summary> Red-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=220,20,60 </summary>
			RED, /// <summary> Red-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,0,0 </summary>
			FIRE_BRICK, /// <summary> Red-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=178,34,34 </summary>
			DARK_RED, /// <summary> Red-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=139,0,0 </summary>
			PINK, /// <summary> Pink-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,192,203 </summary>
			LIGHT_PINK, /// <summary> Pink-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,182,193 </summary>
			HOT_PINK, /// <summary> Pink-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,105,180 </summary>
			DEEP_PINK, /// <summary> Pink-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,20,147 </summary>
			MEDIUM_VIOLET_RED, /// <summary> Pink-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=199,21,133 </summary>
			PALE_VIOLET_RED, /// <summary> Pink-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=219,112,147 </summary>
			CORAL, /// <summary> Orange-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,127,80 </summary>
			TOMATO, /// <summary> Orange-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,99,71 </summary>
			ORANGE_RED, /// <summary> Orange-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,69,0 </summary>
			DARK_ORANGE, /// <summary> Orange-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,140,0 </summary>
			ORANGE, /// <summary> Orange-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,165,0 </summary>
			GOLD, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,215,0 </summary>
			YELLOW, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,255,0 </summary>
			LIGHT_YELLOW, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,255,224 </summary>
			LEMON_CHIFFON, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,250,205 </summary>
			LIGHT_GOLDENROD_YELLOW, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=250,250,210 </summary>
			PAPAYA_WHIP, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,239,213 </summary>
			MOCCASIN, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,228,181 </summary>
			PEACH_PUFF, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,218,185 </summary>
			PALE_GOLDENROD, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=238,232,170 </summary>
			KHAKI, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=240,230,140 </summary>
			DARK_KHAKI, /// <summary> Yellow-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=189,183,107 </summary>
			LAVENDER, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=230,230,250 </summary>
			THISTLE, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=216,191,216 </summary>
			PLUM, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=221,160,221 </summary>
			VIOLET, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=238,130,238 </summary>
			ORCHID, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=218,112,214 </summary>
			FUCHSIA, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,0,255 </summary>
			MAGENTA, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,0,255 </summary>
			MEDIUM_ORCHID, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=186,85,211 </summary>
			BLUE_VIOLET, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=138,43,226 </summary>
			DARK_VIOLET, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=148,0,211 </summary>
			DARK_ORCHID, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=153,50,204 </summary>
			DARK_MAGENTA, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=139,0,139 </summary>
			PURPLE, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=128,0,128 </summary>
			INDIGO, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=75,0,130 </summary>
			STATE_BLUE, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=106,90,205 </summary>
			DARK_STATE_BLUE, /// <summary> Purple-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=72,61,139 </summary>
			MEDIUM_STATE_BLUE, /// <summary> Purple-Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=123,104,238 </summary>
			GREEN_YELLOW, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=173,255,47 </summary>
			CHARTREUSE, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=127,255,0 </summary>
			LAWN_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=124,252,0 </summary>
			LIME, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,255,0 </summary>
			LIME_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=50,205,50 </summary>
			PALE_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=152,251,152 </summary>
			LIGHT_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=144,238,144 </summary>
			MEDIUM_SPRING_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,250,154 </summary>
			SPRING_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,255,127 </summary>
			MEDIUM_SEA_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=60,179,113 </summary>
			SEA_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=46,139,87 </summary>
			FOREST_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=34,139,34 </summary>
			GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,128,0 </summary>
			DARK_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,100,0 </summary>
			YELLOW_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=154,205,50 </summary>
			OLIVE_DRAB, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=107,142,35 </summary>
			OLIVE, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=128,128,0 </summary>
			DARK_OLIVE_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=85,107,47 </summary>
			MEDIUM_AQUAMARINE, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=102,205,170 </summary>
			DARK_SEA_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=143,188,143 </summary>
			LIGHT_SEA_GREEN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=32,178,170 </summary>
			DARK_CYAN, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,139,139 </summary>
			TEAL, /// <summary> Green-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,128,128 </summary>
			AQUA, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,255,255 </summary>
			CYAN, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,255,255 </summary>
			LIGHT_CYAN, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=224,255,255 </summary>
			PALE_TURQUOISE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=175,238,238 </summary>
			AQUAMARINE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=127,255,212 </summary>
			TURQUOISE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=64,224,208 </summary>
			MEDIUM_TURQUOISE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=72,209,204 </summary>
			DARK_TURQUOISE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,206,209 </summary>
			CADET_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=95,158,160 </summary>
			STEEL_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=70,130,180 </summary>
			LIGHT_STEEL_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=176,196,222 </summary>
			POWDER_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=176,224,230 </summary>
			LIGHT_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=173,216,230 </summary>
			SKY_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=135,206,235 </summary>
			LIGHT_SKY_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=135,206,250 </summary>
			DEEP_SKY_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,191,255 </summary>
			DODGER_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=30,144,255 </summary>
			CORNFLOWER_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=100,149,237 </summary>
			ROYAL_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=65,105,225 </summary>
			MEDIUM_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,0,205 </summary>
			DARK_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,0,139 </summary>
			NAVY, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,0,128 </summary>
			MIDNIGHT_BLUE, /// <summary> Blue-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=25,25,112 </summary>
			CORNSILK, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,248,220 </summary>
			BLANCHED_ALMOND, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,235,205 </summary>
			BISQUE, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,228,196 </summary>
			NAVAJO_WHITE, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,222,173 </summary>
			WHEAT, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=245,222,179 </summary>
			BURLY_WOOD, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=222,184,135 </summary>
			TAN, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=210,180,140 </summary>
			ROSY_BROWN, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=188,143,143 </summary>
			SANDY_BROWN, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=244,164,96 </summary>
			GOLDENROD, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=218,165,32 </summary>
			DARK_GOLDENROD, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=184,134,11 </summary>
			PERU, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=205,133,63 </summary>
			CHOCOLATE, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=210,105,30 </summary>
			SADDLE_BROWN, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=139,69,19 </summary>
			SIENNA, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=160,82,45 </summary>
			BROWN, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=165,42,42 </summary>
			MAROON, /// <summary> Brown-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=128,0,0 </summary>
			WHITE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,255,255 </summary>
			SNOW, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,250,250 </summary>
			HONEYDEW, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=240,255,240 </summary>
			MINT_CREAM, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=245,255,250 </summary>
			AZURE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=240,255,255 </summary>
			ALICE_BLUE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=240,248,255 </summary>
			GHOST_WHITE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=248,248,255 </summary>
			WHITE_SMOKE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=245,245,245 </summary>
			SEASHELL, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,245,238 </summary>
			BEIGE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=245,245,220 </summary>
			OLD_LACE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=253,245,230 </summary>
			FLORAL_WHITE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,250,240 </summary>
			IVORY, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,255,240 </summary>
			ANTIQUE_WHITE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=250,235,215 </summary>
			LINEN, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=250,240,230 </summary>
			LAVENDER_BLUSH, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,240,245 </summary>
			MISTY_ROSE, /// <summary> White-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=255,228,225 </summary>
			GAINSBORO, /// <summary> Grey-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=220,220,220 </summary>
			LIGHT_GREY, /// <summary> Grey-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=211,211,211 </summary>
			SILVER, /// <summary> Grey-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=192,192,192 </summary>
			DARK_GREY, /// <summary> Grey-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=169,169,169 </summary>
			GREY, /// <summary> Grey-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=128,128,128 </summary>
			DIM_GREY, /// <summary> Grey-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=105,105,105 </summary>
			LIGHT_SLATE_GREY, /// <summary> Grey-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=119,136,153 </summary>
			SLATE_GREY, /// <summary> Grey-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=112,128,144 </summary>
			BLACK /// <summary> Grey-group color. http://www.w3schools.com/colors/color_tryit.asp?rgb=0,0,0 </summary>
		}; /* end enum ColorName */
	} /* end namespace ColorNames */

	/// <summary>The representation of color in the game engine.</summary>
	/// <remarks>The color is a simple adapter for the Math library's <code>Vector4D</code> class.</remarks>
	class Color
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API explicit Color(ColorNames::ColorName colorName);
		RENDERING_API Color(Math::Real red, Math::Real green, Math::Real blue, Math::Real alpha = REAL_ONE);
		RENDERING_API explicit Color(const Math::Vector3D& rgbVector);
		RENDERING_API explicit Color(const Math::Vector4D& rgbaVector);
		RENDERING_API Color(const Color& color);
		RENDERING_API Color(Color&& color);
		RENDERING_API ~Color();
		RENDERING_API Color& operator=(const Color& color);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static, non-virtual member functions begin ==================== */
	public:
		/// <summary> Retrieves the red component of the color. </summary>
		/// <returns> The red component of the color. </returns>
		RENDERING_API Math::Real GetRed() const { return m_values.GetX(); }

		/// <summary> Retrieves the green component of the color. </summary>
		/// <returns> The green component of the color. </returns>
		RENDERING_API Math::Real GetGreen() const { return m_values.GetY(); }

		/// <summary> Retrieves the blue component of the color. </summary>
		/// <returns> The blue component of the color. </returns>
		RENDERING_API Math::Real GetBlue() const { return m_values.GetZ(); }

		/// <summary> Retrieves the alpha component of the color. </summary>
		/// <returns> The alpha component of the color. </returns>
		RENDERING_API Math::Real GetAlpha() const { return m_values.GetW(); }

		RENDERING_API void SetRed(Math::Real red) { m_values.SetX(red); }
		RENDERING_API void SetGreen(Math::Real green) { m_values.SetY(green); }
		RENDERING_API void SetBlue(Math::Real blue) { m_values.SetZ(blue); }
		RENDERING_API void SetAlpha(Math::Real alpha) { m_values.SetW(alpha); }

		RENDERING_API Color Lerp(const Color& color, Math::Real lerpFactor) const
		{
			return Color(m_values.Lerp(color.m_values, lerpFactor));
		}
		RENDERING_API std::string ToString() const
		{
			std::stringstream ss("");
			ss << "(red=" << GetRed() << "; green=" << GetGreen() << "; blue=" << GetBlue() << "; alpha=" << GetAlpha() << ")";
			return ss.str();
		}
		/* ==================== Non-static, non-virtual member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Math::Vector4D m_values;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Color */

} /* end namespace Rendering */

#endif /* __RENDERING_COLOR_H__ */