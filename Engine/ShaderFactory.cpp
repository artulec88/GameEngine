#include "stdafx.h"
#include "ShaderFactory.h"

#include <Utility\IConfig.h>

Engine::ShaderFactory::ShaderFactory(const std::string& shadersDirectory) :
	m_shadersDirectory(shadersDirectory)
	// TODO: If we initialize the map this way then Shader's copy ctrs will be used (instead of move ctrs). Google that!
	//m_shaderType2ShaderMap({ { ShaderTypes::AMBIENT, std::move(Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightShader", "ForwardAmbient"))) },
	//{ ShaderTypes::AMBIENT_TERRAIN, std::move(Shader(GET_CONFIG_VALUE_STR_ENGINE("ambientLightTerrainShader", "forward-ambient-terrain")))} })
{
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightShader", "forward-ambient.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightTerrainShader", "forward-ambient-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_FOG_LINEAR_PLANE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightFogLinearPlaneBasedShader", "forward-ambient-fog-linear-plane-based.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightFogExponentialPlaneBasedShader", "forward-ambient-fog-exponential-plane-based.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_FOG_LINEAR_RANGE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightFogLinearRangeBasedShader", "forward-ambient-fog-linear-range-based.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightFogExponentialRangeBasedShader", "forward-ambient-fog-exponential-range-based.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightFogLinearPlaneBasedTerrainShader", "forward-ambient-fog-linear-plane-based-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightFogExponentialPlaneBasedTerrainShader", "forward-ambient-fog-exponential-plane-based-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightFogLinearRangeBasedTerrainShader", "forward-ambient-fog-linear-range-based-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultAmbientLightFogExponentialRangeBasedTerrainShader", "forward-ambient-fog-exponential-range-based-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::SHADOW_MAP, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultShadowMapShader", "ShadowMapGenerator.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::SHADOW_MAP_CUBE, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultCubeShadowMapShader", "CubeShadowMapGenerator.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::FILTER_NULL, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultNullFilterShader", "Filter-null.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::FILTER_GAUSSIAN_BLUR, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultGaussBlurFilterShader", "filter-gaussBlur7x1.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::FILTER_FXAA, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultFxaaFilterShader", "filter-fxaa.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::SKYBOX, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultSkyboxShader", "skybox-shader.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::SKYBOX_PROCEDURAL, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultSkyboxProceduralShader", "skybox-procedural-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::TEXT_SIMPLE, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultTextShader", "text-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::TEXT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultTextShader2", "text-shader-2.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::WATER, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultWaterShader", "water-shader.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::WATER_NO_DIRECTIONAL_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultWaterNoDirectionalLightShader", "water-no-directional-light-shader.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::BILLBOARD, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultBillboardShader", "billboard-shader.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::PARTICLES, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultParticleShader", "particle-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderTypes::GUI, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultGuiShader", "gui-shader.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::DIRECTIONAL_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultDirectionalLightShader", "forward-directional.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultDirectionalLightTerrainShader", "forward-directional-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::DIRECTIONAL_LIGHT_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultDirectionalLightNoShadowShader", "forward-directional-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultDirectionalLightNoShadowTerrainShader", "forward-directional-terrain-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::POINT_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultPointLightShader", "forward-point.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::POINT_LIGHT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultPointLightTerrainShader", "forward-point-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::POINT_LIGHT_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultPointLightNoShadowShader", "forward-point-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::POINT_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultPointLightNoShadowTerrainShader", "forward-point-terrain-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::SPOT_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultSpotLightShader", "forward-spot.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::SPOT_LIGHT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultSpotLightTerrainShader", "forward-spot-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::SPOT_LIGHT_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultSpotLightNoShadowShader", "forward-spot-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderTypes::SPOT_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_ENGINE("defaultSpotLightNoShadowTerrainShader", "forward-spot-terrain-no-shadows.glsl")));
}


Engine::ShaderFactory::~ShaderFactory()
{
}

void Engine::ShaderFactory::CreateShader(ShaderTypes::ShaderType shaderType, const std::string& shaderFileName)
{
	m_shaderType2ShaderMap.emplace(shaderType, Rendering::Shader(shaderFileName));
}