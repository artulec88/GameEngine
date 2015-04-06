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
	cameraCount(-1),
	applyFilterEnabled(GET_CONFIG_VALUE("applyFilterEnabled", true)),
	backgroundColor(GET_CONFIG_VALUE("ClearColorRed", 0.0f),
		GET_CONFIG_VALUE("ClearColorGreen", 0.0f),
		GET_CONFIG_VALUE("ClearColorBlue", 0.0f),
		GET_CONFIG_VALUE("ClearColorAlpha", 1.0f)),
	shadowsEnabled(GET_CONFIG_VALUE("shadowsEnabled", true)),
	window(window),
	m_threadWindow(threadWindow),
	vao(0),
	isMouseEnabled(true),
	ambientLightFogEnabled(GET_CONFIG_VALUE("ambientLightFogEnabled", true)),
	ambientLightFogColor(GET_CONFIG_VALUE("ambientLightFogColor_x", 0.7f),
		GET_CONFIG_VALUE("ambientLightFogColor_y", 0.7f),
		GET_CONFIG_VALUE("ambientLightFogColor_z", 0.7f)),
	ambientLightFogStart(GET_CONFIG_VALUE("ambientLightFogStart", 8.0f)),
	ambientLightFogEnd(GET_CONFIG_VALUE("ambientLightFogEnd", 50.0f)),
	m_ambientDaytimeColor(GET_CONFIG_VALUE("ambientDaytimeColorRed", 0.2f),
		GET_CONFIG_VALUE("ambientDaytimeColorGreen", 0.2f),
		GET_CONFIG_VALUE("ambientDaytimeColorBlue", 0.2f)),
	m_ambientSunNearHorizonColor(GET_CONFIG_VALUE("ambientSunNearHorizonColorRed", 0.1f),
		GET_CONFIG_VALUE("ambientSunNearHorizonColorGreen", 0.1f),
		GET_CONFIG_VALUE("ambientSunNearHorizonColorBlue", 0.1f)),
	m_ambientNighttimeColor(GET_CONFIG_VALUE("ambientNighttimeColorRed", 0.02f),
		GET_CONFIG_VALUE("ambientNighttimeColorGreen", 0.02f),
		GET_CONFIG_VALUE("ambientNighttimeColorBlue", 0.02f)),
	currentLight(NULL),
	currentCameraIndex(0),
	m_currentCamera(NULL),
	m_mainMenuCamera(NULL),
	altCamera(Matrix4D::Identity(), Transform()),
	cubeMapNode(NULL),
	cubeMapShader(NULL),
	cubeMapTextureDay(NULL),
	cubeMapTextureNight(NULL),
	defaultShader(NULL),
	defaultShaderFogEnabled(NULL),
	//textShader(NULL),
	shadowMapShader(NULL),
	nullFilterShader(NULL),
	gaussBlurFilterShader(NULL),
	fxaaFilterShader(NULL),
	fontTexture(NULL),
	textRenderer(NULL),
	lightMatrix(Math::Matrix4D::Scale(REAL_ZERO, REAL_ZERO, REAL_ZERO)),
	fxaaSpanMax(GET_CONFIG_VALUE("fxaaSpanMax", 8.0f)),
	fxaaReduceMin(GET_CONFIG_VALUE("fxaaReduceMin", REAL_ONE / 128.0f)),
	fxaaReduceMul(GET_CONFIG_VALUE("fxaaReduceMul", REAL_ONE / 8.0f))
#ifdef ANT_TWEAK_BAR_ENABLED
	,propertiesBar(NULL),
	cameraBar(NULL),
	lightsBar(NULL),
	previousFrameCameraIndex(0),
	renderToTextureTestingEnabled(false)
