#include "StdAfx.h"
#include "Renderer.h"
#include "GameManager.h"
#include "CoreEngine.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "TextRenderer.h"
#include "ShadowInfo.h"
#include "MeshRenderer.h"
#include "Utility\IConfig.h"
#include "Utility\ILogger.h"
#include "Utility\FileManager.h"
//#include "Utility\FileNotFoundException.h"
#include "Math\FloatingPoint.h"

#include <stddef.h>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace Rendering;
using namespace Utility;
using namespace Math;

/* static */ const Matrix4D Renderer::BIAS_MATRIX(Matrix4D(0.5f /* scale matrix */) * Matrix4D(REAL_ONE, REAL_ONE, REAL_ONE /* translation matrix */)); // FIXME: Check matrix multiplication
///* static */ const int Renderer::SHADOW_MAPS_COUNT = 11;

Renderer::Renderer(GLFWwindow* window, GLFWwindow* threadWindow) :
	m_applyFilterEnabled(GET_CONFIG_VALUE("applyFilterEnabled", true)),
	m_backgroundColor(GET_CONFIG_VALUE("ClearColorRed", REAL_ZERO),
		GET_CONFIG_VALUE("ClearColorGreen", REAL_ZERO),
		GET_CONFIG_VALUE("ClearColorBlue", REAL_ZERO),
		GET_CONFIG_VALUE("ClearColorAlpha", REAL_ONE)),
	//m_shadowsEnabled(GET_CONFIG_VALUE("shadowsEnabled", true)),
	//m_pointLightShadowsEnabled(GET_CONFIG_VALUE("pointLightShadowsEnabled", false)),
	m_window(window),
	m_threadWindow(threadWindow),
	m_vao(0),
	m_fogEnabled(GET_CONFIG_VALUE("fogEnabled", true)),
	m_fogColor(GET_CONFIG_VALUE("fogColor_x", 0.7f),
		GET_CONFIG_VALUE("fogColor_y", 0.7f),
		GET_CONFIG_VALUE("fogColor_z", 0.7f)),
	m_fogStart(GET_CONFIG_VALUE("fogStart", 8.0f)),
	m_fogEnd(GET_CONFIG_VALUE("fogEnd", 50.0f)),
	m_fogDensityFactor(GET_CONFIG_VALUE("fogDensityFactor", 0.2f)),
	m_fogFallOffType(static_cast<FogEffect::FogFallOffType>(GET_CONFIG_VALUE("fogFallOffType", 0))),
	m_fogCalculationType(static_cast<FogEffect::FogCalculationType>(GET_CONFIG_VALUE("fogCalculationType", 0))),
	m_ambientLightEnabled(GET_CONFIG_VALUE("ambientLightEnabled", true)),
	m_ambientDaytimeColor(GET_CONFIG_VALUE("ambientDaytimeColorRed", 0.2f),
		GET_CONFIG_VALUE("ambientDaytimeColorGreen", 0.2f),
		GET_CONFIG_VALUE("ambientDaytimeColorBlue", 0.2f)),
	m_ambientSunNearHorizonColor(GET_CONFIG_VALUE("ambientSunNearHorizonColorRed", 0.1f),
		GET_CONFIG_VALUE("ambientSunNearHorizonColorGreen", 0.1f),
		GET_CONFIG_VALUE("ambientSunNearHorizonColorBlue", 0.1f)),
	m_ambientNighttimeColor(GET_CONFIG_VALUE("ambientNighttimeColorRed", 0.02f),
		GET_CONFIG_VALUE("ambientNighttimeColorGreen", 0.02f),
		GET_CONFIG_VALUE("ambientNighttimeColorBlue", 0.02f)),
	m_ambientLight(m_ambientDaytimeColor),
	m_currentLight(NULL),
	m_pointLight(NULL),
	m_spotLight(NULL),
	m_currentCameraIndex(0),
	m_currentCamera(NULL),
	m_mainMenuCamera(NULL),
	m_altCamera(Math::Matrix4D(), Transform()),
	m_filterTexture(NULL),
	m_filterMaterial(NULL),
	m_filterTransform(Vector3D(), Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO) /* to make the plane face towards the camera. See "OpenGL Game Rendering Tutorial: Shadow Mapping Preparations" https://www.youtube.com/watch?v=kyjDP68s9vM&index=8&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl (starts around 14:10) */, REAL_ONE),
	m_filterMesh(NULL),
	m_terrainNode(NULL),
	m_ambientShaderTerrain(NULL),
	m_ambientShader(NULL),
	m_shadowMapShader(NULL),
	m_nullFilterShader(NULL),
	m_gaussBlurFilterShader(NULL),
	m_fxaaFilterShader(NULL),
	m_fxaaSpanMax(GET_CONFIG_VALUE("fxaaSpanMax", 8.0f)),
	m_fxaaReduceMin(GET_CONFIG_VALUE("fxaaReduceMin", REAL_ONE / 128.0f)),
	m_fxaaReduceMul(GET_CONFIG_VALUE("fxaaReduceMul", REAL_ONE / 8.0f)),
	m_skyboxAngle(REAL_ZERO, Math::Unit::RADIAN),
	m_skyboxAngleStep(GET_CONFIG_VALUE("skyboxAngleStep", 0.02f), Math::Unit::RADIAN), // TODO: This variable should be dependant of the clock speed in CoreEngine.
	m_skyboxNode(NULL),
	m_skyboxShader(NULL),
	m_skyboxProceduralShader(NULL),
	m_skyboxTextureDay(NULL),
	m_skyboxTextureNight(NULL),
	m_defaultShadowMinVariance(GET_CONFIG_VALUE("defaultShadowMinVariance", 0.00002f)),
	m_cubeMapShader(NULL),
	m_cubeShadowMap(NULL),
	//m_shadowMaps(), // Gives a compiler warning C4351: new behavior: elements of array will be default initialized
	//m_shadowMapTempTargets(), // Gives a compiler warning C4351: new behavior: elements of array will be default initialized
	m_lights(),
	m_directionalAndSpotLights(),
	m_pointLights(),
	m_cameras(),
	m_samplerMap(),
	m_lightMatrix(REAL_ZERO /* scale matrix */),
	m_fontTexture(NULL),
	m_textRenderer(NULL),
	m_defaultClipPlane(REAL_ZERO, -REAL_ONE, REAL_ZERO, 1000000 /* a high value so that nothing is culled by the clipping plane */),
	m_waterWaveStrength(GET_CONFIG_VALUE("waterWaveStrength", 0.04f)),
	m_waterShineDamper(GET_CONFIG_VALUE("waterShineDamper", 20.0f)),
	m_waterReflectivity(GET_CONFIG_VALUE("waterReflectivity", 0.5f)),
	m_waterWaveSpeed(GET_CONFIG_VALUE("waterWaveSpeed", 0.0003f)),
	m_waterMoveFactor(GET_CONFIG_VALUE("waterMoveFactor", 0.0f)),
	m_waterNodes(),
	m_waterRefractionClippingPlane(GET_CONFIG_VALUE("waterRefractionClippingPlaneNormal_x", REAL_ZERO),
		GET_CONFIG_VALUE("waterRefractionClippingPlaneNormal_y", -REAL_ONE), GET_CONFIG_VALUE("waterRefractionClippingPlaneNormal_z", REAL_ZERO),
		GET_CONFIG_VALUE("waterRefractionClippingPlaneOriginDistance", 10.0f)),
	m_waterReflectionClippingPlane(GET_CONFIG_VALUE("waterReflectionClippingPlaneNormal_x", REAL_ZERO),
		GET_CONFIG_VALUE("waterReflectionClippingPlaneNormal_y", REAL_ONE), GET_CONFIG_VALUE("waterReflectionClippingPlaneNormal_z", REAL_ZERO),
		GET_CONFIG_VALUE("waterReflectionClippingPlaneOriginDistance", REAL_ZERO)),
	m_waterDUDVTexture(NULL),
	m_waterNormalMap(NULL),
	m_waterRefractionTexture(NULL),
	m_waterReflectionTexture(NULL),
	m_waterLightReflectionEnabled(true),
	m_waterFresnelEffectFactor(GET_CONFIG_VALUE("waterFresnelEffectFactor", 2.0f)),
	m_waterNormalVerticalFactor(GET_CONFIG_VALUE("waterNormalVerticalFactor", 3.0f)),
	m_waterShader(NULL),
	m_waterNoDirectionalLightShader(NULL),
	m_billboardShader(NULL),
	m_billboardNodes()
#ifdef ANT_TWEAK_BAR_ENABLED
	,m_cameraCountMinusOne(0),
	m_previousFrameCameraIndex(0),
	m_propertiesBar(NULL),
	m_cameraBar(NULL),
	m_lightsBar(NULL),
	//m_cameraMembers(), // Gives a compiler warning C4351: new behavior: elements of array will be default initialized
	m_cameraType()
#endif
#ifdef DEBUG_RENDERING_ENABLED
	,m_guiTextures(NULL),
	m_debugQuad(NULL),
	m_debugShader(NULL)
