#include "StdAfx.h"
#include "Color.h"

#include "Utility/ILogger.h"
#include "Utility/StringUtility.h"

rendering::Color::Color(const std::string& colorName) noexcept :
	Color(Value(utility::string_utility::MakeLowercase(colorName).c_str(), COLOR_NAMES))
{
}

rendering::Color::Color(color_ids::ColorId colorId) noexcept
{
	switch (colorId)
	{
	case color_ids::INDIAN_RED: m_values.Set(0.803922f, 0.360784f, 0.360784f, REAL_ONE); break;
	case color_ids::LIGHT_CORAL: m_values.Set(0.941176f, 0.501961f, 0.501961f, REAL_ONE); break;
	case color_ids::SALMON: m_values.Set(0.980392f, 0.501961f, 0.447059f, REAL_ONE); break;
	case color_ids::DARK_SALMON: m_values.Set(0.913725f, 0.588235f, 0.478431f, REAL_ONE); break;
	case color_ids::LIGHT_SALMON: m_values.Set(1.0f, 0.627451f, 0.478431f, REAL_ONE); break;
	case color_ids::CRIMSON: m_values.Set(0.862745f, 0.078431f, 0.235294f, REAL_ONE); break;
	case color_ids::RED: m_values.Set(1.0f, 0.0f, 0.0f, REAL_ONE); break;
	case color_ids::FIRE_BRICK: m_values.Set(0.698039f, 0.133333f, 0.133333f, REAL_ONE); break;
	case color_ids::DARK_RED: m_values.Set(0.545098f, 0.0f, 0.0f, REAL_ONE); break;
	case color_ids::PINK: m_values.Set(1.0f, 0.752941f, 0.796078f, REAL_ONE); break;
	case color_ids::LIGHT_PINK: m_values.Set(1.0f, 0.713725f, 0.756863f, REAL_ONE); break;
	case color_ids::HOT_PINK: m_values.Set(1.0f, 0.411765f, 0.705882f, REAL_ONE); break;
	case color_ids::DEEP_PINK: m_values.Set(1.0f, 0.078431f, 0.576471f, REAL_ONE); break;
	case color_ids::MEDIUM_VIOLET_RED: m_values.Set(0.780392f, 0.082353f, 0.521569f, REAL_ONE); break;
	case color_ids::PALE_VIOLET_RED: m_values.Set(0.858824f, 0.439216f, 0.576471f, REAL_ONE); break;
	case color_ids::CORAL: m_values.Set(1.0f, 0.498039f, 0.313725f, REAL_ONE); break;
	case color_ids::TOMATO: m_values.Set(1.0f, 0.388235f, 0.278431f, REAL_ONE); break;
	case color_ids::ORANGE_RED: m_values.Set(1.0f, 0.270588f, 0.0f, REAL_ONE); break;
	case color_ids::DARK_ORANGE: m_values.Set(1.0f, 0.549020f, 0.0f, REAL_ONE); break;
	case color_ids::ORANGE: m_values.Set(1.0f, 0.647059f, 0.0f, REAL_ONE); break;
	case color_ids::GOLD: m_values.Set(1.0f, 0.843137f, 0.0f, REAL_ONE); break;
	case color_ids::YELLOW: m_values.Set(1.0f, 1.0f, 0.0f, REAL_ONE); break;
	case color_ids::LIGHT_YELLOW: m_values.Set(1.0f, 1.0f, 0.878431f, REAL_ONE); break;
	case color_ids::LEMON_CHIFFON: m_values.Set(1.0f, 0.980392f, 0.803922f, REAL_ONE); break;
	case color_ids::LIGHT_GOLDENROD_YELLOW: m_values.Set(0.980392f, 0.980392f, 0.823529f, REAL_ONE); break;
	case color_ids::PAPAYA_WHIP: m_values.Set(1.0f, 0.937255f, 0.835294f, REAL_ONE); break;
	case color_ids::MOCCASIN: m_values.Set(1.0f, 0.894118f, 0.709804f, REAL_ONE); break;
	case color_ids::PEACH_PUFF: m_values.Set(1.0f, 0.854902f, 0.725490f, REAL_ONE); break;
	case color_ids::PALE_GOLDENROD: m_values.Set(0.933333f, 0.909804f, 0.666667f, REAL_ONE); break;
	case color_ids::KHAKI: m_values.Set(0.941176f, 0.901961f, 0.549020f, REAL_ONE); break;
	case color_ids::DARK_KHAKI: m_values.Set(0.741176f, 0.717647f, 0.419608f, REAL_ONE); break;
	case color_ids::LAVENDER: m_values.Set(0.901961f, 0.901961f, 0.980392f, REAL_ONE); break;
	case color_ids::THISTLE: m_values.Set(0.847059f, 0.749020f, 0.847059f, REAL_ONE); break;
	case color_ids::PLUM: m_values.Set(0.866667f, 0.627451f, 0.866667f, REAL_ONE); break;
	case color_ids::VIOLET: m_values.Set(0.933333f, 0.509804f, 0.933333f, REAL_ONE); break;
	case color_ids::ORCHID: m_values.Set(0.854902f, 0.439216f, 0.839216f, REAL_ONE); break;
	case color_ids::FUCHSIA: /* The same color as magenta */ m_values.Set(1.0f, 0.0f, 1.0f, REAL_ONE); break;
	case color_ids::MAGENTA: /* The same color as fuchsia */ m_values.Set(1.0f, 0.0f, 1.0f, REAL_ONE); break;
	case color_ids::MEDIUM_ORCHID: m_values.Set(0.729412f, 0.333333f, 0.827451f, REAL_ONE); break;
	case color_ids::BLUE_VIOLET: m_values.Set(0.541176f, 0.168627f, 0.886275f, REAL_ONE); break;
	case color_ids::DARK_VIOLET: m_values.Set(0.580392f, 0.0f, 0.827451f, REAL_ONE); break;
	case color_ids::DARK_ORCHID: m_values.Set(0.6f, 0.196078f, 0.8f, REAL_ONE); break;
	case color_ids::DARK_MAGENTA: m_values.Set(0.545098f, 0.0f, 0.545098f, REAL_ONE); break;
	case color_ids::PURPLE: m_values.Set(0.501961f, 0.0f, 0.501961f, REAL_ONE); break;
	case color_ids::INDIGO: m_values.Set(0.294118f, 0.0f, 0.509804f, REAL_ONE); break;
	case color_ids::STATE_BLUE: m_values.Set(0.415686f, 0.352941f, 0.803922f, REAL_ONE); break;
	case color_ids::DARK_STATE_BLUE: m_values.Set(0.282353f, 0.239216f, 0.545098f, REAL_ONE); break;
	case color_ids::MEDIUM_STATE_BLUE: m_values.Set(0.494118f, 0.407843f, 0.933333f, REAL_ONE); break;
	case color_ids::GREEN_YELLOW: m_values.Set(0.678431f, 1.0f, 0.184314f, REAL_ONE); break;
	case color_ids::CHARTREUSE: m_values.Set(0.498039f, 1.0f, 0.0f, REAL_ONE); break;
	case color_ids::LAWN_GREEN: m_values.Set(0.486275f, 0.988235f, 0.0f, REAL_ONE); break;
	case color_ids::LIME: m_values.Set(0.0f, 1.0f, 0.0f, REAL_ONE); break;
	case color_ids::LIME_GREEN: m_values.Set(0.196078f, 0.803922f, 0.196078f, REAL_ONE); break;
	case color_ids::PALE_GREEN: m_values.Set(0.596078f, 0.984314f, 0.596078f, REAL_ONE); break;
	case color_ids::LIGHT_GREEN: m_values.Set(0.564706f, 0.933333f, 0.564706f, REAL_ONE); break;
	case color_ids::MEDIUM_SPRING_GREEN: m_values.Set(0.0f, 0.980392f, 0.603922f, REAL_ONE); break;
	case color_ids::SPRING_GREEN: m_values.Set(0.0f, 1.0f, 0.498039f, REAL_ONE); break;
	case color_ids::MEDIUM_SEA_GREEN: m_values.Set(0.235294f, 0.701961f, 0.443137f, REAL_ONE); break;
	case color_ids::SEA_GREEN: m_values.Set(0.180392f, 0.545098f, 0.341176f, REAL_ONE); break;
	case color_ids::FOREST_GREEN: m_values.Set(0.133333f, 0.545098f, 0.133333f, REAL_ONE); break;
	case color_ids::GREEN: m_values.Set(0.0f, 0.501961f, 0.0f, REAL_ONE); break;
	case color_ids::DARK_GREEN: m_values.Set(0.0f, 0.392157f, 0.0f, REAL_ONE); break;
	case color_ids::YELLOW_GREEN: m_values.Set(0.603922f, 0.803922f, 0.196078f, REAL_ONE); break;
	case color_ids::OLIVE_DRAB: m_values.Set(0.419608f, 0.556863f, 0.137255f, REAL_ONE); break;
	case color_ids::OLIVE: m_values.Set(0.501961f, 0.0f, 0.501961f, REAL_ONE); break;
	case color_ids::DARK_OLIVE_GREEN: m_values.Set(0.333333f, 0.419608f, 0.184314f, REAL_ONE); break;
	case color_ids::MEDIUM_AQUAMARINE: m_values.Set(0.4f, 0.803922f, 0.666667f, REAL_ONE); break;
	case color_ids::DARK_SEA_GREEN: m_values.Set(0.560784f, 0.737255f, 0.560784f, REAL_ONE); break;
	case color_ids::LIGHT_SEA_GREEN: m_values.Set(0.125490f, 0.698039f, 0.666667f, REAL_ONE); break;
	case color_ids::DARK_CYAN: m_values.Set(0.0f, 0.545098f, 0.545098f, REAL_ONE); break;
	case color_ids::TEAL: m_values.Set(0.0f, 0.501961f, 0.501961f, REAL_ONE); break;
	case color_ids::AQUA: /* The same color as cyan */ m_values.Set(0.0f, 1.0f, 1.0f, REAL_ONE); break;
	case color_ids::CYAN: /* The same color as aqua */ m_values.Set(0.0f, 1.0f, 1.0f, REAL_ONE); break;
	case color_ids::LIGHT_CYAN: m_values.Set(0.878431f, 1.0f, 1.0f, REAL_ONE); break;
	case color_ids::PALE_TURQUOISE: m_values.Set(0.686275f, 0.933333f, 0.933333f, REAL_ONE); break;
	case color_ids::AQUAMARINE: m_values.Set(0.498039f, 1.0f, 0.831373f, REAL_ONE); break;
	case color_ids::TURQUOISE: m_values.Set(0.250980f, 0.878431f, 0.815686f, REAL_ONE); break;
	case color_ids::MEDIUM_TURQUOISE: m_values.Set(0.282353f, 0.819608f, 0.8f, REAL_ONE); break;
	case color_ids::DARK_TURQUOISE: m_values.Set(0.0f, 0.807843f, 0.819608f, REAL_ONE); break;
	case color_ids::CADET_BLUE: m_values.Set(0.372549f, 0.619608f, 0.627451f, REAL_ONE); break;
	case color_ids::STEEL_BLUE: m_values.Set(0.274510f, 0.509804f, 0.705882f, REAL_ONE); break;
	case color_ids::LIGHT_STEEL_BLUE: m_values.Set(0.690196f, 0.768627f, 0.870588f, REAL_ONE); break;
	case color_ids::POWDER_BLUE: m_values.Set(0.690196f, 0.878431f, 0.901961f, REAL_ONE); break;
	case color_ids::LIGHT_BLUE: m_values.Set(0.678431f, 0.847059f, 0.901961f, REAL_ONE); break;
	case color_ids::SKY_BLUE: m_values.Set(0.529412f, 0.807843f, 0.921569f, REAL_ONE); break;
	case color_ids::LIGHT_SKY_BLUE: m_values.Set(0.529412f, 0.807843f, 0.980392f, REAL_ONE); break;
	case color_ids::DEEP_SKY_BLUE: m_values.Set(0.0f, 0.749020f, 1.0f, REAL_ONE); break;
	case color_ids::DODGER_BLUE: m_values.Set(0.117647f, 0.564706f, 1.0f, REAL_ONE); break;
	case color_ids::CORNFLOWER_BLUE: m_values.Set(0.392157f, 0.584314f, 0.929412f, REAL_ONE); break;
	case color_ids::ROYAL_BLUE: m_values.Set(0.254902f, 0.411765f, 0.882353f, REAL_ONE); break;
	case color_ids::MEDIUM_BLUE: m_values.Set(0.0f, 0.0f, 0.803922f, REAL_ONE); break;
	case color_ids::DARK_BLUE: m_values.Set(0.0f, 0.0f, 0.545098f, REAL_ONE); break;
	case color_ids::NAVY: m_values.Set(0.0f, 0.0f, 0.501961f, REAL_ONE); break;
	case color_ids::MIDNIGHT_BLUE: m_values.Set(0.098039f, 0.098039f, 0.439216f, REAL_ONE); break;
	case color_ids::CORNSILK: m_values.Set(1.0f, 0.972549f, 0.862745f, REAL_ONE); break;
	case color_ids::BLANCHED_ALMOND: m_values.Set(1.0f, 0.921569f, 0.803922f, REAL_ONE); break;
	case color_ids::BISQUE: m_values.Set(1.0f, 0.894118f, 0.768627f, REAL_ONE); break;
	case color_ids::NAVAJO_WHITE: m_values.Set(1.0f, 0.870588f, 0.678431f, REAL_ONE); break;
	case color_ids::WHEAT: m_values.Set(0.960784f, 0.870588f, 0.701961f, REAL_ONE); break;
	case color_ids::BURLY_WOOD: m_values.Set(0.870588f, 0.721569f, 0.529412f, REAL_ONE); break;
	case color_ids::TAN: m_values.Set(0.823529f, 0.705882f, 0.549020f, REAL_ONE); break;
	case color_ids::ROSY_BROWN: m_values.Set(0.737255f, 0.560784f, 0.560784f, REAL_ONE); break;
	case color_ids::SANDY_BROWN: m_values.Set(0.956863f, 0.643137f, 0.376471f, REAL_ONE); break;
	case color_ids::GOLDENROD: m_values.Set(0.854902f, 0.647059f, 0.125490f, REAL_ONE); break;
	case color_ids::DARK_GOLDENROD: m_values.Set(0.721569f, 0.525490f, 0.043137f, REAL_ONE); break;
	case color_ids::PERU: m_values.Set(0.803922f, 0.521569f, 0.247059f, REAL_ONE); break;
	case color_ids::CHOCOLATE: m_values.Set(0.823529f, 0.411765f, 0.117647f, REAL_ONE); break;
	case color_ids::SADDLE_BROWN: m_values.Set(0.545098f, 0.270588f, 0.074510f, REAL_ONE); break;
	case color_ids::SIENNA: m_values.Set(0.627451f, 0.321569f, 0.176471f, REAL_ONE); break;
	case color_ids::BROWN: m_values.Set(0.647059f, 0.164706f, 0.164706f, REAL_ONE); break;
	case color_ids::MAROON: m_values.Set(0.501961f, 0.0f, 0.0f, REAL_ONE); break;
	case color_ids::WHITE: m_values.Set(1.0f, 1.0f, 1.0f, REAL_ONE); break;
	case color_ids::SNOW: m_values.Set(1.0f, 0.980392f, 0.980392f, REAL_ONE); break;
	case color_ids::HONEYDEW: m_values.Set(0.941176f, 1.0f, 0.941176f, REAL_ONE); break;
	case color_ids::MINT_CREAM: m_values.Set(0.960784f, 1.0f, 0.980392f, REAL_ONE); break;
	case color_ids::AZURE: m_values.Set(0.941176f, 1.0f, 1.0f, REAL_ONE); break;
	case color_ids::ALICE_BLUE: m_values.Set(0.941176f, 0.972549f, 1.0f, REAL_ONE); break;
	case color_ids::GHOST_WHITE: m_values.Set(0.972549f, 0.972549f, 1.0f, REAL_ONE); break;
	case color_ids::WHITE_SMOKE: m_values.Set(0.960784f, 0.960784f, 0.960784f, REAL_ONE); break;
	case color_ids::SEASHELL: m_values.Set(1.0f, 0.960784f, 0.933333f, REAL_ONE); break;
	case color_ids::BEIGE: m_values.Set(0.960784f, 0.960784f, 0.862745f, REAL_ONE); break;
	case color_ids::OLD_LACE: m_values.Set(0.992157f, 0.960784f, 0.901961f, REAL_ONE); break;
	case color_ids::FLORAL_WHITE: m_values.Set(1.0f, 0.980392f, 0.941176f, REAL_ONE); break;
	case color_ids::IVORY: m_values.Set(1.0f, 1.0f, 0.941176f, REAL_ONE); break;
	case color_ids::ANTIQUE_WHITE: m_values.Set(0.980392f, 0.921569f, 0.843137f, REAL_ONE); break;
	case color_ids::LINEN: m_values.Set(0.980392f, 0.941176f, 0.901961f, REAL_ONE); break;
	case color_ids::LAVENDER_BLUSH: m_values.Set(1.0f, 0.941176f, 0.960784f, REAL_ONE); break;
	case color_ids::MISTY_ROSE: m_values.Set(1.0f, 0.894118f, 0.882353f, REAL_ONE); break;
	case color_ids::GAINSBORO: m_values.Set(0.862745f, 0.862745f, 0.862745f, REAL_ONE); break;
	case color_ids::LIGHT_GREY: m_values.Set(0.827451f, 0.827451f, 0.827451f, REAL_ONE); break;
	case color_ids::SILVER: m_values.Set(0.752941f, 0.752941f, 0.752941f, REAL_ONE); break;
	case color_ids::DARK_GREY: m_values.Set(0.662745f, 0.662745f, 0.662745f, REAL_ONE); break;
	case color_ids::GREY: m_values.Set(0.501961f, 0.501961f, 0.501961f, REAL_ONE); break;
	case color_ids::DIM_GREY: m_values.Set(0.411765f, 0.411765f, 0.411765f, REAL_ONE); break;
	case color_ids::LIGHT_SLATE_GREY: m_values.Set(0.466667f, 0.533333f, 0.6f, REAL_ONE); break;
	case color_ids::SLATE_GREY: m_values.Set(0.439216f, 0.501961f, 0.564706f, REAL_ONE); break;
	case color_ids::BLACK: m_values.Set(0.0f, 0.0f, 0.0f, REAL_ONE); break;
	default:
		WARNING_LOG_RENDERING("Unknown color ID \"", colorId, "\" has been specified. Using red color as default");
		m_values.Set(REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ONE);
		break;
	}
}