#endif
{
	LOG(Info, LOGPLACE, "Creating Renderer instance started");
	PrintGlReport();
	SetCallbacks();

	SetSamplerSlot("diffuse", 0);
	SetSamplerSlot("normalMap", 1);
	SetSamplerSlot("displacementMap", 2);
	SetSamplerSlot("shadowMap", 3);
	SetSamplerSlot("cubeMapDay", 0);
	SetSamplerSlot("cubeMapNight", 1);
	SetSamplerSlot("filterTexture", 0);
	SetSamplerSlot("diffuse2", 4);

	glGenVertexArrays(1, &vao);

#ifndef ANT_TWEAK_BAR_ENABLED
	SetVector3D("ambientFogColor", ambientLightFogColor);
	SetReal("ambientFogStart", ambientLightFogStart);
	SetReal("ambientFogEnd", ambientLightFogEnd);
	SetVector3D("ambientIntensity", m_ambientLight);
#endif

	//SetTexture("shadowMap",
	//	new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR,
	//		GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true,
	//		GL_COLOR_ATTACHMENT0 /* we're going to render color information */)); // variance shadow mapping
	//SetTexture("shadowMapTempTarget", new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0));
	defaultShader = new Shader(GET_CONFIG_VALUE_STR("defaultShader", "ForwardAmbient"));
	defaultShaderFogEnabled = new Shader(GET_CONFIG_VALUE_STR("defaultShaderFogEnabled", "ForwardAmbientFogEnabled"));
	shadowMapShader = new Shader(GET_CONFIG_VALUE_STR("shadowMapShader", "ShadowMapGenerator"));
	nullFilterShader = new Shader(GET_CONFIG_VALUE_STR("nullFilterShader", "Filter-null"));
	gaussBlurFilterShader = new Shader(GET_CONFIG_VALUE_STR("gaussBlurFilterShader", "filter-gaussBlur7x1"));
	fxaaFilterShader = new Shader(GET_CONFIG_VALUE_STR("fxaaFilterShader", "filter-fxaa"));
	altCamera.GetTransform().Rotate(Vector3D(0, 1, 0), Angle(180));

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	tempTarget = new Texture(width, height, NULL, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);

	planeMaterial = new Material(tempTarget, 1, 8);
	planeTransform.SetScale(REAL_ONE);
	planeTransform.Rotate(Vector3D(1, 0, 0), Angle(90));
	planeTransform.Rotate(Vector3D(0, 0, 1), Angle(180));
	planeMesh = new Mesh("..\\Models\\plane4.obj");
	planeMesh->Initialize();

	InitializeCubeMap();

	for (int i = 0; i < SHADOW_MAPS_COUNT; ++i)
	{
		int shadowMapSize = 1 << (i + 1);
		shadowMaps[i] = new Texture(shadowMapSize, shadowMapSize, NULL, GL_TEXTURE_2D, GL_LINEAR,
			GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true, GL_COLOR_ATTACHMENT0 /* we're going to render color information */);
		shadowMapTempTargets[i] = new Texture(shadowMapSize, shadowMapSize, NULL, GL_TEXTURE_2D, GL_LINEAR,
			GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true, GL_COLOR_ATTACHMENT0 /* we're going to render color information */);
	}

	fontTexture = new Texture("..\\Textures\\Holstein.tga", GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);
	textRenderer = new TextRenderer(fontTexture);

	SetTexture("displayTexture", new Texture(width, height, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0));
#ifndef ANT_TWEAK_BAR_ENABLED
	SetReal("fxaaSpanMax", fxaaSpanMax);
	SetReal("fxaaReduceMin", fxaaReduceMin);
	SetReal("fxaaReduceMul", fxaaReduceMul);
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

	LOG(Debug, LOGPLACE, "Creating Renderer instance finished");
}


Renderer::~Renderer(void)
{
	LOG(Notice, LOGPLACE, "Destroying rendering engine...");
	
	glDeleteVertexArrays(1, &vao);

	//if (cameras != NULL)
	//{
	//	delete [] cameras;
	//	cameras = NULL;
	//}
	//if (currentLight != NULL)
	//{
	//	delete currentLight;
	//	currentLight = NULL;
	//}
	// TODO: Deallocating the lights member variable
	// TODO: Deallocating the cameras member variable

	SAFE_DELETE(defaultShader);
	SAFE_DELETE(defaultShaderFogEnabled);
	SAFE_DELETE(shadowMapShader);
	SAFE_DELETE(nullFilterShader);
	SAFE_DELETE(gaussBlurFilterShader);
	SAFE_DELETE(fxaaFilterShader);
	//SAFE_DELETE(altCameraNode);
	//SAFE_DELETE(cubeMapNode);
	SAFE_DELETE(cubeMapShader);
	SAFE_DELETE(planeMaterial);
	SAFE_DELETE(planeMesh);

	// TODO: fontTexture uses the same texture as the fontTexture used in CoreEngine class. That's why we shouldn't SAFE_DELETE font texture here.
	// Of course, we should deal with it later on more appropriately.
	//SetTexture("fontTexture", NULL);
	//SAFE_DELETE(fontTexture);
	SAFE_DELETE(textRenderer);

	SetTexture("shadowMap", NULL);
	for (int i = 0; i < SHADOW_MAPS_COUNT; ++i)
	{
		SAFE_DELETE(shadowMaps[i]);
		SAFE_DELETE(shadowMapTempTargets[i]);
	}

#ifdef ANT_TWEAK_BAR_ENABLED
	TwTerminate(); // Terminate AntTweakBar
#endif
	glfwTerminate(); // Terminate GLFW
	LOG(Notice, LOGPLACE, "Rendering engine destroyed");
}

