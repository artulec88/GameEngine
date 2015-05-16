#include "StdAfx.h"
#include "PointLight.h"
//#include "CoreEngine.h"
#include "Shader.h"
#include "Utility\IConfig.h"

using namespace Rendering;
using namespace Utility;

/* static */ bool PointLight::pointLightsEnabled = true;

PointLight::PointLight(const Color& color /* = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE) */, Math::Real intensity /* = REAL_ZERO */,
		const Attenuation& attenuation /* = Attenuation(REAL_ZERO, REAL_ZERO, REAL_ONE) */) :
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
//	ASSERT(m_shader != NULL);
//	if (m_shader == NULL)
//	{
//		LOG(Utility::Critical, LOGPLACE, "Cannot initialize directional light. Shader is NULL.");
//		exit(EXIT_FAILURE);
//	}
//
//	SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("pointLightShaderTerrain", "forward-point-terrain")));
//	ASSERT(m_terrainShader != NULL);
//	if (m_terrainShader == NULL)
//	{
//		LOG(Utility::Critical, LOGPLACE, "Cannot initialize directional light. Terrain shader is NULL.");
//		exit(EXIT_FAILURE);
//	}
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
	gTemp += deltaTime * 0.0001f;
	if (gTemp > 20.0f * Math::M_PI)
	{
		gTemp = REAL_ZERO;
	}
	GetTransform().SetPos(GetTransform().GetPos() + (Math::Vector3D(sin(gTemp) / 2000, cos(gTemp) / 4000, cos(gTemp) / 2000)));
	//GetTransform().SetPos(GetTransform().GetPos() + (Math::Vector3D(REAL_ZERO, cos(gTemp) / 2000, REAL_ZERO)));
}