#include "StdAfx.h"
#include "PointLight.h"
#include "Shader.h"
#include "Utility/IConfig.h"

rendering::lighting::PointLight::PointLight(const math::Transform& transform, const Color& color, math::Real intensity, int shaderId,
	int terrainShaderId, int noShadowShaderId, int noShadowTerrainShaderId, const Attenuation& attenuation) :
	BaseLight(transform, color, intensity, shaderId, terrainShaderId, noShadowShaderId, noShadowTerrainShaderId, false),
	m_attenuation(attenuation),
	m_range(CalculateRange())
{
	// Beware of using new operator in the constructor (See e.g. http://herbsutter.com/2008/07/25/constructor-exceptions-in-c-c-and-java/)
}

rendering::lighting::PointLight::~PointLight()
{
}

math::Real rendering::lighting::PointLight::CalculateRange() const
{
	// TODO: If exponent equals 0.0 then we will divide by zero at the end of this function.
	const auto a = m_attenuation.GetExponent();
	const auto b = m_attenuation.GetLinear();

	const auto maxColorElement = m_color.GetRed() > m_color.GetGreen() ?
		(m_color.GetRed() > m_color.GetBlue() ? m_color.GetRed() : m_color.GetBlue()) :
		m_color.GetGreen() > m_color.GetBlue() ? m_color.GetGreen() : m_color.GetBlue();
	const auto colorDepth = GET_CONFIG_VALUE_RENDERING("colorDepth", 256); // TODO: Think about moving this constant somewhere else and also make it a static constexpr.
	const auto c = m_attenuation.GetConstant() - colorDepth * m_intensity * maxColorElement;

	return (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
}

//float gTemp = REAL_ZERO; // TODO: Just temporary. Remove in the future.

//void PointLight::Update(math::Real deltaTime)
//{
//	gTemp += deltaTime * 0.0001f; // TODO: Do not use hard-coded values! Ever!
//	if (gTemp > 20.0f * math::PI) // TODO: Do not use hard-coded values! Ever!
//	{
//		gTemp = REAL_ZERO;
//	}
//	GetTransform().SetPos(GetTransform().GetPos() + (math::Vector3D(sin(gTemp) / 2000, cos(gTemp) / 4000, cos(gTemp) / 2000))); // TODO: Do not use hard-coded values! Ever!
//	//GetTransform().SetPos(GetTransform().GetPos() + (math::Vector3D(REAL_ZERO, cos(gTemp) / 2000, REAL_ZERO)));
//}