void Renderer::SetCallbacks()
{
	ASSERT(window != NULL);
	if (window == NULL)
	{
		LOG(Critical, LOGPLACE, "Setting GLFW callbacks failed. Window is NULL.");
		exit(EXIT_FAILURE);
	}
	glfwSetWindowCloseCallback(window, &GameManager::WindowCloseEventCallback);
	glfwSetWindowSizeCallback(window, GameManager::WindowResizeCallback);
	glfwSetKeyCallback(window, &GameManager::KeyEventCallback);
	//glfwSetCharCallback(window, GameManager::CharEventCallback);
	//glfwSetMousePosCallback(window, GameManager::MouseMotionCallback);
	glfwSetCursorPosCallback(window, &GameManager::MousePosCallback);
	glfwSetMouseButtonCallback(window, &GameManager::MouseEventCallback);
	glfwSetScrollCallback(window, &GameManager::ScrollEventCallback);
}

void Renderer::InitializeCubeMap()
{
	std::string cubeMapDayDirectory = "..\\Textures\\" + GET_CONFIG_VALUE_STR("skyboxDayDirectory", "SkyboxDebug");
	std::string cubeMapNightDirectory = "..\\Textures\\" + GET_CONFIG_VALUE_STR("skyboxNightDirectory", "SkyboxDebug");
	cubeMapTextureDay = InitializeCubeMapTexture(cubeMapDayDirectory);
	cubeMapTextureNight = InitializeCubeMapTexture(cubeMapNightDirectory);

	//SetTexture("cubeMapDay", cubeMapTextureDay);
	//SetTexture("cubeMapNight", cubeMapTextureNight);

	Material* cubeMapMaterial = new Material(cubeMapTextureDay, "cubeMapDay");
	cubeMapMaterial->SetAdditionalTexture(cubeMapTextureNight, "cubeMapNight");

	cubeMapNode = new GameNode();
	cubeMapNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\" + GET_CONFIG_VALUE_STR("skyboxModel", "cube.obj")), cubeMapMaterial));
	cubeMapNode->GetTransform().SetPos(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	cubeMapNode->GetTransform().SetScale(50.0f);
	cubeMapShader = new Shader((GET_CONFIG_VALUE_STR("skyboxShader", "skybox-shader")));
}

Texture* Renderer::InitializeCubeMapTexture(const std::string& cubeMapTextureDirectory)
{
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
		LOG(Utility::Error, LOGPLACE, "Cannot locate the right face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapNegXFaceFileFound)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot locate the left face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapPosYFaceFileFound)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot locate the up face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapNegYFaceFileFound)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot locate the down face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapPosZFaceFileFound)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot locate the front face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	if (!cubeMapNegZFaceFileFound)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot locate the back face of the cube map");
		// TODO: Set default texture for the missing face instead of just exiting
		exit(EXIT_FAILURE);
	}
	Texture* cubeMapTexture = new Texture(cubeMapPosXFaceFileName, cubeMapNegXFaceFileName, cubeMapPosYFaceFileName, cubeMapNegYFaceFileName, cubeMapPosZFaceFileName, cubeMapNegZFaceFileName);
	if (cubeMapTexture == NULL)
	{
		LOG(Error, LOGPLACE, "Cube map texture is NULL");
		exit(EXIT_FAILURE);
	}
	return cubeMapTexture;
}

