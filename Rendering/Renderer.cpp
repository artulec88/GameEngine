#include "StdAfx.h"
#include "Renderer.h"
//#include "CoreEngine.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ShadowInfo.h"
#include "Mesh.h"
#include "Utility\IConfig.h"
#include "Utility\ILogger.h"
#include "Utility\FileManager.h"
//#include "Utility\FileNotFoundException.h"
#include "Math\FloatingPoint.h"

#include <stddef.h>
#include <iomanip>
#include <sstream>
#include <algorithm>

/* static */ const Math::Matrix4D Rendering::Renderer::BIAS_MATRIX(Math::Matrix4D(0.5f /* scale matrix */) * Math::Matrix4D(REAL_ONE, REAL_ONE, REAL_ONE /* translation matrix */)); // FIXME: Check matrix multiplication
///* static */ const Matrix4D Renderer::BIAS_MATRIX;
///* static */ const int Renderer::SHADOW_MAPS_COUNT = 11;

Rendering::Renderer::Renderer(int windowWidth, int windowHeight) :
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_applyFilterEnabled(GET_CONFIG_VALUE_RENDERING("applyFilterEnabled", true)),
	m_backgroundColor(GET_CONFIG_VALUE_RENDERING("ClearColorRed", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("ClearColorGreen", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("ClearColorBlue", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("ClearColorAlpha", REAL_ONE)),
	//m_shadowsEnabled(GET_CONFIG_VALUE_RENDERING("shadowsEnabled", true)),
	//m_pointLightShadowsEnabled(GET_CONFIG_VALUE_RENDERING("pointLightShadowsEnabled", false)),
	m_fogEnabled(GET_CONFIG_VALUE_RENDERING("fogEnabled", true)),
	m_fogColor(GET_CONFIG_VALUE_RENDERING("fogColor_x", 0.7f),
		GET_CONFIG_VALUE_RENDERING("fogColor_y", 0.7f),
		GET_CONFIG_VALUE_RENDERING("fogColor_z", 0.7f)),
	m_fogStart(GET_CONFIG_VALUE_RENDERING("fogStart", 8.0f)),
	m_fogEnd(GET_CONFIG_VALUE_RENDERING("fogEnd", 50.0f)),
	m_fogDensityFactor(GET_CONFIG_VALUE_RENDERING("fogDensityFactor", 0.2f)),
	m_fogGradient(GET_CONFIG_VALUE_RENDERING("fogGradient", 0.005f)),
	m_fogFallOffType(static_cast<FogEffect::FogFallOffType>(GET_CONFIG_VALUE_RENDERING("fogFallOffType", 0))),
	m_fogCalculationType(static_cast<FogEffect::FogCalculationType>(GET_CONFIG_VALUE_RENDERING("fogCalculationType", 0))),
	m_ambientLightEnabled(GET_CONFIG_VALUE_RENDERING("ambientLightEnabled", true)),
	m_ambientDaytimeColor(GET_CONFIG_VALUE_RENDERING("ambientDaytimeColorRed", 0.2f),
		GET_CONFIG_VALUE_RENDERING("ambientDaytimeColorGreen", 0.2f),
		GET_CONFIG_VALUE_RENDERING("ambientDaytimeColorBlue", 0.2f)),
	m_ambientSunNearHorizonColor(GET_CONFIG_VALUE_RENDERING("ambientSunNearHorizonColorRed", 0.1f),
		GET_CONFIG_VALUE_RENDERING("ambientSunNearHorizonColorGreen", 0.1f),
		GET_CONFIG_VALUE_RENDERING("ambientSunNearHorizonColorBlue", 0.1f)),
	m_ambientNighttimeColor(GET_CONFIG_VALUE_RENDERING("ambientNighttimeColorRed", 0.02f),
		GET_CONFIG_VALUE_RENDERING("ambientNighttimeColorGreen", 0.02f),
		GET_CONFIG_VALUE_RENDERING("ambientNighttimeColorBlue", 0.02f)),
	m_ambientLight(m_ambientDaytimeColor),
	m_currentLight(NULL),
	m_currentPointLight(NULL),
	m_spotLight(NULL),
	m_currentCameraIndex(0),
	m_currentCamera(NULL),
	m_tempCamera(NULL),
	m_mainMenuCamera(NULL),
	m_altCamera(Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), Math::Matrix4D(), 0.005f),
	m_filterTexture(NULL),
	m_filterMaterial(NULL),
	m_filterTransform(Math::Vector3D(), Math::Quaternion(REAL_ZERO, sqrtf(2.0f) / 2, sqrtf(2.0f) / 2, REAL_ZERO) /* to make the plane face towards the camera. See "OpenGL Game Rendering Tutorial: Shadow Mapping Preparations" https://www.youtube.com/watch?v=kyjDP68s9vM&index=8&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl (starts around 14:10) */, REAL_ONE),
	m_filterMesh(NULL),
	m_shaderFactory(),
	m_fxaaSpanMax(GET_CONFIG_VALUE_RENDERING("fxaaSpanMax", 8.0f)),
	m_fxaaReduceMin(GET_CONFIG_VALUE_RENDERING("fxaaReduceMin", REAL_ONE / 128.0f)),
	m_fxaaReduceMul(GET_CONFIG_VALUE_RENDERING("fxaaReduceMul", REAL_ONE / 8.0f)),
	m_defaultShadowMinVariance(GET_CONFIG_VALUE_RENDERING("defaultShadowMinVariance", 0.00002f)),
	m_cubeShadowMap(NULL),
	//m_shadowMaps(), // Gives a compiler warning C4351: new behavior: elements of array will be default initialized
	//m_shadowMapTempTargets(), // Gives a compiler warning C4351: new behavior: elements of array will be default initialized
	m_directionalLightsCount(0),
	m_lights(),
	m_directionalAndSpotLights(),
	m_pointLights(),
	m_cameras(),
	m_samplerMap(),
	m_lightMatrix(REAL_ZERO /* scale matrix */),
	//m_defaultFont(GET_CONFIG_VALUE_STR_RENDERING("defaultFontTextureAtlas", "segoe.png"), GET_CONFIG_VALUE_STR_RENDERING("defaultFontMetaData", "segoe.fnt")),
	m_fontMaterial(NULL),
	m_defaultFontSize(GET_CONFIG_VALUE_RENDERING("defaultFontSize", 32.0f)),
	m_defaultFontColor(GET_CONFIG_VALUE_RENDERING("defaultTextColorRed", REAL_ONE),
		GET_CONFIG_VALUE_RENDERING("defaultTextColorGreen", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("defaultTextColorBlue", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("defaultTextColorAlpha", REAL_ZERO)),
	m_textVertexBuffer(0),
	m_textTextureCoordBuffer(0),
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
	m_waterDUDVTexture(NULL),
	m_waterNormalMap(NULL),
	m_waterRefractionTexture(NULL),
	m_waterReflectionTexture(NULL),
	m_waterLightReflectionEnabled(false),
	m_waterFresnelEffectFactor(GET_CONFIG_VALUE_RENDERING("waterFresnelEffectFactor", 2.0f)),
	m_waterNormalVerticalFactor(GET_CONFIG_VALUE_RENDERING("waterNormalVerticalFactor", 3.0f)),
	m_particleQuad(NULL),
	m_mappedValues()
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
	m_debugQuad(NULL)
#endif
#ifdef CALCULATE_RENDERING_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("Renderer"))
#endif
{
	START_PROFILING;
	NOTICE_LOG_RENDERING("Creating Renderer instance started");

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

	//SetTexture("shadowMap",
	//	new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR,
	//		GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true,
	//		GL_COLOR_ATTACHMENT0 /* we're going to render color information */)); // variance shadow mapping
	//SetTexture("shadowMapTempTarget", new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0));

	//m_altCamera.GetTransform().Rotate(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Angle(180));

	m_filterTexture = new Texture(windowWidth, windowHeight, NULL, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);

	m_filterMaterial = new Material(m_filterTexture);
	m_filterMesh = new Mesh("plane4.obj");
	m_filterMesh->Initialize();

	//m_cubeShadowMap = new CubeShadowMapTexture(width, height);
	m_cubeShadowMap = new CubeShadowMap();
	m_cubeShadowMap->Init(windowWidth, windowHeight);

	for (int i = 0; i < SHADOW_MAPS_COUNT; ++i)
	{
		int shadowMapSize = 1 << (i + 1); // generates a sequence of numbers: 2, 4, 8, 16, ..., 2^(SHADOW_MAPS_COUNT).
		m_shadowMaps[i] = new Texture(shadowMapSize, shadowMapSize, NULL, GL_TEXTURE_2D, GL_NEAREST,
			GL_RG32F /* 2 components- R and G- for mean and variance */,
			GL_RGBA, true /* we do want clamping so that for the pixels outside the shadow map range we don't return some value from a completely different point in the scene */,
			GL_COLOR_ATTACHMENT0 /* we're going to render color information */);
		m_shadowMapTempTargets[i] = new Texture(shadowMapSize, shadowMapSize, NULL, GL_TEXTURE_2D, GL_NEAREST,
			GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true, GL_COLOR_ATTACHMENT0 /* we're going to render color information */);
	}

	m_fontMaterial = new Material(new Texture("Holstein.tga", GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false));
	//m_fontMaterial = new Material(new Texture("Holstein.tga" /* GET_CONFIG_VALUE_STR_RENDERING("fontTextureAtlas", "Holstein.tga") */, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false));
	glGenBuffers(1, &m_textVertexBuffer);
	glGenBuffers(1, &m_textTextureCoordBuffer);

	m_waterDUDVTexture = new Texture(GET_CONFIG_VALUE_STR_RENDERING("waterDUDVMap", "waterDUDV.png"));
	m_waterNormalMap = new Texture(GET_CONFIG_VALUE_STR_RENDERING("waterNormalMap", "waterNormalMap.png"));
	m_waterReflectionTexture = new Texture(GET_CONFIG_VALUE_RENDERING("waterReflectionTextureWidth", 320), GET_CONFIG_VALUE_RENDERING("waterReflectionTextureHeight", 180), NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RGB, GL_RGBA, false, GL_COLOR_ATTACHMENT0);
	unsigned char* data[] = { NULL, NULL };
	GLfloat filters[] = { GL_LINEAR, GL_LINEAR };
	GLenum internalFormats[] = { GL_RGB, GL_DEPTH_COMPONENT32 };
	GLenum formats[] = { GL_RGBA, GL_DEPTH_COMPONENT };
	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
	m_waterRefractionTexture = new Texture(2, GET_CONFIG_VALUE_RENDERING("waterRefractionTextureWidth", 1280), GET_CONFIG_VALUE_RENDERING("waterRefractionTextureHeight", 720), data, GL_TEXTURE_2D, filters, internalFormats, formats, false, attachments);
	//m_waterRefractionTexture = new Texture(GET_CONFIG_VALUE_RENDERING("waterRefractionTextureWidth", 1280), GET_CONFIG_VALUE_RENDERING("waterRefractionTextureHeight", 720), NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RGB, GL_RGBA, false, GL_COLOR_ATTACHMENT0);

	Math::Vector2D particleVertexPositions[] = { Math::Vector2D(-0.5f, -0.5f), Math::Vector2D(-0.5f, 0.5f), Math::Vector2D(0.5f, -0.5f), Math::Vector2D(0.5f, 0.5f) };
	const int maxParticlesCount = GET_CONFIG_VALUE_RENDERING("maxParticlesCount", 10000);
#ifdef TEXTURE_ATLAS_OFFSET_CALCULATION
	m_particleQuad = new InstanceMesh(particleVertexPositions, 4, maxParticlesCount, 21);
#else
	m_particleQuad = new InstanceMesh(particleVertexPositions, 4, maxParticlesCount, 17);
#endif
	m_particleInstanceVboData.reserve(maxParticlesCount * m_particleQuad->GetInstanceDataLength());

	m_mappedValues.SetTexture("displayTexture", new Texture(windowWidth, windowHeight, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0));
#ifndef ANT_TWEAK_BAR_ENABLED
	SetReal("fxaaSpanMax", m_fxaaSpanMax);
	SetReal("fxaaReduceMin", m_fxaaReduceMin);
	SetReal("fxaaReduceMul", m_fxaaReduceMul);
#endif

	/* ==================== Creating a "Main menu camera" begin ==================== */
	const Math::Real defaultFoV = GET_CONFIG_VALUE_RENDERING("defaultCameraFoV", 70.0f);
	const Math::Real defaultAspectRatio = GET_CONFIG_VALUE_RENDERING("defaultCameraAspectRatio", static_cast<Math::Real>(800) / 600);
	const Math::Real defaultNearPlane = GET_CONFIG_VALUE_RENDERING("defaultCameraNearPlane", 0.1f);
	const Math::Real defaultFarPlane = GET_CONFIG_VALUE_RENDERING("defaultCameraFarPlane", 1000.0f);

	Math::Angle fov(GET_CONFIG_VALUE_RENDERING("mainMenuCameraFoV", defaultFoV), Math::Unit::DEGREE);
	Math::Real aspectRatio = GET_CONFIG_VALUE_RENDERING("mainMenuCameraAspectRatio", defaultAspectRatio);
	Math::Real zNearPlane = GET_CONFIG_VALUE_RENDERING("mainMenuCameraNearPlane", defaultNearPlane);
	Math::Real zFarPlane = GET_CONFIG_VALUE_RENDERING("mainMenuCameraFarPlane", defaultFarPlane);
	m_mainMenuCamera = new Camera(Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Quaternion(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE),
		fov, aspectRatio, zNearPlane, zFarPlane, 0.005f);
	m_currentCamera = m_mainMenuCamera;
	/* ==================== Creating a "Main menu camera" end ==================== */

#ifdef DEBUG_RENDERING_ENABLED
	m_guiTextures.push_back(new GuiTexture("chessboard3.jpg", Math::Vector2D(0.5f, 0.5f), Math::Vector2D(0.25f, 0.25f)));
	//m_guiTextures.push_back(new GuiTexture("crate.jpg", Math::Vector2D(0.45f, 0.45f), Math::Vector2D(0.25f, 0.25f)));
	//m_guiTextures.push_back(new GuiTexture("verdana.png", Math::Vector2D(0.45f, 0.45f), Math::Vector2D(0.25f, 0.25f)));
	Math::Vector2D quadVertexPositions[] = { Math::Vector2D(-REAL_ONE, REAL_ONE), Math::Vector2D(REAL_ONE, REAL_ONE), Math::Vector2D(-REAL_ONE, -REAL_ONE), Math::Vector2D(REAL_ONE, -REAL_ONE) };
	m_debugQuad = new GuiMesh(quadVertexPositions, 4);
#endif

	/* ==================== Initializing physics logger begin ==================== */
	std::string loggingLevel = GET_CONFIG_VALUE_STR_RENDERING("LoggingLevel", "Info");
	Utility::Logging::ILogger::GetLogger("Rendering").Fill(loggingLevel, Utility::Logging::INFO);
	/* ==================== Initializing physics logger end ==================== */

	NOTICE_LOG_RENDERING("Creating Renderer instance finished");
	STOP_PROFILING;
}


Rendering::Renderer::~Renderer(void)
{
	INFO_LOG_RENDERING("Destroying rendering engine...");
	START_PROFILING;

	glDeleteBuffers(1, &m_textVertexBuffer);
	glDeleteBuffers(1, &m_textTextureCoordBuffer);

	//SAFE_DELETE(m_currentLight);
	// TODO: Deallocating the lights member variable
	// TODO: Deallocating the cameras member variable

	//SAFE_DELETE(altCameraNode);
	SAFE_DELETE(m_cubeShadowMap);
	SAFE_DELETE(m_filterMaterial);
	SAFE_DELETE(m_filterMesh);

	// TODO: m_fontTexture uses the same texture as the fontTexture used in CoreEngine class. That's why we shouldn't SAFE_DELETE font texture here.
	// Of course, we should deal with it later on more appropriately.
	//SetTexture("fontTexture", NULL);
	SAFE_DELETE(m_fontMaterial);

	m_mappedValues.SetTexture("waterReflectionTexture", NULL);
	m_mappedValues.SetMultitexture("waterRefractionTexture", NULL, 0);
	m_mappedValues.SetMultitexture("waterDepthMap", NULL, 1);
	m_mappedValues.SetTexture("waterDUDVMap", NULL);
	m_mappedValues.SetTexture("waterNormalMap", NULL);
	SAFE_DELETE(m_waterDUDVTexture);
	SAFE_DELETE(m_waterNormalMap);
	SAFE_DELETE(m_waterRefractionTexture);
	SAFE_DELETE(m_waterReflectionTexture);
	
	SAFE_DELETE(m_particleQuad);
	//for (std::vector<GameNode*>::iterator billboardNodeItr = m_billboardNodes.begin(); billboardNodeItr != m_billboardNodes.end(); ++billboardNodeItr)
	//{
	//	SAFE_DELETE(*billboardNodeItr);
	//}
	//m_billboardNodes.clear();

	m_mappedValues.SetTexture("shadowMap", NULL);
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
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
	TwTerminate(); // Terminate AntTweakBar
#endif

	STOP_PROFILING;
	NOTICE_LOG_RENDERING("Rendering engine destroyed");
}

//void Renderer::AddTerrainNode(GameNode* terrainNode)
//{
//	CHECK_CONDITION_RENDERING(terrainNode != NULL, Utility::ERR, "Cannot register terrain node. Given terrain node is NULL.");
//	m_terrainNodes.push_back(terrainNode);
//}

//void Renderer::AddWaterNode(GameNode* waterNode)
//{
//	CHECK_CONDITION_EXIT_RENDERING(waterNode != NULL, Utility::EMERGENCY, "Adding water node failed. The water node is NULL.");
//	if (m_waterNodes.empty())
//	{
//		INFO_LOG_RENDERING("Adding first water node to the rendering engine. Enabling clipping planes, creating reflection, refraction textures and the water shader.");
//	}
//	m_waterNodes.push_back(waterNode);
//}

//void Renderer::AddBillboardNode(GameNode* billboardNode)
//{
//	CHECK_CONDITION_EXIT_RENDERING(billboardNode != NULL, Utility::EMERGENCY, "Adding billboard node failed. The given billboard node is NULL.");
//	m_billboardNodes.push_back(billboardNode);
//}

//void Renderer::AddSkyboxNode(GameNode* skyboxNode)
//{
//	CHECK_CONDITION_EXIT_RENDERING(skyboxNode != NULL, Utility::EMERGENCY, "Adding skybox node failed. The given skybox node is NULL.");
//	CHECK_CONDITION_RENDERING(m_skyboxNode == NULL, Utility::WARNING, "The currently assigned skybox node is being overwritten");
//	m_skyboxNode = skyboxNode;
//}

/* TODO: Remove in the future */
struct CameraDirection
{
	GLenum cubemapFace;
	Math::Quaternion rotation;
};

CameraDirection gCameraDirections[6 /* number of cube map faces */] =
{
	//{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) },
	//{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Vector3D(-REAL_ONE, REAL_ZERO, REAL_ZERO), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) },
	//{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE) },
	//{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO), Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE) },
	//{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) },
	//{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE), Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO) }

	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Math::Quaternion(Math::Matrix4D(Math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO), Math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Math::Quaternion(Math::Matrix4D(Math::Vector3D(-REAL_ONE, REAL_ZERO, REAL_ZERO), Math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Math::Quaternion(Math::Matrix4D(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Math::Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Math::Quaternion(Math::Matrix4D(Math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO), Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE))) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Math::Quaternion(Math::Matrix4D(Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE), Math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Math::Quaternion(Math::Matrix4D(Math::Vector3D(REAL_ZERO, REAL_ZERO, -REAL_ONE), Math::Vector3D(REAL_ZERO, -REAL_ONE, REAL_ZERO))) }
};

void Rendering::Renderer::InitRenderScene()
{
	START_PROFILING;

	Rendering::CheckErrorCode(__FUNCTION__, "Started scene rendering");

	CHECK_CONDITION_EXIT_RENDERING(!m_cameras.empty() && m_currentCameraIndex >= 0 && m_currentCameraIndex < m_cameras.size() && m_cameras[m_currentCameraIndex] != NULL,
		Utility::EMERGENCY, "Rendering failed. There is no proper camera set up (current camera index = ", m_currentCameraIndex, ")");

#ifdef ANT_TWEAK_BAR_ENABLED
	m_mappedValues.SetVector3D("ambientFogColor", m_fogColor);
	m_mappedValues.SetReal("ambientFogStart", m_fogStart);
	m_mappedValues.SetReal("ambientFogEnd", m_fogEnd);
	m_mappedValues.SetReal("ambientFogDensityFactor", m_fogDensityFactor);
	m_mappedValues.SetReal("ambientFogGradient", m_fogGradient);
	m_mappedValues.SetVector3D("ambientIntensity", m_ambientLight);
	m_mappedValues.SetReal("fxaaSpanMax", m_fxaaSpanMax);
	m_mappedValues.SetReal("fxaaReduceMin", m_fxaaReduceMin);
	m_mappedValues.SetReal("fxaaReduceMul", m_fxaaReduceMul);
	m_mappedValues.SetVector4D("clipPlane", m_defaultClipPlane); // The workaround for some drivers ignoring the glDisable(GL_CLIP_DISTANCE0) method
	CheckCameraIndexChange();
#endif

	STOP_PROFILING;
}

void Rendering::Renderer::BindDisplayTexture()
{
	m_mappedValues.GetTexture("displayTexture")->BindAsRenderTarget();
}

void Rendering::Renderer::BindWaterReflectionTexture()
{
	m_waterReflectionTexture->BindAsRenderTarget();
}

void Rendering::Renderer::BindWaterRefractionTexture()
{
	m_waterRefractionTexture->BindAsRenderTarget();
}

void Rendering::Renderer::InitWaterNodesRendering()
{
	m_mappedValues.SetTexture("waterReflectionTexture", m_waterReflectionTexture);
	m_mappedValues.SetMultitexture("waterRefractionTexture", m_waterRefractionTexture, 0);
	m_mappedValues.SetMultitexture("waterDepthMap", m_waterRefractionTexture, 1);
	m_mappedValues.SetTexture("waterDUDVMap", m_waterDUDVTexture);
	m_mappedValues.SetTexture("waterNormalMap", m_waterNormalMap);
	//m_waterMoveFactor = fmod(m_waterMoveFactor + m_waterWaveSpeed * CoreEngine::GetCoreEngine()->GetClockSpeed(), REAL_ONE);
	m_waterMoveFactor += m_waterWaveSpeed * 1.0f; // Instead, use Core::CoreEngine::GetCoreEngine()->GetClockSpeed();
	if (m_waterMoveFactor > REAL_ONE)
	{
		m_waterMoveFactor -= REAL_ONE;
		CHECK_CONDITION_ALWAYS_RENDERING(m_waterMoveFactor < REAL_ONE, Utility::Logging::ERR, "Water move factor is still greater than 1.0. It is equal to ", m_waterMoveFactor); // TODO: Remove "ALWAYS" in the future
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

void Rendering::Renderer::FinalizeRenderScene()
{
	START_PROFILING;
	m_mappedValues.SetVector3D("inverseFilterTextureSize",
		Math::Vector3D(REAL_ONE / m_mappedValues.GetTexture("displayTexture")->GetWidth(), REAL_ONE / m_mappedValues.GetTexture("displayTexture")->GetHeight(), REAL_ZERO));

#ifdef DEBUG_RENDERING_ENABLED
	RenderDebugNodes();
#endif

	ApplyFilter((Rendering::antiAliasingMethod == Rendering::Aliasing::FXAA) ? m_shaderFactory.GetShader(ShaderTypes::FILTER_FXAA) : m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), m_mappedValues.GetTexture("displayTexture"), NULL);
	Rendering::CheckErrorCode(__FUNCTION__, "Finished scene rendering");
	STOP_PROFILING;
}

void Rendering::Renderer::Render(const Mesh& mesh, const Material* material, const Math::Transform& transform, const Shader& shader) const
{
	//START_PROFILING;
	shader.Bind();
	shader.UpdateUniforms(transform, material, this);
	mesh.Draw();
	//STOP_PROFILING;
}

//void Renderer::SetClippingPlane(const Math::Vector4D& clippingPlane)
//{
//	m_mappedValues.SetVector4D("clipPlane", clippingPlane);
//}

void Rendering::Renderer::EnableWaterReflectionClippingPlane(Math::Real height)
{
	m_waterReflectionClippingPlane.SetW(height);
	m_mappedValues.SetVector4D("clipPlane", m_waterReflectionClippingPlane);
}

void Rendering::Renderer::EnableWaterRefractionClippingPlane(Math::Real height)
{
	m_waterRefractionClippingPlane.SetW(height);
	m_mappedValues.SetVector4D("clipPlane", m_waterRefractionClippingPlane);
}

void Rendering::Renderer::DisableClippingPlanes()
{
	// Now that we rendered the scene into the reflection and refraction textures for the water surface,
	// we want to disable the clipping planes completely. Unfortunately, it seems some drivers simply ignore the
	// below glDisable(GL_CLIP_DISTANCE0) call, so we need a trick. In order to have no clipping we simply
	// set the clipping plane distance from the origin very high. This way there are no fragments that can be culled
	// and as a result we render the whole scene.
	// glDisable(GL_CLIP_DISTANCE0); // Disabled plane clipping // glDisable(GL_CLIP_PLANE0);
	m_mappedValues.SetVector4D("clipPlane", m_defaultClipPlane); // The workaround for some drivers ignoring the glDisable(GL_CLIP_DISTANCE0) method
}

//void Rendering::Renderer::RenderSceneWithPointLights(const GameNode& gameNode)
//{
//	START_PROFILING;
//	if (!Lighting::PointLight::ArePointLightsEnabled())
//	{
//		STOP_PROFILING;
//		return;
//	}
//
//	for (std::vector<Lighting::PointLight*>::iterator pointLightItr = m_pointLights.begin(); pointLightItr != m_pointLights.end(); ++pointLightItr)
//	{
//		m_pointLight = (*pointLightItr);
//		if (!m_pointLight->IsEnabled())
//		{
//			continue;
//		}
//		//if (m_shadowsEnabled && m_pointLightShadowsEnabled)
//		//{
//		glCullFace(GL_FRONT);
//		const int NUMBER_OF_CUBE_MAP_FACES = 6;
//
//		glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX); // TODO: Replace FLT_MAX with REAL_MAX
//		m_altCamera.GetTransform().SetPos(m_pointLight->GetTransform().GetTransformedPos());
//		for (unsigned int i = 0; i < NUMBER_OF_CUBE_MAP_FACES; ++i)
//		{
//			Rendering::CheckErrorCode(__FUNCTION__, "Point light shadow mapping");
//			//DEBUG_LOG_RENDERING("Binding the cube face #", i);
//			m_cubeShadowMap->BindForWriting(gCameraDirections[i].cubemapFace);
//			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//
//			m_altCamera.GetTransform().SetRot(gCameraDirections[i].rotation); // TODO: Set the rotation correctly
//
//			Camera* temp = m_currentCamera;
//			m_currentCamera = &m_altCamera;
//
//			for (std::vector<GameNode*>::const_iterator terrainNodeItr = m_terrainNodes.begin(); terrainNodeItr != m_terrainNodes.end(); ++terrainNodeItr)
//			{
//				(*terrainNodeItr)->Render(m_shaderFactory.GetShader(ShaderTypes::SHADOW_MAP_CUBE), this);
//			}
//			gameNode.Render(m_shaderFactory.GetShader(ShaderTypes::SHADOW_MAP_CUBE), this);
//
//			m_currentCamera = temp;
//		}
//		//}
//
//		RenderSceneWithLight(m_pointLight, gameNode);
//	}
//	m_pointLight = NULL;
//	STOP_PROFILING;
//}

//void Rendering::Renderer::RenderSceneWithLight(Lighting::BaseLight* light, const GameNode& gameNode, bool isCastingShadowsEnabled /* = true */)
//{
//	START_PROFILING;
//	CHECK_CONDITION_EXIT_RENDERING(light != NULL, Utility::EMERGENCY, "Cannot render the scene. The light is NULL.");
//	DEBUG_LOG_RENDERING("Rendering scene with light.");
//	glCullFace(Rendering::glCullFaceMode);
//	GetTexture("displayTexture")->BindAsRenderTarget();
//	if (!Rendering::glBlendEnabled)
//	{
//		glEnable(GL_BLEND);
//	}
//	glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both weights equal to 1
//	glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore. TODO: What if ambient lighting is disabled?
//	glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image
//
//	gameNode.Render(isCastingShadowsEnabled ? light->GetShader() : light->GetNoShadowShader(), this);
//	for (std::vector<GameNode*>::const_iterator terrainNodeItr = m_terrainNodes.begin(); terrainNodeItr != m_terrainNodes.end(); ++terrainNodeItr)
//	{
//		(*terrainNodeItr)->Render(isCastingShadowsEnabled ? light->GetTerrainShader() : light->GetNoShadowTerrainShader(), this);
//	}
//
//	glDepthFunc(Rendering::glDepthTestFunc);
//	glDepthMask(GL_TRUE);
//	if (!Rendering::glBlendEnabled)
//	{
//		glDisable(GL_BLEND);
//	}
//	else
//	{
//		glBlendFunc(Rendering::glBlendSfactor, Rendering::glBlendDfactor);
//	}
//	STOP_PROFILING;
//}

void Rendering::Renderer::RenderText(Text::Alignment alignment, int y, const std::string& str) const
{
	RenderText(alignment, y, str, m_defaultFontSize, m_defaultFontColor);
}

void Rendering::Renderer::RenderText(Text::Alignment alignment, int y, const std::string& str, Math::Real fontSize) const
{
	RenderText(alignment, y, str, fontSize, m_defaultFontColor);
}

void Rendering::Renderer::RenderText(Text::Alignment alignment, int y, const std::string& str, const Math::Vector4D& fontColor) const
{
	RenderText(alignment, y, str, m_defaultFontSize, fontColor);
}

void Rendering::Renderer::RenderText(Text::Alignment alignment, int y, const std::string& str, Math::Real fontSize, const Math::Vector4D& fontColor) const
{
	int x = 0;
	switch (alignment)
	{
	case Text::LEFT:
		x = 0;
		break;
	case Text::RIGHT:
		x = static_cast<int>(m_windowWidth - str.size() * fontSize);
		break;
	case Text::CENTER:
		x = static_cast<int>(m_windowWidth - str.size() * fontSize) / 2;
		DEBUG_LOG_RENDERING("Drawing string \"", str, "\": x = ", x, ", window width = ", m_windowWidth);
		break;
	default:
		WARNING_LOG_RENDERING("Incorrect alignment type used (", alignment, "). The text will start at default position x = ", x);
	}
	RenderText(x, y, str, fontSize, fontColor);
}

void Rendering::Renderer::RenderText(int x, int y, const std::string& str) const
{
	RenderText(x, y, str, m_defaultFontSize, m_defaultFontColor);
}

void Rendering::Renderer::RenderText(int x, int y, const std::string& str, Math::Real fontSize) const
{
	RenderText(x, y, str, fontSize, m_defaultFontColor);
}

void Rendering::Renderer::RenderText(int x, int y, const std::string& str, const Math::Vector4D& fontColor) const
{
	RenderText(x, y, str, m_defaultFontSize, fontColor);
}

void Rendering::Renderer::RenderText(int x, int y, const std::string& str, Math::Real fontSize, const Math::Vector4D& fontColor) const
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started main text rendering function");
	DELOCUST_LOG_RENDERING("Started drawing string \"", str, "\"");

	Rendering::CheckErrorCode("TextRenderer::RenderString", "Started drawing a string");

	std::vector<Math::Vector2D> vertices;
	vertices.reserve(str.size() * 6);
	std::vector<Math::Vector2D> textureCoords;
	textureCoords.reserve(str.size() * 6);
	Math::Real yReal = static_cast<Math::Real>(y);
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		// Our vertices need to be represented in the clipping space, that is why we convert the X and Y components of the vertices
		// below from the screen space coordinates ([0..screenWidth][0..screenHeight]) to clip space coordinates ([-1..1][-1..1]).
		// The conversion is done by first subtracting the half of width / height (for X and Y components respectively) and then dividing the result by the same value.
		const Math::Real leftX = ((2.0f * (x + (i * fontSize) - m_windowWidth)) / m_windowWidth) + 1.0f;
		const Math::Real rightX = ((2.0f * (x + (i * fontSize) + fontSize - m_windowWidth)) / m_windowWidth) + 1.0f;
		const Math::Real bottomY = (2.0f * (m_windowHeight - (yReal + fontSize)) / m_windowHeight) - 1.0f;
		const Math::Real topY = (2.0f * (m_windowHeight - yReal) / m_windowHeight) - 1.0f;
		const Math::Vector2D upLeftVec(leftX, topY);
		const Math::Vector2D upRightVec(rightX, topY);
		const Math::Vector2D downRightVec(rightX, bottomY);
		const Math::Vector2D downLeftVec(leftX, bottomY);
		//CRITICAL_LOG_RENDERING("str = \"", str, "\" upRightVec = ", upRightVec.ToString());

		vertices.push_back(upLeftVec);
		vertices.push_back(downLeftVec);
		vertices.push_back(upRightVec);
		vertices.push_back(downRightVec);
		vertices.push_back(upRightVec);
		vertices.push_back(downLeftVec);

		const Math::Real oneOverSixteen = REAL_ONE / 16.0f;
		int ch = static_cast<int>(str[i]);
		Math::Real xUV = static_cast<Math::Real>(ch % 16) * oneOverSixteen;
		Math::Real yUV = REAL_ONE - ((static_cast<Math::Real>(ch / 16) * oneOverSixteen) + oneOverSixteen);
		//INFO_LOG_RENDERING("character=\"", str[i], "\"\t ascii value=", ch, ", xUV = ", xUV, ", yUV = ", yUV);

		Math::Vector2D upLeftUV(xUV, REAL_ONE - (yUV + oneOverSixteen));
		Math::Vector2D upRightUV(xUV + oneOverSixteen, REAL_ONE - (yUV + oneOverSixteen));
		Math::Vector2D downRightUV(xUV + oneOverSixteen, REAL_ONE - yUV);
		Math::Vector2D downLeftUV(xUV, REAL_ONE - yUV);
		textureCoords.push_back(upLeftUV);
		textureCoords.push_back(downLeftUV);
		textureCoords.push_back(upRightUV);
		textureCoords.push_back(downRightUV);
		textureCoords.push_back(upRightUV);
		textureCoords.push_back(downLeftUV);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Math::Vector2D), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_textTextureCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(Math::Vector2D), &textureCoords[0], GL_STATIC_DRAW);

	m_shaderFactory.GetShader(ShaderTypes::TEXT_SIMPLE).Bind();

	//Updating uniforms
	m_fontMaterial->SetVector4D("textColor", fontColor);
	m_fontMaterial->SetReal("screenWidth", static_cast<Math::Real>(m_windowWidth));
	m_fontMaterial->SetReal("screenHeight", static_cast<Math::Real>(m_windowHeight));
	m_shaderFactory.GetShader(ShaderTypes::TEXT_SIMPLE).UpdateUniforms(Math::Transform() /* In the future the text transform should be given as one of the parameters */, m_fontMaterial, this);
	//textShader->SetUniformMatrix("MVP", Math::Matrix4D(x, y, REAL_ZERO) * projection);
	//fontTexture->Bind(25);
	//textShader->SetUniformi("R_fontTexture", 25);	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_textTextureCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (Rendering::glDepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
	}
	glCullFace(GL_FRONT);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both wages equal to 1
	//glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore
	//glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image
	/**
	* TODO: We should first save the blend-specific parameters and restore them once glDrawArrays function is finished.
	* See how it is done for GL_DEPTH_TEST here, in this function.
	*/
	// Save the current blending state
	if (!Rendering::glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	/**
	* This effectively means:
	* newColorInFramebuffer = currentAlphaInFramebuffer * current color in framebuffer +
	* (1 - currentAlphaInFramebuffer) * shader's output color
	*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
	// Now restore the blending state
	glBlendFunc(Rendering::glBlendSfactor, Rendering::glBlendDfactor);
	if (!Rendering::glBlendEnabled)
	{
		glDisable(GL_BLEND);
	}
	glCullFace(Rendering::glCullFaceMode);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glCullFace(GL_BACK);
	if (Rendering::glDepthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	Rendering::CheckErrorCode(__FUNCTION__, "Finished main text rendering function");
}

//void Rendering::Renderer::RenderText(const Text::GuiTextControl& guiText) const
//{
//	Rendering::CheckErrorCode(__FUNCTION__, "Started main text rendering function");
//	//CRITICAL_LOG_RENDERING("Started drawing string (number of lines = ", guiText.GetLinesCount(), ") at screen position \"", guiText.GetScreenPosition().ToString(), "\"");
//	//glDisable(GL_CULL_FACE);
//	if (Rendering::glDepthTestEnabled)
//	{
//		glDisable(GL_DEPTH_TEST);
//	}
//	if (!Rendering::glBlendEnabled)
//	{
//		glEnable(GL_BLEND);
//	}
//	/**
//	* This effectively means:
//	* newColorInFramebuffer = currentAlphaInFramebuffer * current color in framebuffer +
//	* (1 - currentAlphaInFramebuffer) * shader's output color
//	*/
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	const Shader* textShader = m_shaderFactory.GetShader(ShaderTypes::TEXT);
//	textShader->Bind();
//	textShader->SetUniformVector2D("translation", guiText.GetScreenPosition());
//	textShader->SetUniformVector2D("offset", guiText.GetOffset());
//	textShader->SetUniformVector3D("textColor", guiText.GetColor());
//	textShader->SetUniformVector3D("outlineColor", guiText.GetOutlineColor());
//	textShader->SetUniformf("characterWidth", guiText.GetCharacterWidth());
//	textShader->SetUniformf("characterEdgeTransitionDistance", guiText.GetCharacterEdgeTransitionWidth());
//	textShader->SetUniformf("borderWidth", guiText.GetBorderWidth());
//	textShader->SetUniformf("borderEdgeTransitionDistance", guiText.GetBorderEdgeTransitionWidth());
//	guiText.GetFont()->Bind();
//	textShader->SetUniformi("fontAtlas", 0);
//	
//	guiText.Draw();
//
//	if (Rendering::glDepthTestEnabled)
//	{
//		glEnable(GL_DEPTH_TEST);
//	}
//	if (!Rendering::glBlendEnabled)
//	{
//		glDisable(GL_BLEND);
//	}
//	//glEnable(GL_CULL_FACE);
//	Rendering::CheckErrorCode(__FUNCTION__, "Finished main text rendering function");
//}

void Rendering::Renderer::RenderGuiControl(const Controls::GuiControl& guiControl) const
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started main GUI control rendering function");
	//CRITICAL_LOG_RENDERING("Started drawing GUI control at screen position \"", guiControl.GetScreenPosition().ToString(), "\"");
	//glDisable(GL_CULL_FACE);
	if (Rendering::glDepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
	}
	if (!Rendering::glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	/**
	* This effectively means:
	* newColorInFramebuffer = currentAlphaInFramebuffer * current color in framebuffer +
	* (1 - currentAlphaInFramebuffer) * shader's output color
	*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	guiControl.Draw(*this);

	if (Rendering::glDepthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (!Rendering::glBlendEnabled)
	{
		glDisable(GL_BLEND);
	}
	//glEnable(GL_CULL_FACE);
	Rendering::CheckErrorCode(__FUNCTION__, "Finished main text rendering function");
}

void Rendering::Renderer::RenderParticles(const ParticleTexture* particleTexture, const Particle* particles, size_t particlesCount) const
{
	START_PROFILING;
	Rendering::CheckErrorCode(__FUNCTION__, "Started particles rendering");
	//CHECK_CONDITION_ALWAYS_RENDERING(particlesCount <= particles.size(), Utility::ERR,
	//	"The number of alive particles (", particlesCount, ") exceeds the size of the specified vector of particles (", particles.size(), ")");
	if (particlesCount <= 0)
	{
		return;
	}
	//DEBUG_LOG_RENDERING("Rendering particles started. There are ", particlesCount, " particles currently in the game.");
	const Shader& particleShader = m_shaderFactory.GetShader(ShaderTypes::PARTICLES);
	particleShader.Bind(); // TODO: This can be performed once and not each time we call this function (during one render-pass of course).
	particleTexture->Bind();
	particleShader.SetUniformi("particleTexture", 0);
	particleShader.SetUniformf("textureAtlasRowsCount", static_cast<Math::Real>(particleTexture->GetRowsCount()));
	if (Rendering::glDepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
	}
	if (!Rendering::glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	glBlendFunc(GL_SRC_ALPHA, particleTexture->IsAdditive() ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
	
	m_particleInstanceVboData.clear();
	const Math::Matrix4D cameraViewMatrix = m_currentCamera->GetViewMatrix();
	for (int i = 0; i < particlesCount; ++i)
	{
		Math::Matrix4D modelMatrix(particles[i].GetPosition());
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
		
		Math::Quaternion particleRotation(Math::Vector3D(0.0f, 0.0f, 1.0f), particles[i].GetRotation());
		modelMatrix = modelMatrix * particleRotation.ToRotationMatrix() * Math::Matrix4D(particles[i].GetScale());
		
		Math::Matrix4D mvpMatrix = m_currentCamera->GetViewProjection() * modelMatrix;
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
		Math::Vector2D textureOffset0;
		Math::Vector2D textureOffset1;
		Math::Real textureAtlasBlendFactor;
		particles[i].CalculateTextureAtlasInfo(particleTexture->GetRowsCount(), textureOffset0, textureOffset1, textureAtlasBlendFactor);
		m_particleInstanceVboData.push_back(textureOffset0.GetX());
		m_particleInstanceVboData.push_back(textureOffset0.GetY());
		m_particleInstanceVboData.push_back(textureOffset1.GetX());
		m_particleInstanceVboData.push_back(textureOffset1.GetY());
		m_particleInstanceVboData.push_back(textureAtlasBlendFactor);
#else
		m_particleInstanceVboData.push_back(particles[i].CalculateLifeStageFactor());
#endif
	}
	m_particleQuad->Draw(&m_particleInstanceVboData[0], m_particleInstanceVboData.size(), particlesCount);
	if (Rendering::glDepthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (!Rendering::glBlendEnabled)
	{
		glDisable(GL_BLEND);
	}
	Rendering::CheckErrorCode(__FUNCTION__, "Finished particles rendering");
	STOP_PROFILING;
}

void Rendering::Renderer::RenderLoadingScreen(Math::Real loadingProgress) const
{
	START_PROFILING;
	BindAsRenderTarget();
	ClearScreen();
	//if (m_cameras.empty() || m_cameras[m_currentCameraIndex] == NULL /* TODO: Check if m_currentCameraIndex is within correct range */)
	//{
	//	//DELOCUST_LOG_RENDERING("Rendering main menu with a \"main menu camera\".");
	//	m_currentCamera = m_mainMenuCamera;
	//}
	//else
	//{
	//	m_currentCamera = m_cameras[m_currentCameraIndex];
	//}

	std::stringstream ss;
	int progress = static_cast<int>(loadingProgress * 100.0f);
	ss << progress << "%";
	RenderText(Text::CENTER, 350, "Loading...");
	RenderText(Text::CENTER, 250, ss.str());
	STOP_PROFILING;
}

bool Rendering::Renderer::InitShadowMap()
{
	const ShadowInfo* shadowInfo = m_currentLight->GetShadowInfo();
	int shadowMapIndex = (shadowInfo == NULL) ? 0 : shadowInfo->GetShadowMapSizeAsPowerOf2() - 1;
	CHECK_CONDITION_EXIT_RENDERING(shadowMapIndex < SHADOW_MAPS_COUNT, ERR, "Incorrect shadow map size. Shadow map index must be an integer from range [0; ", SHADOW_MAPS_COUNT, "), but equals ", shadowMapIndex, ".");
	m_mappedValues.SetTexture("shadowMap", m_shadowMaps[shadowMapIndex]); // TODO: Check what would happen if we didn't set texture here?
	m_shadowMaps[shadowMapIndex]->BindAsRenderTarget();
	ClearScreen(Color(REAL_ONE /* completely in light */ /* TODO: When at night it should be REAL_ZERO */, REAL_ONE /* we want variance to be also cleared */, REAL_ZERO, REAL_ZERO)); // everything is in light (we can clear the COLOR_BUFFER_BIT)

	if ( /* (m_shadowEnabled) && */ (shadowInfo != NULL))
	{
		m_altCamera.SetProjection(shadowInfo->GetProjection());
		ShadowCameraTransform shadowCameraTransform = m_currentLight->CalcShadowCameraTransform(m_currentCamera->GetPos(), m_currentCamera->GetRot());
		m_altCamera.SetPos(shadowCameraTransform.m_pos);
		m_altCamera.SetRot(shadowCameraTransform.m_rot);

		//CRITICAL_LOG_RENDERING("AltCamera.GetViewProjection() = \"", m_altCamera.GetViewProjection().ToString(), "\"");
		m_lightMatrix = BIAS_MATRIX * m_altCamera.GetViewProjection(); // FIXME: Check matrix multiplication
		m_mappedValues.SetReal("shadowLightBleedingReductionFactor", shadowInfo->GetLightBleedingReductionAmount());
		m_mappedValues.SetReal("shadowVarianceMin", shadowInfo->GetMinVariance());

		m_tempCamera = m_currentCamera;
		m_currentCamera = &m_altCamera;

		if (shadowInfo->IsFlipFacesEnabled())
		{
			glCullFace(GL_FRONT);
		}
		return true; // shadows enabled
	}
	else // current light does not cast shadow or shadowing is disabled at all
	{
		// we set the light matrix this way so that, if no shadow should be cast
		// everything in the scene will be mapped to the same point
		m_lightMatrix.SetScaleMatrix(REAL_ZERO, REAL_ZERO, REAL_ZERO);
		m_mappedValues.SetReal("shadowLightBleedingReductionFactor", REAL_ZERO);
		m_mappedValues.SetReal("shadowVarianceMin", m_defaultShadowMinVariance);
		return false; // shadows disabled
	}
}

void Rendering::Renderer::FinalizeShadowMapRendering()
{
	const ShadowInfo* shadowInfo = m_currentLight->GetShadowInfo();
	if (shadowInfo != NULL)
	{
		int shadowMapIndex = shadowInfo->GetShadowMapSizeAsPowerOf2() - 1;
		if (shadowInfo->IsFlipFacesEnabled())
		{
			glCullFace(GL_BACK);
		}

		m_currentCamera = m_tempCamera;

		if (m_applyFilterEnabled)
		{
			//ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), GetTexture("shadowMap"), GetTexture("shadowMapTempTarget"));
			//ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_NULL), GetTexture("shadowMapTempTarget"), GetTexture("shadowMap"));
			if (!Math::AlmostEqual(shadowInfo->GetShadowSoftness(), REAL_ZERO))
			{
				BlurShadowMap(shadowMapIndex, shadowInfo->GetShadowSoftness());
			}
		}
	}
}

const Rendering::Shader& Rendering::Renderer::GetAmbientShader() const
{
	START_PROFILING;
	if (m_fogEnabled)
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);
		
		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		if (m_fogFallOffType == FogEffect::LINEAR)
		{
			if (m_fogCalculationType == FogEffect::PLANE_BASED)
			{
				STOP_PROFILING;
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_FOG_LINEAR_PLANE_BASED);
			}
			else if (m_fogCalculationType == FogEffect::RANGE_BASED)
			{
				STOP_PROFILING;
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_FOG_LINEAR_RANGE_BASED);
			}
		}
		else if (m_fogFallOffType == FogEffect::EXPONENTIAL)
		{
			if (m_fogCalculationType == FogEffect::PLANE_BASED)
			{
				STOP_PROFILING;
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_PLANE_BASED);
			}
			else if (m_fogCalculationType == FogEffect::RANGE_BASED)
			{
				STOP_PROFILING;
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_FOG_EXPONENTIAL_RANGE_BASED);
			}
		}
	}
	else if (m_ambientLightEnabled)
	{
		STOP_PROFILING;
		return m_shaderFactory.GetShader(ShaderTypes::AMBIENT);
	}
	WARNING_LOG_RENDERING("Correct ambient shader cannot be determined. Returning the standard ambient shader.");
	STOP_PROFILING;
	return m_shaderFactory.GetShader(ShaderTypes::AMBIENT);
}

