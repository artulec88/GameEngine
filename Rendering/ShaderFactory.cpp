#include "stdafx.h"
#include "ShaderFactory.h"
#include "ShaderIDs.h"

#include "Utility/IConfig.h"

rendering::ShaderFactory::ShaderFactory(const std::string& shadersDirectory) :
	m_shadersDirectory(shadersDirectory)
	// TODO: If we initialize the map this way then Shader's copy ctrs will be used (instead of move ctrs). Google that!
	//m_shaderType2ShaderMap({ { shader_ids::AMBIENT, std::move(Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightShader", "ForwardAmbient"))) },
	//{ shader_ids::AMBIENT_TERRAIN, std::move(Shader(GET_CONFIG_VALUE_STR_RENDERING("ambientLightTerrainShader", "forward-ambient-terrain")))} })
{
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightShader", "forward-ambient.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT_TERRAIN, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightTerrainShader", "forward-ambient-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT_FOG_LINEAR_PLANE_BASED, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogLinearPlaneBasedShader", "forward-ambient-fog-linear-plane-based.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogExponentialPlaneBasedShader", "forward-ambient-fog-exponential-plane-based.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT_FOG_LINEAR_RANGE_BASED, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogLinearRangeBasedShader", "forward-ambient-fog-linear-range-based.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogExponentialRangeBasedShader", "forward-ambient-fog-exponential-range-based.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogLinearPlaneBasedTerrainShader", "forward-ambient-fog-linear-plane-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogExponentialPlaneBasedTerrainShader", "forward-ambient-fog-exponential-plane-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogLinearRangeBasedTerrainShader", "forward-ambient-fog-linear-range-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultAmbientLightFogExponentialRangeBasedTerrainShader", "forward-ambient-fog-exponential-range-based-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::SHADOW_MAP, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultShadowMapShader", "shadowMapGenerator.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::SHADOW_MAP_CUBE, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultCubeShadowMapShader", "CubeShadowMapGenerator.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::FILTER_NULL, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultNullFilterShader", "Filter-null.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::FILTER_GAUSSIAN_BLUR, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultGaussBlurFilterShader", "filter-gaussBlur7x1.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::FILTER_FXAA, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultFxaaFilterShader", "filter-fxaa.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::SKYBOX, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultSkyboxShader", "skybox-shader.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::SKYBOX_PROCEDURAL, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultSkyboxProceduralShader", "skybox-procedural-shader.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::TEXT_SIMPLE, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultTextShader", "text-shader.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::TEXT, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultTextShader2", "text-shader-2.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::WATER, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultWaterShader", "water-shader.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::WATER_NO_DIRECTIONAL_LIGHT, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultWaterNoDirectionalLightShader", "water-no-directional-light-shader.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::BILLBOARD, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultBillboardShader", "billboard-shader.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::PARTICLES, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultParticleShader", "particle-shader.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::PARTICLES_COLORS, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultParticleColorShader", "particle-shader-color.glsl"))),
		m_shaderType2ShaderMap.emplace(shader_ids::PARTICLES_UNIFORM_COLORS, Shader(m_shadersDirectory,
			GET_CONFIG_VALUE_STR_RENDERING("defaultParticleUniformColorShader", "particle-shader-uniform-color.glsl"))),
	m_shaderType2ShaderMap.emplace(shader_ids::GUI, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultGuiShader", "gui-shader.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::DIRECTIONAL_LIGHT, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultDirectionalLightShader", "forward-directional.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::DIRECTIONAL_LIGHT_TERRAIN, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultDirectionalLightTerrainShader", "forward-directional-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::DIRECTIONAL_LIGHT_NO_SHADOWS, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultDirectionalLightNoShadowShader", "forward-directional-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultDirectionalLightNoShadowTerrainShader", "forward-directional-terrain-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::POINT_LIGHT, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultPointLightShader", "forward-point.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::POINT_LIGHT_TERRAIN, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultPointLightTerrainShader", "forward-point-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::POINT_LIGHT_NO_SHADOWS, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultPointLightNoShadowShader", "forward-point-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::POINT_LIGHT_TERRAIN_NO_SHADOWS, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultPointLightNoShadowTerrainShader", "forward-point-terrain-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::SPOT_LIGHT, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultSpotLightShader", "forward-spot.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::SPOT_LIGHT_TERRAIN, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultSpotLightTerrainShader", "forward-spot-terrain.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::SPOT_LIGHT_NO_SHADOWS, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultSpotLightNoShadowShader", "forward-spot-no-shadows.glsl")));
	m_shaderType2ShaderMap.emplace(shader_ids::SPOT_LIGHT_TERRAIN_NO_SHADOWS, Shader(m_shadersDirectory,
		GET_CONFIG_VALUE_STR_RENDERING("defaultSpotLightNoShadowTerrainShader", "forward-spot-terrain-no-shadows.glsl")));
}


rendering::ShaderFactory::~ShaderFactory()
{
}

const rendering::Shader* rendering::ShaderFactory::CreateShader(int shaderId, const std::string& shaderFileName)
{
	const auto shaderPair = m_shaderType2ShaderMap.insert(std::make_pair(shaderId, Shader(m_shadersDirectory, shaderFileName)));
	CHECK_CONDITION_RENDERING(shaderPair.second, Utility::Logging::WARNING, "Shader \"", shaderFileName, "\" has already been created.");
	return &shaderPair.first->second;
}