void Renderer::Render(const GameNode& gameNode)
{
	Rendering::CheckErrorCode("Renderer::Render", "Started the Render function");
	// TODO: Expand with Stencil buffer once it is used

	AdjustAmbientLightAccordingToCurrentTime();

#ifdef ANT_TWEAK_BAR_ENABLED
	SetVector3D("ambientFogColor", ambientLightFogColor);
	SetReal("ambientFogStart", ambientLightFogStart);
	SetReal("ambientFogEnd", ambientLightFogEnd);
	SetVector3D("ambientIntensity", m_ambientLight);
	SetReal("fxaaSpanMax", fxaaSpanMax);
	SetReal("fxaaReduceMin", fxaaReduceMin);
	SetReal("fxaaReduceMul", fxaaReduceMul);
	CheckCameraIndexChange();
#endif
	GetTexture("displayTexture")->BindAsRenderTarget();
	//BindAsRenderTarget();

	ClearScreen();
	if (cameras.empty() || cameras.at(currentCameraIndex) == NULL)
	{
		LOG(Emergency, LOGPLACE, "Rendering failed. There is no proper camera set up.");
		exit(EXIT_FAILURE);
		// TODO: Instead of exit maybe just use the default camera (??)
	}
	m_currentCamera = cameras[currentCameraIndex];
	if (ambientLightFogEnabled)
	{
		gameNode.RenderAll(defaultShaderFogEnabled, this); // Ambient rendering with fog enabled
	}
	else
	{
		gameNode.RenderAll(defaultShader, this); // Ambient rendering with disabled fog
	}

	for (std::vector<BaseLight*>::iterator lightItr = lights.begin(); lightItr != lights.end(); ++lightItr)
	{
		currentLight = (*lightItr);
		if (!currentLight->IsEnabled())
		{
			continue;
		}
		ShadowInfo* shadowInfo = currentLight->GetShadowInfo();
		int shadowMapIndex = (shadowInfo == NULL) ? 0 : shadowInfo->GetShadowMapSizeAsPowerOf2() - 1;
		ASSERT(shadowMapIndex < SHADOW_MAPS_COUNT);
		if (shadowMapIndex >= SHADOW_MAPS_COUNT)
		{
			LOG(Error, LOGPLACE, "Incorrect shadow map size. Shadow map index must be an integer from range [0; %d), but equals %d.", SHADOW_MAPS_COUNT, shadowMapIndex);
			shadowMapIndex = 0;
		}
		SetTexture("shadowMap", shadowMaps[shadowMapIndex]);
		shadowMaps[shadowMapIndex]->BindAsRenderTarget();
		glClearColor(REAL_ONE /* completely in light */, REAL_ONE /* we want variance to be also cleared */, REAL_ZERO, REAL_ZERO); // everything is in light (we can clear the COLOR_BUFFER_BIT in the next step)
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		if (shadowInfo != NULL) // The currentLight casts shadows
		{
			altCamera.SetProjection(shadowInfo->GetProjection());
			ShadowCameraTransform shadowCameraTransform = currentLight->CalcShadowCameraTransform(m_currentCamera->GetTransform().GetTransformedPos(), m_currentCamera->GetTransform().GetTransformedRot());
			altCamera.GetTransform().SetPos(shadowCameraTransform.pos);
			altCamera.GetTransform().SetRot(shadowCameraTransform.rot);
			//altCamera.GetTransform().SetPos(currentLight->GetTransform().GetTransformedPos());
			//altCamera.GetTransform().SetRot(currentLight->GetTransform().GetTransformedRot());

			lightMatrix = BIAS_MATRIX * altCamera.GetViewProjection();

			SetReal("shadowLightBleedingReductionFactor", shadowInfo->GetLightBleedingReductionAmount());
			SetReal("shadowVarianceMin", shadowInfo->GetMinVariance());
			bool flipFacesEnabled = shadowInfo->IsFlipFacesEnabled();

			CameraBase* temp = m_currentCamera;
			m_currentCamera = &altCamera;

			if (flipFacesEnabled) { glCullFace(GL_FRONT); }
			gameNode.RenderAll(shadowMapShader, this);
			if (flipFacesEnabled) { glCullFace(GL_BACK); }

			m_currentCamera = temp;

			if (applyFilterEnabled)
			{
				//ApplyFilter(nullFilterShader, GetTexture("shadowMap"), GetTexture("shadowMapTempTarget"));
				//ApplyFilter(nullFilterShader, GetTexture("shadowMapTempTarget"), GetTexture("shadowMap"));
				Real shadowSoftness = shadowInfo->GetShadowSoftness();
				if (!AlmostEqual(shadowSoftness, REAL_ZERO))
				{
					BlurShadowMap(shadowMapIndex, shadowSoftness);
				}
			}
		}
		else // shadowInfo == NULL
		{
			// we set the light matrix this way so that, if no shadow should be cast
			// everything in the scene will be mapped to the same point
			lightMatrix = Math::Matrix4D::Scale(REAL_ZERO, REAL_ZERO, REAL_ZERO);
			SetReal("shadowLightBleedingReductionFactor", REAL_ZERO);
			SetReal("shadowVarianceMin", 0.00002f);
		}
		GetTexture("displayTexture")->BindAsRenderTarget();
		//BindAsRenderTarget();
		if (!Rendering::glBlendEnabled)
		{
			glEnable(GL_BLEND);
		}
		glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both weights equal to 1
		glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore
		glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image

		gameNode.RenderAll(currentLight->GetShader(), this);

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

		/* ==================== Rendering to texture begin ==================== */
		//if (renderToTextureTestingEnabled)
		//{
		//	CameraBase* temp = m_currentCamera;
		//	m_currentCamera = &altCamera;
		//	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	defaultShader->Bind();
		//	defaultShader->UpdateUniforms(planeTransform, *planeMaterial, this);
		//	planeMesh->Draw();

		//	m_currentCamera = temp;
		//}
		/* ==================== Rendering to texture end ==================== */
	}
	SetVector3D("inverseFilterTextureSize", Vector3D(REAL_ONE / GetTexture("displayTexture")->GetWidth(), REAL_ONE / GetTexture("displayTexture")->GetHeight(), REAL_ZERO));

//#ifdef ANT_TWEAK_BAR_ENABLED
//	TwDraw();
//#endif

	RenderSkybox();

	ApplyFilter(fxaaFilterShader, GetTexture("displayTexture"), NULL);

	//double time = glfwGetTime();
	//std::stringstream ss;
	//ss << "FPS: " << std::setprecision(2) << time << " [ms]";
	//textRenderer->DrawString(0, 5800, ss.str(), this);
	//textRenderer->DrawString(0, 50, "Hello world", this);
}