#endif
#ifdef CALCULATE_RENDERING_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("Renderer"))
#endif
{
	NOTICE_LOG("Creating Renderer instance started");
	START_PROFILING;
	SetCallbacks();

	SetSamplerSlot("diffuse", 0);
	SetSamplerSlot("normalMap", 1);
	SetSamplerSlot("displacementMap", 2);
	SetSamplerSlot("shadowMap", 3);
	SetSamplerSlot("cubeShadowMap", 5);
	SetSamplerSlot("cubeMapDay", 0);
	SetSamplerSlot("cubeMapNight", 1);
	SetSamplerSlot("filterTexture", 0);
	SetSamplerSlot("diffuse2", 4);
	SetSamplerSlot("waterReflectionTexture", 0);
	SetSamplerSlot("waterRefractionTexture", 1);
	SetSamplerSlot("waterDUDVMap", 2);
	SetSamplerSlot("waterNormalMap", 3);
	SetSamplerSlot("waterDepthMap", 4);

	glGenVertexArrays(1, &m_vao);

#ifndef ANT_TWEAK_BAR_ENABLED
	SetVector3D("fogColor", m_fogColor);
	SetReal("fogStart", m_fogStart);
	SetReal("fogEnd", m_fogEnd);
	SetReal("fogDensityFactor", m_fogDensityFactor);
	SetVector3D("ambientIntensity", m_ambientLight);
	SetVector4D("clipPlane", m_defaultClipPlane); // The workaround for some drivers ignoring the glDisable(GL_CLIP_DISTANCE0) method
#endif

	//SetTexture("shadowMap",
	//	new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR,
	//		GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true,
	//		GL_COLOR_ATTACHMENT0 /* we're going to render color information */)); // variance shadow mapping
	//SetTexture("shadowMapTempTarget", new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0));
	m_ambientShader = new Shader(GET_CONFIG_VALUE_STR("ambientLightShader", "ForwardAmbient"));
	m_ambientShaderTerrain = new Shader(GET_CONFIG_VALUE_STR("ambientLightTerrainShader", "forward-ambient-terrain"));
	m_ambientShadersFogEnabledMap[FogEffect::FogKey(FogEffect::LINEAR, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogLinearPlaneBasedShader", "forward-ambient-fog-linear-plane-based"));
	m_ambientShadersFogEnabledMap[FogEffect::FogKey(FogEffect::EXPONENTIAL, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogExponentialPlaneBasedShader", "forward-ambient-fog-exponential-plane-based"));
	m_ambientShadersFogEnabledMap[FogEffect::FogKey(FogEffect::SQUARED_EXPONENTIAL, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogExponentialSquaredPlaneBasedShader", "forward-ambient-fog-exponential-squared-plane-based"));
	m_ambientShadersFogEnabledMap[FogEffect::FogKey(FogEffect::LINEAR, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogLinearRangeBasedShader", "forward-ambient-fog-linear-range-based"));
	m_ambientShadersFogEnabledMap[FogEffect::FogKey(FogEffect::EXPONENTIAL, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogExponentialRangeBasedShader", "forward-ambient-fog-exponential-range-based"));
	m_ambientShadersFogEnabledMap[FogEffect::FogKey(FogEffect::SQUARED_EXPONENTIAL, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogExponentialSquaredRangeBasedShader", "forward-ambient-fog-exponential-squared-range-based"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::FogKey(FogEffect::LINEAR, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogLinearPlaneBasedTerrainShader", "forward-ambient-fog-linear-plane-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::FogKey(FogEffect::EXPONENTIAL, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogExponentialPlaneBasedTerrainShader", "forward-ambient-fog-exponential-plane-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::FogKey(FogEffect::SQUARED_EXPONENTIAL, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogExponentialSquaredPlaneBasedTerrainShader", "forward-ambient-fog-exponential-squared-plane-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::FogKey(FogEffect::LINEAR, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogLinearRangeBasedTerrainShader", "forward-ambient-fog-linear-range-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::FogKey(FogEffect::EXPONENTIAL, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogExponentialRangeBasedTerrainShader", "forward-ambient-fog-exponential-range-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::FogKey(FogEffect::SQUARED_EXPONENTIAL, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightFogExponentialSquaredRangeBasedTerrainShader", "forward-ambient-fog-exponential-squared-range-based-terrain"));

	m_shadowMapShader = new Shader(GET_CONFIG_VALUE_STR("shadowMapShader", "ShadowMapGenerator"));
	m_nullFilterShader = new Shader(GET_CONFIG_VALUE_STR("nullFilterShader", "Filter-null"));
	m_gaussBlurFilterShader = new Shader(GET_CONFIG_VALUE_STR("gaussBlurFilterShader", "filter-gaussBlur7x1"));
	m_fxaaFilterShader = new Shader(GET_CONFIG_VALUE_STR("fxaaFilterShader", "filter-fxaa"));
	//m_altCamera.GetTransform().Rotate(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Angle(180));

	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	m_filterTexture = new Texture(width, height, NULL, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);

	m_filterMaterial = new Material(m_filterTexture);
	m_filterMesh = new Mesh("plane4.obj");
	m_filterMesh->Initialize();

	InitializeCubeMap();

	m_cubeMapShader = new Shader(GET_CONFIG_VALUE_STR("cubeShadowMapShader", "CubeShadowMapGenerator"));
	//m_cubeShadowMap = new CubeShadowMapTexture(width, height);
	m_cubeShadowMap = new CubeShadowMap();
	m_cubeShadowMap->Init(width, height);

	for (int i = 0; i < SHADOW_MAPS_COUNT; ++i)
	{
		int shadowMapSize = 1 << (i + 1); // generates a sequence of numbers: 2, 4, 8, 16, ..., 2^(SHADOW_MAPS_COUNT).
		m_shadowMaps[i] = new Texture(shadowMapSize, shadowMapSize, NULL, GL_TEXTURE_2D, GL_LINEAR,
			GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true, GL_COLOR_ATTACHMENT0 /* we're going to render color information */);
		m_shadowMapTempTargets[i] = new Texture(shadowMapSize, shadowMapSize, NULL, GL_TEXTURE_2D, GL_LINEAR,
			GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true, GL_COLOR_ATTACHMENT0 /* we're going to render color information */);
	}

	m_fontTexture = new Texture("Holstein.tga", GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);
	m_textRenderer = new TextRenderer(m_fontTexture);

	m_waterDUDVTexture = new Texture(GET_CONFIG_VALUE_STR("waterDUDVMap", "waterDUDV.png"));
	m_waterNormalMap = new Texture(GET_CONFIG_VALUE_STR("waterNormalMap", "waterNormalMap.png"));
	m_waterReflectionTexture = new Texture(GET_CONFIG_VALUE("waterReflectionTextureWidth", 320), GET_CONFIG_VALUE("waterReflectionTextureHeight", 180), NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RGB, GL_RGBA, false, GL_COLOR_ATTACHMENT0);
	unsigned char* data[] = { NULL, NULL };
	GLfloat filters[] = { GL_LINEAR, GL_LINEAR };
	GLenum internalFormats[] = { GL_RGB, GL_DEPTH_COMPONENT32 };
	GLenum formats[] = { GL_RGBA, GL_DEPTH_COMPONENT };
	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
	m_waterRefractionTexture = new Texture(2, GET_CONFIG_VALUE("waterRefractionTextureWidth", 1280), GET_CONFIG_VALUE("waterRefractionTextureHeight", 720), data, GL_TEXTURE_2D, filters, internalFormats, formats, false, attachments);
	//m_waterRefractionTexture = new Texture(GET_CONFIG_VALUE("waterRefractionTextureWidth", 1280), GET_CONFIG_VALUE("waterRefractionTextureHeight", 720), NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RGB, GL_RGBA, false, GL_COLOR_ATTACHMENT0);
	m_waterShader = new Shader(GET_CONFIG_VALUE_STR("waterShader", "water-shader"));
	m_waterNoDirectionalLightShader = new Shader(GET_CONFIG_VALUE_STR("waterNoDirectionalLightShader", "water-no-directional-light-shader"));

	m_billboardShader = new Shader(GET_CONFIG_VALUE_STR("billboardShader", "billboard-shader"));

	SetTexture("displayTexture", new Texture(width, height, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0));
#ifndef ANT_TWEAK_BAR_ENABLED
	SetReal("fxaaSpanMax", m_fxaaSpanMax);
	SetReal("fxaaReduceMin", m_fxaaReduceMin);
	SetReal("fxaaReduceMul", m_fxaaReduceMul);
#endif

	/* ==================== Creating a "Main menu camera" begin ==================== */
	const Real defaultFoV = GET_CONFIG_VALUE("defaultCameraFoV", 70.0f);
	const Real defaultAspectRatio = GET_CONFIG_VALUE("defaultCameraAspectRatio", static_cast<Real>(800) / 600);
	const Real defaultNearPlane = GET_CONFIG_VALUE("defaultCameraNearPlane", 0.1f);
	const Real defaultFarPlane = GET_CONFIG_VALUE("defaultCameraFarPlane", 1000.0f);

	Angle fov(GET_CONFIG_VALUE("mainMenuCameraFoV", defaultFoV), Unit::DEGREE);
	Real aspectRatio = GET_CONFIG_VALUE("mainMenuCameraAspectRatio", defaultAspectRatio);
	Real zNearPlane = GET_CONFIG_VALUE("mainMenuCameraNearPlane", defaultNearPlane);
	Real zFarPlane = GET_CONFIG_VALUE("mainMenuCameraFarPlane", defaultFarPlane);
	m_mainMenuCamera = new Camera(fov, aspectRatio, zNearPlane, zFarPlane, Transform());
	/* ==================== Creating a "Main menu camera" end ==================== */

#ifdef DEBUG_RENDERING_ENABLED
	//m_guiTextures.push_back(new GuiTexture("chessboard3.jpg", Math::Vector2D(0.5f, 0.5f), Math::Vector2D(0.25f, 0.25f)));
	//m_guiTextures.push_back(new GuiTexture("crate.jpg", Math::Vector2D(0.45f, 0.45f), Math::Vector2D(0.25f, 0.25f)));
	Math::Vector2D quadVertexPositions[] = { Math::Vector2D(-REAL_ONE, REAL_ONE), Math::Vector2D(REAL_ONE, REAL_ONE), Math::Vector2D(-REAL_ONE, -REAL_ONE), Math::Vector2D(REAL_ONE, -REAL_ONE) };
	m_debugQuad = new GuiMesh(quadVertexPositions, 4);
	m_debugShader = new Shader("debug-shader");
#endif

	STOP_PROFILING;
	NOTICE_LOG("Creating Renderer instance finished");
}


Renderer::~Renderer(void)
{
	INFO_LOG("Destroying rendering engine...");
	START_PROFILING;

	glDeleteVertexArrays(1, &m_vao);

	//SAFE_DELETE(m_currentLight);
	// TODO: Deallocating the lights member variable
	// TODO: Deallocating the cameras member variable

	SAFE_DELETE(m_ambientShader);
	SAFE_DELETE(m_shadowMapShader);
	SAFE_DELETE(m_nullFilterShader);
	SAFE_DELETE(m_gaussBlurFilterShader);
	SAFE_DELETE(m_fxaaFilterShader);
	//SAFE_DELETE(altCameraNode);
	//SAFE_DELETE(m_skyboxNode);
	SAFE_DELETE(m_skyboxShader);
	SAFE_DELETE(m_skyboxProceduralShader);
	SAFE_DELETE(m_cubeShadowMap);
	SAFE_DELETE(m_filterMaterial);
	SAFE_DELETE(m_filterMesh);

	// TODO: m_fontTexture uses the same texture as the fontTexture used in CoreEngine class. That's why we shouldn't SAFE_DELETE font texture here.
	// Of course, we should deal with it later on more appropriately.
	//SetTexture("fontTexture", NULL);
	//SAFE_DELETE(m_fontTexture);
	SAFE_DELETE(m_textRenderer);

	SetTexture("waterReflectionTexture", NULL);
	SetMultitexture("waterRefractionTexture", NULL, 0);
	SetMultitexture("waterDepthMap", NULL, 1);
	SetTexture("waterDUDVMap", NULL);
	SetTexture("waterNormalMap", NULL);
	SAFE_DELETE(m_waterDUDVTexture);
	SAFE_DELETE(m_waterNormalMap);
	SAFE_DELETE(m_waterRefractionTexture);
	SAFE_DELETE(m_waterReflectionTexture);
	SAFE_DELETE(m_waterShader);
	SAFE_DELETE(m_waterNoDirectionalLightShader);
	
	SAFE_DELETE(m_billboardShader);
	for (std::vector<GameNode*>::iterator billboardNodeItr = m_billboardNodes.begin(); billboardNodeItr != m_billboardNodes.end(); ++billboardNodeItr)
	{
		SAFE_DELETE(*billboardNodeItr);
	}
	m_billboardNodes.clear();

	for (std::map<FogEffect::FogKey, Shader*>::iterator ambientLightFogShadersItr = m_ambientShadersFogEnabledMap.begin();
	ambientLightFogShadersItr != m_ambientShadersFogEnabledMap.end(); ambientLightFogShadersItr++)
	{
		SAFE_DELETE(ambientLightFogShadersItr->second);
	}
	m_ambientShadersFogEnabledMap.clear();
	for (std::map<FogEffect::FogKey, Shader*>::iterator ambientLightFogShadersItr = m_ambientShadersFogEnabledTerrainMap.begin();
	ambientLightFogShadersItr != m_ambientShadersFogEnabledTerrainMap.end(); ambientLightFogShadersItr++)
	{
		SAFE_DELETE(ambientLightFogShadersItr->second);
	}
	m_ambientShadersFogEnabledTerrainMap.clear();

	SetTexture("shadowMap", NULL);
	for (int i = 0; i < SHADOW_MAPS_COUNT; ++i)
	{
		SAFE_DELETE(m_shadowMaps[i]);
		SAFE_DELETE(m_shadowMapTempTargets[i]);
	}

#ifdef DEBUG_RENDERING_ENABLED
	for (std::vector<GuiTexture*>::iterator guiTextureItr = m_guiTextures.begin(); guiTextureItr != m_guiTextures.end(); ++guiTextureItr)
	{
		SAFE_DELETE(*guiTextureItr);
	}
	m_guiTextures.clear();
	SAFE_DELETE(m_debugQuad);
	SAFE_DELETE(m_debugShader);
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
	TwTerminate(); // Terminate AntTweakBar
#endif
	glfwTerminate(); // Terminate GLFW

	STOP_PROFILING;
	NOTICE_LOG("Rendering engine destroyed");
}

void Renderer::SetCallbacks()
{
	CHECK_CONDITION_EXIT(m_window != NULL, Critical, "Setting GLFW callbacks failed. The window is NULL.");
	glfwSetWindowCloseCallback(m_window, &CoreEngine::WindowCloseEventCallback);
	glfwSetWindowSizeCallback(m_window, CoreEngine::WindowResizeCallback);
	glfwSetKeyCallback(m_window, &CoreEngine::KeyEventCallback);
	//glfwSetCharCallback(m_window, CoreEngine::CharEventCallback);
	//glfwSetMousePosCallback(m_window, CoreEngine::MouseMotionCallback);
	glfwSetCursorPosCallback(m_window, &CoreEngine::MousePosCallback);
	glfwSetMouseButtonCallback(m_window, &CoreEngine::MouseEventCallback);
	glfwSetScrollCallback(m_window, &CoreEngine::ScrollEventCallback);
}

void Renderer::InitializeCubeMap()
{
	START_PROFILING;
	std::string cubeMapDayDirectory = GET_CONFIG_VALUE_STR("skyboxDayDirectory", "SkyboxDebug");
	std::string cubeMapNightDirectory = GET_CONFIG_VALUE_STR("skyboxNightDirectory", "SkyboxDebug");
	m_skyboxTextureDay = InitializeCubeMapTexture(cubeMapDayDirectory);
	m_skyboxTextureNight = InitializeCubeMapTexture(cubeMapNightDirectory);

	//SetTexture("cubeMapDay", m_skyboxTextureDay);
	//SetTexture("cubeMapNight", m_skyboxTextureNight);

	Material* cubeMapMaterial = new Material(m_skyboxTextureDay, "cubeMapDay");
	cubeMapMaterial->SetAdditionalTexture(m_skyboxTextureNight, "cubeMapNight");

	m_skyboxNode = new GameNode();
	m_skyboxNode->AddComponent(new MeshRenderer(new Mesh(GET_CONFIG_VALUE_STR("skyboxModel", "cube.obj")), cubeMapMaterial));
	m_skyboxNode->GetTransform().SetPos(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	m_skyboxNode->GetTransform().SetScale(5.0f); /* TODO: Don't use hardcoded values! Ever! */
	m_skyboxShader = new Shader(GET_CONFIG_VALUE_STR("skyboxShader", "skybox-shader"));
	m_skyboxProceduralShader = new Shader(GET_CONFIG_VALUE_STR("skyboxProceduralShader", "skybox-procedural"));
	STOP_PROFILING;
}

Texture* Renderer::InitializeCubeMapTexture(const std::string& cubeMapTextureDirectory)
{
	START_PROFILING;
	const std::string DIRECTORY_PATH_SEPARATOR = "\\"; // for Windows it's "\", but for Unix it's "/"
	const std::string EXPECTED_POS_X_FACE_FILENAME = "right";
	const std::string EXPECTED_NEG_X_FACE_FILENAME = "left";
	const std::string EXPECTED_POS_Y_FACE_FILENAME = "up";
	const std::string EXPECTED_NEG_Y_FACE_FILENAME = "down";
	const std::string EXPECTED_POS_Z_FACE_FILENAME = "front";
	const std::string EXPECTED_NEG_Z_FACE_FILENAME = "back";

	FileManager fileManager;
	std::vector<std::string> filenames = fileManager.ListAllFilesInDirectory(CoreEngine::GetCoreEngine()->GetTexturesDirectory() + cubeMapTextureDirectory);
	bool cubeMapPosXFaceFileFound = false; std::string cubeMapPosXFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegXFaceFileFound = false; std::string cubeMapNegXFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapPosYFaceFileFound = false; std::string cubeMapPosYFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegYFaceFileFound = false; std::string cubeMapNegYFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapPosZFaceFileFound = false; std::string cubeMapPosZFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegZFaceFileFound = false; std::string cubeMapNegZFaceFileName = cubeMapTextureDirectory + DIRECTORY_PATH_SEPARATOR;
	for (std::vector<std::string>::const_iterator filenameItr = filenames.begin(); filenameItr != filenames.end(); ++filenameItr)
	{
		if ((!cubeMapPosXFaceFileFound) && ((*filenameItr).find(EXPECTED_POS_X_FACE_FILENAME) != std::string::npos))
		{
			cubeMapPosXFaceFileFound = true;
			cubeMapPosXFaceFileName += (*filenameItr);
		}
		if ((!cubeMapNegXFaceFileFound) && ((*filenameItr).find(EXPECTED_NEG_X_FACE_FILENAME) != std::string::npos))
		{
			cubeMapNegXFaceFileFound = true;
			cubeMapNegXFaceFileName += (*filenameItr);
		}
		if ((!cubeMapPosYFaceFileFound) && ((*filenameItr).find(EXPECTED_POS_Y_FACE_FILENAME) != std::string::npos))
		{
			cubeMapPosYFaceFileFound = true;
			cubeMapPosYFaceFileName += (*filenameItr);
		}
		if ((!cubeMapNegYFaceFileFound) && ((*filenameItr).find(EXPECTED_NEG_Y_FACE_FILENAME) != std::string::npos))
		{
			cubeMapNegYFaceFileFound = true;
			cubeMapNegYFaceFileName += (*filenameItr);
		}
		if ((!cubeMapPosZFaceFileFound) && ((*filenameItr).find(EXPECTED_POS_Z_FACE_FILENAME) != std::string::npos))
		{
			cubeMapPosZFaceFileFound = true;
			cubeMapPosZFaceFileName += (*filenameItr);
		}
		if ((!cubeMapNegZFaceFileFound) && ((*filenameItr).find(EXPECTED_NEG_Z_FACE_FILENAME) != std::string::npos))
		{
			cubeMapNegZFaceFileFound = true;
			cubeMapNegZFaceFileName += (*filenameItr);
		}
	}
	CHECK_CONDITION_EXIT(cubeMapPosXFaceFileFound, Utility::Error, "Cannot locate the right face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT(cubeMapNegXFaceFileFound, Utility::Error, "Cannot locate the left face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT(cubeMapPosYFaceFileFound, Utility::Error, "Cannot locate the up face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT(cubeMapNegYFaceFileFound, Utility::Error, "Cannot locate the down face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT(cubeMapPosZFaceFileFound, Utility::Error, "Cannot locate the front face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	CHECK_CONDITION_EXIT(cubeMapNegZFaceFileFound, Utility::Error, "Cannot locate the back face of the cube map"); // TODO: Set default texture for the missing face instead of just exiting
	Texture* cubeMapTexture = new Texture(cubeMapPosXFaceFileName, cubeMapNegXFaceFileName, cubeMapPosYFaceFileName, cubeMapNegYFaceFileName, cubeMapPosZFaceFileName, cubeMapNegZFaceFileName);
	if (cubeMapTexture == NULL)
	{
		ERROR_LOG("Cube map texture is NULL");
		exit(EXIT_FAILURE);
	}
	STOP_PROFILING;
	return cubeMapTexture;
}

void Renderer::RegisterTerrainNode(GameNode* terrainNode)
{
	CHECK_CONDITION(terrainNode != NULL, Utility::Error, "Cannot register terrain node. Given terrain node is NULL.");
	CHECK_CONDITION(m_terrainNode == NULL, Utility::Warning, "Replacing already set terrain noed with a different one.");
	m_terrainNode = terrainNode;
}

void Renderer::AddWaterNode(GameNode* waterNode)
{
	CHECK_CONDITION_EXIT(waterNode != NULL, Utility::Emergency, "Adding water node failed. The water node is NULL.");
	if (m_waterNodes.empty())
	{
		INFO_LOG("Adding first water node to the rendering engine. Enabling clipping planes, creating reflection, refraction textures and the water shader.");
	}
	m_waterNodes.push_back(waterNode);
}

void Renderer::AddBillboardNode(GameNode* billboardNode)
{
	CHECK_CONDITION_EXIT(billboardNode != NULL, Utility::Emergency, "Adding billboard node failed. The given billboard node is NULL.");
	m_billboardNodes.push_back(billboardNode);
}

/* TODO: Remove in the future */
struct CameraDirection
{
	GLenum cubemapFace;
	Quaternion rotation;
};

CameraDirection gCameraDirections[6 /* number of cube map faces */] =
{
	//{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) },
	//{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Vector3D(-REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) },
	//{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE) },
	//{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE) },
	//{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) },
	//{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) }

	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Quaternion(Matrix4D(Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Quaternion(Matrix4D(Vector3D(-REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Quaternion(Matrix4D(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Quaternion(Matrix4D(Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE))) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Quaternion(Matrix4D(Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Quaternion(Matrix4D(Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) }
};

void Renderer::Render(const GameNode& gameNode)
{
	START_PROFILING;
	Rendering::CheckErrorCode("Renderer::Render", "Started main render function");
	CHECK_CONDITION_EXIT(!m_cameras.empty() && m_currentCameraIndex >= 0 && m_currentCameraIndex < m_cameras.size() && m_cameras[m_currentCameraIndex] != NULL,
		Utility::Emergency, "Rendering failed. There is no proper camera set up (current camera index = %d)", m_currentCameraIndex);

	AdjustAmbientLightAccordingToCurrentTime();

#ifdef ANT_TWEAK_BAR_ENABLED
	SetVector3D("fogColor", m_fogColor);
	SetReal("fogStart", m_fogStart);
	SetReal("fogEnd", m_fogEnd);
	SetReal("fogDensityFactor", m_fogDensityFactor);
	SetVector3D("ambientIntensity", m_ambientLight);
	SetReal("fxaaSpanMax", m_fxaaSpanMax);
	SetReal("fxaaReduceMin", m_fxaaReduceMin);
	SetReal("fxaaReduceMul", m_fxaaReduceMul);
	SetVector4D("clipPlane", m_defaultClipPlane); // The workaround for some drivers ignoring the glDisable(GL_CLIP_DISTANCE0) method
	CheckCameraIndexChange();
#endif
	if (!m_waterNodes.empty())
	{
		//RenderWaterTextures(gameNode);
	}

	GetTexture("displayTexture")->BindAsRenderTarget();
	//BindAsRenderTarget();

	ClearScreen();
	m_currentCamera = m_cameras[m_currentCameraIndex];

	RenderSceneWithAmbientLight(gameNode);

	//RenderSceneWithPointLights(gameNode);

	for (std::vector<Lighting::BaseLight*>::iterator lightItr = m_directionalAndSpotLights.begin(); lightItr != m_directionalAndSpotLights.end(); ++lightItr)
	{
		m_currentLight = (*lightItr);
		if (!m_currentLight->IsEnabled())
		{
			continue;
		}

		ShadowInfo* shadowInfo = m_currentLight->GetShadowInfo();
		int shadowMapIndex = (shadowInfo == NULL) ? 0 : shadowInfo->GetShadowMapSizeAsPowerOf2() - 1;
		CHECK_CONDITION_EXIT(shadowMapIndex < SHADOW_MAPS_COUNT, Error, "Incorrect shadow map size. Shadow map index must be an integer from range [0; %d), but equals %d.", SHADOW_MAPS_COUNT, shadowMapIndex);
		SetTexture("shadowMap", m_shadowMaps[shadowMapIndex]); // TODO: Check what would happen if we didn't set texture here?
		m_shadowMaps[shadowMapIndex]->BindAsRenderTarget();
		ClearScreen(Color(REAL_ONE /* completely in light */ /* TODO: When at night it should be REAL_ZERO */, REAL_ONE /* we want variance to be also cleared */, REAL_ZERO, REAL_ZERO)); // everything is in light (we can clear the COLOR_BUFFER_BIT)
		if (/*(m_shadowsEnabled) && */ (shadowInfo != NULL)) // The currentLight casts shadows
		{
			m_altCamera.SetProjection(shadowInfo->GetProjection());
			ShadowCameraTransform shadowCameraTransform = m_currentLight->CalcShadowCameraTransform(m_currentCamera->GetTransform().GetTransformedPos(), m_currentCamera->GetTransform().GetTransformedRot());
			m_altCamera.GetTransform().SetPos(shadowCameraTransform.m_pos);
			m_altCamera.GetTransform().SetRot(shadowCameraTransform.m_rot);

			//CRITICAL_LOG("AltCamera.GetViewProjection() = \"%s\"", m_altCamera.GetViewProjection().ToString().c_str());
			m_lightMatrix = BIAS_MATRIX * m_altCamera.GetViewProjection(); // FIXME: Check matrix multiplication

			SetReal("shadowLightBleedingReductionFactor", shadowInfo->GetLightBleedingReductionAmount());
			SetReal("shadowVarianceMin", shadowInfo->GetMinVariance());

			CameraBase* temp = m_currentCamera;
			m_currentCamera = &m_altCamera;

			if (shadowInfo->IsFlipFacesEnabled())
			{
				glCullFace(GL_FRONT);
			}
			m_terrainNode->RenderAll(m_shadowMapShader, this);
			gameNode.RenderAll(m_shadowMapShader, this);
			if (shadowInfo->IsFlipFacesEnabled())
			{
				glCullFace(GL_BACK);
			}

			m_currentCamera = temp;

			if (m_applyFilterEnabled)
			{
				//ApplyFilter(m_nullFilterShader, GetTexture("shadowMap"), GetTexture("shadowMapTempTarget"));
				//ApplyFilter(m_nullFilterShader, GetTexture("shadowMapTempTarget"), GetTexture("shadowMap"));
				if (!AlmostEqual(shadowInfo->GetShadowSoftness(), REAL_ZERO))
				{
					BlurShadowMap(shadowMapIndex, shadowInfo->GetShadowSoftness());
				}
			}
		}
		else // current light does not cast shadow or shadowing is disabled at all
		{
			// we set the light matrix this way so that, if no shadow should be cast
			// everything in the scene will be mapped to the same point
			m_lightMatrix.SetScaleMatrix(REAL_ZERO, REAL_ZERO, REAL_ZERO);
			SetReal("shadowLightBleedingReductionFactor", REAL_ZERO);
			SetReal("shadowVarianceMin", m_defaultShadowMinVariance);
		}
		RenderSceneWithLight(m_currentLight, gameNode);
	}
	SetVector3D("inverseFilterTextureSize", Vector3D(REAL_ONE / GetTexture("displayTexture")->GetWidth(), REAL_ONE / GetTexture("displayTexture")->GetHeight(), REAL_ZERO));

	if (!m_waterNodes.empty())
	{
		//RenderWaterNodes(); // normal rendering of water quads
	}
	if (!m_billboardNodes.empty())
	{
		//RenderBillboardNodes(); // rendering billboards
	}

	//RenderSkybox();

#ifdef DEBUG_RENDERING_ENABLED
	//RenderDebugNodes();
#endif


	ApplyFilter((Rendering::antiAliasingMethod == Rendering::Aliasing::FXAA) ? m_fxaaFilterShader : m_nullFilterShader, GetTexture("displayTexture"), NULL);
	STOP_PROFILING;
}

void Renderer::RenderWaterTextures(const GameNode& gameNode)
{
	START_PROFILING;
	CHECK_CONDITION_RETURN_VOID_ALWAYS(!m_waterNodes.empty(), Utility::Debug, "There are no water nodes registered in the rendering engine");
	// TODO: For now we only support one water node (you can see that in the "distance" calculation). In the future there might be more.
	CHECK_CONDITION(m_waterNodes.size() == 1, Utility::Warning, "For now the rendering engine supports only one water node in the game engine, but there are %d created.", m_waterNodes.size());
	RenderWaterReflectionTexture(gameNode);
	RenderWaterRefractionTexture(gameNode);
	
	// Now that we rendered the scene into the reflection and refraction textures for the water surface,
	// we want to disable the clipping planes completely. Unfortunately, it seems some drivers simply ignore the
	// below glDisable(GL_CLIP_DISTANCE0) call, so we need a trick. In order to have no clipping we simply
	// set the clipping plane distance from the origin very high. This way there are no fragments that can be culled
	// and as a result we render the whole scene.
	// glDisable(GL_CLIP_DISTANCE0); // Disabled plane clipping // glDisable(GL_CLIP_PLANE0);
	SetVector4D("clipPlane", m_defaultClipPlane); // The workaround for some drivers ignoring the glDisable(GL_CLIP_DISTANCE0) method
	//BindAsRenderTarget();
	STOP_PROFILING;
}

void Renderer::RenderWaterNodes()
{
	START_PROFILING;
	CHECK_CONDITION_RETURN_VOID_ALWAYS(!m_waterNodes.empty(), Utility::Debug, "There are no water nodes registered in the rendering engine");
	SetTexture("waterReflectionTexture", m_waterReflectionTexture);
	SetMultitexture("waterRefractionTexture", m_waterRefractionTexture, 0);
	SetMultitexture("waterDepthMap", m_waterRefractionTexture, 1);
	SetTexture("waterDUDVMap", m_waterDUDVTexture);
	SetTexture("waterNormalMap", m_waterNormalMap);
	//m_waterMoveFactor = fmod(m_waterMoveFactor + m_waterWaveSpeed * CoreEngine::GetCoreEngine()->GetClockSpeed(), REAL_ONE);
	m_waterMoveFactor += m_waterWaveSpeed * CoreEngine::GetCoreEngine()->GetClockSpeed();
	if (m_waterMoveFactor > REAL_ONE)
	{
		m_waterMoveFactor -= REAL_ONE;
		CHECK_CONDITION_ALWAYS(m_waterMoveFactor < REAL_ONE, Utility::Error, "Water move factor is still greater than 1.0. It is equal to %.3f", m_waterMoveFactor); // TODO: Remove "ALWAYS" in the future
	}
	SetReal("waterMoveFactor", m_waterMoveFactor);
	SetReal("nearPlane", 0.1f /* TODO: This value should be always equal to the near plane of the current camera, but it is not easy for us to get this value */);
	SetReal("farPlane", 1000.0f /* TODO: This value should be always equal to the far plane of the current camera, but it is not easy for us to get this value */);
	SetReal("waterWaveStrength", m_waterWaveStrength);
	SetReal("waterShineDamper", m_waterShineDamper);
	SetReal("waterReflectivity", m_waterReflectivity);
	SetReal("waterFresnelEffectFactor", m_waterFresnelEffectFactor);
	SetReal("waterNormalVerticalFactor", m_waterNormalVerticalFactor);
	for (std::vector<GameNode*>::const_iterator waterNodeItr = m_waterNodes.begin(); waterNodeItr != m_waterNodes.end(); ++waterNodeItr)
	{
		if (m_waterLightReflectionEnabled)
		{
			(*waterNodeItr)->RenderAll(m_waterShader, this);
		}
		else
		{
			(*waterNodeItr)->RenderAll(m_waterNoDirectionalLightShader, this);
		}
	}
	STOP_PROFILING;
}

void Renderer::RenderBillboardNodes()
{
	START_PROFILING;
	DEBUG_LOG("Rendering billboards started");
	for (std::vector<GameNode*>::const_iterator billboardNodeItr = m_billboardNodes.begin(); billboardNodeItr != m_billboardNodes.end(); ++billboardNodeItr)
	{
		(*billboardNodeItr)->RenderAll(m_billboardShader, this);
	}
	STOP_PROFILING;
}

void Renderer::RenderWaterReflectionTexture(const GameNode& gameNode)
{
	START_PROFILING;
	CHECK_CONDITION_RETURN_VOID_ALWAYS(!m_waterNodes.empty(), Utility::Debug, "There are no water nodes registered in the rendering engine");
	
	Transform& cameraTransform = m_currentCamera->GetTransform();
	const Math::Real cameraHeight = cameraTransform.GetTransformedPos().GetY();
	Math::Real distance = 2.0f * (cameraHeight - m_waterNodes.front()->GetTransform().GetTransformedPos().GetY());
	cameraTransform.GetPos().SetY(cameraHeight - distance); // TODO: use m_altCamera instead of the main camera.
	cameraTransform.GetRot().InvertPitch();

	m_waterReflectionClippingPlane.SetW(-m_waterNodes.front()->GetTransform().GetTransformedPos().GetY() + 0.1f /* add 0.1f to remove some glitches on the water surface */);
	SetVector4D("clipPlane", m_waterReflectionClippingPlane);
	m_waterReflectionTexture->BindAsRenderTarget();
	ClearScreen(REAL_ONE, REAL_ONE, REAL_ONE, REAL_ONE);

	glDisable(GL_DEPTH_TEST);
	RenderSkybox();
	RenderSceneWithAmbientLight(gameNode);

	RenderSceneWithPointLights(gameNode);
	for (std::vector<Lighting::BaseLight*>::iterator lightItr = m_directionalAndSpotLights.begin(); lightItr != m_directionalAndSpotLights.end(); ++lightItr)
	{
		m_currentLight = (*lightItr);
		if (!m_currentLight->IsEnabled())
		{
			continue;
		}
		RenderSceneWithLight(m_currentLight, gameNode, false);
	}
	//SetVector3D("inverseFilterTextureSize", Vector3D(REAL_ONE / m_waterReflectionTexture->GetWidth(), REAL_ONE / m_waterReflectionTexture->GetHeight(), REAL_ZERO));

	glEnable(GL_DEPTH_TEST);

	//if (Rendering::antiAliasingMethod == Rendering::Aliasing::FXAA)
	//{
	//	ApplyFilter(m_fxaaFilterShader, m_waterReflectionTexture, NULL);
	//}
	//else
	//{
	//	ApplyFilter(m_nullFilterShader, m_waterReflectionTexture, NULL);
	//}

	//BindAsRenderTarget();
	
	cameraTransform.GetPos().SetY(cameraHeight); // TODO: use m_altCamera instead of the main camera.
	cameraTransform.GetRot().InvertPitch();

	STOP_PROFILING;
}

void Renderer::RenderWaterRefractionTexture(const GameNode& gameNode)
{
	START_PROFILING;
	CHECK_CONDITION_RETURN_VOID_ALWAYS(!m_waterNodes.empty(), Utility::Debug, "There are no water nodes registered in the rendering engine");

	m_waterRefractionClippingPlane.SetW(m_waterNodes.front()->GetTransform().GetTransformedPos().GetY());
	SetVector4D("clipPlane", m_waterRefractionClippingPlane);
	m_waterRefractionTexture->BindAsRenderTarget();
	ClearScreen(REAL_ONE, REAL_ONE, REAL_ONE, REAL_ONE);

	//glDisable(GL_DEPTH_TEST);
	RenderSkybox();
	RenderSceneWithAmbientLight(gameNode);

	RenderSceneWithPointLights(gameNode);
	for (std::vector<Lighting::BaseLight*>::iterator lightItr = m_directionalAndSpotLights.begin(); lightItr != m_directionalAndSpotLights.end(); ++lightItr)
	{
		m_currentLight = (*lightItr);
		if (!m_currentLight->IsEnabled())
		{
			continue;
		}

		RenderSceneWithLight(m_currentLight, gameNode, false);
	}
	//SetVector3D("inverseFilterTextureSize", Vector3D(REAL_ONE / m_waterReflectionTexture->GetWidth(), REAL_ONE / m_waterReflectionTexture->GetHeight(), REAL_ZERO));

	//glEnable(GL_DEPTH_TEST);

	//if (Rendering::antiAliasingMethod == Rendering::Aliasing::FXAA)
	//{
	//	ApplyFilter(m_fxaaFilterShader, m_waterReflectionTexture, NULL);
	//}
	//else
	//{
	//	ApplyFilter(m_nullFilterShader, m_waterReflectionTexture, NULL);
	//}

	//BindAsRenderTarget();

	STOP_PROFILING;
}

void Renderer::RenderSceneWithAmbientLight(const GameNode& gameNode)
{
	START_PROFILING;
	if (m_fogEnabled)
	{
		//DEBUG_LOG("Fog fall-off type: %d. Fog distance calculation type: %d", m_fogFallOffType, m_fogCalculationType);
		Shader* fogShader = m_ambientShadersFogEnabledMap[FogEffect::FogKey(m_fogFallOffType, m_fogCalculationType)];
		Shader* fogTerrainShader = m_ambientShadersFogEnabledTerrainMap[FogEffect::FogKey(m_fogFallOffType, m_fogCalculationType)];
		CHECK_CONDITION_EXIT(fogShader != NULL, Utility::Emergency, "Cannot render the scene with ambient light. The fog shader is NULL.");
		CHECK_CONDITION_EXIT(fogTerrainShader != NULL, Utility::Emergency, "Cannot render terrain with ambient light. The terrain fog shader is NULL.");
		m_terrainNode->RenderAll(fogTerrainShader, this); // Ambient rendering with fog enabled for terrain node
		gameNode.RenderAll(fogShader, this); // Ambient rendering with fog enabled
	}
	else if (m_ambientLightEnabled)
	{
		m_terrainNode->RenderAll(m_ambientShaderTerrain, this); // Ambient rendering with fog disabled for terrain node
		gameNode.RenderAll(m_ambientShader, this); // Ambient rendering with disabled fog
	}
	STOP_PROFILING;
}

void Renderer::RenderSceneWithPointLights(const GameNode& gameNode)
{
	START_PROFILING;
	if (!Lighting::PointLight::ArePointLightsEnabled())
	{
		STOP_PROFILING;
		return;
	}

	for (std::vector<Lighting::PointLight*>::iterator pointLightItr = m_pointLights.begin(); pointLightItr != m_pointLights.end(); ++pointLightItr)
	{
		m_pointLight = (*pointLightItr);
		if (!m_pointLight->IsEnabled())
		{
			continue;
		}
		//if (m_shadowsEnabled && m_pointLightShadowsEnabled)
		//{
		glCullFace(GL_FRONT);
		const int NUMBER_OF_CUBE_MAP_FACES = 6;

		glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX); // TODO: Replace FLT_MAX with REAL_MAX
		m_altCamera.GetTransform().SetPos(m_pointLight->GetTransform().GetTransformedPos());
		for (unsigned int i = 0; i < NUMBER_OF_CUBE_MAP_FACES; ++i)
		{
			Rendering::CheckErrorCode(__FUNCTION__, "Point light shadow mapping");
			//DEBUG_LOG("Binding the cube face #%d", i);
			m_cubeShadowMap->BindForWriting(gCameraDirections[i].cubemapFace);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			m_altCamera.GetTransform().SetRot(gCameraDirections[i].rotation); // TODO: Set the rotation correctly

			CameraBase* temp = m_currentCamera;
			m_currentCamera = &m_altCamera;

			m_terrainNode->RenderAll(m_cubeMapShader, this);
			gameNode.RenderAll(m_cubeMapShader, this);

			m_currentCamera = temp;
		}
		//}

		RenderSceneWithLight(m_pointLight, gameNode);
	}
	m_pointLight = NULL;
	STOP_PROFILING;
}

void Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled /* = true */)
{
	START_PROFILING;
	CHECK_CONDITION_EXIT(light != NULL, Utility::Emergency, "Cannot render the scene. The light is NULL.");
	INFO_LOG("Rendering scene with light.");
	glCullFace(Rendering::glCullFaceMode);
	GetTexture("displayTexture")->BindAsRenderTarget();
	if (!Rendering::glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both weights equal to 1
	glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore
	glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image

	m_terrainNode->RenderAll(isCastingShadowsEnabled ? light->GetTerrainShader() : light->GetNoShadowTerrainShader(), this);
	gameNode.RenderAll(isCastingShadowsEnabled ? light->GetShader() : light->GetNoShadowShader(), this);

	glDepthFunc(Rendering::glDepthTestFunc);
	glDepthMask(GL_TRUE);
	if (!Rendering::glBlendEnabled)
	{
		glDisable(GL_BLEND);
	}
	else
	{
		glBlendFunc(Rendering::glBlendSfactor, Rendering::glBlendDfactor);
	}
	STOP_PROFILING;
}

void Renderer::RenderMainMenu(const MenuEntry& menuEntry)
{
	START_PROFILING;
	BindAsRenderTarget();
	ClearScreen(m_backgroundColor);
	if (m_cameras.empty() || m_cameras[m_currentCameraIndex] == NULL /* TODO: Check if m_currentCameraIndex is within correct range */)
	{
		//DELOCUST_LOG("Rendering main menu with a \"main menu camera\".");
		m_currentCamera = m_mainMenuCamera;
	}
	else
	{
		m_currentCamera = m_cameras[m_currentCameraIndex];
	}

	//double time = glfwGetTime();
	//std::stringstream ss;
	//ss << "FPS: " << std::setprecision(2) << time << " [ms]";
	//m_textRenderer->DrawString(0, 5800, ss.str(), this);
	//m_textRenderer->DrawString(0, 50, "This is main menu", this);
	int menuEntryChildrenCount = menuEntry.GetChildrenCount();
	for (int i = 0; i < menuEntryChildrenCount; ++i)
	{
		m_textRenderer->DrawString(Text::CENTER, 350 - 100 * i, menuEntry.GetChildrenText(i), this,
			menuEntry.IsChildMenuEntrySelected(i) ? MenuEntry::GetSelectedMenuEntryTextColor() : MenuEntry::GetNotSelectedMenuEntryTextColor());
	}
	STOP_PROFILING;
}

void Renderer::RenderLoadingScreen(Math::Real loadingProgress)
{
	START_PROFILING;
	BindAsRenderTarget();
	ClearScreen();
	if (m_cameras.empty() || m_cameras[m_currentCameraIndex] == NULL /* TODO: Check if m_currentCameraIndex is within correct range */)
	{
		//DELOCUST_LOG("Rendering main menu with a \"main menu camera\".");
		m_currentCamera = m_mainMenuCamera;
	}
	else
	{
		m_currentCamera = m_cameras[m_currentCameraIndex];
	}

	std::stringstream ss;
	int progress = static_cast<int>(loadingProgress * 100.0f);
	ss << progress << "%";
	m_textRenderer->DrawString(Text::CENTER, 350, "Loading...", this);
	m_textRenderer->DrawString(Text::CENTER, 250, ss.str(), this);
	STOP_PROFILING;
}

void Renderer::AdjustAmbientLightAccordingToCurrentTime()
{
	START_PROFILING;
	/* ==================== Adjusting the time variables begin ==================== */
	Math::Real daytimeTransitionFactor, dayNightMixFactor;
	Rendering::GameTime::Daytime daytime = CoreEngine::GetCoreEngine()->GetCurrentDaytime(daytimeTransitionFactor);

	switch (daytime)
	{
	case GameTime::NIGHT:
		dayNightMixFactor = REAL_ZERO;
		m_ambientLight = m_ambientNighttimeColor;
		break;
	case GameTime::BEFORE_DAWN:
		dayNightMixFactor = REAL_ZERO;
		m_ambientLight = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, daytimeTransitionFactor);
		break;
	case GameTime::SUNRISE:
		dayNightMixFactor = daytimeTransitionFactor;
		m_ambientLight = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, daytimeTransitionFactor);
		break;
	case GameTime::DAY:
		dayNightMixFactor = REAL_ONE;
		m_ambientLight = m_ambientDaytimeColor;
		break;
	case GameTime::SUNSET:
		dayNightMixFactor = daytimeTransitionFactor;
		m_ambientLight = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, daytimeTransitionFactor);
		break;
	case GameTime::AFTER_DUSK:
		dayNightMixFactor = REAL_ZERO;
		m_ambientLight = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, daytimeTransitionFactor);
		break;
	}
	//DEBUG_LOG("Sunset started. Time = %.1f\tDayNightMixFactor = %.4f. Ambient light = (%s)",
	//	timeOfDay, dayNightMixFactor, m_ambientLight.ToString().c_str());
	SetReal("dayNightMixFactor", dayNightMixFactor);
	/* ==================== Adjusting the time variables end ==================== */
	STOP_PROFILING;
}

void Renderer::RenderSkybox()
{
	START_PROFILING;
	m_skyboxNode->GetTransform().SetPos(m_currentCamera->GetTransform().GetTransformedPos());
	m_skyboxNode->GetTransform().SetRot(Quaternion(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_skyboxAngle));
	m_skyboxAngle += m_skyboxAngleStep;
	if (m_fogEnabled)
	{
		STOP_PROFILING;
		return;
	}

	//glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	/**
	 * By default (GL_LESS) we tell OpenGL that an incoming fragment wins the depth test if its Z value is less than the stored one.
	 * However, in the case of a skybox the Z value is always the far Z. The far Z is clipped when the depth test function is set to "less than".
	 * To make it part of the scene we change the depth function to "less than or equal".
	 */
	glDepthFunc(GL_LEQUAL);
	m_skyboxNode->RenderAll(m_skyboxShader, this);
	glDepthFunc(Rendering::glDepthTestFunc);
	glCullFace(Rendering::glCullFaceMode);
	//glEnable(GL_DEPTH_TEST);
	Rendering::CheckErrorCode("Renderer::Render", "Rendering skybox");
	STOP_PROFILING;
}

void Renderer::BlurShadowMap(int shadowMapIndex, Real blurAmount /* how many texels we move per sample */)
{
	START_PROFILING;
	Texture* shadowMap = m_shadowMaps[shadowMapIndex];
	Texture* shadowMapTempTarget = m_shadowMapTempTargets[shadowMapIndex];
	if (shadowMap == NULL)
	{
		ERROR_LOG("Shadow map %d is NULL. Cannot perform the blurring process.", shadowMapIndex);
		STOP_PROFILING;
		return;
	}
	if (shadowMapTempTarget == NULL)
	{
		ERROR_LOG("Temporary shadow map target %d is NULL. Cannot perform the blurring process.", shadowMapIndex);
		STOP_PROFILING;
		return;
	}

	SetVector3D("blurScale", Vector3D(blurAmount / shadowMap->GetWidth(), REAL_ZERO, REAL_ZERO));
	ApplyFilter(m_gaussBlurFilterShader, shadowMap, shadowMapTempTarget);
	
	SetVector3D("blurScale", Vector3D(REAL_ZERO, blurAmount / shadowMap->GetHeight(), REAL_ZERO));
	ApplyFilter(m_gaussBlurFilterShader, shadowMapTempTarget, shadowMap);
	STOP_PROFILING;
}

// You cannot read and write from the same texture at the same time. That's why we use dest texture as a temporary texture to store the result
void Renderer::ApplyFilter(Shader* filterShader, Texture* source, Texture* dest)
{
	START_PROFILING;
	CHECK_CONDITION_EXIT(filterShader != NULL, Utility::Critical, "Cannot apply a filter. Filtering shader is NULL.");
	CHECK_CONDITION_EXIT(source != NULL, Utility::Critical, "Cannot apply a filter. Source texture is NULL.");
	CHECK_CONDITION_EXIT(source != dest, Utility::Critical, "Cannot apply a filter. Both source and destination textures point to the same location in memory.");
	if (dest == NULL)
	{
		DELOCUST_LOG("Binding window as a render target for filtering");
		BindAsRenderTarget();
	}
	else
	{
		DELOCUST_LOG("Binding texture as a render target for filtering");
		dest->BindAsRenderTarget();
	}

	//DEBUG_LOG("Applying a filter to the source texture");
	
	SetTexture("filterTexture", source);

	m_altCamera.SetProjection(Math::Matrix4D::IDENTITY_MATRIX);
	m_altCamera.GetTransform().SetPos(Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO));
	m_altCamera.GetTransform().SetRot(Quaternion(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Angle(180.0f)));

	CameraBase* temp = m_currentCamera;
	m_currentCamera = &m_altCamera;

	glClear(GL_DEPTH_BUFFER_BIT);
	filterShader->Bind();
	filterShader->UpdateUniforms(m_filterTransform, *m_filterMaterial, this);
	m_filterMesh->Draw();

	m_currentCamera = temp;
	SetTexture("filterTexture", NULL);
	STOP_PROFILING;
}

size_t Renderer::NextCamera()
{
	if (m_currentCameraIndex == m_cameras.size() - 1)
	{
		m_currentCameraIndex = -1;
	}
	return SetCurrentCamera(m_currentCameraIndex + 1);
}

size_t Renderer::PrevCamera()
{
	if (m_currentCameraIndex == 0)
	{
		m_currentCameraIndex = m_cameras.size();
	}
	return SetCurrentCamera(m_currentCameraIndex - 1);
}

size_t Renderer::SetCurrentCamera(size_t cameraIndex)
{
	CHECK_CONDITION((cameraIndex >= 0) && (cameraIndex < m_cameras.size()), Error, "Incorrect current camera index. Passed %d when the correct range is (%d, %d).", cameraIndex, 0, m_cameras.size());
	m_currentCameraIndex = cameraIndex;
#ifndef ANT_TWEAK_BAR_ENABLED
	NOTICE_LOG("Switched to camera #%d", m_currentCameraIndex + 1);
	//DEBUG_LOG("%s", m_cameras[m_currentCameraIndex]->ToString().c_str());
#endif
	return m_currentCameraIndex;
}

void Renderer::AddLight(Lighting::BaseLight* light)
{
	Lighting::DirectionalLight* directionalLight = dynamic_cast<Lighting::DirectionalLight*>(light);
	if (directionalLight != NULL)
	{
		INFO_LOG("Directional light with intensity = %.2f is being added to directional / spot lights vector", directionalLight->GetIntensity());
		m_waterLightReflectionEnabled = true;
		m_directionalAndSpotLights.push_back(directionalLight);
	}
	else
	{
		Lighting::SpotLight* spotLight = dynamic_cast<Lighting::SpotLight*>(light);
		if (spotLight != NULL)
		{
			INFO_LOG("Spot light with intensity = %.2f is being added to directional / spot lights vector", spotLight->GetIntensity());
			m_directionalAndSpotLights.push_back(spotLight);
		}
		else
		{
			Lighting::PointLight* pointLight = dynamic_cast<Lighting::PointLight*>(light);
			if (pointLight != NULL)
			{
				INFO_LOG("Point light with intensity = %.2f is being added to point lights vector", pointLight->GetIntensity());
				m_pointLights.push_back(pointLight);
			}
			else
			{
				EMERGENCY_LOG("Adding the light of unknown type. It is neither a directional nor spot nor point light.");
			}
		}
	}
	m_lights.push_back(light);
}

void Renderer::AddCamera(CameraBase* camera)
{
	m_cameras.push_back(camera);
#ifdef ANT_TWEAK_BAR_ENABLED
	++m_cameraCountMinusOne;
#endif
}

void Renderer::BindAsRenderTarget()
{
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}

void Renderer::BindCubeShadowMap(unsigned int textureUnit) const
{
	m_cubeShadowMap->BindForReading(textureUnit);
}

#ifdef DEBUG_RENDERING_ENABLED
void Renderer::RenderDebugNodes()
{
	m_debugShader->Bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	for (std::vector<GuiTexture*>::const_iterator guiTextureItr = m_guiTextures.begin(); guiTextureItr != m_guiTextures.end(); ++guiTextureItr)
	{
		(*guiTextureItr)->Bind(0);
		m_debugShader->SetUniformMatrix("guiTransformationMatrix", (*guiTextureItr)->GetTransformationMatrix());
		m_debugShader->SetUniformi("guiTexture", 0);
		m_debugQuad->Draw();
	}
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::AddLine(const Math::Vector3D& fromPosition, const Math::Vector3D& toPosition, const Color& color,
	Math::Real lineWidth /* = REAL_ONE */, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG("Debug line rendering is not yet supported by the engine");
}

void Renderer::AddSphere(const Math::Sphere& sphere, const Color& color,
	Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG("Debug sphere rendering is not yet supported by the engine");
}

void Renderer::AddCross(const Math::Vector3D& position, const Color& color, Math::Real size,
	Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG("Debug cross rendering is not yet supported by the engine");
}

void Renderer::AddCircle(const Math::Vector3D& centerPosition, const Math::Vector3D& planeNormal, Math::Real radius, const Color& color,
	Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG("Debug circle rendering is not yet supported by the engine");
}

void Renderer::AddAxes(const Transform& transform, const Color& color, Math::Real size,
	Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG("Debug axes rendering is not yet supported by the engine");
}

void Renderer::AddTriangle(const Math::Vector3D& v0, const Math::Vector3D& v1, const Math::Vector3D& v2, const Color& color,
	Math::Real lineWidth /* = REAL_ONE */, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG("Debug triangle rendering is not yet supported by the engine");
}

void Renderer::AddAABB(const Math::AABB& aabb, const Color& color, Math::Real lineWidth /* = REAL_ONE */, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG("Debug AABB rendering is not yet supported by the engine");
}

void Renderer::AddOBB(const Math::OBB& obb, const Color& color, Math::Real lineWidth /* = REAL_ONE */, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG("Debug OBB rendering is not yet supported by the engine");
}

void Renderer::AddString(const Math::Vector3D& pos, const char* text, const Color& color, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG("Debug text rendering is not yet supported by the engine");
}
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
void Renderer::InitializeTweakBars()
{
	INFO_LOG("Initializing rendering engine's tweak bars");

#ifdef RENDERER_PROPERTIES_BAR
	DEBUG_LOG("Initializing rendering engine's properties tweak bar");
	// TODO: CameraMembers[0] ("Position") is not displayed correctly, because at 0 address lies the pointer to parentGameNode
	m_cameraMembers[0].Name = "Projection"; m_cameraMembers[1].Name = "FoV"; m_cameraMembers[2].Name = "AspectRatio"; m_cameraMembers[3].Name = "NearPlane"; m_cameraMembers[4].Name = "FarPlane";
	m_cameraMembers[0].Type = matrix4DType; m_cameraMembers[1].Type = angleType; m_cameraMembers[2].Type = TW_TYPE_FLOAT; m_cameraMembers[3].Type = TW_TYPE_FLOAT; m_cameraMembers[4].Type = TW_TYPE_FLOAT;
	m_cameraMembers[0].Offset = 8; m_cameraMembers[1].Offset = 80; m_cameraMembers[2].Offset = 92; m_cameraMembers[3].Offset = 100; m_cameraMembers[4].Offset = 108;
	m_cameraMembers[0].DefString = ""; m_cameraMembers[1].DefString = ""; m_cameraMembers[2].DefString = " step=0.01 "; m_cameraMembers[3].DefString = ""; m_cameraMembers[4].DefString = "";
	m_cameraType = TwDefineStruct("Camera", m_cameraMembers, 5, sizeof(Rendering::Camera), NULL, NULL);

	m_propertiesBar = TwNewBar("PropertiesBar");
	TwAddVarRW(m_propertiesBar, "bgColor", TW_TYPE_COLOR3F, &m_backgroundColor, " label='Background color' ");
	TwAddVarRW(m_propertiesBar, "currentCamera", TW_TYPE_UINT32, &m_currentCameraIndex, " label='Current camera' ");
	TwAddVarRW(m_propertiesBar, "applyFilterEnabled", TW_TYPE_BOOLCPP, &m_applyFilterEnabled, " label='Apply filter' ");
	TwAddVarRW(m_propertiesBar, "ambientLightEnabled", TW_TYPE_BOOLCPP, &m_ambientLightEnabled, " label='Enabled' group='Ambient light' ");
	TwAddVarRO(m_propertiesBar, "ambientLight", TW_TYPE_COLOR3F, &m_ambientLight, " label='Color' group='Ambient light'");
	TwAddVarRO(m_propertiesBar, "ambientLightDaytime", TW_TYPE_COLOR3F, &m_ambientDaytimeColor, " label='Daytime color' group='Ambient light'");
	TwAddVarRO(m_propertiesBar, "ambientLightSunNearHorizon", TW_TYPE_COLOR3F, &m_ambientSunNearHorizonColor, " label='Sun near horizon color' group='Ambient light'");
	TwAddVarRO(m_propertiesBar, "ambientLightNighttime", TW_TYPE_COLOR3F, &m_ambientNighttimeColor, " label='Nighttime color' group='Ambient light'");
	TwAddVarRW(m_propertiesBar, "fogEnabled", TW_TYPE_BOOLCPP, &m_fogEnabled, " label='Enabled' group='Fog' ");
	TwAddVarRW(m_propertiesBar, "fogColor", TW_TYPE_COLOR3F, &m_fogColor, " label='Color' group='Fog' ");
	TwAddVarRW(m_propertiesBar, "fogStart", TW_TYPE_REAL, &m_fogStart, " label='Start' group='Fog' step=0.5 min=0.5");
	TwAddVarRW(m_propertiesBar, "fogEnd", TW_TYPE_REAL, &m_fogEnd, " label='End' group='Fog' step=0.5 min=1.0");
	TwAddVarRW(m_propertiesBar, "fogDensityFactor", TW_TYPE_REAL, &m_fogDensityFactor, " label='Density factor' group='Fog' step=0.02 min=0.01 max=5.0 ");
	TwAddVarRW(m_propertiesBar, "fogFallOffType", fogFallOffType, &m_fogFallOffType, " label='Fall-off type' group='Fog' ");
	TwAddVarRW(m_propertiesBar, "fogCalculationType", fogCalculationType, &m_fogCalculationType, " label='Calculation type' group='Fog' ");
	TwAddVarRW(m_propertiesBar, "directionalLightsEnabled", TW_TYPE_BOOLCPP, Lighting::DirectionalLight::GetDirectionalLightsEnabled(), " label='Directional light' group=Lights");
	TwAddVarRW(m_propertiesBar, "pointLightsEnabled", TW_TYPE_BOOLCPP, Lighting::PointLight::ArePointLightsEnabled(), " label='Point lights' group=Lights");
	TwAddVarRW(m_propertiesBar, "spotLightsEnabled", TW_TYPE_BOOLCPP, Lighting::SpotLight::GetSpotLightsEnabled(), " label='Spot lights' group=Lights");
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

	TwSetParam(m_propertiesBar, "currentCamera", "max", TW_PARAM_INT32, 1, &m_cameraCountMinusOne);
	TwSetParam(m_propertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup
	DEBUG_LOG("Initializing rendering engine's properties tweak bar finished");
#endif

#ifdef CAMERA_TWEAK_BAR
	DEBUG_LOG("Initializing rendering engine's cameras tweak bar");
	m_cameraBar = TwNewBar("CamerasBar");
	if (m_cameras.empty() || m_cameras[m_currentCameraIndex] == NULL)
	{
		ERROR_LOG("Cannot properly initialize rendering engine's cameras bar. No cameras setup by the game manager.");
		
		//TwAddVarRW(cameraBar, "cameraVar", m_cameraType,  m_mainMenuCamera, " label='Camera' group=Camera ");
		//TwAddVarRW(cameraBar, "MainMenuCamera.Pos", vector3DType, &m_mainMenuCamera->GetTransform().GetPos(), " label='MainMenuCamera.Pos' group=Camera ");
		//TwAddVarRW(cameraBar, "MainMenuCamera.Rot", TW_TYPE_QUAT4F, &m_mainMenuCamera->GetTransform().GetRot(), " label='MainMenuCamera.Rot' group=Camera ");
	}
	else
	{
		TwAddVarRW(m_cameraBar, "cameraVar", m_cameraType, m_cameras[m_currentCameraIndex], " label='Camera' group=Camera ");
		char cameraIndexStr[256];
		char cameraDefStr[256];
		_snprintf_s(cameraIndexStr, 256, 255, "camera[%zd].Pos", m_currentCameraIndex);
		_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%zd].Pos' group=Camera ", m_currentCameraIndex);
		TwAddVarRW(m_cameraBar, cameraIndexStr, vector3DType, &m_cameras[m_currentCameraIndex]->GetTransform().GetPos(), cameraDefStr);
		_snprintf_s(cameraIndexStr, 256, 255, "camera[%zd].Rot", m_currentCameraIndex);
		_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%zd].Rot' group=Camera ", m_currentCameraIndex);
		TwAddVarRW(m_cameraBar, cameraIndexStr, TW_TYPE_QUAT4F, &m_cameras[m_currentCameraIndex]->GetTransform().GetRot(), cameraDefStr);
	}
	
	TwDefine(" CamerasBar/Camera opened=true ");
	TwSetParam(m_cameraBar, NULL, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup
	DEBUG_LOG("Initializing rendering engine's cameras tweak bar finished");
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
	DEBUG_LOG("Initializing rendering engine's tweak bars finished");
}

void Renderer::CheckCameraIndexChange()
{
#ifdef CAMERA_TWEAK_BAR
	if (m_cameras.empty() || m_previousFrameCameraIndex == m_currentCameraIndex)
	{
		return;
	}
	NOTICE_LOG("Switched to camera #%d", m_currentCameraIndex + 1);
	//DEBUG_LOG("%s", m_cameras[m_currentCameraIndex]->ToString().c_str());

	TwRemoveAllVars(m_cameraBar);
	TwAddVarRW(m_cameraBar, "cameraVar", m_cameraType,  m_cameras[m_currentCameraIndex], " label='Camera' group=Camera ");
	char cameraIndexStr[256];
	char cameraDefStr[256];
	_snprintf_s(cameraIndexStr, 256, 255, "camera[%zd].Pos", m_currentCameraIndex);
	_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%zd].Pos' group=Camera ", m_currentCameraIndex);
	TwAddVarRW(m_cameraBar, cameraIndexStr, vector3DType, &m_cameras[m_currentCameraIndex]->GetTransform().GetPos(), cameraDefStr);
	//_snprintf(cameraIndexStr, 255, "camera[%d].Angle", m_currentCameraIndex);
	//_snprintf(cameraDefStr, 255, " label='Camera[%d].Angle' ", m_currentCameraIndex);
	//TwAddVarRW(m_cameraBar, cameraIndexStr, angleType, &tempAngle, cameraDefStr);
	TwDefine(" CamerasBar/Camera opened=true ");

	m_previousFrameCameraIndex = m_currentCameraIndex;
#endif
}
#endif