#include "StdAfx.h"
#include "Color.h"

#include "Utility\ILogger.h"
#include "Utility\StringUtility.h"

Rendering::Color::Color(const std::string& colorName) :
	Color(Value(Utility::StringUtility::MakeLowercase(colorName).c_str(), COLOR_NAMES))
{
}

Rendering::Color::Color(ColorNames::ColorName colorName)
{
	switch (colorName)
	{
	case ColorNames::INDIAN_RED: m_values.Set(0.803922f, 0.360784f, 0.360784f, REAL_ONE); break;
	case ColorNames::LIGHT_CORAL: m_values.Set(0.941176f, 0.501961f, 0.501961f, REAL_ONE); break;
	case ColorNames::SALMON: m_values.Set(0.980392f, 0.501961f, 0.447059f, REAL_ONE); break;
	case ColorNames::DARK_SALMON: m_values.Set(0.913725f, 0.588235f, 0.478431f, REAL_ONE); break;
	case ColorNames::LIGHT_SALMON: m_values.Set(1.0f, 0.627451f, 0.478431f, REAL_ONE); break;
	case ColorNames::CRIMSON: m_values.Set(0.862745f, 0.078431f, 0.235294f, REAL_ONE); break;
	case ColorNames::RED: m_values.Set(1.0f, 0.0f, 0.0f, REAL_ONE); break;
	case ColorNames::FIRE_BRICK: m_values.Set(0.698039f, 0.133333f, 0.133333f, REAL_ONE); break;
	case ColorNames::DARK_RED: m_values.Set(0.545098f, 0.0f, 0.0f, REAL_ONE); break;
	case ColorNames::PINK: m_values.Set(1.0f, 0.752941f, 0.796078f, REAL_ONE); break;
	case ColorNames::LIGHT_PINK: m_values.Set(1.0f, 0.713725f, 0.756863f, REAL_ONE); break;
	case ColorNames::HOT_PINK: m_values.Set(1.0f, 0.411765f, 0.705882f, REAL_ONE); break;
	case ColorNames::DEEP_PINK: m_values.Set(1.0f, 0.078431f, 0.576471f, REAL_ONE); break;
	case ColorNames::MEDIUM_VIOLET_RED: m_values.Set(0.780392f, 0.082353f, 0.521569f, REAL_ONE); break;
	case ColorNames::PALE_VIOLET_RED: m_values.Set(0.858824f, 0.439216f, 0.576471f, REAL_ONE); break;
	case ColorNames::CORAL: m_values.Set(1.0f, 0.498039f, 0.313725f, REAL_ONE); break;
	case ColorNames::TOMATO: m_values.Set(1.0f, 0.388235f, 0.278431f, REAL_ONE); break;
	case ColorNames::ORANGE_RED: m_values.Set(1.0f, 0.270588f, 0.0f, REAL_ONE); break;
	case ColorNames::DARK_ORANGE: m_values.Set(1.0f, 0.549020f, 0.0f, REAL_ONE); break;
	case ColorNames::ORANGE: m_values.Set(1.0f, 0.647059f, 0.0f, REAL_ONE); break;
	case ColorNames::GOLD: m_values.Set(1.0f, 0.843137f, 0.0f, REAL_ONE); break;
	case ColorNames::YELLOW: m_values.Set(1.0f, 1.0f, 0.0f, REAL_ONE); break;
	case ColorNames::LIGHT_YELLOW: m_values.Set(1.0f, 1.0f, 0.878431f, REAL_ONE); break;
	case ColorNames::LEMON_CHIFFON: m_values.Set(1.0f, 0.980392f, 0.803922f, REAL_ONE); break;
	case ColorNames::LIGHT_GOLDENROD_YELLOW: m_values.Set(0.980392f, 0.980392f, 0.823529f, REAL_ONE); break;
	case ColorNames::PAPAYA_WHIP: m_values.Set(1.0f, 0.937255f, 0.835294f, REAL_ONE); break;
	case ColorNames::MOCCASIN: m_values.Set(1.0f, 0.894118f, 0.709804f, REAL_ONE); break;
	case ColorNames::PEACH_PUFF: m_values.Set(1.0f, 0.854902f, 0.725490f, REAL_ONE); break;
	case ColorNames::PALE_GOLDENROD: m_values.Set(0.933333f, 0.909804f, 0.666667f, REAL_ONE); break;
	case ColorNames::KHAKI: m_values.Set(0.941176f, 0.901961f, 0.549020f, REAL_ONE); break;
	case ColorNames::DARK_KHAKI: m_values.Set(0.741176f, 0.717647f, 0.419608f, REAL_ONE); break;
	case ColorNames::LAVENDER: m_values.Set(0.901961f, 0.901961f, 0.980392f, REAL_ONE); break;
	case ColorNames::THISTLE: m_values.Set(0.847059f, 0.749020f, 0.847059f, REAL_ONE); break;
	case ColorNames::PLUM: m_values.Set(0.866667f, 0.627451f, 0.866667f, REAL_ONE); break;
	case ColorNames::VIOLET: m_values.Set(0.933333f, 0.509804f, 0.933333f, REAL_ONE); break;
	case ColorNames::ORCHID: m_values.Set(0.854902f, 0.439216f, 0.839216f, REAL_ONE); break;
	case ColorNames::FUCHSIA: /* The same color as magenta */ m_values.Set(1.0f, 0.0f, 1.0f, REAL_ONE); break;
	case ColorNames::MAGENTA: /* The same color as fuchsia */ m_values.Set(1.0f, 0.0f, 1.0f, REAL_ONE); break;
	case ColorNames::MEDIUM_ORCHID: m_values.Set(0.729412f, 0.333333f, 0.827451f, REAL_ONE); break;
	case ColorNames::BLUE_VIOLET: m_values.Set(0.541176f, 0.168627f, 0.886275f, REAL_ONE); break;
	case ColorNames::DARK_VIOLET: m_values.Set(0.580392f, 0.0f, 0.827451f, REAL_ONE); break;
	case ColorNames::DARK_ORCHID: m_values.Set(0.6f, 0.196078f, 0.8f, REAL_ONE); break;
	case ColorNames::DARK_MAGENTA: m_values.Set(0.545098f, 0.0f, 0.545098f, REAL_ONE); break;
	case ColorNames::PURPLE: m_values.Set(0.501961f, 0.0f, 0.501961f, REAL_ONE); break;
	case ColorNames::INDIGO: m_values.Set(0.294118f, 0.0f, 0.509804f, REAL_ONE); break;
	case ColorNames::STATE_BLUE: m_values.Set(0.415686f, 0.352941f, 0.803922f, REAL_ONE); break;
	case ColorNames::DARK_STATE_BLUE: m_values.Set(0.282353f, 0.239216f, 0.545098f, REAL_ONE); break;
	case ColorNames::MEDIUM_STATE_BLUE: m_values.Set(0.494118f, 0.407843f, 0.933333f, REAL_ONE); break;
	case ColorNames::GREEN_YELLOW: m_values.Set(0.678431f, 1.0f, 0.184314f, REAL_ONE); break;
	case ColorNames::CHARTREUSE: m_values.Set(0.498039f, 1.0f, 0.0f, REAL_ONE); break;
	case ColorNames::LAWN_GREEN: m_values.Set(0.486275f, 0.988235f, 0.0f, REAL_ONE); break;
	case ColorNames::LIME: m_values.Set(0.0f, 1.0f, 0.0f, REAL_ONE); break;
	case ColorNames::LIME_GREEN: m_values.Set(0.196078f, 0.803922f, 0.196078f, REAL_ONE); break;
	case ColorNames::PALE_GREEN: m_values.Set(0.596078f, 0.984314f, 0.596078f, REAL_ONE); break;
	case ColorNames::LIGHT_GREEN: m_values.Set(0.564706f, 0.933333f, 0.564706f, REAL_ONE); break;
	case ColorNames::MEDIUM_SPRING_GREEN: m_values.Set(0.0f, 0.980392f, 0.603922f, REAL_ONE); break;
	case ColorNames::SPRING_GREEN: m_values.Set(0.0f, 1.0f, 0.498039f, REAL_ONE); break;
	case ColorNames::MEDIUM_SEA_GREEN: m_values.Set(0.235294f, 0.701961f, 0.443137f, REAL_ONE); break;
	case ColorNames::SEA_GREEN: m_values.Set(0.180392f, 0.545098f, 0.341176f, REAL_ONE); break;
	case ColorNames::FOREST_GREEN: m_values.Set(0.133333f, 0.545098f, 0.133333f, REAL_ONE); break;
	case ColorNames::GREEN: m_values.Set(0.0f, 0.501961f, 0.0f, REAL_ONE); break;
	case ColorNames::DARK_GREEN: m_values.Set(0.0f, 0.392157f, 0.0f, REAL_ONE); break;
	case ColorNames::YELLOW_GREEN: m_values.Set(0.603922f, 0.803922f, 0.196078f, REAL_ONE); break;
	case ColorNames::OLIVE_DRAB: m_values.Set(0.419608f, 0.556863f, 0.137255f, REAL_ONE); break;
	case ColorNames::OLIVE: m_values.Set(0.501961f, 0.0f, 0.501961f, REAL_ONE); break;
	case ColorNames::DARK_OLIVE_GREEN: m_values.Set(0.333333f, 0.419608f, 0.184314f, REAL_ONE); break;
	case ColorNames::MEDIUM_AQUAMARINE: m_values.Set(0.4f, 0.803922f, 0.666667f, REAL_ONE); break;
	case ColorNames::DARK_SEA_GREEN: m_values.Set(0.560784f, 0.737255f, 0.560784f, REAL_ONE); break;
	case ColorNames::LIGHT_SEA_GREEN: m_values.Set(0.125490f, 0.698039f, 0.666667f, REAL_ONE); break;
	case ColorNames::DARK_CYAN: m_values.Set(0.0f, 0.545098f, 0.545098f, REAL_ONE); break;
	case ColorNames::TEAL: m_values.Set(0.0f, 0.501961f, 0.501961f, REAL_ONE); break;
	case ColorNames::AQUA: /* The same color as cyan */ m_values.Set(0.0f, 1.0f, 1.0f, REAL_ONE); break;
	case ColorNames::CYAN: /* The same color as aqua */ m_values.Set(0.0f, 1.0f, 1.0f, REAL_ONE); break;
	case ColorNames::LIGHT_CYAN: m_values.Set(0.878431f, 1.0f, 1.0f, REAL_ONE); break;
	case ColorNames::PALE_TURQUOISE: m_values.Set(0.686275f, 0.933333f, 0.933333f, REAL_ONE); break;
	case ColorNames::AQUAMARINE: m_values.Set(0.498039f, 1.0f, 0.831373f, REAL_ONE); break;
	case ColorNames::TURQUOISE: m_values.Set(0.250980f, 0.878431f, 0.815686f, REAL_ONE); break;
	case ColorNames::MEDIUM_TURQUOISE: m_values.Set(0.282353f, 0.819608f, 0.8f, REAL_ONE); break;
	case ColorNames::DARK_TURQUOISE: m_values.Set(0.0f, 0.807843f, 0.819608f, REAL_ONE); break;
	case ColorNames::CADET_BLUE: m_values.Set(0.372549f, 0.619608f, 0.627451f, REAL_ONE); break;
	case ColorNames::STEEL_BLUE: m_values.Set(0.274510f, 0.509804f, 0.705882f, REAL_ONE); break;
	case ColorNames::LIGHT_STEEL_BLUE: m_values.Set(0.690196f, 0.768627f, 0.870588f, REAL_ONE); break;
	case ColorNames::POWDER_BLUE: m_values.Set(0.690196f, 0.878431f, 0.901961f, REAL_ONE); break;
	case ColorNames::LIGHT_BLUE: m_values.Set(0.678431f, 0.847059f, 0.901961f, REAL_ONE); break;
	case ColorNames::SKY_BLUE: m_values.Set(0.529412f, 0.807843f, 0.921569f, REAL_ONE); break;
	case ColorNames::LIGHT_SKY_BLUE: m_values.Set(0.529412f, 0.807843f, 0.980392f, REAL_ONE); break;
	case ColorNames::DEEP_SKY_BLUE: m_values.Set(0.0f, 0.749020f, 1.0f, REAL_ONE); break;
	case ColorNames::DODGER_BLUE: m_values.Set(0.117647f, 0.564706f, 1.0f, REAL_ONE); break;
	case ColorNames::CORNFLOWER_BLUE: m_values.Set(0.392157f, 0.584314f, 0.929412f, REAL_ONE); break;
	case ColorNames::ROYAL_BLUE: m_values.Set(0.254902f, 0.411765f, 0.882353f, REAL_ONE); break;
	case ColorNames::MEDIUM_BLUE: m_values.Set(0.0f, 0.0f, 0.803922f, REAL_ONE); break;
	case ColorNames::DARK_BLUE: m_values.Set(0.0f, 0.0f, 0.545098f, REAL_ONE); break;
	case ColorNames::NAVY: m_values.Set(0.0f, 0.0f, 0.501961f, REAL_ONE); break;
	case ColorNames::MIDNIGHT_BLUE: m_values.Set(0.098039f, 0.098039f, 0.439216f, REAL_ONE); break;
	case ColorNames::CORNSILK: m_values.Set(1.0f, 0.972549f, 0.862745f, REAL_ONE); break;
	case ColorNames::BLANCHED_ALMOND: m_values.Set(1.0f, 0.921569f, 0.803922f, REAL_ONE); break;
	case ColorNames::BISQUE: m_values.Set(1.0f, 0.894118f, 0.768627f, REAL_ONE); break;
	case ColorNames::NAVAJO_WHITE: m_values.Set(1.0f, 0.870588f, 0.678431f, REAL_ONE); break;
	case ColorNames::WHEAT: m_values.Set(0.960784f, 0.870588f, 0.701961f, REAL_ONE); break;
	case ColorNames::BURLY_WOOD: m_values.Set(0.870588f, 0.721569f, 0.529412f, REAL_ONE); break;
	case ColorNames::TAN: m_values.Set(0.823529f, 0.705882f, 0.549020f, REAL_ONE); break;
	case ColorNames::ROSY_BROWN: m_values.Set(0.737255f, 0.560784f, 0.560784f, REAL_ONE); break;
	case ColorNames::SANDY_BROWN: m_values.Set(0.956863f, 0.643137f, 0.376471f, REAL_ONE); break;
	case ColorNames::GOLDENROD: m_values.Set(0.854902f, 0.647059f, 0.125490f, REAL_ONE); break;
	case ColorNames::DARK_GOLDENROD: m_values.Set(0.721569f, 0.525490f, 0.043137f, REAL_ONE); break;
	case ColorNames::PERU: m_values.Set(0.803922f, 0.521569f, 0.247059f, REAL_ONE); break;
	case ColorNames::CHOCOLATE: m_values.Set(0.823529f, 0.411765f, 0.117647f, REAL_ONE); break;
	case ColorNames::SADDLE_BROWN: m_values.Set(0.545098f, 0.270588f, 0.074510f, REAL_ONE); break;
	case ColorNames::SIENNA: m_values.Set(0.627451f, 0.321569f, 0.176471f, REAL_ONE); break;
	case ColorNames::BROWN: m_values.Set(0.647059f, 0.164706f, 0.164706f, REAL_ONE); break;
	case ColorNames::MAROON: m_values.Set(0.501961f, 0.0f, 0.0f, REAL_ONE); break;
	case ColorNames::WHITE: m_values.Set(1.0f, 1.0f, 1.0f, REAL_ONE); break;
	case ColorNames::SNOW: m_values.Set(1.0f, 0.980392f, 0.980392f, REAL_ONE); break;
	case ColorNames::HONEYDEW: m_values.Set(0.941176f, 1.0f, 0.941176f, REAL_ONE); break;
	case ColorNames::MINT_CREAM: m_values.Set(0.960784f, 1.0f, 0.980392f, REAL_ONE); break;
	case ColorNames::AZURE: m_values.Set(0.941176f, 1.0f, 1.0f, REAL_ONE); break;
	case ColorNames::ALICE_BLUE: m_values.Set(0.941176f, 0.972549f, 1.0f, REAL_ONE); break;
	case ColorNames::GHOST_WHITE: m_values.Set(0.972549f, 0.972549f, 1.0f, REAL_ONE); break;
	case ColorNames::WHITE_SMOKE: m_values.Set(0.960784f, 0.960784f, 0.960784f, REAL_ONE); break;
	case ColorNames::SEASHELL: m_values.Set(1.0f, 0.960784f, 0.933333f, REAL_ONE); break;
	case ColorNames::BEIGE: m_values.Set(0.960784f, 0.960784f, 0.862745f, REAL_ONE); break;
	case ColorNames::OLD_LACE: m_values.Set(0.992157f, 0.960784f, 0.901961f, REAL_ONE); break;
	case ColorNames::FLORAL_WHITE: m_values.Set(1.0f, 0.980392f, 0.941176f, REAL_ONE); break;
	case ColorNames::IVORY: m_values.Set(1.0f, 1.0f, 0.941176f, REAL_ONE); break;
	case ColorNames::ANTIQUE_WHITE: m_values.Set(0.980392f, 0.921569f, 0.843137f, REAL_ONE); break;
	case ColorNames::LINEN: m_values.Set(0.980392f, 0.941176f, 0.901961f, REAL_ONE); break;
	case ColorNames::LAVENDER_BLUSH: m_values.Set(1.0f, 0.941176f, 0.960784f, REAL_ONE); break;
	case ColorNames::MISTY_ROSE: m_values.Set(1.0f, 0.894118f, 0.882353f, REAL_ONE); break;
	case ColorNames::GAINSBORO: m_values.Set(0.862745f, 0.862745f, 0.862745f, REAL_ONE); break;
	case ColorNames::LIGHT_GREY: m_values.Set(0.827451f, 0.827451f, 0.827451f, REAL_ONE); break;
	case ColorNames::SILVER: m_values.Set(0.752941f, 0.752941f, 0.752941f, REAL_ONE); break;
	case ColorNames::DARK_GREY: m_values.Set(0.662745f, 0.662745f, 0.662745f, REAL_ONE); break;
	case ColorNames::GREY: m_values.Set(0.501961f, 0.501961f, 0.501961f, REAL_ONE); break;
	case ColorNames::DIM_GREY: m_values.Set(0.411765f, 0.411765f, 0.411765f, REAL_ONE); break;
	case ColorNames::LIGHT_SLATE_GREY: m_values.Set(0.466667f, 0.533333f, 0.6f, REAL_ONE); break;
	case ColorNames::SLATE_GREY: m_values.Set(0.439216f, 0.501961f, 0.564706f, REAL_ONE); break;
	case ColorNames::BLACK: m_values.Set(0.0f, 0.0f, 0.0f, REAL_ONE); break;
	default:
		WARNING_LOG_RENDERING("Unknown color name \"", colorName, "\" has been specified. Using red color as default");
		m_values.Set(REAL_ONE, REAL_ZERO, REAL_ZERO, REAL_ONE);
		break;
	}
}