void Renderer::RenderMainMenu(const MenuEntry& menuEntry)
{
	BindAsRenderTarget();
	ClearScreen();
	if (cameras.empty() || cameras.at(currentCameraIndex) == NULL)
	{
		//LOG(Delocust, LOGPLACE, "Rendering main menu with a \"main menu camera\".");
		m_currentCamera = m_mainMenuCamera;
	}
	else
	{
		m_currentCamera = cameras[currentCameraIndex];
	}

	//double time = glfwGetTime();
	//std::stringstream ss;
	//ss << "FPS: " << std::setprecision(2) << time << " [ms]";
	//textRenderer->DrawString(0, 5800, ss.str(), this);
	//textRenderer->DrawString(0, 50, "This is main menu", this);
	int menuEntryChildrenCount = menuEntry.GetChildrenCount();
	for (int i = 0; i < menuEntryChildrenCount; ++i)
	{
		textRenderer->DrawString(Text::CENTER, 350 - 100 * i, menuEntry.GetChildrenText(i), this,
			menuEntry.IsChildMenuEntrySelected(i) ? MenuEntry::GetSelectedMenuEntryTextColor() : MenuEntry::GetNotSelectedMenuEntryTextColor());
	}
}

void Renderer::RenderLoadingScreen(Math::Real loadingProgress)
{
	BindAsRenderTarget();
	ClearScreen();
	if (cameras.empty() || cameras.at(currentCameraIndex) == NULL)
	{
		//LOG(Delocust, LOGPLACE, "Rendering main menu with a \"main menu camera\".");
		m_currentCamera = m_mainMenuCamera;
	}
	else
	{
		m_currentCamera = cameras[currentCameraIndex];
	}

	std::stringstream ss;
	int progress = static_cast<int>(loadingProgress * 100);
	ss << progress << "\%";
	textRenderer->DrawString(Text::CENTER, 350, "Loading...", this);
	textRenderer->DrawString(Text::CENTER, 250, ss.str(), this);
}

void Renderer::AdjustAmbientLightAccordingToCurrentTime()
{
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
	//LOG(Utility::Debug, LOGPLACE, "Sunset started. Time = %.1f\tDayNightMixFactor = %.4f. Ambient light = (%s)",
	//	timeOfDay, dayNightMixFactor, m_ambientLight.ToString().c_str());
	SetReal("dayNightMixFactor", dayNightMixFactor);
	/* ==================== Adjusting the time variables end ==================== */
}

void Renderer::RenderSkybox()
{
	cubeMapNode->GetTransform().SetPos(m_currentCamera->GetTransform().GetTransformedPos());
	if (ambientLightFogEnabled)
	{
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
	cubeMapNode->RenderAll(cubeMapShader, this);
	glDepthFunc(Rendering::glDepthTestFunc);
	glCullFace(Rendering::glCullFaceMode);
	//glEnable(GL_DEPTH_TEST);
	Rendering::CheckErrorCode("Renderer::Render", "Rendering skybox");
	/* ==================== Rendering skybox end ==================== */
}

void Renderer::BlurShadowMap(int shadowMapIndex, Real blurAmount /* how many texels we move per sample */)
{
	Texture* shadowMap = shadowMaps[shadowMapIndex];
	Texture* shadowMapTempTarget = shadowMapTempTargets[shadowMapIndex];
	if (shadowMap == NULL)
	{
		LOG(Error, LOGPLACE, "Shadow map %d is NULL. Cannot perform the blurring process.", shadowMapIndex);
		return;
	}
	if (shadowMapTempTarget == NULL)
	{
		LOG(Error, LOGPLACE, "Temporary shadow map target %d is NULL. Cannot perform the blurring process.", shadowMapIndex);
		return;
	}

	SetVector3D("blurScale", Vector3D(blurAmount / shadowMap->GetWidth(), REAL_ZERO, REAL_ZERO));
	ApplyFilter(gaussBlurFilterShader, shadowMap, shadowMapTempTarget);
	
	SetVector3D("blurScale", Vector3D(REAL_ZERO, blurAmount / shadowMap->GetHeight(), REAL_ZERO));
	ApplyFilter(gaussBlurFilterShader, shadowMapTempTarget, shadowMap);
}

// You cannot read and write from the same texture at the same time. That's why we use dest texture as a temporary texture to store the result
void Renderer::ApplyFilter(Shader* filterShader, Texture* source, Texture* dest)
{
	if (filterShader == NULL)
	{
		LOG(Error, LOGPLACE, "Cannot apply a filter. Filtering shader is NULL.");
		return;
	}
	ASSERT(source != NULL);
	if (source == NULL)
	{
		LOG(Emergency, LOGPLACE, "Cannot apply a filter. Source texture is NULL.");
		return;
	}
	ASSERT(source != dest);
	if (source == dest)
	{
		LOG(Error, LOGPLACE, "Cannot apply a filter. Both source and destination textures point to the same Texture in memory.");
		return;
	}
	if (dest == NULL)
	{
		LOG(Delocust, LOGPLACE, "Binding window as a render target for filtering");
		BindAsRenderTarget();
	}
	else
	{
		LOG(Delocust, LOGPLACE, "Binding texture as a render target for filtering");
		dest->BindAsRenderTarget();
	}

	LOG(Debug, LOGPLACE, "Applying a filter to the source texture");
	
	SetTexture("filterTexture", source);

	altCamera.SetProjection(Matrix4D::Identity());
	altCamera.GetTransform().SetPos(Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO));
	altCamera.GetTransform().SetRot(Quaternion(Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO), Angle(180.0f)));

	CameraBase* temp = m_currentCamera;
	m_currentCamera = &altCamera;

	glClear(GL_DEPTH_BUFFER_BIT);
	filterShader->Bind();
	filterShader->UpdateUniforms(planeTransform, *planeMaterial, this);
	planeMesh->Draw();

	m_currentCamera = temp;
	SetTexture("filterTexture", NULL);
}

