#include "StdAfx.h"
#include "Renderer.h"
#include "Game.h"
//#include "PhongShader.h"
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

// Include GLM- a library for 3D mathematics
//#include <glm/glm.hpp>

using namespace Rendering;
using namespace Utility;
using namespace Math;

/* static */ const Matrix4D Renderer::BIAS_MATRIX(Matrix4D::Scale(0.5f, 0.5f, 0.5f) * Matrix4D::Translation(1.0f, 1.0f, 1.0f));

Renderer::Renderer(GLFWwindow* window) :
	cameraCount(-1),
	applyFilterEnabled(GET_CONFIG_VALUE("applyFilterEnabled", true)),
	backgroundColor(GET_CONFIG_VALUE("ClearColorRed", 0.0f),
		GET_CONFIG_VALUE("ClearColorGreen", 0.0f),
		GET_CONFIG_VALUE("ClearColorBlue", 0.0f),
		GET_CONFIG_VALUE("ClearColorAlpha", 1.0f)),
	shadowsEnabled(GET_CONFIG_VALUE("shadowsEnabled", true)),
	window(window),
	vao(0),
	isMouseEnabled(true),
	ambientLightFogStart(GET_CONFIG_VALUE("ambientLightFogStart", 8.0f)),
	ambientLightFogEnd(GET_CONFIG_VALUE("ambientLightFogEnd", 50.0f)),
	ambientLight(GET_CONFIG_VALUE("ambientLight_x", 0.02f),
		GET_CONFIG_VALUE("ambientLight_y", 0.02f),
		GET_CONFIG_VALUE("ambientLight_z", 0.02f)),
	currentLight(NULL),
	currentCameraIndex(0),
	currentCamera(NULL),
	altCamera(Matrix4D::Identity()),
	altCameraNode(NULL),
	cubeMapNode(NULL),
	cubeMapShader(NULL),
	cubeMapTexture(NULL),
	defaultShader(NULL),
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
	SetSamplerSlot("cubeMap", 0);
	SetSamplerSlot("filterTexture", 0);
	SetSamplerSlot("diffuse2", 4);

	glGenVertexArrays(1, &vao);

#ifndef ANT_TWEAK_BAR_ENABLED
	SetReal("ambientFogStart", ambientLightFogStart);
	SetReal("ambientFogEnd", ambientLightFogEnd);
	SetVector3D("ambientIntensity", ambientLight);
#endif

	//SetTexture("shadowMap",
	//	new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR,
	//		GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true,
	//		GL_COLOR_ATTACHMENT0 /* we're going to render color information */)); // variance shadow mapping
	//SetTexture("shadowMapTempTarget", new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0));
	defaultShader = new Shader((GET_CONFIG_VALUE_STR("defaultShader", "ForwardAmbient")));
	shadowMapShader = new Shader((GET_CONFIG_VALUE_STR("shadowMapShader", "ShadowMapGenerator")));
	nullFilterShader = new Shader((GET_CONFIG_VALUE_STR("nullFilterShader", "Filter-null")));
	gaussBlurFilterShader = new Shader((GET_CONFIG_VALUE_STR("gaussBlurFilterShader", "filter-gaussBlur7x1")));
	fxaaFilterShader = new Shader((GET_CONFIG_VALUE_STR("fxaaFilterShader", "filter-fxaa")));
	altCameraNode = new GameNode();
	altCameraNode->AddComponent(&altCamera);
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

	//fontTexture = new Texture("..\\Textures\\Holstein.tga", GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);
	//textRenderer = new TextRenderer(fontTexture);

	SetTexture("displayTexture", new Texture(width, height, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0));
#ifndef ANT_TWEAK_BAR_ENABLED
	SetReal("fxaaSpanMax", fxaaSpanMax);
	SetReal("fxaaReduceMin", fxaaReduceMin);
	SetReal("fxaaReduceMul", fxaaReduceMul);
#endif

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
	SAFE_DELETE(shadowMapShader);
	SAFE_DELETE(nullFilterShader);
	SAFE_DELETE(gaussBlurFilterShader);
	SAFE_DELETE(fxaaFilterShader);
	//SAFE_DELETE(altCameraNode);
	//SAFE_DELETE(cubeMapNode);
	SAFE_DELETE(cubeMapShader);
	SAFE_DELETE(planeMaterial);
	SAFE_DELETE(planeMesh);

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
	glfwSetWindowCloseCallback(window, &Game::WindowCloseEventCallback);
	glfwSetWindowSizeCallback(window, Game::WindowResizeCallback);
	glfwSetKeyCallback(window, &Game::KeyEventCallback);
	//glfwSetCharCallback(window, Game::CharEventCallback);
	//glfwSetMousePosCallback(window, Game::MouseMotionCallback);
	glfwSetCursorPosCallback(window, &Game::MousePosCallback);
	glfwSetMouseButtonCallback(window, &Game::MouseEventCallback);
	glfwSetScrollCallback(window, &Game::ScrollEventCallback);
}