const Rendering::Shader& Rendering::Renderer::GetAmbientTerrainShader() const
{
	START_PROFILING;
	if (m_fogEnabled)
	{
		//DEBUG_LOG_RENDERING("Fog fall-off type: ", m_fogFallOffType, ". Fog distance calculation type: ", m_fogCalculationType);
		// TODO: A very ugly way. If we decide to add more fog fall off or calculation types then we will surely have a big problem in here.
		if (m_fogFallOffType == FogEffect::LINEAR)
		{
			if (m_fogCalculationType == FogEffect::PLANE_BASED)
			{
				STOP_PROFILING;
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED);
			}
			else if (m_fogCalculationType == FogEffect::RANGE_BASED)
			{
				STOP_PROFILING;
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED);
			}
		}
		else if (m_fogFallOffType == FogEffect::EXPONENTIAL)
		{
			if (m_fogCalculationType == FogEffect::PLANE_BASED)
			{
				STOP_PROFILING;
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED);
			}
			else if (m_fogCalculationType == FogEffect::RANGE_BASED)
			{
				STOP_PROFILING;
				return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED);
			}
		}
	}
	else if (m_ambientLightEnabled)
	{
		STOP_PROFILING;
		return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN);
	}
	WARNING_LOG_RENDERING("Correct ambient terrain shader cannot be determined. Returning the standard ambient terrain shader.");
	STOP_PROFILING;
	return m_shaderFactory.GetShader(ShaderTypes::AMBIENT_TERRAIN);
}