void Renderer::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Renderer::ClearScreen() const
{
	if (ambientLightFogEnabled)
	{
		Vector3D fogColor = ambientLightFogColor * m_ambientLight;
		glClearColor(fogColor.GetX(), fogColor.GetY(), fogColor.GetZ(), REAL_ONE);
	}
	else
	{
		glClearColor(backgroundColor.GetRed(), backgroundColor.GetGreen(), backgroundColor.GetBlue(), backgroundColor.GetAlpha());
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline CameraBase& Renderer::GetCurrentCamera()
{
	if (m_currentCamera == NULL)
	{
		LOG(Emergency, LOGPLACE, "Current camera is NULL");
		exit(EXIT_FAILURE);
	}
	return *m_currentCamera;
}

unsigned int Renderer::NextCamera()
{
	if (currentCameraIndex == cameras.size() - 1)
	{
		currentCameraIndex = -1;
	}
	return SetCurrentCamera(currentCameraIndex + 1);
}

unsigned int Renderer::PrevCamera()
{
	if (currentCameraIndex == 0)
	{
		currentCameraIndex = cameras.size();
	}
	return SetCurrentCamera(currentCameraIndex - 1);
}

unsigned int Renderer::SetCurrentCamera(unsigned int cameraIndex)
{
	//m_currentCamera->Deactivate();
	ASSERT((cameraIndex >= 0) && (cameraIndex < cameras.size()));
	if ( (cameraIndex < 0) || (cameraIndex >= cameras.size()) )
	{
		LOG(Error, LOGPLACE, "Incorrect current camera index. Passed %d when the correct range is (%d, %d).", cameraIndex, 0, cameras.size());
		LOG(Notice, LOGPLACE, "Setting current camera index to 0 (i.e. first camera)");
		this->currentCameraIndex = 0;
	}
	else
	{
		this->currentCameraIndex = cameraIndex;
	}
#ifndef ANT_TWEAK_BAR_ENABLED
	LOG(Notice, LOGPLACE, "Switched to camera #%d", this->currentCameraIndex + 1);
	//LOG(Debug, LOGPLACE, "%s", cameras[this->currentCameraIndex]->ToString().c_str());
#endif
	//cameras[this->currentCameraIndex]->Activate();
	return this->currentCameraIndex;
}

inline void Renderer::AddLight(BaseLight* light)
{
	lights.push_back(light);
}

inline void Renderer::AddCamera(CameraBase* camera)
{
	cameras.push_back(camera);
	++cameraCount;
}

void Renderer::PrintGlReport()
{
	LOG(Info, LOGPLACE, "Vendor:\t%s", (const char*)glGetString(GL_VENDOR));
	LOG(Info, LOGPLACE, "Renderer name:\t%s", (const char*)glGetString(GL_RENDERER));
	LOG(Info, LOGPLACE, "OpenGL version:\t%s", (const char*)glGetString(GL_VERSION));
	LOG(Info, LOGPLACE, "GLSL version:\t%s", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	//LOG(Error, LOGPLACE, "OpenGL extensions: ", (const char*)glGetString(GL_EXTENSIONS));
}

bool Renderer::IsCloseRequested() const
{
	return glfwWindowShouldClose(window) != 0;
}

void Renderer::UpdateUniformStruct(const Transform& transform, const Material& material, const Shader& shader, const std::string& uniformName, const std::string& uniformType)
{
	//throw uniformType + " is not supported by the rendering engine";
	LOG(Error, LOGPLACE, "Uniform name \"%s\" of type \"%s\" is not supported by the rendering engine", uniformName.c_str(), uniformType.c_str());
}

void Renderer::BindAsRenderTarget()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}

unsigned int Renderer::GetSamplerSlot(const std::string& samplerName) const
{
	/* TODO: Add assertions and checks */
	std::map<std::string, unsigned int>::const_iterator samplerItr = samplerMap.find(samplerName);
	if (samplerItr == samplerMap.end())
	{
		LOG(Utility::Error, LOGPLACE, "Sampler name \"%s\" has not been found in the sampler map.", samplerName.c_str());
		return 0;
		//exit(EXIT_FAILURE);
	}
	return samplerItr->second;
}

#ifdef ANT_TWEAK_BAR_ENABLED
/**
 * This function must be called after AntTweakBar library is initalized.
 * See Rendering::InitializeTweakBars()
 */
void Renderer::InitializeTweakBars()
{
	LOG(Debug, LOGPLACE, "Initializing rendering engine's tweak bars");
	AntTweakBarTypes::InitializeTweakBarTypes();

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	TwWindowSize(width, height);

#ifdef RENDERER_PROPERTIES_BAR
	// TODO: CameraMembers[0] ("Position") is not displayed correctly, because at 0 address lies the pointer to parentGameNode
	cameraMembers[0].Name = "Projection"; cameraMembers[1].Name = "FoV"; cameraMembers[2].Name = "AspectRatio"; cameraMembers[3].Name = "NearPlane"; cameraMembers[4].Name = "FarPlane";
	cameraMembers[0].Type = matrix4DType; cameraMembers[1].Type = angleType; cameraMembers[2].Type = TW_TYPE_FLOAT; cameraMembers[3].Type = TW_TYPE_FLOAT; cameraMembers[4].Type = TW_TYPE_FLOAT;
	cameraMembers[0].Offset = 8; cameraMembers[1].Offset = 80; cameraMembers[2].Offset = 92; cameraMembers[3].Offset = 100; cameraMembers[4].Offset = 108;
	cameraMembers[0].DefString = ""; cameraMembers[1].DefString = ""; cameraMembers[2].DefString = " step=0.01 "; cameraMembers[3].DefString = ""; cameraMembers[4].DefString = "";
	cameraType = TwDefineStruct("Camera", cameraMembers, 5, sizeof(Rendering::Camera), NULL, NULL);

	propertiesBar = TwNewBar("PropertiesBar");
	TwAddVarRW(propertiesBar, "bgColor", TW_TYPE_COLOR3F, &backgroundColor, " label='Background color' ");
	TwAddVarRW(propertiesBar, "currentCamera", TW_TYPE_UINT32, &currentCameraIndex, " label='Current camera' ");
	TwAddVarRW(propertiesBar, "applyFilterEnabled", TW_TYPE_BOOLCPP, &applyFilterEnabled, " label='Apply filter' ");
	TwAddVarRW(propertiesBar, "ambientLight", TW_TYPE_COLOR3F, &m_ambientLight, " label='Color' group='Ambient light'");
	TwAddVarRW(propertiesBar, "ambientLightFogEnabled", TW_TYPE_BOOLCPP, &ambientLightFogEnabled, " label='Fog enabled' group='Ambient light' ");
	TwAddVarRW(propertiesBar, "ambientLightFogColor", TW_TYPE_COLOR3F, &ambientLightFogColor, " label='Fog color' group='Ambient light' ");
	TwAddVarRW(propertiesBar, "ambientLightFogStart", TW_TYPE_REAL, &ambientLightFogStart, " label='Fog start' group='Ambient light' step=0.5 min=0.5");
	TwAddVarRW(propertiesBar, "ambientLightFogEnd", TW_TYPE_REAL, &ambientLightFogEnd, " label='Fog end' group='Ambient light' step=0.5 min=1.0");
	TwAddVarRW(propertiesBar, "directionalLightsEnabled", TW_TYPE_BOOLCPP, DirectionalLight::GetDirectionalLightsEnabled(), " label='Directional light' group=Lights");
	TwAddVarRW(propertiesBar, "pointLightsEnabled", TW_TYPE_BOOLCPP, PointLight::GetPointLightsEnabled(), " label='Point lights' group=Lights");
	TwAddVarRW(propertiesBar, "spotLightsEnabled", TW_TYPE_BOOLCPP, SpotLight::GetSpotLightsEnabled(), " label='Spot lights' group=Lights");
	TwAddVarRW(propertiesBar, "shadowsEnabled", TW_TYPE_BOOLCPP, &shadowsEnabled, " label='Render shadows' group=Shadows");
	TwAddVarRW(propertiesBar, "fxaaSpanMax", TW_TYPE_REAL, &fxaaSpanMax, " min=0.0 step=0.1 label='Max span' group='FXAA' ");
	TwAddVarRW(propertiesBar, "fxaaReduceMin", TW_TYPE_REAL, &fxaaReduceMin, " min=0.00001 step=0.000002 label='Min reduce' group='FXAA' ");
	TwAddVarRW(propertiesBar, "fxaaReduceMul", TW_TYPE_REAL, &fxaaReduceMul, " min=0.0 step=0.01 label='Reduce scale' group='FXAA' ");

	TwSetParam(propertiesBar, "currentCamera", "max", TW_PARAM_INT32, 1, &cameraCount);
	TwSetParam(propertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "false"); // Hide the bar at startup
#endif

#ifdef CAMERA_TWEAK_BAR
	cameraBar = TwNewBar("CamerasBar");
	if (cameras.empty() || cameras[currentCameraIndex] == NULL)
	{
		TwAddVarRW(cameraBar, "cameraVar", cameraType,  m_mainMenuCamera, " label='Camera' group=Camera ");
		TwAddVarRW(cameraBar, "MainMenuCamera.Pos", vector3DType, &m_mainMenuCamera->GetTransform().GetPos(), " label='MainMenuCamera.Pos' group=Camera ");
		TwAddVarRW(cameraBar, "MainMenuCamera.Rot", TW_TYPE_QUAT4F, &m_mainMenuCamera->GetTransform().GetRot(), " label='MainMenuCamera.Rot' group=Camera ");
	}
	else
	{
		TwAddVarRW(cameraBar, "cameraVar", cameraType,  cameras[currentCameraIndex], " label='Camera' group=Camera ");
		char cameraIndexStr[256];
		char cameraDefStr[256];
		_snprintf_s(cameraIndexStr, 256, 255, "camera[%d].Pos", currentCameraIndex);
		_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%d].Pos' group=Camera ", currentCameraIndex);
		TwAddVarRW(cameraBar, cameraIndexStr, vector3DType, &cameras[currentCameraIndex]->GetTransform().GetPos(), cameraDefStr);
		_snprintf_s(cameraIndexStr, 256, 255, "camera[%d].Rot", currentCameraIndex);
		_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%d].Rot' group=Camera ", currentCameraIndex);
		TwAddVarRW(cameraBar, cameraIndexStr, TW_TYPE_QUAT4F, &cameras[currentCameraIndex]->GetTransform().GetRot(), cameraDefStr);
	}
	
	TwDefine(" CamerasBar/Camera opened=true ");
	TwSetParam(cameraBar, NULL, "visible", TW_PARAM_CSTRING, 1, "false"); // Hide the bar at startup
#endif

#ifdef LIGHTS_TWEAK_BAR
	//TODO: Doesn't work yet.
	lightsBar = TwNewBar("LightsBar");
	for (std::vector<BaseLight*>::iterator lightItr = lights.begin(); lightItr != lights.end(); ++lightItr)
	{
		currentLight = *lightItr;
		currentLight->InitializeTweakBar(lightsBar);
	}
#endif

	//TwBar* altCameraBar = TwNewBar("AltCameraBar");
	//TwAddVarRW(altCameraBar, "cameraVar", cameraType,  &altCamera, " label='Camera' group=Camera ");
	//TwAddVarRW(altCameraBar, "altCameraPos", vector3DType, &altCamera.GetTransform().GetPos(), " label='AltCamera.Pos' group=Camera ");
	//TwAddVarRW(altCameraBar, "altCameraRot", TW_TYPE_QUAT4F, &altCamera.GetTransform().GetRot(), " label='AltCamera.Rot' group=Camera ");
	//TwDefine(" AltCameraBar/Camera opened=true ");
	LOG(Info, LOGPLACE, "Initializing rendering engine's tweak bars finished");
}

