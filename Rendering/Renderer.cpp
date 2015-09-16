#include "StdAfx.h"
#include "Renderer.h"
#include "GameManager.h"
#include "CoreEngine.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "TextRenderer.h"
//#include "ForwardAmbientShader.h"
//#include "ForwardDirectionalShader.h"
#include "ShadowInfo.h"
#include "MeshRenderer.h"
#include "Utility\IConfig.h"
#include "Utility\ILogger.h"
#include "Utility\FileManager.h"
#include "Utility\FileNotFoundException.h"
#include "Math\FloatingPoint.h"

#include <stddef.h>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace Rendering;
using namespace Utility;
using namespace Math;

/* static */ const Matrix4D Renderer::BIAS_MATRIX(Matrix4D::Scale(0.5f, 0.5f, 0.5f) * Matrix4D::Translation(1.0f, 1.0f, 1.0f));

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
	m_ambientLightFogEnabled(GET_CONFIG_VALUE("ambientLightFogEnabled", true)),
	m_ambientLightFogColor(GET_CONFIG_VALUE("ambientLightFogColor_x", 0.7f),
		GET_CONFIG_VALUE("ambientLightFogColor_y", 0.7f),
		GET_CONFIG_VALUE("ambientLightFogColor_z", 0.7f)),
	m_ambientLightFogStart(GET_CONFIG_VALUE("ambientLightFogStart", 8.0f)),
	m_ambientLightFogEnd(GET_CONFIG_VALUE("ambientLightFogEnd", 50.0f)),
	m_ambientLightFogDensityFactor(GET_CONFIG_VALUE("ambientLightFogDensityFactor", 0.2f)),
	m_ambientLightFogFallOffType(static_cast<FogEffect::FogFallOffType>(GET_CONFIG_VALUE("ambientLightFogFallOffType", 0))),
	m_ambientLightFogCalculationType(static_cast<FogEffect::FogCalculationType>(GET_CONFIG_VALUE("ambientLightFogCalculationType", 0))),
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
	m_altCamera(Matrix4D::Identity(), Transform()),
	m_filterTexture(NULL),
	m_filterMaterial(NULL),
	m_filterTransform(Vector3D(), Quaternion(REAL_ZERO, sqrtf(2)/2, sqrtf(2)/2, REAL_ZERO) /* to make the plane face towards the camera. See "OpenGL Game Rendering Tutorial: Shadow Mapping Preparations" https://www.youtube.com/watch?v=kyjDP68s9vM&index=8&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl (starts around 14:10) */, REAL_ONE),
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
	m_skyboxNode(NULL),
	m_skyboxShader(NULL),
	m_skyboxTextureDay(NULL),
	m_skyboxTextureNight(NULL),
	m_cubeMapShader(NULL),
	m_cubeShadowMap(NULL),
	//m_shadowMaps(), // Gives a compiler warning C4351: new behavior: elements of array will be default initialized
	//m_shadowMapTempTargets(), // Gives a compiler warning C4351: new behavior: elements of array will be default initialized
	m_lights(),
	m_directionalAndSpotLights(),
	m_pointLights(),
	m_cameras(),
	m_samplerMap(),
	m_lightMatrix(Matrix4D::Scale(REAL_ZERO, REAL_ZERO, REAL_ZERO)),
	m_fontTexture(NULL),
	m_textRenderer(NULL)
#ifdef ANT_TWEAK_BAR_ENABLED
	,m_cameraCountMinusOne(0),
	m_previousFrameCameraIndex(0),
	m_propertiesBar(NULL),
	m_cameraBar(NULL),
	m_lightsBar(NULL),
	//m_cameraMembers(), // Gives a compiler warning C4351: new behavior: elements of array will be default initialized
	m_cameraType()