void Rendering::Renderer::AdjustAmbientLightAccordingToCurrentTime(Utility::Timing::Daytime dayTime, Math::Real dayTimeTransitionFactor)
{
	START_PROFILING;
	/* ==================== Adjusting the time variables begin ==================== */

	Math::Real dayNightMixFactor;

	switch (dayTime)
	{
	case Utility::Timing::NIGHT:
		dayNightMixFactor = REAL_ZERO;
		m_ambientLight = m_ambientNighttimeColor;
		break;
	case Utility::Timing::BEFORE_DAWN:
		dayNightMixFactor = REAL_ZERO;
		m_ambientLight = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, dayTimeTransitionFactor); // move copy assignment
		break;
	case Utility::Timing::SUNRISE:
		dayNightMixFactor = dayTimeTransitionFactor;
		m_ambientLight = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, dayTimeTransitionFactor); // move copy assignment
		break;
	case Utility::Timing::DAY:
		dayNightMixFactor = REAL_ONE;
		m_ambientLight = m_ambientDaytimeColor;
		break;
	case Utility::Timing::SUNSET:
		dayNightMixFactor = dayTimeTransitionFactor;
		m_ambientLight = m_ambientSunNearHorizonColor.Lerp(m_ambientDaytimeColor, dayTimeTransitionFactor); // move copy assignment
		break;
	case Utility::Timing::AFTER_DUSK:
		dayNightMixFactor = REAL_ZERO;
		m_ambientLight = m_ambientNighttimeColor.Lerp(m_ambientSunNearHorizonColor, dayTimeTransitionFactor); // move copy assignment
		break;
	}

	m_mappedValues.SetReal("dayNightMixFactor", dayNightMixFactor);
	/* ==================== Adjusting the time variables end ==================== */
	STOP_PROFILING;
}

