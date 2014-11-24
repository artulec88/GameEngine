#include "StdAfx.h"
#include "Renderer.h"
#include "Game.h"
//#include "PhongShader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
//#include "ForwardAmbientShader.h"
//#include "ForwardDirectionalShader.h"
#include "ShadowInfo.h"
#include "Utility\IConfig.h"
#include "Utility\ILogger.h"
#include "Utility\FileNotFoundException.h"

#include <sstream>

// Include GLM- a library for 3D mathematics
//#include <glm/glm.hpp>

using namespace Rendering;
using namespace Utility;
using namespace Math;

/* static */ const Matrix4D Renderer::BIAS_MATRIX(Matrix4D::Scale(0.5f, 0.5f, 0.5f) * Matrix4D::Translation(1.0f, 1.0f, 1.0f));

Renderer::Renderer(int width, int height, std::string title) :
	window(NULL),
	vao(0),
	isFullscreen(false),
	isMouseEnabled(true),
	ambientLight(GET_CONFIG_VALUE("ambientLight_x", 0.02f),
		GET_CONFIG_VALUE("ambientLight_y", 0.02f),
		GET_CONFIG_VALUE("ambientLight_z", 0.02f)),
	currentLight(NULL),
	currentCameraIndex(0),
	currentCamera(NULL),
	altCamera(Matrix4D::Identity()),
	altCameraNode(NULL),
	defaultShader(NULL),
	shadowMapShader(NULL),
	nullFilterShader(NULL),
	gaussBlurFilterShader(NULL),
	shadowMapWidth(GET_CONFIG_VALUE("shadowMapWidth", 1024)),
	shadowMapHeight(GET_CONFIG_VALUE("shadowMapHeight", 1024))
{
	LOG(Info, LOGPLACE, "Creating Renderer instance started");

	//samplerMap.insert(std::pair<std::string, unsigned int>("diffuse", 0));
	//samplerMap.insert(std::pair<std::string, unsigned int>("normalMap", 1));
	//samplerMap.insert(std::pair<std::string, unsigned int>("displacementMap", 2));
	//samplerMap.insert(std::pair<std::string, unsigned int>("shadowMap", 3));
	//samplerMap.insert(std::pair<std::string, unsigned int>("filterTexture", 0));
	SetSamplerSlot("diffuse", 0);
	SetSamplerSlot("normalMap", 1);
	SetSamplerSlot("displacementMap", 2);
	SetSamplerSlot("shadowMap", 3);
	SetSamplerSlot("filterTexture", 0);

	SetVector3D("ambientIntensity", ambientLight);

	InitGraphics(width, height, title);
	PrintGlReport();

	SetTexture("shadowMap", new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F /* 2 components- R and G- for mean and variance */, GL_RGBA, true, GL_COLOR_ATTACHMENT0 /* we're going to render color information */)); // variance shadow mapping
	SetTexture("shadowMapTempTarget", new Texture(shadowMapWidth, shadowMapHeight, NULL, GL_TEXTURE_2D, GL_LINEAR, GL_RG32F, GL_RGBA, true, GL_COLOR_ATTACHMENT0));
	defaultShader = new Shader((GET_CONFIG_VALUE_STR("defaultShader", "ForwardAmbient")));
	shadowMapShader = new Shader((GET_CONFIG_VALUE_STR("shadowMapShader", "ShadowMapGenerator")));
	nullFilterShader = new Shader((GET_CONFIG_VALUE_STR("nullFilterShader", "Filter-null")));
	gaussBlurFilterShader = new Shader((GET_CONFIG_VALUE_STR("gaussBlurFilterShader", "Filter-gaussBlur7x1")));
	altCameraNode = new GameNode();
	altCameraNode->AddComponent(&altCamera);
	altCamera.GetTransform().Rotate(Vector3D(0, 1, 0), Angle(180));

	int tempWidth = width;
	int tempHeight = height;
	tempTarget = new Texture(tempWidth, tempHeight, NULL, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0);

	planeMaterial = new Material(tempTarget, 1, 8);
	planeTransform.SetScale(Vector3D(1, 1, 1));
	planeTransform.Rotate(Vector3D(1, 0, 0), Angle(90));
	planeTransform.Rotate(Vector3D(0, 0, 1), Angle(180));
	planeMesh = new Mesh("..\\Models\\plane.obj");

	LOG(Delocust, LOGPLACE, "Creating Renderer instance finished");
}