#endif
#ifdef CALCULATE_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("Renderer"))
#endif
{
	INFO_LOG("Creating Renderer instance started");
	START_PROFILING;
	PrintGlReport();
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

	glGenVertexArrays(1, &m_vao);

#ifndef ANT_TWEAK_BAR_ENABLED
	SetVector3D("ambientFogColor", m_ambientLightFogColor);
	SetReal("ambientFogStart", m_ambientLightFogStart);
	SetReal("ambientFogEnd", m_ambientLightFogEnd);
	SetReal("ambientFogDensityFactor", m_ambientLightFogDensityFactor);
	SetVector3D("ambientIntensity", m_ambientLight);
#endif

	//SetTexture("shadowMap",
	//	new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR,
	//		GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true,
	//		GL_COLOR_ATTACHMENT0 /* we're going to render color information */)); // variance shadow mapping
	//SetTexture("shadowMapTempTarget", new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0));
	m_ambientShader = new Shader(GET_CONFIG_VALUE_STR("ambientLightShader", "ForwardAmbient"));
	m_ambientShaderTerrain = new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderTerrain", "forward-ambient-terrain"));
	m_ambientShadersFogEnabledMap[FogEffect::Fog(FogEffect::LINEAR, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogLinearPlaneBased", "forward-ambient-fog-linear-plane-based"));
	m_ambientShadersFogEnabledMap[FogEffect::Fog(FogEffect::EXPONENTIAL, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogExponentialPlaneBased", "forward-ambient-fog-exponential-plane-based"));
	m_ambientShadersFogEnabledMap[FogEffect::Fog(FogEffect::SQUARED_EXPONENTIAL, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogExponentialSquaredPlaneBased", "forward-ambient-fog-exponential-squared-plane-based"));
	m_ambientShadersFogEnabledMap[FogEffect::Fog(FogEffect::LINEAR, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogLinearRangeBased", "forward-ambient-fog-linear-range-based"));
	m_ambientShadersFogEnabledMap[FogEffect::Fog(FogEffect::EXPONENTIAL, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogExponentialRangeBased", "forward-ambient-fog-exponential-range-based"));
	m_ambientShadersFogEnabledMap[FogEffect::Fog(FogEffect::SQUARED_EXPONENTIAL, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogExponentialSquaredRangeBased", "forward-ambient-fog-exponential-squared-range-based"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::Fog(FogEffect::LINEAR, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogLinearPlaneBasedTerrain", "forward-ambient-fog-linear-plane-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::Fog(FogEffect::EXPONENTIAL, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogExponentialPlaneBasedTerrain", "forward-ambient-fog-exponential-plane-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::Fog(FogEffect::SQUARED_EXPONENTIAL, FogEffect::PLANE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogExponentialSquaredPlaneBasedTerrain", "forward-ambient-fog-exponential-squared-plane-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::Fog(FogEffect::LINEAR, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogLinearRangeBasedTerrain", "forward-ambient-fog-linear-range-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::Fog(FogEffect::EXPONENTIAL, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogExponentialRangeBasedTerrain", "forward-ambient-fog-exponential-range-based-terrain"));
	m_ambientShadersFogEnabledTerrainMap[FogEffect::Fog(FogEffect::SQUARED_EXPONENTIAL, FogEffect::RANGE_BASED)] =
		new Shader(GET_CONFIG_VALUE_STR("ambientLightShaderFogExponentialSquaredRangeBasedTerrain", "forward-ambient-fog-exponential-squared-range-based-terrain"));

	m_shadowMapShader = new Shader(GET_CONFIG_VALUE_STR("shadowMapShader", "ShadowMapGenerator"));
	m_nullFilterShader = new Shader(GET_CONFIG_VALUE_STR("nullFilterShader", "Filter-null"));
	m_gaussBlurFilterShader = new Shader(GET_CONFIG_VALUE_STR("gaussBlurFilterShader", "filter-gaussBlur7x1"));
	m_fxaaFilterShader = new Shader(GET_CONFIG_VALUE_STR("fxaaFilterShader", "filter-fxaa"));
	m_altCamera.GetTransform().Rotate(Vector3D(0, 1, 0), Angle(180));

	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	m_filterTexture = new Texture(width, height, NULL, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);

	m_filterMaterial = new Material(m_filterTexture);
	m_filterMesh = new Mesh("..\\Models\\plane4.obj");
	m_filterMesh->Initialize();

	InitializeCubeMap();

	m_cubeMapShader = new Shader(GET_CONFIG_VALUE_STR("cubeShadowMapShader", "CubeShadowMapGenerator"));
	//m_cubeShadowMap = new CubeShadowMapTexture(width, height);
	m_cubeShadowMap = new CubeShadowMap();
	m_cubeShadowMap->Init(width, height);

	for (int i = 0; i < SHADOW_MAPS_COUNT; ++i)
	{
		int shadowMapSize = 1 << (i + 1);
		m_shadowMaps[i] = new Texture(shadowMapSize, shadowMapSize, NULL, GL_TEXTURE_2D, GL_LINEAR,
			GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true, GL_COLOR_ATTACHMENT0 /* we're going to render color information */);
		m_shadowMapTempTargets[i] = new Texture(shadowMapSize, shadowMapSize, NULL, GL_TEXTURE_2D, GL_LINEAR,
			GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true, GL_COLOR_ATTACHMENT0 /* we're going to render color information */);
	}

	m_fontTexture = new Texture("..\\Textures\\Holstein.tga", GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);
	m_textRenderer = new TextRenderer(m_fontTexture);

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
	SAFE_DELETE(m_cubeShadowMap);
	SAFE_DELETE(m_filterMaterial);
	SAFE_DELETE(m_filterMesh);

	// TODO: m_fontTexture uses the same texture as the fontTexture used in CoreEngine class. That's why we shouldn't SAFE_DELETE font texture here.
	// Of course, we should deal with it later on more appropriately.
	//SetTexture("fontTexture", NULL);
	//SAFE_DELETE(m_fontTexture);
	SAFE_DELETE(m_textRenderer);

	for (std::map<FogEffect::Fog, Shader*>::iterator ambientLightFogShadersItr = m_ambientShadersFogEnabledMap.begin();
		ambientLightFogShadersItr != m_ambientShadersFogEnabledMap.end(); ambientLightFogShadersItr++)
	{
		SAFE_DELETE(ambientLightFogShadersItr->second);
	}
	m_ambientShadersFogEnabledMap.clear();
	for (std::map<FogEffect::Fog, Shader*>::iterator ambientLightFogShadersItr = m_ambientShadersFogEnabledTerrainMap.begin();
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
	glfwSetWindowCloseCallback(m_window, &GameManager::WindowCloseEventCallback);
	glfwSetWindowSizeCallback(m_window, GameManager::WindowResizeCallback);
	glfwSetKeyCallback(m_window, &GameManager::KeyEventCallback);
	//glfwSetCharCallback(m_window, GameManager::CharEventCallback);
	//glfwSetMousePosCallback(m_window, GameManager::MouseMotionCallback);
	glfwSetCursorPosCallback(m_window, &GameManager::MousePosCallback);
	glfwSetMouseButtonCallback(m_window, &GameManager::MouseEventCallback);
	glfwSetScrollCallback(m_window, &GameManager::ScrollEventCallback);
}

void Renderer::InitializeCubeMap()
{
	START_PROFILING;
	std::string cubeMapDayDirectory = "..\\Textures\\" + GET_CONFIG_VALUE_STR("skyboxDayDirectory", "SkyboxDebug");
	std::string cubeMapNightDirectory = "..\\Textures\\" + GET_CONFIG_VALUE_STR("skyboxNightDirectory", "SkyboxDebug");
	m_skyboxTextureDay = InitializeCubeMapTexture(cubeMapDayDirectory);
	m_skyboxTextureNight = InitializeCubeMapTexture(cubeMapNightDirectory);

	//SetTexture("cubeMapDay", m_skyboxTextureDay);
	//SetTexture("cubeMapNight", m_skyboxTextureNight);

	Material* cubeMapMaterial = new Material(m_skyboxTextureDay, "cubeMapDay");
	cubeMapMaterial->SetAdditionalTexture(m_skyboxTextureNight, "cubeMapNight");

	m_skyboxNode = new GameNode();
	m_skyboxNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\" + GET_CONFIG_VALUE_STR("skyboxModel", "cube.obj")), cubeMapMaterial));
	m_skyboxNode->GetTransform().SetPos(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	m_skyboxNode->GetTransform().SetScale(5.0f);
	m_skyboxShader = new Shader((GET_CONFIG_VALUE_STR("skyboxShader", "skybox-shader")));
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
	std::vector<std::string> filenames = fileManager.ListAllFilesInDirectory(cubeMapTextureDirectory);
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
	if (!cubeMapPosXFaceFileFound)
	{
		ERROR_LOG("Cannot locate the right face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapNegXFaceFileFound)
	{
		ERROR_LOG("Cannot locate the left face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapPosYFaceFileFound)
	{
		ERROR_LOG("Cannot locate the up face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapNegYFaceFileFound)
	{
		ERROR_LOG("Cannot locate the down face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapPosZFaceFileFound)
	{
		ERROR_LOG("Cannot locate the front face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapNegZFaceFileFound)
	{
		ERROR_LOG("Cannot locate the back face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
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
	CHECK_CONDITION_EXIT_ALWAYS(terrainNode != NULL; Error, "Cannot register terrain node. Given terrain node is NULL.");
	if (m_terrainNode != NULL)
	{
		WARNING_LOG("Replacing already set terrain node with a different one.");
	}
	m_terrainNode = terrainNode;
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

	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Quaternion(Matrix4D::RotationFromDirection(Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Quaternion(Matrix4D::RotationFromDirection(Vector3D(-REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Quaternion(Matrix4D::RotationFromDirection(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Quaternion(Matrix4D::RotationFromDirection(Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE))) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Quaternion(Matrix4D::RotationFromDirection(Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Quaternion(Matrix4D::RotationFromDirection(Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) }
};

void Renderer::Render(const GameNode& gameNode)
{
	START_PROFILING;
	Rendering::CheckErrorCode("Renderer::Render", "Started main render function");
	// TODO: Expand with Stencil buffer once it is used

	AdjustAmbientLightAccordingToCurrentTime();

#ifdef ANT_TWEAK_BAR_ENABLED
	SetVector3D("ambientFogColor", m_ambientLightFogColor);
	SetReal("ambientFogStart", m_ambientLightFogStart);
	SetReal("ambientFogEnd", m_ambientLightFogEnd);
	SetReal("ambientFogDensityFactor", m_ambientLightFogDensityFactor);
	SetVector3D("ambientIntensity", m_ambientLight);
	SetReal("fxaaSpanMax", m_fxaaSpanMax);
	SetReal("fxaaReduceMin", m_fxaaReduceMin);
	SetReal("fxaaReduceMul", m_fxaaReduceMul);
	CheckCameraIndexChange();
#endif
	GetTexture("displayTexture")->BindAsRenderTarget();
	//BindAsRenderTarget();

	ClearScreen();
	if (m_cameras.empty() || m_cameras.at(m_currentCameraIndex) == NULL)
	{
		EMERGENCY_LOG("Rendering failed. There is no proper camera set up.");
		exit(EXIT_FAILURE);
		// TODO: Instead of exit maybe just use the default camera (??)
	}
	m_currentCamera = m_cameras[m_currentCameraIndex];

	RenderSceneWithAmbientLight(gameNode);

	RenderSceneWithPointLights(gameNode);

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
		glClearColor(REAL_ONE /* completely in light */ /* TODO: When at night it should be REAL_ZERO */, REAL_ONE /* we want variance to be also cleared */, REAL_ZERO, REAL_ZERO); // everything is in light (we can clear the COLOR_BUFFER_BIT in the next step)
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		if (/*(m_shadowsEnabled) && */ (shadowInfo != NULL)) // The currentLight casts shadows
		{
			m_altCamera.SetProjection(shadowInfo->GetProjection());
			ShadowCameraTransform shadowCameraTransform = m_currentLight->CalcShadowCameraTransform(m_currentCamera->GetTransform().GetTransformedPos(), m_currentCamera->GetTransform().GetTransformedRot());
			m_altCamera.GetTransform().SetPos(shadowCameraTransform.pos);
			m_altCamera.GetTransform().SetRot(shadowCameraTransform.rot);
			//m_altCamera.GetTransform().SetPos(m_currentLight->GetTransform().GetTransformedPos());
			//m_altCamera.GetTransform().SetRot(m_currentLight->GetTransform().GetTransformedRot());

			m_lightMatrix = BIAS_MATRIX * m_altCamera.GetViewProjection();

			SetReal("shadowLightBleedingReductionFactor", shadowInfo->GetLightBleedingReductionAmount());
			SetReal("shadowVarianceMin", shadowInfo->GetMinVariance());
			bool flipFacesEnabled = shadowInfo->IsFlipFacesEnabled();

			CameraBase* temp = m_currentCamera;
			m_currentCamera = &m_altCamera;

			if (flipFacesEnabled) { glCullFace(GL_FRONT); }
			m_terrainNode->RenderAll(m_shadowMapShader, this);
			gameNode.RenderAll(m_shadowMapShader, this);
			if (flipFacesEnabled) { glCullFace(GL_BACK); }

			m_currentCamera = temp;

			if (m_applyFilterEnabled)
			{
				//ApplyFilter(m_nullFilterShader, GetTexture("shadowMap"), GetTexture("shadowMapTempTarget"));
				//ApplyFilter(m_nullFilterShader, GetTexture("shadowMapTempTarget"), GetTexture("shadowMap"));
				Real shadowSoftness = shadowInfo->GetShadowSoftness();
				if (!AlmostEqual(shadowSoftness, REAL_ZERO))
				{
					BlurShadowMap(shadowMapIndex, shadowSoftness);
				}
			}
		}
		else // current light does not cast shadow or shadowing is disabled at all
		{
			// we set the light matrix this way so that, if no shadow should be cast
			// everything in the scene will be mapped to the same point
			m_lightMatrix = Math::Matrix4D::Scale(REAL_ZERO, REAL_ZERO, REAL_ZERO);
			SetReal("shadowLightBleedingReductionFactor", REAL_ZERO);
			SetReal("shadowVarianceMin", 0.00002f /* do not use hard-coded values */);
		}
		RenderSceneWithLight(m_currentLight, gameNode);

		/* ==================== Rendering to texture begin ==================== */
		//if (renderToTextureTestingEnabled)
		//{
		//	CameraBase* temp = m_currentCamera;
		//	m_currentCamera = &m_altCamera;
		//	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	m_ambientShader->Bind();
		//	m_ambientShader->UpdateUniforms(m_filterTransform, *m_filterMaterial, this);
		//	m_filterMesh->Draw();

		//	m_currentCamera = temp;
		//}
		/* ==================== Rendering to texture end ==================== */
	}
	SetVector3D("inverseFilterTextureSize", Vector3D(REAL_ONE / GetTexture("displayTexture")->GetWidth(), REAL_ONE / GetTexture("displayTexture")->GetHeight(), REAL_ZERO));

	RenderSkybox();

	if (Rendering::antiAliasingMethod == Rendering::FXAA)
	{
		ApplyFilter(m_fxaaFilterShader, GetTexture("displayTexture"), NULL);
	}
	else
	{
		ApplyFilter(m_nullFilterShader, GetTexture("displayTexture"), NULL);
	}
	STOP_PROFILING;
}

void Renderer::RenderSceneWithAmbientLight(const GameNode& gameNode)
{
	START_PROFILING;
	if (m_ambientLightFogEnabled)
	{
		//CRITICAL_LOG("Ambient fog fall-off type: %d. Fog distance calculation type: %d", m_ambientLightFogFallOffType, m_ambientLightFogCalculationType);
		Shader* fogShader = m_ambientShadersFogEnabledMap[FogEffect::Fog(m_ambientLightFogFallOffType, m_ambientLightFogCalculationType)];
		Shader* fogTerrainShader = m_ambientShadersFogEnabledTerrainMap[FogEffect::Fog(m_ambientLightFogFallOffType, m_ambientLightFogCalculationType)];
		CHECK_CONDITION_RETURN_ALWAYS(fogShader != NULL, Utility::Emergency, "Cannot render the scene with ambient light. The fog shader is NULL");
		CHECK_CONDITION_RETURN_ALWAYS(fogTerrainShader != NULL, Utility::Emergency, "Cannot render terrain with ambient light. The terrain fog shader is NULL");
		m_terrainNode->RenderAll(fogTerrainShader, this); // Ambient rendering with fog enabled for terrain node
		gameNode.RenderAll(fogShader, this); // Ambient rendering with fog enabled
	}
	else
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

void Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode)
{
	START_PROFILING;
	glCullFace(Rendering::glCullFaceMode);
	GetTexture("displayTexture")->BindAsRenderTarget();
	if (!Rendering::glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both weights equal to 1
	glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore
	glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image

	m_terrainNode->RenderAll(light->GetTerrainShader(), this);
	gameNode.RenderAll(light->GetShader(), this);

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
	ClearScreen();
	if (m_cameras.empty() || m_cameras.at(m_currentCameraIndex) == NULL)
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
	if (m_cameras.empty() || m_cameras.at(m_currentCameraIndex) == NULL)
	{
		//DELOCUST_LOG("Rendering main menu with a \"main menu camera\".");
		m_currentCamera = m_mainMenuCamera;
	}
	else
	{
		m_currentCamera = m_cameras[m_currentCameraIndex];
	}

	std::stringstream ss;
	int progress = static_cast<int>(loadingProgress * 100);
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

// TODO: Move these two variables to Renderer class
Math::Real skyboxAngle = REAL_ZERO;
Math::Real skyboxAngleStep = 0.02f; // TODO: This variable should be dependant of the clock speed in CoreEngine

void Renderer::RenderSkybox()
{
	START_PROFILING;
	m_skyboxNode->GetTransform().SetPos(m_currentCamera->GetTransform().GetTransformedPos());
	m_skyboxNode->GetTransform().SetRot(Quaternion(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Math::Angle(skyboxAngle)));
	skyboxAngle += skyboxAngleStep;
	if (m_ambientLightFogEnabled)
	{
		STOP_PROFILING;
		return;
	}

	/* ==================== Rendering skybox begin ==================== */
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
	/* ==================== Rendering skybox end ==================== */
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
	if (filterShader == NULL)
	{
		ERROR_LOG("Cannot apply a filter. Filtering shader is NULL.");
		STOP_PROFILING;
		return;
	}
	CHECK_CONDITION_RETURN_ALWAYS(source != NULL, Emergency, "Cannot apply a filter. Source texture is NULL.");
	CHECK_CONDITION_RETURN_ALWAYS(source != dest, Error, "Cannot apply a filter. Both source and destination textures point to the same Texture in memory.");
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

	m_altCamera.SetProjection(Matrix4D::Identity());
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

unsigned int Renderer::NextCamera()
{
	if (m_currentCameraIndex == m_cameras.size() - 1)
	{
		m_currentCameraIndex = -1;
	}
	return SetCurrentCamera(m_currentCameraIndex + 1);
}

unsigned int Renderer::PrevCamera()
{
	if (m_currentCameraIndex == 0)
	{
		m_currentCameraIndex = m_cameras.size();
	}
	return SetCurrentCamera(m_currentCameraIndex - 1);
}

unsigned int Renderer::SetCurrentCamera(unsigned int cameraIndex)
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
		DEBUG_LOG("Directional light with intensity = %.2f is being added to directional / spot lights vector", directionalLight->GetIntensity());
		m_directionalAndSpotLights.push_back(directionalLight);
	}
	else
	{
		Lighting::SpotLight* spotLight = dynamic_cast<Lighting::SpotLight*>(light);
		if (spotLight != NULL)
		{
			DEBUG_LOG("Spot light with intensity = %.2f is being added to directional / spot lights vector", spotLight->GetIntensity());
			m_directionalAndSpotLights.push_back(spotLight);
		}
		else
		{
			Lighting::PointLight* pointLight = dynamic_cast<Lighting::PointLight*>(light);
			if (pointLight != NULL)
			{
				DEBUG_LOG("Point light with intensity = %.2f is being added to point lights vector", pointLight->GetIntensity());
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

void Renderer::PrintGlReport()
{
	INFO_LOG("OpenGL report: Vendor:\"%s\"; Renderer name:\"%s\"; Version:\"%s\"; GLSL version:\"%s\"", (const char*)glGetString(GL_VENDOR),
		(const char*)glGetString(GL_RENDERER), (const char*)glGetString(GL_VERSION), (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	//INFO_LOG("OpenGL extensions: ", (const char*)glGetString(GL_EXTENSIONS));
}

void Renderer::UpdateUniformStruct(const Transform& transform, const Material& material, const Shader& shader, const std::string& uniformName, const std::string& uniformType)
{
	ERROR_LOG("Uniform name \"%s\" of type \"%s\" is not supported by the rendering engine", uniformName.c_str(), uniformType.c_str());
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

#ifdef ANT_TWEAK_BAR_ENABLED
/**
 * This function must be called after AntTweakBar library is initalized.
 * See Rendering::InitializeTweakBars()
 */
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
	TwAddVarRO(m_propertiesBar, "ambientLight", TW_TYPE_COLOR3F, &m_ambientLight, " label='Color' group='Ambient light'");
	TwAddVarRO(m_propertiesBar, "ambientLightDaytime", TW_TYPE_COLOR3F, &m_ambientDaytimeColor, " label='Daytime color' group='Ambient light'");
	TwAddVarRO(m_propertiesBar, "ambientLightSunNearHorizon", TW_TYPE_COLOR3F, &m_ambientSunNearHorizonColor, " label='Sun near horizon color' group='Ambient light'");
	TwAddVarRO(m_propertiesBar, "ambientLightNighttime", TW_TYPE_COLOR3F, &m_ambientNighttimeColor, " label='Nighttime color' group='Ambient light'");
	TwAddVarRW(m_propertiesBar, "ambientLightFogEnabled", TW_TYPE_BOOLCPP, &m_ambientLightFogEnabled, " label='Fog enabled' group='Ambient light' ");
	TwAddVarRW(m_propertiesBar, "ambientLightFogColor", TW_TYPE_COLOR3F, &m_ambientLightFogColor, " label='Fog color' group='Ambient light' ");
	TwAddVarRW(m_propertiesBar, "ambientLightFogStart", TW_TYPE_REAL, &m_ambientLightFogStart, " label='Fog start' group='Ambient light' step=0.5 min=0.5");
	TwAddVarRW(m_propertiesBar, "ambientLightFogEnd", TW_TYPE_REAL, &m_ambientLightFogEnd, " label='Fog end' group='Ambient light' step=0.5 min=1.0");
	TwAddVarRW(m_propertiesBar, "ambientLightFogDensityFactor", TW_TYPE_REAL, &m_ambientLightFogDensityFactor, " label='Fog density factor' group='Ambient light' step=0.02 min=0.01 max=5.0 ");
	TwAddVarRW(m_propertiesBar, "ambientLightFogFallOffType", fogFallOffType, &m_ambientLightFogFallOffType, " label='Fog fall-off type' group='Ambient light' ");
	TwAddVarRW(m_propertiesBar, "ambientLightFogCalculationType", fogCalculationType, &m_ambientLightFogCalculationType, " label='Fog calculation type' group='Ambient light' ");
	TwAddVarRW(m_propertiesBar, "directionalLightsEnabled", TW_TYPE_BOOLCPP, Lighting::DirectionalLight::GetDirectionalLightsEnabled(), " label='Directional light' group=Lights");
	TwAddVarRW(m_propertiesBar, "pointLightsEnabled", TW_TYPE_BOOLCPP, Lighting::PointLight::ArePointLightsEnabled(), " label='Point lights' group=Lights");
	TwAddVarRW(m_propertiesBar, "spotLightsEnabled", TW_TYPE_BOOLCPP, Lighting::SpotLight::GetSpotLightsEnabled(), " label='Spot lights' group=Lights");
	//TwAddVarRW(m_propertiesBar, "shadowsEnabled", TW_TYPE_BOOLCPP, &m_shadowsEnabled, " label='Render shadows' group=Shadows");
	//TwAddVarRW(m_propertiesBar, "pointLightShadowsEnabled", TW_TYPE_BOOLCPP, &m_pointLightShadowsEnabled, " label='Render point light shadows' group=Shadows ");
	TwAddVarRW(m_propertiesBar, "fxaaSpanMax", TW_TYPE_REAL, &m_fxaaSpanMax, " min=0.0 step=0.1 label='Max span' group='FXAA' ");
	TwAddVarRW(m_propertiesBar, "fxaaReduceMin", TW_TYPE_REAL, &m_fxaaReduceMin, " min=0.00001 step=0.000002 label='Min reduce' group='FXAA' ");
	TwAddVarRW(m_propertiesBar, "fxaaReduceMul", TW_TYPE_REAL, &m_fxaaReduceMul, " min=0.0 step=0.01 label='Reduce scale' group='FXAA' ");

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
		_snprintf_s(cameraIndexStr, 256, 255, "camera[%d].Pos", m_currentCameraIndex);
		_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%d].Pos' group=Camera ", m_currentCameraIndex);
		TwAddVarRW(m_cameraBar, cameraIndexStr, vector3DType, &m_cameras[m_currentCameraIndex]->GetTransform().GetPos(), cameraDefStr);
		_snprintf_s(cameraIndexStr, 256, 255, "camera[%d].Rot", m_currentCameraIndex);
		_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%d].Rot' group=Camera ", m_currentCameraIndex);
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
	_snprintf_s(cameraIndexStr, 256, 255, "camera[%d].Pos", m_currentCameraIndex);
	_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%d].Pos' group=Camera ", m_currentCameraIndex);
	TwAddVarRW(m_cameraBar, cameraIndexStr, vector3DType, &m_cameras[m_currentCameraIndex]->GetTransform().GetPos(), cameraDefStr);
	//_snprintf(cameraIndexStr, 255, "camera[%d].Angle", m_currentCameraIndex);
	//_snprintf(cameraDefStr, 255, " label='Camera[%d].Angle' ", m_currentCameraIndex);
	//TwAddVarRW(m_cameraBar, cameraIndexStr, angleType, &tempAngle, cameraDefStr);
	TwDefine(" CamerasBar/Camera opened=true ");

	m_previousFrameCameraIndex = m_currentCameraIndex;
#endif
}
#endif