void Renderer::InitializeCubeMap()
{
	const std::string DIRECTORY_PATH_SEPARATOR = "\\"; // for Windows it's "\", but for Unix it's "/"
	const std::string EXPECTED_POS_X_FACE_FILENAME = "right";
	const std::string EXPECTED_NEG_X_FACE_FILENAME = "left";
	const std::string EXPECTED_POS_Y_FACE_FILENAME = "up";
	const std::string EXPECTED_NEG_Y_FACE_FILENAME = "down";
	const std::string EXPECTED_POS_Z_FACE_FILENAME = "front";
	const std::string EXPECTED_NEG_Z_FACE_FILENAME = "back";

	//cubeMapTexture = new Texture("..\\Textures\\rightDebug.jpg", "..\\Textures\\leftDebug.jpg", "..\\Textures\\upDebug.jpg", "..\\Textures\\downDebug.jpg", "..\\Textures\\frontDebug.jpg", "..\\Textures\\backDebug.jpg");
	std::string cubeMapDirectory = "..\\Textures\\" + GET_CONFIG_VALUE_STR("skyboxDirectory", "SkyboxDebug");
	FileManager fileManager;
	std::vector<std::string> filenames = fileManager.ListAllFilesInDirectory(cubeMapDirectory);
	//std::find(vector.begin(), vector.end(), item)!=vector.end()
	//std::string cubeMapFileExtension = GET_CONFIG_VALUE_STR("skyboxFileExtensions", "jpg");
	bool cubeMapPosXFaceFileFound = false; std::string cubeMapPosXFaceFileName = cubeMapDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegXFaceFileFound = false; std::string cubeMapNegXFaceFileName = cubeMapDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapPosYFaceFileFound = false; std::string cubeMapPosYFaceFileName = cubeMapDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegYFaceFileFound = false; std::string cubeMapNegYFaceFileName = cubeMapDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapPosZFaceFileFound = false; std::string cubeMapPosZFaceFileName = cubeMapDirectory + DIRECTORY_PATH_SEPARATOR;
	bool cubeMapNegZFaceFileFound = false; std::string cubeMapNegZFaceFileName = cubeMapDirectory + DIRECTORY_PATH_SEPARATOR;
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
	cubeMapTexture = new Texture(cubeMapPosXFaceFileName, cubeMapNegXFaceFileName, cubeMapPosYFaceFileName, cubeMapNegYFaceFileName, cubeMapPosZFaceFileName, cubeMapNegZFaceFileName);
	if (cubeMapTexture == NULL)
	{
		LOG(Error, LOGPLACE, "Cube map texture is NULL");
		exit(EXIT_FAILURE);
	}
	SetTexture("cubeMap", cubeMapTexture);
	cubeMapNode = new GameNode();
	cubeMapNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\" + GET_CONFIG_VALUE_STR("skyboxModel", "cube.obj")), new Material(cubeMapTexture)));
	cubeMapNode->GetTransform().SetPos(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	cubeMapNode->GetTransform().SetScale(50.0f);
	cubeMapShader = new Shader((GET_CONFIG_VALUE_STR("skyboxShader", "skybox-shader")));
}

void Renderer::Render(GameNode& gameNode)
{
	Rendering::CheckErrorCode("Renderer::Render", "Started the Render function");
	// TODO: Expand with Stencil buffer once it is used
#ifdef ANT_TWEAK_BAR_ENABLED
	SetReal("ambientFogStart", ambientLightFogStart);
	SetReal("ambientFogEnd", ambientLightFogEnd);
	SetVector3D("ambientIntensity", ambientLight);
	SetReal("fxaaSpanMax", fxaaSpanMax);
	SetReal("fxaaReduceMin", fxaaReduceMin);
	SetReal("fxaaReduceMul", fxaaReduceMul);
	CheckCameraIndexChange();
#endif
	GetTexture("displayTexture")->BindAsRenderTarget();
	//BindAsRenderTarget();

	ClearScreen();
	currentCamera = cameras[currentCameraIndex];
	gameNode.RenderAll(defaultShader, this); // Ambient rendering

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
			ShadowCameraTransform shadowCameraTransform = currentLight->CalcShadowCameraTransform(currentCamera->GetTransform().GetTransformedPos(), currentCamera->GetTransform().GetTransformedRot());
			altCamera.GetTransform().SetPos(shadowCameraTransform.pos);
			altCamera.GetTransform().SetRot(shadowCameraTransform.rot);
			//altCamera.GetTransform().SetPos(currentLight->GetTransform().GetTransformedPos());
			//altCamera.GetTransform().SetRot(currentLight->GetTransform().GetTransformedRot());

			lightMatrix = BIAS_MATRIX * altCamera.GetViewProjection();

			SetReal("shadowLightBleedingReductionFactor", shadowInfo->GetLightBleedingReductionAmount());
			SetReal("shadowVarianceMin", shadowInfo->GetMinVariance());
			bool flipFacesEnabled = shadowInfo->IsFlipFacesEnabled();

			Camera* temp = currentCamera;
			currentCamera = &altCamera;

			if (flipFacesEnabled) { glCullFace(GL_FRONT); }
			gameNode.RenderAll(shadowMapShader, this);
			if (flipFacesEnabled) { glCullFace(GL_BACK); }

			currentCamera = temp;

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
		glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both wages equal to 1
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
		//	Camera* temp = currentCamera;
		//	currentCamera = &altCamera;
		//	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	defaultShader->Bind();
		//	defaultShader->UpdateUniforms(planeTransform, *planeMaterial, this);
		//	planeMesh->Draw();

		//	currentCamera = temp;
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

void Renderer::RenderSkybox()
{
	/* ==================== Rendering skybox begin ==================== */
	//glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	/**
	 * By default (GL_LESS) we tell OpenGL that an incoming fragment wins the depth test if its Z value is less than the stored one.
	 * However, in the case of a skybox the Z value is always the far Z. The far Z is clipped when the depth test function is set to "less than".
	 * To make it part of the scene we change the depth function to "less than or equal".
	 */
	glDepthFunc(GL_LEQUAL);
	cubeMapNode->GetTransform().SetPos(currentCamera->GetTransform().GetTransformedPos());
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

	Camera* temp = currentCamera;
	currentCamera = &altCamera;

	glClear(GL_DEPTH_BUFFER_BIT);
	filterShader->Bind();
	filterShader->UpdateUniforms(planeTransform, *planeMaterial, this);
	planeMesh->Draw();

	currentCamera = temp;
	SetTexture("filterTexture", NULL);
}

void Renderer::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Renderer::ClearScreen() const
{
	glClearColor(backgroundColor.GetRed(), backgroundColor.GetGreen(), backgroundColor.GetBlue(), backgroundColor.GetAlpha());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline Camera& Renderer::GetCurrentCamera()
{
	if (currentCamera == NULL)
	{
		LOG(Emergency, LOGPLACE, "Current camera is NULL");
		exit(EXIT_FAILURE);
	}
	return *currentCamera;
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
	//currentCamera->Deactivate();
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

inline void Renderer::AddCamera(Camera* camera)
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
	TwAddVarRW(propertiesBar, "ambientLight", TW_TYPE_COLOR3F, &ambientLight, " label='Color' group='Ambient light'");
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
#endif

#ifdef CAMERA_TWEAK_BAR
	cameraBar = TwNewBar("CamerasBar");
	TwAddVarRW(cameraBar, "cameraVar", cameraType,  cameras[currentCameraIndex], " label='Camera' group=Camera ");
	char cameraIndexStr[256];
	char cameraDefStr[256];
	_snprintf_s(cameraIndexStr, 256, 255, "camera[%d].Pos", currentCameraIndex);
	_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%d].Pos' group=Camera ", currentCameraIndex);
	TwAddVarRW(cameraBar, cameraIndexStr, vector3DType, &cameras[currentCameraIndex]->GetTransform().GetPos(), cameraDefStr);
	_snprintf_s(cameraIndexStr, 256, 255, "camera[%d].Rot", currentCameraIndex);
	_snprintf_s(cameraDefStr, 256, 255, " label='Camera[%d].Rot' group=Camera ", currentCameraIndex);
	TwAddVarRW(cameraBar, cameraIndexStr, TW_TYPE_QUAT4F, &cameras[currentCameraIndex]->GetTransform().GetRot(), cameraDefStr);
	TwDefine(" CamerasBar/Camera opened=true ");
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