Renderer::~Renderer(void)
{
	LOG(Notice, LOGPLACE, "Destroying rendering engine...");
	
	//glDeleteVertexArrays(1, &vao);

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
	//SAFE_DELETE(altCameraNode);
	SAFE_DELETE(planeMaterial);
	SAFE_DELETE(planeMesh);

	glfwTerminate();
	LOG(Notice, LOGPLACE, "Rendering engine destroyed");
}

void Renderer::InitGraphics(int width, int height, const std::string& title)
{
	LOG(Info, LOGPLACE, "Initializing Renderer started");
	InitGlfw(width, height, title);
	SetGlfwCallbacks();
	InitGlew();

	glClearColor(GET_CONFIG_VALUE("ClearColorRed", 0.0f), GET_CONFIG_VALUE("ClearColorGreen", 0.0f), GET_CONFIG_VALUE("ClearColorBlue", 0.0f), GET_CONFIG_VALUE("ClearColorAlpha", 0.0f));

	glFrontFace(GL_CW); // every face I draw in clockwise order is a front face
	glCullFace(GL_BACK); // cull the back face
	glEnable(GL_CULL_FACE); // culling faces enabled. Cull triangles which normal is not towards the camera
	glEnable(GL_DEPTH_TEST); // to enable depth tests
	glEnable(GL_DEPTH_CLAMP); // prevents the camera to clip through the mesh

	//glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_FRAMEBUFFER_SRGB); // Essentialy gives free gamma correction for better contrast. TODO: Test it!

	//LOG(Notice, LOGPLACE, "Using OpenGL version %s", GetOpenGLVersion().c_str());
}