Rendering::Color::Color(Math::Real red, Math::Real green, Math::Real blue, Math::Real alpha /* = REAL_ONE */) :
	m_values(red, green, blue, alpha)
{
}

Rendering::Color::Color(const Math::Vector3D& rgbVector) :
	m_values(rgbVector.GetX(), rgbVector.GetY(), rgbVector.GetZ(), REAL_ONE)
{
}

Rendering::Color::Color(const Math::Vector4D& rgbaVector) :
	m_values(rgbaVector)
{
}

Rendering::Color::Color(const Color& color) :
	m_values(color.m_values)
{
}

Rendering::Color::Color(Color&& color) :
	m_values(std::move(color.m_values))
{
}

Rendering::Color& Rendering::Color::operator=(const Color& color)
{
	m_values = color.m_values;
	return *this;
}

Rendering::Color::~Color()
{
}

Rendering::Color Rendering::Color::operator+(const Color& color) const
{
	return Color(GetRed() + color.GetRed(), GetGreen() + color.GetGreen(), GetBlue() + color.GetBlue(), GetAlpha() + color.GetAlpha());
}

Rendering::Color& Rendering::Color::operator+=(const Color& color)
{
	m_values.Set(GetRed() + color.GetRed(), GetGreen() + color.GetGreen(), GetBlue() + color.GetBlue(), GetAlpha() + color.GetAlpha());
	return *this;
}

Rendering::Color Rendering::Color::operator*(Math::Real value) const
{
	return Color(GetRed() * value, GetGreen() * value, GetBlue() * value, GetAlpha() * value);
}

Rendering::Color& Rendering::Color::operator*=(Math::Real value)
{
	m_values.Set(GetRed() * value, GetGreen() * value, GetBlue() * value, GetAlpha() * value);
	return *this;
}

Rendering::Color Rendering::Color::operator*(const Color& color) const
{
	return Color(GetRed() * color.GetRed(), GetGreen() * color.GetGreen(), GetBlue() * color.GetBlue(), GetAlpha() * color.GetAlpha());
}

Rendering::Color& Rendering::Color::operator*=(const Color& color)
{
	m_values.Set(GetRed() * color.GetRed(), GetGreen() * color.GetGreen(), GetBlue() * color.GetBlue(), GetAlpha() * color.GetAlpha());
	return *this;
}