//void Rendering::Renderer::RenderSkybox()
//{
//	START_PROFILING;
//	m_skyboxNode->GetTransform().SetPos(m_currentCamera->GetTransform().GetTransformedPos());
//	m_skyboxNode->GetTransform().SetRot(Quaternion(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), m_skyboxAngle));
//	m_skyboxAngle += m_skyboxAngleStep;
//	if (m_fogEnabled)
//	{
//		STOP_PROFILING;
//		return;
//	}
//
//	//glDisable(GL_DEPTH_TEST);
//	glCullFace(GL_FRONT);
//	/**
//	 * By default (GL_LESS) we tell OpenGL that an incoming fragment wins the depth test if its Z value is less than the stored one.
//	 * However, in the case of a skybox the Z value is always the far Z. The far Z is clipped when the depth test function is set to "less than".
//	 * To make it part of the scene we change the depth function to "less than or equal".
//	 */
//	glDepthFunc(GL_LEQUAL);
//	m_skyboxNode->Render(m_shaderFactory.GetShader(ShaderTypes::SKYBOX), this);
//	glDepthFunc(Rendering::glDepthTestFunc);
//	glCullFace(Rendering::glCullFaceMode);
//	//glEnable(GL_DEPTH_TEST);
//	Rendering::CheckErrorCode("Renderer::Render", "Rendering skybox");
//	STOP_PROFILING;
//}