void Renderer::InitGlfw(int width, int height, const std::string& title)
{
	LOG(Debug, LOGPLACE, "Initializing GLFW started");
	if( !glfwInit() )
	{
		LOG(Critical, LOGPLACE, "Failed to initalize GLFW");
		exit(EXIT_FAILURE);
		// throw FileNotFoundException(); // TODO: throw another exception in the future
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // TODO: Do not hard-code any values
	glfwWindowHint(GLFW_VERSION_MAJOR, 3); // TODO: Do not hard-code any values
	glfwWindowHint(GLFW_VERSION_MINOR, 3); // TODO: Do not hard-code any values
#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // So that glBegin / glEnd etc. work
#else
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	window = glfwCreateWindow(width, height, title.c_str(), NULL /* glfwGetPrimaryMonitor()- for fullscreen */, NULL); // Open a window and create its OpenGL context
	if (window == NULL)
	{
		LOG(Critical, LOGPLACE, "Failed to open GLFW window.  If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed below
	glfwSetCursorPos(window, width / 2, height / 2); // Set cursor position to the middle point
	//glfwSwapInterval(1);
	glfwSetTime(0.0);
	LOG(Debug, LOGPLACE, "Initializing GLFW finished successfully");
}

void Renderer::SetGlfwCallbacks()
{
	glfwSetWindowCloseCallback(window, &Game::WindowCloseEventCallback);
	//glfwSetWindowSizeCallback(window, Game::WindowResizeCallback);
	glfwSetKeyCallback(window, &Game::KeyEventCallback);
	//glfwSetCharCallback(window, Game::CharEventCallback);
	//glfwSetMousePosCallback(window, Game::MouseMotionCallback);
	glfwSetCursorPosCallback(window, &Game::MousePosCallback);
	glfwSetMouseButtonCallback(window, &Game::MouseEventCallback);
	glfwSetScrollCallback(window, &Game::ScrollEventCallback);
}

void Renderer::InitGlew() const
{
	LOG(Info, LOGPLACE, "Initializing GLEW...");
	glewExperimental = true; // Needed in core profile
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		LOG(Error, LOGPLACE, "Error while initializing GLEW: %s", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	if (GLEW_VERSION_2_0)
	{
		LOG(Info, LOGPLACE, "OpenGL 2.0 supported");
	}
	else
	{
		LOG(Info, LOGPLACE, "OpenGL 2.0 NOT supported");
		exit(EXIT_FAILURE);
	}

	LOG(Notice, LOGPLACE, "Using GLEW version %s", glewGetString(GLEW_VERSION));
}

void Renderer::Render(GameNode& gameNode)
{
	// TODO: Expand with Stencil buffer once it is used
	BindAsRenderTarget();
	ClearScreen();
	currentCamera = cameras[currentCameraIndex];
	gameNode.RenderAll(defaultShader, this); // Ambient rendering

	for (std::vector<BaseLight*>::iterator lightItr = lights.begin(); lightItr != lights.end(); ++lightItr)
	{
		currentLight = (*lightItr);
		ShadowInfo* shadowInfo = currentLight->GetShadowInfo();
		GetTexture("shadowMap")->BindAsRenderTarget(); // rendering to texture
		//glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
		ClearScreen();
		if (shadowInfo != NULL) // The currentLight casts shadows
		{
			altCamera.SetProjection(shadowInfo->GetProjection());
			altCamera.GetTransform().SetTranslation(currentLight->GetTransform().GetTransformedPos());
			altCamera.GetTransform().SetRotation(currentLight->GetTransform().GetTransformedRot());

			lightMatrix = BIAS_MATRIX * altCamera.GetViewProjection();

			Camera* temp = currentCamera;
			currentCamera = &altCamera;

			SetReal("shadowLightBleedingReductionFactor", shadowInfo->GetLightBleedingReductionAmount());
			SetReal("shadowVarianceMin", shadowInfo->GetMinVariance());

			if (shadowInfo->IsFlipFacesEnabled()) { glCullFace(GL_FRONT); }
			gameNode.RenderAll(shadowMapShader, this);
			if (shadowInfo->IsFlipFacesEnabled()) { glCullFace(GL_BACK); }

			currentCamera = temp;

			ApplyFilter(nullFilterShader, GetTexture("shadowMap"), GetTexture("shadowMapTempTarget"));
			ApplyFilter(nullFilterShader, GetTexture("shadowMapTempTarget"), GetTexture("shadowMap"));
			
			//BlurShadowMap(GetTexture("shadowMap"), shadowInfo->GetShadowSoftness());
		}
		BindAsRenderTarget();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both wages equal to 1
		glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore
		glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image

		gameNode.RenderAll(currentLight->GetShader(), this);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
}

void Renderer::BlurShadowMap(Texture* shadowMap, Real blurAmount)
{
	// TODO: Check if texture is ok
	SetVector3D("blurScale", Vector3D(static_cast<Real>(1.0f) / (shadowMap->GetWidth() * blurAmount), static_cast<Real>(0.0f), static_cast<Real>(0.0f)));
	ApplyFilter(gaussBlurFilterShader, shadowMap, GetTexture("shadowMapTempTarget"));
	
	SetVector3D("blurScale", Vector3D(static_cast<Real>(0.0f), static_cast<Real>(1.0f) / (shadowMap->GetHeight() * blurAmount), static_cast<Real>(0.0f)));
	ApplyFilter(gaussBlurFilterShader, GetTexture("shadowMapTempTarget"), shadowMap);
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
		LOG(Debug, LOGPLACE, "Binding window as a render target for filtering");
		BindAsRenderTarget();
	}
	else
	{
		LOG(Debug, LOGPLACE, "Binding texture as a render target for filtering");
		dest->BindAsRenderTarget();
	}

	LOG(Info, LOGPLACE, "Applying a filter to the source texture");
	
	SetTexture("filterTexture", source);

	altCamera.SetProjection(Matrix4D::Identity());
	altCamera.GetTransform().SetTranslation(Vector3D(static_cast<Real>(0.0f), static_cast<Real>(0.0f), static_cast<Real>(0.0f)));
	altCamera.GetTransform().SetRotation(Quaternion(Vector3D(static_cast<Real>(0.0f), static_cast<Real>(1.0f), static_cast<Real>(0.0f)), Angle(static_cast<Real>(180.0f))));

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
	//glClearColor();
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
		LOG(Notice, LOGPLACE, "Setting current camera index to 1");
		this->currentCameraIndex = 0;
	}
	else
	{
		this->currentCameraIndex = cameraIndex;
	}
	LOG(Notice, LOGPLACE, "Switched to camera #%d", this->currentCameraIndex + 1);
	LOG(Debug, LOGPLACE, "%s", cameras[this->currentCameraIndex]->ToString().c_str());
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
}

void Renderer::PrintGlReport()
{
	// TODO: Fix this function
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
