#include "StdAfx.h"
#include "Renderer.h"
#include "ShadowInfo.h"
#include "Mesh.h"
#include "MeshIDs.h"
#include "GuiImageControl.h"
#include "AntTweakBarTypes.h"
#include "ShaderIDs.h"

#include "Math/FloatingPoint.h"

#include "Utility/IConfig.h"
#include "Utility/ILogger.h"
//#include "Utility/FileNotFoundException.h"

//#include <iomanip>

// TODO: BIAS_MATRIX could and should be a constexpr!
/* static */ const math::Matrix4D rendering::Renderer::BIAS_MATRIX(math::Matrix4D(0.5f /* scale matrix */) * math::Matrix4D(REAL_ONE, REAL_ONE, REAL_ONE /* translation matrix */)); // FIXME: Check matrix multiplication
///* static */ const Matrix4D Renderer::BIAS_MATRIX;

rendering::Renderer::Renderer(int windowWidth, int windowHeight, const std::string& modelsDirectory, const std::string& texturesDirectory, const std::string& shadersDirectory,
	const std::string& fontsDirectory, aliasing::AntiAliasingMethod antiAliasingMethod) :
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_antiAliasingMethod(antiAliasingMethod),
	m_applyFilterEnabled(GET_CONFIG_VALUE_RENDERING("applyFilterEnabled", true)),
	m_backgroundColor(GET_CONFIG_VALUE_RENDERING("ClearColorRed", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("ClearColorGreen", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("ClearColorBlue", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("ClearColorAlpha", REAL_ONE)),
	//m_shadowsEnabled(GET_CONFIG_VALUE_RENDERING("shadowsEnabled", true)),
	//m_pointLightShadowsEnabled(GET_CONFIG_VALUE_RENDERING("pointLightShadowsEnabled", false)),
	m_fogInfo(Color(GET_CONFIG_VALUE_RENDERING("fogColorRed", 0.7f), GET_CONFIG_VALUE_RENDERING("fogColorGreen", 0.7f), GET_CONFIG_VALUE_RENDERING("fogColorBlue", 0.7f)),
		GET_CONFIG_VALUE_RENDERING("fogStart", 8.0f), GET_CONFIG_VALUE_RENDERING("fogEnd", 50.0f), GET_CONFIG_VALUE_RENDERING("fogDensityFactor", 0.2f),
		GET_CONFIG_VALUE_RENDERING("fogGradient", 0.005f), static_cast<fog_effect::FogFallOffType>(GET_CONFIG_VALUE_RENDERING("fogFallOffType", 0)),
		static_cast<fog_effect::FogCalculationType>(GET_CONFIG_VALUE_RENDERING("fogCalculationType", 0)), GET_CONFIG_VALUE_RENDERING("fogEnabled", true)),
	m_ambientLightEnabled(GET_CONFIG_VALUE_RENDERING("ambientLightEnabled", true)),
	m_currentLight(nullptr),
	m_currentPointLight(nullptr),
	//m_currentSpotLight(nullptr),
	m_currentCamera(nullptr),
	m_tempCamera(nullptr),
	m_shaderFactory(shadersDirectory),
	m_fontFactory(m_shaderFactory.GetShader(shader_ids::TEXT), texturesDirectory, fontsDirectory),
	m_meshFactory(modelsDirectory, texturesDirectory),
	m_textureFactory(texturesDirectory),
	m_displayTexture(windowWidth, windowHeight, nullptr, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, GL_REPEAT, GL_COLOR_ATTACHMENT0),
	m_filterCamera(math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), math::Quaternion(math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), math::Angle(180.0f)), math::Matrix4D::IDENTITY_MATRIX, 0.005f),
	m_altCamera(math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), math::Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), math::Matrix4D(), 0.005f),
	m_filterTransform(math::Vector3D(), math::Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO) /* to make the plane face towards the camera. See "OpenGL Game Rendering Tutorial: Shadow Mapping Preparations" https://www.youtube.com/watch?v=kyjDP68s9vM&index=8&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl (starts around 14:10) */, REAL_ONE),
	m_filterMesh(m_meshFactory.GetMesh(mesh_ids::SIMPLE_PLANE)),
	m_fxaaSpanMax(GET_CONFIG_VALUE_RENDERING("fxaaSpanMax", 8.0f)),
	m_fxaaReduceMin(GET_CONFIG_VALUE_RENDERING("fxaaReduceMin", REAL_ONE / 128.0f)),
	m_fxaaReduceMul(GET_CONFIG_VALUE_RENDERING("fxaaReduceMul", REAL_ONE / 8.0f)),
	m_defaultShadowMinVariance(GET_CONFIG_VALUE_RENDERING("defaultShadowMinVariance", 0.00002f)),
	m_cubeShadowMap(),
	m_shadowMaps({ Texture(2, 2, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, GL_CLAMP_TO_EDGE /* we want clamping so that for the pixels outside the shadow map range we don't return some value from a completely different point in the scene */, GL_COLOR_ATTACHMENT0 /* we're going to render color information */),
		Texture(4, 4, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(8, 8, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(16, 16, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(32, 32, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(64, 64, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(128, 128, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(256, 256, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(512, 512, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(1024, 1024, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(2048, 2048, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0) }),
	m_shadowMapTempTargets({ Texture(2, 2, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, GL_CLAMP_TO_EDGE /* we do want clamping so that for the pixels outside the shadow map range we don't return some value from a completely different point in the scene */, GL_COLOR_ATTACHMENT0 /* we're going to render color information */),
		Texture(4, 4, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(8, 8, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(16, 16, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(32, 32, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(64, 64, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(128, 128, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(256, 256, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(512, 512, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(1024, 1024, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0),
		Texture(2048, 2048, nullptr, GL_TEXTURE_2D, GL_NEAREST, GL_RG32F, GL_RGBA, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0) }),
	m_samplerMap(),
	m_lightMatrix(REAL_ZERO /* scale matrix */),
	m_defaultClipPlane(REAL_ZERO, -REAL_ONE, REAL_ZERO, 1000000 /* a high value so that nothing is culled by the clipping plane */),
	m_waterWaveStrength(GET_CONFIG_VALUE_RENDERING("waterWaveStrength", 0.04f)),
	m_waterShineDamper(GET_CONFIG_VALUE_RENDERING("waterShineDamper", 20.0f)),
	m_waterReflectivity(GET_CONFIG_VALUE_RENDERING("waterReflectivity", 0.5f)),
	m_waterWaveSpeed(GET_CONFIG_VALUE_RENDERING("waterWaveSpeed", 0.0003f)),
	m_waterMoveFactor(GET_CONFIG_VALUE_RENDERING("waterMoveFactor", 0.0f)),
	m_waterRefractionClippingPlane(GET_CONFIG_VALUE_RENDERING("waterRefractionClippingPlaneNormal_x", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("waterRefractionClippingPlaneNormal_y", -REAL_ONE), GET_CONFIG_VALUE_RENDERING("waterRefractionClippingPlaneNormal_z", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("waterRefractionClippingPlaneOriginDistance", 10.0f)),
	m_waterReflectionClippingPlane(GET_CONFIG_VALUE_RENDERING("waterReflectionClippingPlaneNormal_x", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("waterReflectionClippingPlaneNormal_y", REAL_ONE), GET_CONFIG_VALUE_RENDERING("waterReflectionClippingPlaneNormal_z", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("waterReflectionClippingPlaneOriginDistance", REAL_ZERO)),
	m_waterDuDvTexture(texturesDirectory + GET_CONFIG_VALUE_STR_RENDERING("waterDUDVMap", "waterDUDV.png")), // TODO: This texture will not be available from the textures factory.
	m_waterNormalMap(texturesDirectory + GET_CONFIG_VALUE_STR_RENDERING("waterNormalMap", "waterNormalMap.png")), // TODO: This texture will not be available from the textures factory.
	m_waterRefractionTexture(2, GET_CONFIG_VALUE_RENDERING("waterRefractionTextureWidth", 1280), GET_CONFIG_VALUE_RENDERING("waterRefractionTextureHeight", 720), std::vector<unsigned char*>{ nullptr, nullptr }.data(), GL_TEXTURE_2D, std::vector<GLfloat>{ GL_LINEAR, GL_LINEAR }.data(), std::vector<GLenum>{ GL_RGB, GL_DEPTH_COMPONENT32 }.data(), std::vector<GLenum>{ GL_RGBA, GL_DEPTH_COMPONENT }.data(), GL_REPEAT, std::vector<GLenum>{ GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT }.data()),
	m_waterReflectionTexture(GET_CONFIG_VALUE_RENDERING("waterReflectionTextureWidth", 320), GET_CONFIG_VALUE_RENDERING("waterReflectionTextureHeight", 180), nullptr, GL_TEXTURE_2D, GL_LINEAR, GL_RGB, GL_RGBA, GL_REPEAT, GL_COLOR_ATTACHMENT0),
	m_waterLightReflectionEnabled(false),
	m_waterFresnelEffectFactor(GET_CONFIG_VALUE_RENDERING("waterFresnelEffectFactor", 2.0f)),
	m_waterNormalVerticalFactor(GET_CONFIG_VALUE_RENDERING("waterNormalVerticalFactor", 3.0f)),
	m_maxParticlesCount(GET_CONFIG_VALUE_RENDERING("maxParticlesCount", 10000)), // TODO: This variable is also retrieved in the MeshFactory.
	m_particleQuad(dynamic_cast<const InstanceMesh*>(m_meshFactory.GetMesh(mesh_ids::PARTICLE))),
	m_particleInstanceVboData(m_maxParticlesCount * m_particleQuad->GetInstanceDataLength()),
	m_mappedValues()
#ifdef ANT_TWEAK_BAR_ENABLED
	, m_propertiesBar(nullptr),
	m_cameraBar(nullptr),
	m_lightsBar(nullptr),
	//m_cameraMembers(), // Gives a compiler warning C4351: new behavior: elements of array will be default initialized
	m_cameraType()
#endif
#ifdef DEBUG_RENDERING_ENABLED
	//,m_guiTextures(),
#endif
#ifdef PROFILING_RENDERING_MODULE_ENABLED
	,m_classStats(STATS_STORAGE.GetClassStats("Renderer"))
#endif
{
	START_PROFILING_RENDERING(true, "");

	SetSamplerSlot("diffuse", 0);
	SetSamplerSlot("normalMap", 1);
	SetSamplerSlot("displacementMap", 2);
	SetSamplerSlot("shadowMap", 3);
	SetSamplerSlot("cubeShadowMap", 5);
	SetSamplerSlot("cubeMapDay", 0);
	SetSamplerSlot("cubeMapNight", 1);
	SetSamplerSlot("filterTexture", 0);
	SetSamplerSlot("diffuse2", 4);
	SetSamplerSlot("diffuse3", 5);
	SetSamplerSlot("diffuse4", 6);
	SetSamplerSlot("blendMap", 7);
	SetSamplerSlot("waterReflectionTexture", 0);
	SetSamplerSlot("waterRefractionTexture", 1);
	SetSamplerSlot("waterDUDVMap", 2);
	SetSamplerSlot("waterNormalMap", 3);
	SetSamplerSlot("waterDepthMap", 4);

#ifndef ANT_TWEAK_BAR_ENABLED
	m_mappedValues.SetVector3D("ambientFogColor", m_fogColor);
	m_mappedValues.SetReal("ambientFogStart", m_fogStart);
	m_mappedValues.SetReal("ambientFogEnd", m_fogEnd);
	m_mappedValues.SetReal("ambientFogDensityFactor", m_fogDensityFactor);
	m_mappedValues.SetReal("ambientFogGradient", m_fogGradient);
	m_mappedValues.SetVector3D("ambientIntensity", m_ambientLight);
	m_mappedValues.SetVector4D("clipPlane", m_defaultClipPlane); // The workaround for some drivers ignoring the glDisable(GL_CLIP_DISTANCE0) method
#endif

	//m_altCamera.GetTransform().Rotate(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Angle(180));

	m_cubeShadowMap.Init(windowWidth, windowHeight);

	m_mappedValues.SetTexture("displayTexture", &m_displayTexture);
#ifndef ANT_TWEAK_BAR_ENABLED
	SetReal("fxaaSpanMax", m_fxaaSpanMax);
	SetReal("fxaaReduceMin", m_fxaaReduceMin);
	SetReal("fxaaReduceMul", m_fxaaReduceMul);
#endif

#ifdef DEBUG_RENDERING_ENABLED
	m_guiControls.push_back(std::make_unique<controls::GuiImageControl>(&m_shadowMaps[9], math::Vector2D(1.0, 0.0f), math::Angle(30.0f), math::Vector2D(0.25f, 0.25f)));
#endif

	STOP_PROFILING_RENDERING("");
}


rendering::Renderer::~Renderer()
{
	INFO_LOG_RENDERING("Destroying rendering engine...");
	START_PROFILING_RENDERING(true, "");

	//SAFE_DELETE(m_currentLight);
	// TODO: Deallocating the lights member variable
	// TODO: Deallocating the cameras member variable

	//SAFE_DELETE(altCameraNode);

	// TODO: m_fontTexture uses the same texture as the fontTexture used in CoreEngine class. That's why we shouldn't SAFE_DELETE font texture here.
	// Of course, we should deal with it later on more appropriately.
	//SetTexture("fontTexture", nullptr);

	m_mappedValues.SetTexture("waterReflectionTexture", nullptr);
	m_mappedValues.SetMultitexture("waterRefractionTexture", nullptr, 0);
	m_mappedValues.SetMultitexture("waterDepthMap", nullptr, 1);
	m_mappedValues.SetTexture("waterDUDVMap", nullptr);
	m_mappedValues.SetTexture("waterNormalMap", nullptr);

	m_mappedValues.SetTexture("shadowMap", nullptr);

#ifdef ANT_TWEAK_BAR_ENABLED
	TwTerminate(); // Terminate AntTweakBar
#endif

	STOP_PROFILING_RENDERING("");
	NOTICE_LOG_RENDERING("Rendering engine destroyed");
}

const rendering::Mesh* rendering::Renderer::CreateMesh(int meshID, const std::string& meshFileName)
{
	return m_meshFactory.CreateMesh(meshID, meshFileName);
}

const rendering::Mesh* rendering::Renderer::CreateMeshFromSurface(int meshID, const math::Surface& surface)
{
	return m_meshFactory.CreateMeshFromSurface(meshID, surface);
}

const rendering::Shader* rendering::Renderer::CreateShader(int shaderID, const std::string& shaderFileName)
{
	return m_shaderFactory.CreateShader(shaderID, shaderFileName);
}

const rendering::text::Font* rendering::Renderer::CreateFont(int fontID, const std::string& fontTextureFileName, const std::string& fontMetaDataFileName)
{
	return m_fontFactory.CreateFont(fontID, fontTextureFileName, fontMetaDataFileName);
}

const rendering::text::Font* rendering::Renderer::GetFont(int fontID) const
{
	return m_fontFactory.GetFont(fontID);
}

/* TODO: Remove in the future */
struct CameraDirection
{
	GLenum cubemapFace;
	math::Quaternion rotation;
};

CameraDirection gCameraDirections[6 /* number of cube map faces */] =
{
	//{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) },
	//{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Vector3D(-REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) },
	//{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE) },
	//{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE) },
	//{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) },
	//{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) }

	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, math::Quaternion(math::Matrix4D(math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, math::Quaternion(math::Matrix4D(math::Vector3D(-REAL_ONE, REAL_ZERO, REAL_ZERO), math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, math::Quaternion(math::Matrix4D(math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), math::Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, math::Quaternion(math::Matrix4D(math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO), math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE))) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, math::Quaternion(math::Matrix4D(math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE), math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, math::Quaternion(math::Matrix4D(math::Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE), math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) }
};

void rendering::Renderer::InitRenderScene(const Color& ambientLightColor, math::Real dayNightMixFactor)
{
	START_PROFILING_RENDERING(true, "");

	CheckErrorCode(__FUNCTION__, "Started scene rendering");

	//CHECK_CONDITION_EXIT_RENDERING(!m_cameras.empty() && m_currentCameraIndex >= 0 && m_currentCameraIndex < m_cameras.size() && m_cameras[m_currentCameraIndex] != nullptr,
	//	Utility::EMERGENCY, "Rendering failed. There is no proper camera set up (current camera index = ", m_currentCameraIndex, ")");
	
	m_mappedValues.SetReal("dayNightMixFactor", dayNightMixFactor);

#ifdef ANT_TWEAK_BAR_ENABLED
	m_mappedValues.SetVector4D("ambientFogColor", m_fogInfo.GetColor().GetValues());
	m_mappedValues.SetReal("ambientFogStart", m_fogInfo.GetStartDistance());
	m_mappedValues.SetReal("ambientFogEnd", m_fogInfo.GetEndDistance());
	m_mappedValues.SetReal("ambientFogDensityFactor", m_fogInfo.GetDensityFactor());
	m_mappedValues.SetReal("ambientFogGradient", m_fogInfo.GetGradient());
	m_mappedValues.SetVector4D("ambientIntensity", ambientLightColor.GetValues());
	m_mappedValues.SetReal("fxaaSpanMax", m_fxaaSpanMax);
	m_mappedValues.SetReal("fxaaReduceMin", m_fxaaReduceMin);
	m_mappedValues.SetReal("fxaaReduceMul", m_fxaaReduceMul);
	m_mappedValues.SetVector4D("clipPlane", m_defaultClipPlane); // The workaround for some drivers ignoring the glDisable(GL_CLIP_DISTANCE0) method
#endif

	STOP_PROFILING_RENDERING("");
}

void rendering::Renderer::BindDisplayTexture() const
{
	m_mappedValues.GetTexture("displayTexture")->BindAsRenderTarget();
}

void rendering::Renderer::BindWaterReflectionTexture() const
{
	m_waterReflectionTexture.BindAsRenderTarget();
}

void rendering::Renderer::BindWaterRefractionTexture() const
{
	m_waterRefractionTexture.BindAsRenderTarget();
}

void rendering::Renderer::InitWaterNodesRendering()
{
	m_mappedValues.SetTexture("waterReflectionTexture", &m_waterReflectionTexture);
	m_mappedValues.SetMultitexture("waterRefractionTexture", &m_waterRefractionTexture, 0);
	m_mappedValues.SetMultitexture("waterDepthMap", &m_waterRefractionTexture, 1);
	m_mappedValues.SetTexture("waterDUDVMap", &m_waterDuDvTexture);
	m_mappedValues.SetTexture("waterNormalMap", &m_waterNormalMap);
	//m_waterMoveFactor = fmod(m_waterMoveFactor + m_waterWaveSpeed * CoreEngine::GetCoreEngine()->GetClockSpeed(), REAL_ONE);
	m_waterMoveFactor += m_waterWaveSpeed * 1.0f; // Instead, use Core::CoreEngine::GetCoreEngine()->GetClockSpeed();
	if (m_waterMoveFactor > REAL_ONE)
	{
		m_waterMoveFactor -= REAL_ONE;
		CHECK_CONDITION_ALWAYS_RENDERING(m_waterMoveFactor < REAL_ONE, utility::logging::ERR, "Water move factor is still greater than 1.0. It is equal to ", m_waterMoveFactor); // TODO: Remove "ALWAYS" in the future
	}
	m_mappedValues.SetReal("waterMoveFactor", m_waterMoveFactor);
	m_mappedValues.SetReal("nearPlane", 0.1f /* TODO: This value should be always equal to the near plane of the current camera, but it is not easy for us to get this value */);
	m_mappedValues.SetReal("farPlane", 1000.0f /* TODO: This value should be always equal to the far plane of the current camera, but it is not easy for us to get this value */);
	m_mappedValues.SetReal("waterWaveStrength", m_waterWaveStrength);
	m_mappedValues.SetReal("waterShineDamper", m_waterShineDamper);
	m_mappedValues.SetReal("waterReflectivity", m_waterReflectivity);
	m_mappedValues.SetReal("waterFresnelEffectFactor", m_waterFresnelEffectFactor);
	m_mappedValues.SetReal("waterNormalVerticalFactor", m_waterNormalVerticalFactor);
}

void rendering::Renderer::FinalizeRenderScene(int filterShaderID)
{
	START_PROFILING_RENDERING(true, "");
	m_mappedValues.SetVector3D("inverseFilterTextureSize",
		math::Vector3D(REAL_ONE / m_mappedValues.GetTexture("displayTexture")->GetWidth(), REAL_ONE / m_mappedValues.GetTexture("displayTexture")->GetHeight(), REAL_ZERO));

	ApplyFilter(m_shaderFactory.GetShader(filterShaderID), m_mappedValues.GetTexture("displayTexture"), nullptr);
	CheckErrorCode(__FUNCTION__, "Finished scene rendering");
	STOP_PROFILING_RENDERING("");
}

void rendering::Renderer::Render(int meshID, const Material* material, const math::Transform& transform, int shaderID) const
{
	//START_PROFILING_RENDERING(true, "");
	//shader.Bind();
	m_shaderFactory.GetShader(shaderID)->UpdateUniforms(transform, material, this);
	m_meshFactory.GetMesh(meshID)->Draw();
	//STOP_PROFILING_RENDERING;
}

//void Renderer::SetClippingPlane(const math::Vector4D& clippingPlane)
//{
//	m_mappedValues.SetVector4D("clipPlane", clippingPlane);
//}

void rendering::Renderer::EnableWaterReflectionClippingPlane(math::Real height)
{
	m_waterReflectionClippingPlane.w = height;
	m_mappedValues.SetVector4D("clipPlane", m_waterReflectionClippingPlane);
}

void rendering::Renderer::EnableWaterRefractionClippingPlane(math::Real height)
{
	m_waterRefractionClippingPlane.w = height;
	m_mappedValues.SetVector4D("clipPlane", m_waterRefractionClippingPlane);
}

void rendering::Renderer::DisableClippingPlanes()
{
	// Now that we rendered the scene into the reflection and refraction textures for the water surface,
	// we want to disable the clipping planes completely. Unfortunately, it seems some drivers simply ignore the
	// below glDisable(GL_CLIP_DISTANCE0) call, so we need a trick. In order to have no clipping we simply
	// set the clipping plane distance from the origin very high. This way there are no fragments that can be culled
	// and as a result we render the whole scene.
	// glDisable(GL_CLIP_DISTANCE0); // Disabled plane clipping // glDisable(GL_CLIP_PLANE0);
	m_mappedValues.SetVector4D("clipPlane", m_defaultClipPlane); // The workaround for some drivers ignoring the glDisable(GL_CLIP_DISTANCE0) method
}

void rendering::Renderer::RenderGuiControl(const controls::GuiControl& guiControl, int guiControlShaderID) const
{
	CheckErrorCode(__FUNCTION__, "Started main GUI control rendering function");
	//CRITICAL_LOG_RENDERING("Started drawing GUI control at screen position \"", guiControl.GetScreenPosition(), "\"");
	//glDisable(GL_CULL_FACE);
	if (glDepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
	}
	if (!glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	/**
	* This effectively means:
	* newColorInFramebuffer = currentAlphaInFramebuffer * current color in framebuffer +
	* (1 - currentAlphaInFramebuffer) * shader's output color
	*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	guiControl.Draw(m_shaderFactory.GetShader(guiControlShaderID), *this);

	if (glDepthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (!glBlendEnabled)
	{
		glDisable(GL_BLEND);
	}
	//glEnable(GL_CULL_FACE);
	CheckErrorCode(__FUNCTION__, "Finished main GUI control rendering function");
}

void rendering::Renderer::RenderParticles(int particleShaderID, const particles::ParticlesSystem& particlesSystem) const
{
	START_PROFILING_RENDERING(true, "");
	CheckErrorCode(__FUNCTION__, "Started particles rendering");
	//CHECK_CONDITION_ALWAYS_RENDERING(particlesCount <= particles.size(), Utility::ERR,
	//	"The number of alive particles (", particlesCount, ") exceeds the size of the specified vector of particles (", particles.size(), ")");
	if (particlesSystem.GetAliveParticlesCount() <= 0 || !particlesSystem.IsAttributeEnabled(particles::attributes::POSITION))
	{
		return;
	}
	DEBUG_LOG_RENDERING("Rendering particles started. There are ", particlesSystem.GetAliveParticlesCount(), " alive particles currently in the game.");
	const auto particleShader = m_shaderFactory.GetShader(particleShaderID);
	particleShader->Bind(); // TODO: This can be performed once and not each time we call this function (during one render-pass of course).
	const auto particleTexture = static_cast<const particles::ParticleTexture*>(m_textureFactory.GetTexture(particlesSystem.GetTextureId()));
	particleTexture->Bind();
	particleShader->SetUniformi("particleTexture", 0);
	particleShader->SetUniformf("textureAtlasRowsCount", static_cast<math::Real>(particleTexture->GetRowsCount()));
	if (glDepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
	}
	if (!glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	glBlendFunc(GL_SRC_ALPHA, particleTexture->IsAdditive() ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);

	m_particleInstanceVboData.clear();
	const auto cameraViewMatrix = m_currentCamera->GetViewMatrix();
	for (size_t i = 0; i < particlesSystem.GetAliveParticlesCount(); ++i)
	{
		math::Matrix4D modelMatrix(particlesSystem.GetPosition(i));
		// To make the particle always face the camera we can either use the geometry shader (as in the Bilboard shader) or
		// set the 3x3 top-left submatrix of the model matrix to be a transposed version of the 3x3 top-left submatrix of the camera's view matrix.
		modelMatrix.SetElement(0, 0, cameraViewMatrix.GetElement(0, 0));
		modelMatrix.SetElement(0, 1, cameraViewMatrix.GetElement(1, 0));
		modelMatrix.SetElement(0, 2, cameraViewMatrix.GetElement(2, 0));
		modelMatrix.SetElement(1, 0, cameraViewMatrix.GetElement(0, 1));
		modelMatrix.SetElement(1, 1, cameraViewMatrix.GetElement(1, 1));
		modelMatrix.SetElement(1, 2, cameraViewMatrix.GetElement(2, 1));
		modelMatrix.SetElement(2, 0, cameraViewMatrix.GetElement(0, 2));
		modelMatrix.SetElement(2, 1, cameraViewMatrix.GetElement(1, 2));
		modelMatrix.SetElement(2, 2, cameraViewMatrix.GetElement(2, 2));

		if (particlesSystem.IsAttributeEnabled(particles::attributes::ROTATION))
		{
			const auto particleRotation(math::Quaternion(math::Vector3D(0.0f, 0.0f, 1.0f), particlesSystem.GetRotation(i)).ToRotationMatrix());
			if (particlesSystem.IsAttributeEnabled(particles::attributes::SCALE) && !math::AlmostEqual(particlesSystem.GetScale(i), REAL_ONE))
			{
				modelMatrix = modelMatrix * particleRotation * math::Matrix4D(particlesSystem.GetScale(i));
			}
			else
			{
				modelMatrix = modelMatrix * particleRotation;
			}
		}
		else if (particlesSystem.IsAttributeEnabled(particles::attributes::SCALE) && !math::AlmostEqual(particlesSystem.GetScale(i), REAL_ONE))
		{
			modelMatrix = modelMatrix * math::Matrix4D(particlesSystem.GetScale(i));
		}

		const auto mvpMatrix = m_currentCamera->GetViewProjection() * modelMatrix;
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(0, 0));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(0, 1));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(0, 2));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(0, 3));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(1, 0));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(1, 1));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(1, 2));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(1, 3));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(2, 0));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(2, 1));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(2, 2));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(2, 3));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(3, 0));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(3, 1));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(3, 2));
		m_particleInstanceVboData.push_back(mvpMatrix.GetElement(3, 3));

#ifdef TEXTURE_ATLAS_OFFSET_CALCULATION		
		math::Vector2D textureOffset0;
		math::Vector2D textureOffset1;
		math::Real textureAtlasBlendFactor;
		particles[i].CalculateTextureAtlasInfo(particleTexture->GetRowsCount(), textureOffset0, textureOffset1, textureAtlasBlendFactor);
		m_particleInstanceVboData.push_back(textureOffset0.GetX());
		m_particleInstanceVboData.push_back(textureOffset0.GetY());
		m_particleInstanceVboData.push_back(textureOffset1.GetX());
		m_particleInstanceVboData.push_back(textureOffset1.GetY());
		m_particleInstanceVboData.push_back(textureAtlasBlendFactor);
#else
		m_particleInstanceVboData.push_back(particlesSystem.CalculateLifeStageFactor(i));
#endif
	}
	m_particleQuad->Draw(&m_particleInstanceVboData[0], static_cast<int>(m_particleInstanceVboData.size()), particlesSystem.GetAliveParticlesCount());
	if (glDepthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (!glBlendEnabled)
	{
		glDisable(GL_BLEND);
	}
	CheckErrorCode(__FUNCTION__, "Finished particles rendering");
	STOP_PROFILING_RENDERING("");
}

bool rendering::Renderer::InitShadowMap()
{
	const auto shadowInfo = m_currentLight->GetShadowInfo();
	const auto shadowMapIndex = shadowInfo == nullptr ? 0 : shadowInfo->GetShadowMapSizeAsPowerOf2() - 1;
	CHECK_CONDITION_EXIT_RENDERING(shadowMapIndex < SHADOW_MAPS_COUNT, Utility::Logging::ERR, "Incorrect shadow map size. Shadow map index must be an integer from range [0; ", SHADOW_MAPS_COUNT, "), but equals ", shadowMapIndex, ".");
	m_mappedValues.SetTexture("shadowMap", &m_shadowMaps[shadowMapIndex]); // TODO: Check what would happen if we didn't set texture here?
	m_shadowMaps[shadowMapIndex].BindAsRenderTarget();
	ClearScreen(Color(REAL_ONE /* completely in light */ /* TODO: When at night it should be REAL_ZERO */, REAL_ONE /* we want variance to be also cleared */, REAL_ZERO, REAL_ZERO)); // everything is in light (we can clear the COLOR_BUFFER_BIT)

	if ( /* (m_shadowEnabled) && */ shadowInfo != nullptr)
	{
		m_altCamera.SetProjection(shadowInfo->GetProjection());
		const auto shadowCameraTransform = m_currentLight->CalcShadowCameraTransform(m_currentCamera->GetTransform().GetPos(), m_currentCamera->GetTransform().GetRot());
		m_altCamera.GetTransform().SetPos(shadowCameraTransform.pos);
		m_altCamera.GetTransform().SetRot(shadowCameraTransform.rot);

		//CRITICAL_LOG_RENDERING("AltCamera.GetViewProjection() = \"", m_altCamera.GetViewProjection(), "\"");
		m_lightMatrix = BIAS_MATRIX * m_altCamera.GetViewProjection(); // FIXME: Check matrix multiplication
		m_mappedValues.SetReal("shadowLightBleedingReductionFactor", shadowInfo->GetLightBleedingReductionFactor());
		m_mappedValues.SetReal("shadowVarianceMin", shadowInfo->GetMinVariance());

		m_tempCamera = m_currentCamera;
		m_currentCamera = &m_altCamera;

		if (shadowInfo->IsFlipFacesEnabled())
		{
			glCullFace(GL_FRONT);
		}
		return true; // shadows enabled
	}
	
	// current light does not cast shadow or shadowing is disabled at all. In that case we set the light matrix this way so that,
	// if no shadow should be cast everything in the scene will be mapped to the same point
	m_lightMatrix.SetScaleMatrix(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	m_mappedValues.SetReal("shadowLightBleedingReductionFactor", REAL_ZERO);
	m_mappedValues.SetReal("shadowVarianceMin", m_defaultShadowMinVariance);
	return false; // shadows disabled
}

void rendering::Renderer::FinalizeShadowMapRendering(int filterShaderID)
{
	const auto shadowInfo = m_currentLight->GetShadowInfo();
	if (shadowInfo != nullptr)
	{
		const auto shadowMapIndex = shadowInfo->GetShadowMapSizeAsPowerOf2() - 1;
		if (shadowInfo->IsFlipFacesEnabled())
		{
			glCullFace(GL_BACK);
		}

		m_currentCamera = m_tempCamera;

		if (m_applyFilterEnabled)
		{
			//ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), GetTexture("shadowMap"), GetTexture("shadowMapTempTarget"));
			//ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), GetTexture("shadowMapTempTarget"), GetTexture("shadowMap"));
			if (!math::AlmostEqual(shadowInfo->GetShadowSoftness(), REAL_ZERO))
			{
				BlurShadowMap(m_shaderFactory.GetShader(filterShaderID), shadowMapIndex, shadowInfo->GetShadowSoftness());
			}
		}
	}
}

void rendering::Renderer::BlurShadowMap(const Shader* filterShader, int shadowMapIndex, math::Real blurAmount /* how many texels we move per sample */)
{
	START_PROFILING_RENDERING(true, "");
	CHECK_CONDITION_RENDERING(shadowMapIndex >= 0 && shadowMapIndex < SHADOW_MAPS_COUNT, Utility::Logging::EMERGENCY,
		"Cannot perform the blurring process. Specified shadow map index (", shadowMapIndex, ") lies outside of range [0; ", SHADOW_MAPS_COUNT, ").");

	m_mappedValues.SetVector3D("blurScale", math::Vector3D(blurAmount / m_shadowMaps[shadowMapIndex].GetWidth(), REAL_ZERO, REAL_ZERO));
	ApplyFilter(filterShader, &m_shadowMaps[shadowMapIndex], &m_shadowMapTempTargets[shadowMapIndex]);
	
	m_mappedValues.SetVector3D("blurScale", math::Vector3D(REAL_ZERO, blurAmount / m_shadowMaps[shadowMapIndex].GetHeight(), REAL_ZERO));
	ApplyFilter(filterShader, &m_shadowMapTempTargets[shadowMapIndex], &m_shadowMaps[shadowMapIndex]);
	STOP_PROFILING_RENDERING("");
}

// You cannot read and write from the same texture at the same time. That's why we use dest texture as a temporary texture to store the result
void rendering::Renderer::ApplyFilter(const Shader* filterShader, const Texture* source, const Texture* dest)
{
	START_PROFILING_RENDERING(true, "");
	CHECK_CONDITION_EXIT_RENDERING(source != nullptr, Utility::Logging::CRITICAL, "Cannot apply a filter. Source texture is NULL.");
	CHECK_CONDITION_EXIT_RENDERING(source != dest, Utility::Logging::CRITICAL, "Cannot apply a filter. Both source and destination textures point to the same location in memory.");
	if (dest == nullptr)
	{
		DELOCUST_LOG_RENDERING("Binding window as a render target for filtering");
		BindAsRenderTarget();
	}
	else
	{
		DELOCUST_LOG_RENDERING("Binding texture as a render target for filtering");
		dest->BindAsRenderTarget();
	}

	//DEBUG_LOG_RENDERING("Applying a filter to the source texture");
	
	m_mappedValues.SetTexture("filterTexture", source);

	m_tempCamera = m_currentCamera;
	m_currentCamera = &m_filterCamera;

	glClear(GL_DEPTH_BUFFER_BIT);
	filterShader->Bind();
	filterShader->UpdateRendererUniforms(this);
	filterShader->UpdateUniforms(m_filterTransform, nullptr, this);
	m_filterMesh->Draw();

	m_currentCamera = m_tempCamera;
	m_mappedValues.SetTexture("filterTexture", nullptr);
	STOP_PROFILING_RENDERING("");
}

void rendering::Renderer::SetCurrentCamera(const BaseCamera* camera)
{
	CHECK_CONDITION_RENDERING(camera != nullptr, Utility::Logging::ERROR, "Cannot set current camera. Given camera is nullptr.");
	//if (camera == nullptr)
	//{
	//	ERROR_LOG_RENDERING("Cannot set current camera. Given camera is NULL.");
	//}
	m_currentCamera = camera;
}

void rendering::Renderer::BindAsRenderTarget() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, m_windowWidth, m_windowHeight);
}

void rendering::Renderer::InitLightRendering() const
{
	glCullFace(glCullFaceMode);
	m_mappedValues.GetTexture("displayTexture")->BindAsRenderTarget();
	if (!glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both weights equal to 1
	glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore. TODO: What if ambient lighting is disabled?
	glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image
}

void rendering::Renderer::FinalizeLightRendering() const
{
	glDepthFunc(glDepthTestFunc);
	glDepthMask(GL_TRUE);
	if (!glBlendEnabled)
	{
		glDisable(GL_BLEND);
	}
	else
	{
		glBlendFunc(glBlendSfactor, glBlendDfactor);
	}
}

void rendering::Renderer::BindCubeShadowMap(unsigned int textureUnit) const
{
	m_cubeShadowMap.BindForReading(textureUnit);
}

#ifdef DEBUG_RENDERING_ENABLED
void rendering::Renderer::RenderDebugGuiControls(const Shader* guiShader)
{
	guiShader->Bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	for (auto guiControlItr = m_guiControls.begin(); guiControlItr != m_guiControls.end(); ++guiControlItr)
	{
		(*guiControlItr)->Draw(guiShader, *this);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void rendering::Renderer::AddLine(const math::Vector3D& fromPosition, const math::Vector3D& toPosition, const Color& color,
	math::Real lineWidth /* = REAL_ONE */, math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug line rendering is not yet supported by the engine");
}

void rendering::Renderer::AddSphere(const math::Sphere& sphere, const Color& color,
	math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug sphere rendering is not yet supported by the engine");
}

void rendering::Renderer::AddCross(const math::Vector3D& position, const Color& color, math::Real size,
	math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug cross rendering is not yet supported by the engine");
}

void rendering::Renderer::AddCircle(const math::Vector3D& centerPosition, const math::Vector3D& planeNormal, math::Real radius, const Color& color,
	math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug circle rendering is not yet supported by the engine");
}

void rendering::Renderer::AddAxes(const math::Transform& transform, const Color& color, math::Real size,
	math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug axes rendering is not yet supported by the engine");
}

void rendering::Renderer::AddTriangle(const math::Vector3D& v0, const math::Vector3D& v1, const math::Vector3D& v2, const Color& color,
	math::Real lineWidth /* = REAL_ONE */, math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug triangle rendering is not yet supported by the engine");
}

void rendering::Renderer::AddAabb(const math::Aabb& aabb, const Color& color, math::Real lineWidth /* = REAL_ONE */, math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug AABB rendering is not yet supported by the engine");
}

void rendering::Renderer::AddObb(const math::Obb& obb, const Color& color, math::Real lineWidth /* = REAL_ONE */, math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug OBB rendering is not yet supported by the engine");
}

void rendering::Renderer::AddString(const math::Vector3D& pos, const char* text, const Color& color, math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug text rendering is not yet supported by the engine");
}
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
void rendering::Renderer::InitializeTweakBars()
{
	INFO_LOG_RENDERING("Initializing rendering engine's tweak bars");

#ifdef RENDERER_PROPERTIES_BAR
	DEBUG_LOG_RENDERING("Initializing rendering engine's properties tweak bar");
	// TODO: CameraMembers[0] ("Position") is not displayed correctly, because at 0 address lies the pointer to parentGameNode
	m_cameraMembers[0].Name = "Projection"; m_cameraMembers[1].Name = "FoV"; m_cameraMembers[2].Name = "AspectRatio"; m_cameraMembers[3].Name = "NearPlane"; m_cameraMembers[4].Name = "FarPlane";
	m_cameraMembers[0].Type = matrix4DType; m_cameraMembers[1].Type = angleType; m_cameraMembers[2].Type = TW_TYPE_FLOAT; m_cameraMembers[3].Type = TW_TYPE_FLOAT; m_cameraMembers[4].Type = TW_TYPE_FLOAT;
	m_cameraMembers[0].Offset = 8; m_cameraMembers[1].Offset = 80; m_cameraMembers[2].Offset = 92; m_cameraMembers[3].Offset = 100; m_cameraMembers[4].Offset = 108;
	m_cameraMembers[0].DefString = ""; m_cameraMembers[1].DefString = ""; m_cameraMembers[2].DefString = " step=0.01 "; m_cameraMembers[3].DefString = ""; m_cameraMembers[4].DefString = "";
	m_cameraType = TwDefineStruct("Camera", m_cameraMembers, 5, sizeof(Camera), nullptr, nullptr);

	m_propertiesBar = TwNewBar("PropertiesBar");
	TwAddVarRW(m_propertiesBar, "bgColor", TW_TYPE_COLOR3F, &m_backgroundColor, " label='Background color' ");
	TwAddVarRW(m_propertiesBar, "applyFilterEnabled", TW_TYPE_BOOLCPP, &m_applyFilterEnabled, " label='Apply filter' ");
	TwAddVarRW(m_propertiesBar, "ambientLightEnabled", TW_TYPE_BOOLCPP, &m_ambientLightEnabled, " label='Enabled' group='Ambient light' ");
	TwAddVarRW(m_propertiesBar, "fogEnabled", TW_TYPE_BOOLCPP, m_fogInfo.IsEnabledPtr(), " label='Enabled' group='Fog' ");
	TwAddVarRW(m_propertiesBar, "fogColor", TW_TYPE_COLOR4F, m_fogInfo.GetColorPtr(), " label='Color' group='Fog' ");
	TwAddVarRW(m_propertiesBar, "fogStart", TW_TYPE_REAL, m_fogInfo.GetStartDistancePtr(), " label='Start' group='Fog' step=0.5 min=0.5");
	TwAddVarRW(m_propertiesBar, "fogEnd", TW_TYPE_REAL, m_fogInfo.GetEndDistancePtr(), " label='End' group='Fog' step=0.5 min=1.0");
	TwAddVarRW(m_propertiesBar, "fogDensityFactor", TW_TYPE_REAL, m_fogInfo.GetDensityFactorPtr(), " label='Density factor' group='Fog' step=0.0002 min=0.0002 max=2.0 ");
	TwAddVarRW(m_propertiesBar, "fogGradient", TW_TYPE_REAL, m_fogInfo.GetGradientPtr(), " label='Gradient' group='Fog' step=0.1 min=0.1 max=20.0 ");
	TwAddVarRW(m_propertiesBar, "fogFallOffType", fogFallOffType, m_fogInfo.GetFallOffTypePtr(), " label='Fall-off type' group='Fog' ");
	TwAddVarRW(m_propertiesBar, "fogCalculationType", fogCalculationType, m_fogInfo.GetCalculationTypePtr(), " label='Calculation type' group='Fog' ");
	//TwAddVarRW(m_propertiesBar, "shadowsEnabled", TW_TYPE_BOOLCPP, &m_shadowsEnabled, " label='Render shadows' group=Shadows");
	//TwAddVarRW(m_propertiesBar, "pointLightShadowsEnabled", TW_TYPE_BOOLCPP, &m_pointLightShadowsEnabled, " label='Render point light shadows' group=Shadows ");
	TwAddVarRW(m_propertiesBar, "fxaaSpanMax", TW_TYPE_REAL, &m_fxaaSpanMax, " min=0.0 step=0.1 label='Max span' group='FXAA' ");
	TwAddVarRW(m_propertiesBar, "fxaaReduceMin", TW_TYPE_REAL, &m_fxaaReduceMin, " min=0.00001 step=0.000002 label='Min reduce' group='FXAA' ");
	TwAddVarRW(m_propertiesBar, "fxaaReduceMul", TW_TYPE_REAL, &m_fxaaReduceMul, " min=0.0 step=0.01 label='Reduce scale' group='FXAA' ");
	TwAddVarRW(m_propertiesBar, "waterLightReflectionEnabled", TW_TYPE_BOOLCPP, &m_waterLightReflectionEnabled, " label='Reflection enabled' group='Water' ");
	TwAddVarRW(m_propertiesBar, "waterWaveStrength", TW_TYPE_REAL, &m_waterWaveStrength, " min=0.001 step=0.001 label='Wave strength' group='Water' ");
	TwAddVarRW(m_propertiesBar, "waterShineDamper", TW_TYPE_REAL, &m_waterShineDamper, " min=0.2 max=100.0 step=0.2 label='Shine damper' group='Water' ");
	TwAddVarRW(m_propertiesBar, "waterReflectivity", TW_TYPE_REAL, &m_waterReflectivity, " min=0.02 max=10.0 step=0.02 label='Reflectivity' group='Water' ");
	TwAddVarRW(m_propertiesBar, "waterFresnelEffectFactor", TW_TYPE_REAL, &m_waterFresnelEffectFactor, " min=0.1 max=20.0 step=0.1 label='Fresnel factor' group='Water' ");
	TwAddVarRW(m_propertiesBar, "waterNormalVerticalFactor", TW_TYPE_REAL, &m_waterNormalVerticalFactor, " min=0.05 max=10.0 step=0.05 label='Normal vertical factor' group='Water' ");
	TwAddVarRW(m_propertiesBar, "waterWaveSpeed", TW_TYPE_REAL, &m_waterWaveSpeed, " min=0.0 step=0.0001 label='Wave speed' group='Water' ");
	TwAddVarRO(m_propertiesBar, "waterMoveFactor", TW_TYPE_REAL, &m_waterMoveFactor, " min=0.001 step=0.001 label='Move factor' group='Water' ");
	//TwAddVarRO(m_propertiesBar, "refractionClippingPlaneNormal", TW_TYPE_DIR3F, &m_waterRefractionClippingPlane.GetNormal(), " label='Normal' group='Refraction' ");
	//TwAddVarRW(m_propertiesBar, "refractionClippingPlaneOriginDistance", TW_TYPE_REAL, &m_waterRefractionClippingPlane.GetDistance(), " label='Origin distance' group='Refraction' ");
	//TwAddVarRO(m_propertiesBar, "reflectionClippingPlaneNormal", TW_TYPE_DIR3F, &m_waterReflectionClippingPlane.GetNormal(), " label='Normal' group='Reflection' ");
	//TwAddVarRW(m_propertiesBar, "reflectionClippingPlaneOriginDistance", TW_TYPE_REAL, &m_waterReflectionClippingPlane.GetDistance(), " label='Origin distance' group='Reflection' ");

	TwSetParam(m_propertiesBar, nullptr, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup
	DEBUG_LOG_RENDERING("Initializing rendering engine's properties tweak bar finished");
#endif

#ifdef CAMERA_TWEAK_BAR
	DEBUG_LOG_RENDERING("Initializing rendering engine's cameras tweak bar");
	m_cameraBar = TwNewBar("CamerasBar");
	if (m_currentCamera == nullptr)
	{
		ERROR_LOG_RENDERING("Cannot properly initialize rendering engine's cameras bar. No cameras are setup by the game manager.");
	}
	else
	{
		//TwAddVarRW(m_cameraBar, "cameraVar", m_cameraType, m_currentCamera, " label='Camera' group=Camera ");
		//TwAddVarRW(m_cameraBar, "position", vector3DType, &m_currentCamera->GetPos(), " label='Pos' group='Camera' ");
		//TwAddVarRW(m_cameraBar, "rotation", TW_TYPE_QUAT4F, &m_currentCamera->GetRot(), " label='Rot' group='Camera' ");
	}
	
	TwDefine(" CamerasBar/Camera opened=true ");
	TwSetParam(m_cameraBar, nullptr, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup
	DEBUG_LOG_RENDERING("Initializing rendering engine's cameras tweak bar finished");
#endif

#ifdef LIGHTS_TWEAK_BAR
	//TODO: Doesn't work yet.
	m_lightsBar = TwNewBar("LightsBar");
	for (std::vector<BaseLight*>::iterator lightItr = lights.begin(); lightItr != lights.end(); ++lightItr)
	{
		m_currentLight = *lightItr;
		m_currentLight->InitializeTweakBar(lightsBar);
	}
#endif

	//TwBar* altCameraBar = TwNewBar("AltCameraBar");
	//TwAddVarRW(altCameraBar, "cameraVar", m_cameraType,  &m_altCamera, " label='Camera' group=Camera ");
	//TwAddVarRW(altCameraBar, "altCameraPos", vector3DType, &m_altCamera.GetTransform().GetPos(), " label='AltCamera.Pos' group=Camera ");
	//TwAddVarRW(altCameraBar, "altCameraRot", TW_TYPE_QUAT4F, &m_altCamera.GetTransform().GetRot(), " label='AltCamera.Rot' group=Camera ");
	//TwDefine(" AltCameraBar/Camera opened=true ");
	DEBUG_LOG_RENDERING("Initializing rendering engine's tweak bars finished");
}
#endif