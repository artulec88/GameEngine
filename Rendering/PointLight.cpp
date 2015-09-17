#include "StdAfx.h"
#include "PointLight.h"
//#include "CoreEngine.h"
#include "Shader.h"
#include "Utility\IConfig.h"

using namespace Rendering::Lighting;
using namespace Utility;

/* static */ bool PointLight::pointLightsEnabled = true;

PointLight::PointLight(const Rendering::Color& color /* = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE) */, Math::Real intensity /* = REAL_ZERO */,
		const Rendering::Attenuation& attenuation /* = Attenuation(REAL_ZERO, REAL_ZERO, REAL_ONE) */) :
	BaseLight(color, intensity),
	m_attenuation(attenuation)
{
	CalculateRange();
	
	// Beware of using new operator in the constructor (See e.g. http://herbsutter.com/2008/07/25/constructor-exceptions-in-c-c-and-java/)
}

PointLight::~PointLight(void)
{
}

void PointLight::CalculateRange()
{
	Math::Real a = m_attenuation.GetExponent();
	Math::Real b = m_attenuation.GetLinear();

	Math::Real maxColorElement = (m_color.GetRed() > m_color.GetGreen()) ? m_color.GetRed() : m_color.GetGreen();
	if (m_color.GetBlue() > maxColorElement)
	{
		maxColorElement = m_color.GetBlue();
	}
	const int colorDepth = GET_CONFIG_VALUE("ColorDepth", 256);
	Math::Real c = m_attenuation.GetConstant() - colorDepth * m_intensity * maxColorElement;

	m_range = (-b + sqrt(b*b - 4*a*c)) / (2*a);
}

//void PointLight::InitializeShaders()
//{
//	SetShader(new Shader(GET_CONFIG_VALUE_STR("pointLightShader", "forward-point")));
//	CHECK_CONDITION_EXIT(m_shader != NULL, Utility::Critical, "Cannot initialize point light. Shader is NULL.");
//
//	SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("pointLightShaderTerrain", "forward-point-terrain")));
//	CHECK_CONDITION_EXIT(m_terrainShader != NULL, Utility::Critical, "Cannot initialize point light. Terrain shader is NULL.");
//}

bool PointLight::IsEnabled() const
{
	if (!pointLightsEnabled)
	{
		return false;
	}
	return BaseLight::IsEnabled();
}

float gTemp = REAL_ZERO; // TODO: Just temporary. Remove in the future.

void PointLight::Update(Math::Real deltaTime)
{
	gTemp += deltaTime * 0.0001f; // TODO: Do not use hard-coded values! Ever!
	if (gTemp > 20.0f * Math::M_PI) // TODO: Do not use hard-coded values! Ever!
	{
		gTemp = REAL_ZERO;
	}
	GetTransform().SetPos(GetTransform().GetPos() + (Math::Vector3D(sin(gTemp) / 2000, cos(gTemp) / 4000, cos(gTemp) / 2000))); // TODO: Do not use hard-coded values! Ever!
	//GetTransform().SetPos(GetTransform().GetPos() + (Math::Vector3D(REAL_ZERO, cos(gTemp) / 2000, REAL_ZERO)));
}