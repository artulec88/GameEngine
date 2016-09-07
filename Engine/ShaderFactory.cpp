#include "stdafx.h"
#include "ShaderFactory.h"

#include <Utility\IConfig.h>

Engine::ShaderFactory::ShaderFactory(const std::string& shadersDirectory) :
	m_shadersDirectory(shadersDirectory)
	// TODO: If we initialize the map this way then Shader's copy ctrs will be used (instead of move ctrs). Google that!
	//m_shaderType2ShaderMap({ { ShaderTypes::AMBIENT, std::move(Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightShader", "ForwardAmbient"))) },
	//{ ShaderTypes::AMBIENT_TERRAIN, std::move(Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightTerrainShader", "forward-ambient-terrain")))} })
{
	// This way we make sure that Shader's move ctrs are used instead of copy ctrs.
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightShader", "ForwardAmbient"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightTerrainShader", "forward-ambient-terrain"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_FOG_LINEAR_PLANE_BASED, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogLinearPlaneBasedShader", "forward-ambient-fog-linear-plane-based"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogExponentialPlaneBasedShader", "forward-ambient-fog-exponential-plane-based"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_FOG_LINEAR_RANGE_BASED, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogLinearRangeBasedShader", "forward-ambient-fog-linear-range-based"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogExponentialRangeBasedShader", "forward-ambient-fog-exponential-range-based"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogLinearPlaneBasedTerrainShader", "forward-ambient-fog-linear-plane-based-terrain"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogExponentialPlaneBasedTerrainShader", "forward-ambient-fog-exponential-plane-based-terrain"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogLinearRangeBasedTerrainShader", "forward-ambient-fog-linear-range-based-terrain"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogExponentialRangeBasedTerrainShader", "forward-ambient-fog-exponential-range-based-terrain"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SHADOW_MAP, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("shadowMapShader", "ShadowMapGenerator"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SHADOW_MAP_CUBE, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("cubeShadowMapShader", "CubeShadowMapGenerator"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::FILTER_NULL, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("nullFilterShader", "Filter-null"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::FILTER_GAUSSIAN_BLUR, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("gaussBlurFilterShader", "filter-gaussBlur7x1"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::FILTER_FXAA, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("fxaaFilterShader", "filter-fxaa"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SKYBOX, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("skyboxShader", "skybox-shader"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SKYBOX_PROCEDURAL, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("skyboxProceduralShader", "skybox-procedural"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::TEXT_SIMPLE, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("textShader", "text-shader"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::TEXT, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("textShader2", "text-shader-2"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::WATER, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("waterShader", "water-shader"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::WATER_NO_DIRECTIONAL_LIGHT, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("waterNoDirectionalLightShader", "water-no-directional-light-shader"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::BILLBOARD, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("billboardShader", "billboard-shader"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::PARTICLES, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("particleShader", "particle-shader"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::GUI, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("guiShader", "gui-shader"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::DIRECTIONAL_LIGHT, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("directionalLightShader", "forward-directional"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("directionalLightTerrainShader", "forward-directional-terrain"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::DIRECTIONAL_LIGHT_NO_SHADOWS, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("directionalLightNoShadowShader", "forward-directional-no-shadows"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("directionalLightNoShadowTerrainShader", "forward-directional-terrain-no-shadows"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::POINT_LIGHT, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("pointLightShader", "forward-point"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::POINT_LIGHT_TERRAIN, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("pointLightShaderTerrain", "forward-point-terrain"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::POINT_LIGHT_NO_SHADOWS, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("pointLightNoShadowShader", "forward-point-no-shadows"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::POINT_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("pointLightNoShadowTerrainShader", "forward-point-terrain-no-shadows"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SPOT_LIGHT, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("spotLightShader", "forward-spot"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SPOT_LIGHT_TERRAIN, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("spotLightShaderTerrain", "forward-spot-terrain"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SPOT_LIGHT_NO_SHADOWS, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("spotLightNoShadowShader", "forward-spot-no-shadows"))));
	//m_shaderType2ShaderMap.insert(std::make_pair(ShaderTypes::SPOT_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(m_shadersDirectory + GET_CONFIG_VALUE_STR_ENGINE("spotLightNoShadowTerrainShader", "forward-spot-terrain-no-shadows"))));

	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightShader", "forward-ambient.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightTerrainShader", "forward-ambient-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_FOG_LINEAR_PLANE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogLinearPlaneBasedShader", "forward-ambient-fog-linear-plane-based.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogExponentialPlaneBasedShader", "forward-ambient-fog-exponential-plane-based.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_FOG_LINEAR_RANGE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogLinearRangeBasedShader", "forward-ambient-fog-linear-range-based.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogExponentialRangeBasedShader", "forward-ambient-fog-exponential-range-based.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogLinearPlaneBasedTerrainShader", "forward-ambient-fog-linear-plane-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogExponentialPlaneBasedTerrainShader", "forward-ambient-fog-exponential-plane-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogLinearRangeBasedTerrainShader", "forward-ambient-fog-linear-range-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightFogExponentialRangeBasedTerrainShader", "forward-ambient-fog-exponential-range-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::SHADOW_MAP, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("shadowMapShader", "ShadowMapGenerator.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::SHADOW_MAP_CUBE, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("cubeShadowMapShader", "CubeShadowMapGenerator.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::FILTER_NULL, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("nullFilterShader", "Filter-null.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::FILTER_GAUSSIAN_BLUR, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("gaussBlurFilterShader", "filter-gaussBlur7x1.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::FILTER_FXAA, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("fxaaFilterShader", "filter-fxaa.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::SKYBOX, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("skyboxShader", "skybox-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::SKYBOX_PROCEDURAL, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("skyboxProceduralShader", "skybox-procedural-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::TEXT_SIMPLE, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("textShader", "text-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::TEXT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("textShader2", "text-shader-2.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::WATER, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("waterShader", "water-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::WATER_NO_DIRECTIONAL_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("waterNoDirectionalLightShader", "water-no-directional-light-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::BILLBOARD, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("billboardShader", "billboard-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::PARTICLES, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("particleShader", "particle-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::GUI, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("guiShader", "gui-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::DIRECTIONAL_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("directionalLightShader", "forward-directional.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("directionalLightTerrainShader", "forward-directional-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::DIRECTIONAL_LIGHT_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("directionalLightNoShadowShader", "forward-directional-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("directionalLightNoShadowTerrainShader", "forward-directional-terrain-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::POINT_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("pointLightShader", "forward-point.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::POINT_LIGHT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("pointLightTerrainShader", "forward-point-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::POINT_LIGHT_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("pointLightNoShadowShader", "forward-point-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::POINT_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("pointLightNoShadowTerrainShader", "forward-point-terrain-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::SPOT_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("spotLightShader", "forward-spot.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::SPOT_LIGHT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("spotLightTerrainShader", "forward-spot-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::SPOT_LIGHT_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("spotLightNoShadowShader", "forward-spot-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::SPOT_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("spotLightNoShadowTerrainShader", "forward-spot-terrain-no-shadows.glsl")));
}


Engine::ShaderFactory::~ShaderFactory()
{
}

//void Engine::ShaderFactory::CreateShader(ShaderTypes::ShaderType shaderType, const std::string& shaderFileName)
//{
//	m_shaders.push_back()
//}