void Rendering::Renderer::BlurShadowMap(int shadowMapIndex, Math::Real blurAmount /* how many texels we move per sample */)
{
	START_PROFILING;
	Texture* shadowMap = m_shadowMaps[shadowMapIndex];
	Texture* shadowMapTempTarget = m_shadowMapTempTargets[shadowMapIndex];
	if (shadowMap == NULL)
	{
		ERROR_LOG_RENDERING("Shadow map ", shadowMapIndex, " is NULL. Cannot perform the blurring process.");
		STOP_PROFILING;
		return;
	}
	if (shadowMapTempTarget == NULL)
	{
		ERROR_LOG_RENDERING("Temporary shadow map target ", shadowMapIndex, " is NULL. Cannot perform the blurring process.");
		STOP_PROFILING;
		return;
	}

	m_mappedValues.SetVector3D("blurScale", Math::Vector3D(blurAmount / shadowMap->GetWidth(), REAL_ZERO, REAL_ZERO));
	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_GAUSSIAN_BLUR), shadowMap, shadowMapTempTarget);
	
	m_mappedValues.SetVector3D("blurScale", Math::Vector3D(REAL_ZERO, blurAmount / shadowMap->GetHeight(), REAL_ZERO));
	ApplyFilter(m_shaderFactory.GetShader(ShaderTypes::FILTER_GAUSSIAN_BLUR), shadowMapTempTarget, shadowMap);
	STOP_PROFILING;
}