rendering::Color::Color(math::Real red, math::Real green, math::Real blue, math::Real alpha /* = REAL_ONE */) noexcept :
	m_values(red, green, blue, alpha)
{
}

rendering::Color::Color(const math::Vector3D& rgbVector) noexcept :
	Color(rgbVector.x, rgbVector.y, rgbVector.z, REAL_ONE)
{
}

rendering::Color::Color(const math::Vector4D& rgbaVector) noexcept :
	Color(rgbaVector.x, rgbaVector.y, rgbaVector.z, rgbaVector.w)
{
}

rendering::Color::~Color()
{
}

rendering::Color rendering::Color::operator+(const Color& color) const noexcept
{
	return Color(GetRed() + color.GetRed(), GetGreen() + color.GetGreen(), GetBlue() + color.GetBlue(), GetAlpha() + color.GetAlpha());
}

rendering::Color& rendering::Color::operator+=(const Color& color) noexcept
{
	m_values.Set(GetRed() + color.GetRed(), GetGreen() + color.GetGreen(), GetBlue() + color.GetBlue(), GetAlpha() + color.GetAlpha());
	return *this;
}

rendering::Color rendering::Color::operator*(math::Real value) const noexcept
{
	return Color(GetRed() * value, GetGreen() * value, GetBlue() * value, GetAlpha() * value);
}

rendering::Color& rendering::Color::operator*=(math::Real value) noexcept
{
	m_values.Set(GetRed() * value, GetGreen() * value, GetBlue() * value, GetAlpha() * value);
	return *this;
}

rendering::Color rendering::Color::operator*(const Color& color) const noexcept
{
	return Color(GetRed() * color.GetRed(), GetGreen() * color.GetGreen(), GetBlue() * color.GetBlue(), GetAlpha() * color.GetAlpha());
}

rendering::Color& rendering::Color::operator*=(const Color& color) noexcept
{
	m_values.Set(GetRed() * color.GetRed(), GetGreen() * color.GetGreen(), GetBlue() * color.GetBlue(), GetAlpha() * color.GetAlpha());
	return *this;
}