void Renderer::CheckCameraIndexChange()
{
#ifdef CAMERA_TWEAK_BAR
	if (previousFrameCameraIndex == currentCameraIndex)
	{
		return;
	}
	LOG(Notice, LOGPLACE, "Switched to camera #%d", this->currentCameraIndex + 1);
	//LOG(Debug, LOGPLACE, "%s", cameras[this->currentCameraIndex]->ToString().c_str());

	TwRemoveAllVars(cameraBar);
	TwAddVarRW(cameraBar, "cameraVar", cameraType,  cameras[currentCameraIndex], " label='Camera' group=Camera ");
	char cameraIndexStr[256];
	char cameraDefStr[256];
	_snprintf_s(cameraIndexStr, 256, 255, "camera[%d].Pos", currentCameraIndex);
	_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%d].Pos' group=Camera ", currentCameraIndex);
	TwAddVarRW(cameraBar, cameraIndexStr, vector3DType, &cameras[currentCameraIndex]->GetTransform().GetPos(), cameraDefStr);
	//_snprintf(cameraIndexStr, 255, "camera[%d].Angle", currentCameraIndex);
	//_snprintf(cameraDefStr, 255, " label='Camera[%d].Angle' ", currentCameraIndex);
	//TwAddVarRW(cameraBar, cameraIndexStr, angleType, &tempAngle, cameraDefStr);
	TwDefine(" CamerasBar/Camera opened=true ");

	previousFrameCameraIndex = currentCameraIndex;
#endif
}
#endif