// You cannot read and write from the same texture at the same time. That's why we use dest texture as a temporary texture to store the result
void Rendering::Renderer::ApplyFilter(const Shader& filterShader, const Texture* source, const Texture* dest)
{
	START_PROFILING;
	CHECK_CONDITION_EXIT_RENDERING(filterShader != NULL, Utility::CRITICAL, "Cannot apply a filter. Filtering shader is NULL.");
	CHECK_CONDITION_EXIT_RENDERING(source != NULL, Utility::CRITICAL, "Cannot apply a filter. Source texture is NULL.");
	CHECK_CONDITION_EXIT_RENDERING(source != dest, Utility::CRITICAL, "Cannot apply a filter. Both source and destination textures point to the same location in memory.");
	if (dest == NULL)
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

	m_altCamera.SetProjection(Math::Matrix4D::IDENTITY_MATRIX);
	m_altCamera.SetPos(Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO));
	m_altCamera.SetRot(Math::Quaternion(Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Math::Angle(180.0f)));

	Camera* temp = m_currentCamera;
	m_currentCamera = &m_altCamera;

	glClear(GL_DEPTH_BUFFER_BIT);
	filterShader.Bind();
	filterShader.UpdateUniforms(m_filterTransform, m_filterMaterial, this);
	m_filterMesh->Draw();

	m_currentCamera = temp;
	m_mappedValues.SetTexture("filterTexture", NULL);
	STOP_PROFILING;
}

