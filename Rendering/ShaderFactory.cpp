#include "stdafx.h"
#include "ShaderFactory.h"

#include <Utility\IConfig.h>

Rendering::ShaderFactory::ShaderFactory(const std::string& shadersDirectory) :
	m_shadersDirectory(shadersDirectory)
	// TODO: If we initialize the map this way then Shader's copy ctrs will be used (instead of move ctrs). Google that!
	//m_shaderType2ShaderMap({ { ShaderIDs::AMBIENT, std::move(Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightShader", "ForwardAmbient"))) },
	//{ ShaderIDs::AMBIENT_TERRAIN, std::move(Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightTerrainShader", "forward-ambient-terrain")))} })
{
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightShader", "forward-ambient.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT_TERRAIN, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightTerrainShader", "forward-ambient-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT_FOG_LINEAR_PLANE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogLinearPlaneBasedShader", "forward-ambient-fog-linear-plane-based.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogExponentialPlaneBasedShader", "forward-ambient-fog-exponential-plane-based.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT_FOG_LINEAR_RANGE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogLinearRangeBasedShader", "forward-ambient-fog-linear-range-based.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogExponentialRangeBasedShader", "forward-ambient-fog-exponential-range-based.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogLinearPlaneBasedTerrainShader", "forward-ambient-fog-linear-plane-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogExponentialPlaneBasedTerrainShader", "forward-ambient-fog-exponential-plane-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogLinearRangeBasedTerrainShader", "forward-ambient-fog-linear-range-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogExponentialRangeBasedTerrainShader", "forward-ambient-fog-exponential-range-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::SHADOW_MAP, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultShadowMapShader", "ShadowMapGenerator.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::SHADOW_MAP_CUBE, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultCubeShadowMapShader", "CubeShadowMapGenerator.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::FILTER_NULL, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultNullFilterShader", "Filter-null.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::FILTER_GAUSSIAN_BLUR, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultGaussBlurFilterShader", "filter-gaussBlur7x1.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::FILTER_FXAA, Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultFxaaFilterShader", "filter-fxaa.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::SKYBOX, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultSkyboxShader", "skybox-shader.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::SKYBOX_PROCEDURAL, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultSkyboxProceduralShader", "skybox-procedural-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::TEXT_SIMPLE, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultTextShader", "text-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::TEXT, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultTextShader2", "text-shader-2.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::WATER, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultWaterShader", "water-shader.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::WATER_NO_DIRECTIONAL_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultWaterNoDirectionalLightShader", "water-no-directional-light-shader.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::BILLBOARD, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultBillboardShader", "billboard-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::PARTICLES, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultParticleShader", "particle-shader.glsl")));
	m_shaderType2ShaderMap.emplace(ShaderIDs::GUI, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultGuiShader", "gui-shader.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::DIRECTIONAL_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultDirectionalLightShader", "forward-directional.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::DIRECTIONAL_LIGHT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultDirectionalLightTerrainShader", "forward-directional-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::DIRECTIONAL_LIGHT_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultDirectionalLightNoShadowShader", "forward-directional-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultDirectionalLightNoShadowTerrainShader", "forward-directional-terrain-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::POINT_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultPointLightShader", "forward-point.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::POINT_LIGHT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultPointLightTerrainShader", "forward-point-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::POINT_LIGHT_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultPointLightNoShadowShader", "forward-point-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::POINT_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultPointLightNoShadowTerrainShader", "forward-point-terrain-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::SPOT_LIGHT, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultSpotLightShader", "forward-spot.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::SPOT_LIGHT_TERRAIN, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultSpotLightTerrainShader", "forward-spot-terrain.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::SPOT_LIGHT_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultSpotLightNoShadowShader", "forward-spot-no-shadows.glsl")));
	//m_shaderType2ShaderMap.emplace(ShaderIDs::SPOT_LIGHT_TERRAIN_NO_SHADOWS, Rendering::Shader(GET_CONFIG_VALUE_STR_RENDERING("defaultSpotLightNoShadowTerrainShader", "forward-spot-terrain-no-shadows.glsl")));
}


Rendering::ShaderFactory::~ShaderFactory()
{
}

const Rendering::Shader* Rendering::ShaderFactory::CreateShader(int shaderID, const std::string& shaderFileName)
{
	std::pair<std::map<int, Shader>::iterator, bool> shaderPair =
		m_shaderType2ShaderMap.insert(std::make_pair(shaderID, Shader(m_shadersDirectory + shaderFileName)));
	CHECK_CONDITION_RENDERING(shaderPair.second, Utility::Logging::WARNING, "Shader \"", shaderFileName, "\" has already been created.");
	return &shaderPair.first->second;
}