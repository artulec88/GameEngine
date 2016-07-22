#include "stdafx.h"
#include "ShaderFactory.h"

#include <Utility\IConfig.h>

Rendering::ShaderFactory::ShaderFactory() //: // TODO: If we initialize the map this way then Shader's copy ctrs will be used (instead of move ctrs). Google that!
	//m_shaderType2ShaderMap({ { ShaderTypes::AMBIENT, std::move(Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightShader", "ForwardAmbient"))) },
	//{ ShaderTypes::AMBIENT_TERRAIN, std::move(Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightTerrainShader", "forward-ambient-terrain")))} })
{
	// This way we make sure that Shader's move ctrs are used instead of copy ctrs.
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightShader", "ForwardAmbient"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightTerrainShader", "forward-ambient-terrain"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_FOG_LINEAR_PLANE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightFogLinearPlaneBasedShader", "forward-ambient-fog-linear-plane-based"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightFogExponentialPlaneBasedShader", "forward-ambient-fog-exponential-plane-based"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_FOG_LINEAR_RANGE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightFogLinearRangeBasedShader", "forward-ambient-fog-linear-range-based"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightFogExponentialRangeBasedShader", "forward-ambient-fog-exponential-range-based"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightFogLinearPlaneBasedTerrainShader", "forward-ambient-fog-linear-plane-based-terrain"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightFogExponentialPlaneBasedTerrainShader", "forward-ambient-fog-exponential-plane-based-terrain"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightFogLinearRangeBasedTerrainShader", "forward-ambient-fog-linear-range-based-terrain"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightFogExponentialRangeBasedTerrainShader", "forward-ambient-fog-exponential-range-based-terrain"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SHADOW_MAP, Shader(GET_CONFIG_VALUE_STR_RENDERING("shadowMapShader", "ShadowMapGenerator"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SHADOW_MAP_CUBE, Shader(GET_CONFIG_VALUE_STR_RENDERING("cubeShadowMapShader", "CubeShadowMapGenerator"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::FILTER_NULL, Shader(GET_CONFIG_VALUE_STR_RENDERING("nullFilterShader", "Filter-null"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::FILTER_GAUSSIAN_BLUR, Shader(GET_CONFIG_VALUE_STR_RENDERING("gaussBlurFilterShader", "filter-gaussBlur7x1"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::FILTER_FXAA, Shader(GET_CONFIG_VALUE_STR_RENDERING("fxaaFilterShader", "filter-fxaa"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SKYBOX, Shader(GET_CONFIG_VALUE_STR_RENDERING("skyboxShader", "skybox-shader"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SKYBOX_PROCEDURAL, Shader(GET_CONFIG_VALUE_STR_RENDERING("skyboxProceduralShader", "skybox-procedural"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::TEXT_SIMPLE, Shader(GET_CONFIG_VALUE_STR_RENDERING("textShader", "text-shader"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::TEXT, Shader(GET_CONFIG_VALUE_STR_RENDERING("textShader2", "text-shader-2"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::WATER, Shader(GET_CONFIG_VALUE_STR_RENDERING("waterShader", "water-shader"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::WATER_NO_DIRECTIONAL_LIGHT, Shader(GET_CONFIG_VALUE_STR_RENDERING("waterNoDirectionalLightShader", "water-no-directional-light-shader"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::BILLBOARD, Shader(GET_CONFIG_VALUE_STR_RENDERING("billboardShader", "billboard-shader"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::PARTICLES, Shader(GET_CONFIG_VALUE_STR_RENDERING("particleShader", "particle-shader"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::GUI, Shader(GET_CONFIG_VALUE_STR_RENDERING("guiShader", "gui-shader"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::DIRECTIONAL_LIGHT, Shader(GET_CONFIG_VALUE_STR_RENDERING("directionalLightShader", "forward-directional"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN, Shader(GET_CONFIG_VALUE_STR_RENDERING("directionalLightTerrainShader", "forward-directional-terrain"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::DIRECTIONAL_LIGHT_NO_SHADOWS, Shader(GET_CONFIG_VALUE_STR_RENDERING("directionalLightNoShadowShader", "forward-directional-no-shadows"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS, Shader(GET_CONFIG_VALUE_STR_RENDERING("directionalLightNoShadowTerrainShader", "forward-directional-terrain-no-shadows"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::POINT_LIGHT, Shader(GET_CONFIG_VALUE_STR_RENDERING("pointLightShader", "forward-point"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::POINT_LIGHT_TERRAIN, Shader(GET_CONFIG_VALUE_STR_RENDERING("pointLightShaderTerrain", "forward-point-terrain"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::POINT_LIGHT_NO_SHADOWS, Shader(GET_CONFIG_VALUE_STR_RENDERING("pointLightNoShadowShader", "forward-point-no-shadows"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::POINT_LIGHT_TERRAIN_NO_SHADOWS, Shader(GET_CONFIG_VALUE_STR_RENDERING("pointLightNoShadowTerrainShader", "forward-point-terrain-no-shadows"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SPOT_LIGHT, Shader(GET_CONFIG_VALUE_STR_RENDERING("spotLightShader", "forward-spot"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SPOT_LIGHT_TERRAIN, Shader(GET_CONFIG_VALUE_STR_RENDERING("spotLightShaderTerrain", "forward-spot-terrain"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SPOT_LIGHT_NO_SHADOWS, Shader(GET_CONFIG_VALUE_STR_RENDERING("spotLightNoShadowShader", "forward-spot-no-shadows"))));
	m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SPOT_LIGHT_TERRAIN_NO_SHADOWS, Shader(GET_CONFIG_VALUE_STR_RENDERING("spotLightNoShadowTerrainShader", "forward-spot-terrain-no-shadows"))));
}


Rendering::ShaderFactory::~ShaderFactory()
{
}

//void Rendering::ShaderFactory::CreateShader(ShaderTypes::ShaderType shaderType, const std::string& shaderFileName)
//{
//	m_shaders.push_back()
//}