size_t Rendering::Renderer::NextCamera()
{
	if (m_currentCameraIndex == m_cameras.size() - 1)
	{
		m_currentCameraIndex = -1;
	}
	return SetCurrentCamera(m_currentCameraIndex + 1);
}

size_t Rendering::Renderer::PrevCamera()
{
	if (m_currentCameraIndex == 0)
	{
		m_currentCameraIndex = m_cameras.size();
	}
	return SetCurrentCamera(m_currentCameraIndex - 1);
}

//void Rendering::Renderer::SetMenuCameraAsCurrent()
//{
//	m_currentCamera = m_mainMenuCamera;
//}

void Rendering::Renderer::SetCurrentCamera()
{
	m_currentCamera = m_cameras[m_currentCameraIndex];
}

size_t Rendering::Renderer::SetCurrentCamera(size_t cameraIndex)
{
	CHECK_CONDITION_RENDERING((cameraIndex >= 0) && (cameraIndex < m_cameras.size()), ERR, "Incorrect current camera index. Passed ", cameraIndex, " when the correct range is (", 0, ", ", m_cameras.size(), ").");
	m_cameras[m_currentCameraIndex]->Deactivate();
	m_currentCameraIndex = cameraIndex;
	m_cameras[m_currentCameraIndex]->Activate();
#ifndef ANT_TWEAK_BAR_ENABLED
	NOTICE_LOG_RENDERING("Switched to camera #", m_currentCameraIndex + 1);
	//DEBUG_LOG_RENDERING("Current camera parameters: ", m_cameras[m_currentCameraIndex]->ToString());
#endif
	return m_currentCameraIndex;
}

void Rendering::Renderer::AddLight(Lighting::BaseLight* light)
{
	Lighting::DirectionalLight* directionalLight = dynamic_cast<Lighting::DirectionalLight*>(light);
	if (directionalLight != NULL)
	{
		INFO_LOG_RENDERING("Directional light with intensity = ", directionalLight->GetIntensity(), " is being added to directional / spot lights vector");
		m_waterLightReflectionEnabled = true;
		++m_directionalLightsCount;
		m_directionalAndSpotLights.push_back(directionalLight);
	}
	else
	{
		Lighting::SpotLight* spotLight = dynamic_cast<Lighting::SpotLight*>(light);
		if (spotLight != NULL)
		{
			INFO_LOG_RENDERING("Spot light with intensity = ", spotLight->GetIntensity(), " is being added to directional / spot lights vector");
			m_directionalAndSpotLights.push_back(spotLight);
		}
		else
		{
			Lighting::PointLight* pointLight = dynamic_cast<Lighting::PointLight*>(light);
			if (pointLight != NULL)
			{
				INFO_LOG_RENDERING("Point light with intensity = ", pointLight->GetIntensity(), " is being added to point lights vector");
				m_pointLights.push_back(pointLight);
			}
			else
			{
				EMERGENCY_LOG_RENDERING("Adding the light of unknown type. It is neither a directional nor spot nor point light.");
			}
		}
	}
	m_lights.push_back(light);
}

void Rendering::Renderer::AddCamera(Camera* camera)
{
	if (m_cameras.empty())
	{
		camera->Activate();
	}
	m_cameras.push_back(camera);
#ifdef ANT_TWEAK_BAR_ENABLED
	++m_cameraCountMinusOne;
#endif
}

void Rendering::Renderer::BindAsRenderTarget() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, m_windowWidth, m_windowHeight);
}

void Rendering::Renderer::InitLightRendering() const
{
	glCullFace(Rendering::glCullFaceMode);
	m_mappedValues.GetTexture("displayTexture")->BindAsRenderTarget();
	if (!Rendering::glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both weights equal to 1
	glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore. TODO: What if ambient lighting is disabled?
	glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image
}

void Rendering::Renderer::FinalizeLightRendering() const
{
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
}

void Rendering::Renderer::BindCubeShadowMap(unsigned int textureUnit) const
{
	m_cubeShadowMap->BindForReading(textureUnit);
}

#ifdef DEBUG_RENDERING_ENABLED
void Rendering::Renderer::RenderDebugNodes()
{
	const Shader& guiShader = m_shaderFactory.GetShader(ShaderTypes::GUI);
	guiShader.Bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	for (std::vector<GuiTexture*>::const_iterator guiTextureItr = m_guiTextures.begin(); guiTextureItr != m_guiTextures.end(); ++guiTextureItr)
	{
		(*guiTextureItr)->Bind(0);
		guiShader.SetUniformMatrix("guiTransformationMatrix", (*guiTextureItr)->GetTransformationMatrix());
		guiShader.SetUniformi("guiTexture", 0);
		m_debugQuad->Draw();
	}
	//Math::Matrix4D transformationMatrix1(Math::Vector2D(0.74f, 0.74f), Math::Vector2D(0.25f, 0.25f));
	//m_shadowMaps[9]->Bind();
	//m_waterReflectionTexture->Bind();
	//guiShader->SetUniformMatrix("guiTransformationMatrix", transformationMatrix1);
	//guiShader->SetUniformi("guiTexture", 0);
	//m_debugQuad->Draw();

	//Math::Matrix4D transformationMatrix2(Math::Vector2D(0.74f, -0.24f), Math::Vector2D(0.25f, 0.25f));
	//m_waterRefractionTexture->Bind();
	//guiShader->SetUniformMatrix("guiTransformationMatrix", transformationMatrix2);
	//m_debugQuad->Draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Rendering::Renderer::AddLine(const Math::Vector3D& fromPosition, const Math::Vector3D& toPosition, const Color& color,
	Math::Real lineWidth /* = REAL_ONE */, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug line rendering is not yet supported by the engine");
}

void Rendering::Renderer::AddSphere(const Math::Sphere& sphere, const Color& color,
	Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug sphere rendering is not yet supported by the engine");
}

void Rendering::Renderer::AddCross(const Math::Vector3D& position, const Color& color, Math::Real size,
	Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug cross rendering is not yet supported by the engine");
}

void Rendering::Renderer::AddCircle(const Math::Vector3D& centerPosition, const Math::Vector3D& planeNormal, Math::Real radius, const Color& color,
	Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug circle rendering is not yet supported by the engine");
}

void Rendering::Renderer::AddAxes(const Math::Transform& transform, const Color& color, Math::Real size,
	Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug axes rendering is not yet supported by the engine");
}

void Rendering::Renderer::AddTriangle(const Math::Vector3D& v0, const Math::Vector3D& v1, const Math::Vector3D& v2, const Color& color,
	Math::Real lineWidth /* = REAL_ONE */, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug triangle rendering is not yet supported by the engine");
}

void Rendering::Renderer::AddAABB(const Math::AABB& aabb, const Color& color, Math::Real lineWidth /* = REAL_ONE */, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug AABB rendering is not yet supported by the engine");
}

void Rendering::Renderer::AddOBB(const Math::OBB& obb, const Color& color, Math::Real lineWidth /* = REAL_ONE */, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug OBB rendering is not yet supported by the engine");
}

void Rendering::Renderer::AddString(const Math::Vector3D& pos, const char* text, const Color& color, Math::Real duration /* = REAL_ZERO */, bool isDepthTestEnabled /* = true */)
{
	WARNING_LOG_RENDERING("Debug text rendering is not yet supported by the engine");
}
#endif

#ifdef ANT_TWEAK_BAR_ENABLED
void Rendering::Renderer::InitializeTweakBars()
{
	INFO_LOG_RENDERING("Initializing rendering engine's tweak bars");

#ifdef RENDERER_PROPERTIES_BAR
	DEBUG_LOG_RENDERING("Initializing rendering engine's properties tweak bar");
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
	TwAddVarRW(m_propertiesBar, "fogDensityFactor", TW_TYPE_REAL, &m_fogDensityFactor, " label='Density factor' group='Fog' step=0.0002 min=0.0002 max=2.0 ");
	TwAddVarRW(m_propertiesBar, "fogGradient", TW_TYPE_REAL, &m_fogGradient, " label='Gradient' group='Fog' step=0.1 min=0.1 max=20.0 ");
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
	DEBUG_LOG_RENDERING("Initializing rendering engine's properties tweak bar finished");
#endif

#ifdef CAMERA_TWEAK_BAR
	DEBUG_LOG_RENDERING("Initializing rendering engine's cameras tweak bar");
	m_cameraBar = TwNewBar("CamerasBar");
	if (m_cameras.empty() || m_cameras[m_currentCameraIndex] == NULL)
	{
		ERROR_LOG_RENDERING("Cannot properly initialize rendering engine's cameras bar. No cameras are setup by the game manager.");
		
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
		TwAddVarRW(m_cameraBar, cameraIndexStr, vector3DType, &m_cameras[m_currentCameraIndex]->GetPos(), cameraDefStr);
		_snprintf_s(cameraIndexStr, 256, 255, "camera[%zd].Rot", m_currentCameraIndex);
		_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%zd].Rot' group=Camera ", m_currentCameraIndex);
		TwAddVarRW(m_cameraBar, cameraIndexStr, TW_TYPE_QUAT4F, &m_cameras[m_currentCameraIndex]->GetRot(), cameraDefStr);
	}
	
	TwDefine(" CamerasBar/Camera opened=true ");
	TwSetParam(m_cameraBar, NULL, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup
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

void Rendering::Renderer::CheckCameraIndexChange()
{
#ifdef CAMERA_TWEAK_BAR
	if (m_cameras.empty() || m_previousFrameCameraIndex == m_currentCameraIndex)
	{
		return;
	}
	NOTICE_LOG_RENDERING("Switched to camera #", m_currentCameraIndex + 1);
	//DEBUG_LOG_RENDERING("Current camera parameters: ", m_cameras[m_currentCameraIndex]->ToString());

	TwRemoveAllVars(m_cameraBar);
	TwAddVarRW(m_cameraBar, "cameraVar", m_cameraType,  m_cameras[m_currentCameraIndex], " label='Camera' group=Camera ");
	char cameraIndexStr[256];
	char cameraDefStr[256];
	_snprintf_s(cameraIndexStr, 256, 255, "camera[%zd].Pos", m_currentCameraIndex);
	_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%zd].Pos' group=Camera ", m_currentCameraIndex);
	TwAddVarRW(m_cameraBar, cameraIndexStr, vector3DType, &m_cameras[m_currentCameraIndex]->GetPos(), cameraDefStr);
	//_snprintf(cameraIndexStr, 255, "camera[%d].Angle", m_currentCameraIndex);
	//_snprintf(cameraDefStr, 255, " label='Camera[%d].Angle' ", m_currentCameraIndex);
	//TwAddVarRW(m_cameraBar, cameraIndexStr, angleType, &tempAngle, cameraDefStr);
	TwDefine(" CamerasBar/Camera opened=true ");

	m_previousFrameCameraIndex = m_currentCameraIndex;
#endif
}
#endif