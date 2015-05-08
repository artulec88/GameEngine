#include "TestGameManager.h"
#include "MenuGameState.h"
//#include "PlayGameState.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshRenderer.h"
#include "Rendering\LookAtComponent.h"
#include "Rendering\Color.h"
#include "Rendering\LightBuilder.h"
#include "Rendering\LightBuilderDirector.h"

#include "Math\FloatingPoint.h"
#include "Math\Quaternion.h"
//#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

using namespace Game;
using namespace Utility;
using namespace Math;
using namespace Rendering;

#define RENDER_LIGHT_MESHES

TestGameManager::TestGameManager() :
	GameManager(),
	m_resourcesToLoad(32),
	m_resourcesLoaded(0),
	terrainNode(NULL),
	terrainMesh(NULL),
	CAMERA_HEIGHT_UPDATE_INTERVAL(0.01f),
	timeToUpdateCameraHeight(REAL_ZERO),
	boxNode(NULL),
#ifdef ANT_TWEAK_BAR_ENABLED 
	terrainMaterial(NULL),
	boxMaterial(NULL),
	terrainSpecularIntensity(GET_CONFIG_VALUE("defaultSpecularIntensity", 1.0f)),
	terrainSpecularPower(GET_CONFIG_VALUE("defaultSpecularPower", 8.0f)),
	terrainDisplacementScale(GET_CONFIG_VALUE("defaultDisplacementScale", 0.02f)),
	terrainDisplacementOffset(GET_CONFIG_VALUE("defaultDisplacementOffset", -0.5f)),
#endif
	humanCount(2),
	humanNodes(NULL),
	directionalLightNode(NULL),
	pointLightCount(GET_CONFIG_VALUE("pointLightsCount", 1)),
	pointLightNodes(NULL),
	spotLightCount(GET_CONFIG_VALUE("spotLightsCount", 1)),
	spotLightNodes(NULL),
	cameraCount(GET_CONFIG_VALUE("cameraCount", 3)),
	cameraNodes(NULL),
	m_heightMapCalculationEnabled(GET_CONFIG_VALUE("heightmapCalculationEnabled", true))
{
	LOG(Debug, LOGPLACE, "TestGame is being constructed");

	m_gameStateManager->Push(new MenuGameState());
	//m_gameStateManager->Push(new InGameState());
}


TestGameManager::~TestGameManager(void)
{
	SAFE_DELETE_JUST_TABLE(humanNodes);
	SAFE_DELETE_JUST_TABLE(pointLightNodes);
	SAFE_DELETE_JUST_TABLE(spotLightNodes);
	SAFE_DELETE_JUST_TABLE(cameraNodes);
	//SAFE_DELETE(terrainNode);
}

Math::Real TestGameManager::GetLoadingProgress() const
{
	if (m_resourcesLoaded > m_resourcesToLoad)
	{
		LOG(Warning, LOGPLACE, "Resources loaded (%d) exceeds the total number of expected resources (%d)",
			m_resourcesLoaded, m_resourcesToLoad);
		return REAL_ONE;
	}
	return static_cast<Math::Real>(m_resourcesLoaded) / m_resourcesToLoad;
}

void TestGameManager::Load()
{
	LOG(Notice, LOGPLACE, "Initalizing test game");
	ASSERT(!m_isGameLoaded);
	if (m_isGameLoaded)
	{
		LOG(Error, LOGPLACE, "Loading the game will not be performed. The game has already been loaded.");
		return;
	}

	//Material bricks(new Texture("..\\Textures\\bricks.jpg"), specularIntensity, specularPower, Texture("..\\Textures\\bricks_normal.jpg"), Texture("..\\Textures\\bricks_disp.png"), 0.03f, -0.5f);
	//Material bricks2("bricks2_material", Texture("..\\Textures\\bricks2.jpg"), 0.0f, 0, Texture("..\\Textures\\bricks2_normal.jpg"), Texture("..\\Textures\\bricks2_disp.jpg"), 0.04f, -1.0f);
	//Material humanMaterial("human_material", Texture("..\\Textures\\HumanSkin.jpg"), 2, 32);

	terrainNode = new GameNode();
	terrainMesh = new TerrainMesh("..\\Models\\" + GET_CONFIG_VALUE_STR("terrainModel", "terrain02.obj"));
#ifndef ANT_TWEAK_BAR_ENABLED
	Math::Real terrainSpecularIntensity = GET_CONFIG_VALUE("defaultSpecularIntensity", 1.0f);
	Math::Real terrainSpecularPower = GET_CONFIG_VALUE("defaultSpecularPower", 8.0f);
	Math::Real terrainDisplacementScale = GET_CONFIG_VALUE("defaultDisplacementScale", 0.02f);
	Math::Real terrainDisplacementOffset = GET_CONFIG_VALUE("defaultDisplacementOffset", -0.5f);
#endif
	terrainMaterial = new Material(new Texture("..\\Textures\\" + GET_CONFIG_VALUE_STR("terrainDiffuseTexture", "grass2.jpg")), terrainSpecularIntensity, terrainSpecularPower,
		new Texture("..\\Textures\\" + GET_CONFIG_VALUE_STR("terrainNormalMap", "grass_normal.jpg")),
		new Texture("..\\Textures\\" + GET_CONFIG_VALUE_STR("terrainDisplacementMap", "grass_disp.jpg")), terrainDisplacementScale, terrainDisplacementOffset);
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	terrainMaterial->SetAdditionalTexture(new Texture("..\\Textures\\" + GET_CONFIG_VALUE_STR("terrainDiffuseTexture2", "rocks2.jpg")), "diffuse2");
	m_resourcesLoaded += 1; // TODO: Consider creating some prettier solution. This is ugly
	terrainNode->AddComponent(new MeshRenderer(terrainMesh, terrainMaterial));
	//terrainNode->GetTransform().SetPos(0.0f, 0.0f, 5.0f);
	terrainNode->GetTransform().SetScale(30.0f);
	terrainMesh->TransformPositions(terrainNode->GetTransform().GetTransformation());
	//AddToSceneRoot(terrainNode); // Terrain node uses special shaders, so we don't actually add it to the game scene hierarchy. Instead we just register it for the renderer to use it.
	RegisterTerrainNode(terrainNode);

	boxNode = new GameNode();
#ifdef ANT_TWEAK_BAR_ENABLED
	boxMaterial = new Material(new Texture("..\\Textures\\crateBox2.jpg"), 1.0f, 2.0f);
	boxNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\SimpleCrate\\CrateModel.obj"), boxMaterial));
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
#else
	boxNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\SimpleCrate\\CrateModel.obj"), new Material(new Texture("..\\Textures\\crateBox2.jpg"), 1.0f, 2.0f)));
#endif
	boxNode->GetTransform().SetPos(12.0f, 3.5f, 9.0f);
	boxNode->GetTransform().SetScale(0.05f);
	AddToSceneRoot(boxNode);

	////Vector3D rayEndPosition = boxNode->GetTransform().GetTransformedPos() + boxNode->GetTransform().GetTransformedRot().GetForward() * 100.0f;
	////Vertex vertices [] = { Vertex(boxNode->GetTransform().GetTransformedPos()), Vertex(rayEndPosition) };
	////int indices [] = { 0, 1 };
	////boxNode->AddComponent(new MeshRenderer(
	////	new Mesh(vertices, 2, indices, 2, true, GL_LINES),
	////	new Material(new Texture("..\\Textures\\DirectionalLight.png"))));

	GameNode* boxNode2 = new GameNode();
	boxNode2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\cube.obj"),
		new Material(new Texture("..\\Textures\\bricks2.jpg"), 1.0f, 8.0f,
			new Texture("..\\Textures\\bricks2_normal.jpg"),
			new Texture("..\\Textures\\bricks2_disp.jpg"), 0.04f, -1.0f)));
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	boxNode2->GetTransform().SetPos(8.0f, 0.5f, 5.0f);
	boxNode2->GetTransform().Rotate(Quaternion(Vector3D(0.0f, 1.0f, 0.0f), Angle(30.0f)));
	boxNode2->GetTransform().SetScale(0.5f);
	AddToSceneRoot(boxNode2);

	GameNode* testMesh1 = new GameNode();
	testMesh1->GetTransform().SetPos(-2.0f, 2.5f, 2.0f);
	testMesh1->GetTransform().SetScale(1.5f);
	GameNode* testMesh2 = new GameNode();
	testMesh2->GetTransform().SetPos(9.0f, 0.0f, 0.0f);
	//testMesh2->GetTransform().SetScale(1.5f);
	testMesh2->GetTransform().SetRot(Quaternion(Matrix4D::RotationEuler(Angle(90.0f), Angle(90.0f), Angle(0.0f))));
	testMesh1->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane.obj"),
		new Material(new Texture("..\\Textures\\bricks2.jpg"), 0.0f, 0, new Texture("..\\Textures\\bricks2_normal.jpg"), new Texture("..\\Textures\\bricks2_disp.jpg"), 0.04f, -1.0f)));
	m_resourcesLoaded += 4; // TODO: Consider creating some prettier solution. This is ugly
	testMesh2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane.obj"),
		new Material(new Texture("..\\Textures\\bricks2.jpg"), 0.0f, 0)));
	AddToSceneRoot(testMesh1);
	//AddToSceneRoot(testMesh2);
	testMesh1->AddChild(testMesh2);

	GameNode* monkeyNode1 = new GameNode();
	monkeyNode1->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\monkey4.obj"),
		new Material(new Texture("..\\Textures\\chessboard3.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR), 1.0f, 8.0f)));
	monkeyNode1->AddComponent(new LookAtComponent());
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	monkeyNode1->GetTransform().SetPos(-5.0f, 3.75f, 4.0f);
	//monkeyNode1->GetTransform().SetRotation(Quaternion(Vector3D(0, 1, 0), Angle(-45)));
	//stdlog(Info, LOGPLACE, "MonkeyNode1 has ID=%d", monkeyNode1->GetID());
	//monkeyNode1->AddComponent(new LookAtComponent());
	AddToSceneRoot(monkeyNode1);

	GameNode* monkeyNode2 = new GameNode();
	monkeyNode2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\monkey3.obj"),
		new Material(new Texture("..\\Textures\\bricks.jpg"), 2.0f, 32.0f)));
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	monkeyNode2->GetTransform().SetPos(5.0, 3.0, 15.0);
	//monkeyNode2->AddComponent(new LookAtComponent());
	AddToSceneRoot(monkeyNode2);

	GameNode* earthNode = new GameNode();
	earthNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\earth.obj"), new Material(new Texture("..\\Textures\\earth.jpg"))));
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly
	earthNode->GetTransform().SetPos(12.0f, 2.0f, -2.0f);
	AddToSceneRoot(earthNode);

	GameNode* sphereNode = new GameNode();
	sphereNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\sphere.obj"), new Material(new Texture("..\\Textures\\rust.jpg"))));
	m_resourcesLoaded += 2;
	sphereNode->GetTransform().SetPos(-3.0f, 1.5f, -3.0f);
	AddToSceneRoot(sphereNode);

	GameNode* sphereNode2 = new GameNode();
	sphereNode2->AddComponent(new MeshRenderer(new Mesh("..\\Models\\sphere.obj"), new Material(new Texture("..\\Textures\\rust.jpg"))));
	m_resourcesLoaded += 2;
	sphereNode2->GetTransform().SetPos(-3.0f, 2.0f, 3.0f);
	AddToSceneRoot(sphereNode2);

	GameNode* sphereNode3 = new GameNode();
	sphereNode3->AddComponent(new MeshRenderer(new Mesh("..\\Models\\sphere.obj"), new Material(new Texture("..\\Textures\\rust.jpg"))));
	m_resourcesLoaded += 2;
	sphereNode3->GetTransform().SetPos(3.0f, 2.5f, -3.0f);
	AddToSceneRoot(sphereNode3);

	GameNode* sphereNode4 = new GameNode();
	sphereNode4->AddComponent(new MeshRenderer(new Mesh("..\\Models\\sphere.obj"), new Material(new Texture("..\\Textures\\rust.jpg"))));
	m_resourcesLoaded += 2;
	sphereNode4->GetTransform().SetPos(3.0f, 3.0f, 3.0f);
	AddToSceneRoot(sphereNode4);

	srand((unsigned int)time(NULL));

	humanNodes = new GameNode* [humanCount];
	for (int i = 0; i < humanCount; ++i)
	{
		humanNodes[i] = new GameNode();
		humanNodes[i]->AddComponent(new MeshRenderer(new Mesh("..\\Models\\BodyMesh.obj"), new Material(new Texture("..\\Textures\\HumanSkin.jpg"), 2.0f, 32.0f)));
		humanNodes[i]->GetTransform().SetPos(static_cast<Real>(rand() % 20), 1.7f, static_cast<Real>(rand() % 20));
		AddToSceneRoot(humanNodes[i]);
	}
	m_resourcesLoaded += 2; // TODO: Consider creating some prettier solution. This is ugly

	//castleNode = new GameNode();
	//castleNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\castle.obj"), new Material(new Texture("..\\Textures\\HumanSkin.jpg"), 0.5f, 8.0f)));
	//castleNode->GetTransform().SetPos(static_cast<Real>(rand() % 50), 1.0f, static_cast<Real>(rand() % 50));
	//AddToSceneRoot(castleNode);

	GameNode* teapotNode = new GameNode();
	teapotNode->GetTransform().SetPos(3.0f, 4.0f, REAL_ONE);
	teapotNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\teapot.obj"), new Material(new Texture("..\\Textures\\chessboard3.jpg"))));
	m_resourcesLoaded += 1; // TODO: Consider creating some prettier solution. This is ugly
	AddToSceneRoot(teapotNode);

	AddLights(); // Adding all kinds of light (directional, point, spot)

	AddCameras(); // Adding cameras

	ASSERT(m_isGameLoaded);
	m_isGameLoaded = true;
	if (!m_isGameLoaded)
	{
		LOG(Error, LOGPLACE, "The game has not been loaded properly.");
		exit(EXIT_FAILURE);
	}
	LOG(Notice, LOGPLACE, "Initalizing test game finished");
}

void TestGameManager::AddLights()
{
	AddDirectionalLight(); // Adding directional light (if enabled)
	if (pointLightCount > 0)
	{
		LOG(Notice, LOGPLACE, "Creating %d point lights", pointLightCount);
		AddPointLights();
		LOG(Debug, LOGPLACE, "%d point lights created", pointLightCount);
	}
	if (spotLightCount > 0)
	{
		LOG(Notice, LOGPLACE, "Creating %d spot lights...", spotLightCount);
		AddSpotLights();
		LOG(Debug, LOGPLACE, "%d spot lights created", spotLightCount);
	}
}

void TestGameManager::AddDirectionalLight()
{
	// TODO: For now we only check if directionalLightsCount is zero or not.
	// In the future there might be many directional lights enabled (?)
	int directionalLightsCount = GET_CONFIG_VALUE("directionalLightsCount", 1);
	if (directionalLightsCount == 0)
	{
		LOG(Notice, LOGPLACE, "Directional lights disabled");
		return;
	}
	LOG(Info, LOGPLACE, "Directional lights enabled");
	directionalLightNode = new GameNode();

	const Vector3D defaultDirectionalLightPos(GET_CONFIG_VALUE("defaultDirectionalLightPosX", 0.0f), GET_CONFIG_VALUE("defaultDirectionalLightPosY", 0.0f), GET_CONFIG_VALUE("defaultDirectionalLightPosZ", 0.0f));
	const Angle defaultDirectionalLightRotationX(GET_CONFIG_VALUE("defaultDirectionalLightAngleX", -45.0f));
	const Angle defaultDirectionalLightRotationY(GET_CONFIG_VALUE("defaultDirectionalLightAngleY", 0.0f));
	const Angle defaultDirectionalLightRotationZ(GET_CONFIG_VALUE("defaultDirectionalLightAngleZ", 0.0f));

	Real xPos = GET_CONFIG_VALUE("directionalLightPosX", defaultDirectionalLightPos.GetX());
	Real yPos = GET_CONFIG_VALUE("directionalLightPosY", defaultDirectionalLightPos.GetY());
	Real zPos = GET_CONFIG_VALUE("directionalLightPosZ", defaultDirectionalLightPos.GetZ());
	Vector3D position(xPos, yPos, zPos);
	directionalLightNode->GetTransform().SetPos(position);

	Angle angleX(GET_CONFIG_VALUE("directionalLightAngleX", defaultDirectionalLightRotationX.GetAngleInDegrees()));
	Angle angleY(GET_CONFIG_VALUE("directionalLightAngleY", defaultDirectionalLightRotationY.GetAngleInDegrees()));
	Angle angleZ(GET_CONFIG_VALUE("directionalLightAngleZ", defaultDirectionalLightRotationZ.GetAngleInDegrees()));
	Matrix4D rotMatrix = Matrix4D::RotationEuler(angleX, angleY, angleZ);
	LOG(Debug, LOGPLACE, "angleX=%.1f, angleY=%.1f, angleZ=%.1f, rotMatrix =\n%s", angleX.GetAngleInDegrees(), angleY.GetAngleInDegrees(), angleZ.GetAngleInDegrees(), rotMatrix.ToString().c_str());
	Quaternion rot(rotMatrix);
	Quaternion rot2(Vector3D(1, 0, 0), angleX);
	LOG(Debug, LOGPLACE, "rotMatrix =\n%s\n rot =\n%s\n rot.ToRotationMatrix() =\n%s\n rot2.ToRotationMatrix() = \n%s",
		rotMatrix.ToString().c_str(),
		rot.ToString().c_str(),
		rot.ToRotationMatrix().ToString().c_str(),
		rot2.ToRotationMatrix().ToString().c_str());
	directionalLightNode->GetTransform().SetRot(rot);
	//directionalLightNode->GetTransform().SetRot(Quaternion(Vector3D(1, 0, 0), Angle(90.0f)));
	//directionalLightNode->GetTransform().Rotate(Vector3D(0, 1, 0), Angle(45.0f));

	Rendering::DirectionalLightBuilder directionalLightBuilder;
	Rendering::LightBuilderDirector lightBuilderDirector(directionalLightBuilder);
	lightBuilderDirector.Construct();
	directionalLightNode->AddComponent(directionalLightBuilder.GetLight());
	AddToSceneRoot(directionalLightNode);

	// Rendering a small box around point light node position to let the user see the source
#ifdef RENDER_LIGHT_MESHES
	//Material directionalLightMaterial("directionalLight_material", Texture("..\\Textures\\DirectionalLight.png"), 1, 8);
	//Material directionalLightLineMaterial("directionalLightLine_material", Texture("..\\Textures\\DirectionalLight.png"), 1, 8);

	directionalLightNode->GetTransform().SetScale(0.4f);
	directionalLightNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\DirectionalLight.obj"),
		new Material(new Texture("..\\Textures\\DirectionalLight.png"), 1.0f, 8.0f)));
		
	Vector3D forwardVec = directionalLightNode->GetTransform().GetTransformedRot().GetForward().Normalized();
	Vector3D rayEndPosition = forwardVec * 2.0f;
	//LOG(Delocust, LOGPLACE, "light position = %s;\t light rotation = %s;\t light forward vector = %s;\t light end pos = %s",
	//	position.ToString().c_str(),
	//	directionalLightNode->GetTransform().GetTransformedRot().ToString().c_str(),
	//	forwardVec.ToString().c_str(),
	//	(position + rayEndPosition).ToString().c_str());
	Vertex vertices [] = { Vertex(Vector3D()), Vertex(rayEndPosition) };
	int indices [] = { 0, 1 };
	directionalLightNode->AddComponent(new MeshRenderer(
		new Mesh(vertices, 2, indices, 2, false, GL_LINES),
		new Material(new Texture("..\\Textures\\DirectionalLight.png"), 1.0f, 8.0f)));
#endif
}

void TestGameManager::AddPointLights()
{
	if (pointLightCount < 1)
	{
		return;
	}

	const Vector3D defaultPointLightPos(GET_CONFIG_VALUE("defaultPointLightPosX", 0.0f), GET_CONFIG_VALUE("defaultPointLightPosY", 0.0f), GET_CONFIG_VALUE("defaultPointLightPosZ", 0.0f));
	const Angle defaultPointLightRotationX(GET_CONFIG_VALUE("defaultPointLightAngleX", -45.0f));
	const Angle defaultPointLightRotationY(GET_CONFIG_VALUE("defaultPointLightAngleY", 0.0f));
	const Angle defaultPointLightRotationZ(GET_CONFIG_VALUE("defaultPointLightAngleZ", 0.0f));
	const Color defaultPointLightColor(GET_CONFIG_VALUE("defaultPointLightColorRed", 0.0f), GET_CONFIG_VALUE("defaultPointLightColorGreen", 0.0f), GET_CONFIG_VALUE("defaultPointLightColorBlue", 1.0f));
	const Real defaultPointLightIntensity(GET_CONFIG_VALUE("defaultPointLightIntensity", 10.0f));
	const Attenuation defaultPointLightAttenuation(GET_CONFIG_VALUE("defaultPointLightAttenuationConstant", 0.0f), GET_CONFIG_VALUE("defaultPointLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE("defaultPointLightAttenuationExponent", 0.0f));
	pointLightNodes = new GameNode* [pointLightCount];
	for (int i = 0; i < pointLightCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		std::string pointLightIndexStr = ss.str();

		pointLightNodes[i] = new GameNode();
		
		Real xPos = GET_CONFIG_VALUE("pointLightPosX_" + pointLightIndexStr, defaultPointLightPos.GetX());
		Real yPos = GET_CONFIG_VALUE("pointLightPosY_" + pointLightIndexStr, defaultPointLightPos.GetY());
		Real zPos = GET_CONFIG_VALUE("pointLightPosZ_" + pointLightIndexStr, defaultPointLightPos.GetZ());
		pointLightNodes[i]->GetTransform().SetPos(xPos, yPos, zPos);
		
		Angle angleX(GET_CONFIG_VALUE("pointLightAngleX_" + pointLightIndexStr, defaultPointLightRotationX.GetAngleInDegrees()));
		Angle angleY(GET_CONFIG_VALUE("pointLightAngleY_" + pointLightIndexStr, defaultPointLightRotationY.GetAngleInDegrees()));
		Angle angleZ(GET_CONFIG_VALUE("pointLightAngleZ_" + pointLightIndexStr, defaultPointLightRotationZ.GetAngleInDegrees()));
		Matrix4D rotMatrix = Matrix4D::RotationEuler(angleX, angleY, angleZ);
		Quaternion rot(rotMatrix);
		pointLightNodes[i]->GetTransform().SetRot(rot);

		Real red = GET_CONFIG_VALUE("pointLightColorRed_" + pointLightIndexStr, defaultPointLightColor.GetRed());
		Real green = GET_CONFIG_VALUE("pointLightColorGreen_" + pointLightIndexStr, defaultPointLightColor.GetGreen());
		Real blue = GET_CONFIG_VALUE("pointLightColorBlue_" + pointLightIndexStr, defaultPointLightColor.GetBlue());
		Color color(red, green, blue);
		
		Real intensity = GET_CONFIG_VALUE("pointLightIntensity_" + pointLightIndexStr, defaultPointLightIntensity);
		
		Real constant = GET_CONFIG_VALUE("pointLightAttenuationConstant_" + pointLightIndexStr, defaultPointLightAttenuation.GetConstant());
		Real linear = GET_CONFIG_VALUE("pointLightAttenuationLinear_" + pointLightIndexStr, defaultPointLightAttenuation.GetLinear());
		Real exponent = GET_CONFIG_VALUE("pointLightAttenuationExponent_" + pointLightIndexStr, defaultPointLightAttenuation.GetExponent());
		Attenuation attenuation(constant, linear, exponent);

		pointLightNodes[i]->AddComponent(new PointLight(color, intensity, attenuation));
		
		// Rendering a small box around point light node position to let the user see the source
#ifdef RENDER_LIGHT_MESHES
		pointLightNodes[i]->AddComponent(new MeshRenderer(
			new Mesh("..\\Models\\PointLight.obj"),
			new Material(new Texture("..\\Textures\\PointLight.png"), 1.0f, 8.0f)));
		pointLightNodes[i]->GetTransform().SetScale(0.1f);
#endif
		
		AddToSceneRoot(pointLightNodes[i]);
	}
}

void TestGameManager::AddSpotLights()
{
	if (spotLightCount < 1)
	{
		return;
	}
	const Vector3D defaultSpotLightPos(GET_CONFIG_VALUE("defaultSpotLightPosX", 0.0f), GET_CONFIG_VALUE("defaultSpotLightPosY", 0.0f), GET_CONFIG_VALUE("defaultSpotLightPosZ", 0.0f));
	const Angle defaultSpotLightRotationX(GET_CONFIG_VALUE("defaultSpotLightAngleX", -45.0f));
	const Angle defaultSpotLightRotationY(GET_CONFIG_VALUE("defaultSpotLightAngleY", 0.0f));
	const Angle defaultSpotLightRotationZ(GET_CONFIG_VALUE("defaultSpotLightAngleZ", 0.0f));
	const Color defaultSpotLightColor(GET_CONFIG_VALUE("defaultSpotLightColorRed", 0.0f), GET_CONFIG_VALUE("defaultSpotLightColorGreen", 0.0f), GET_CONFIG_VALUE("defaultSpotLightColorBlue", 1.0f));
	const Real defaultSpotLightIntensity(GET_CONFIG_VALUE("defaultSpotLightIntensity", 4.0f));
	const Attenuation defaultSpotLightAttenuation(GET_CONFIG_VALUE("defaultSpotLightAttenuationConstant", 0.5f), GET_CONFIG_VALUE("defaultSpotLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE("defaultSpotLightAttenuationExponent", 0.05f));
	const Angle defaultSpotLightViewAngle(GET_CONFIG_VALUE("defaultSpotLightViewAngle", 120.0f), Unit::DEGREE);
	const int defaultSpotLightShadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("defaultSpotLightShadowMapSizeAsPowerOf2", 10); // 2 ^ 10 = 1024
	const Real defaultSpotLightShadowSoftness(GET_CONFIG_VALUE("defaultSpotLightShadowSoftness", 1.0f));
	const Real defaultSpotLightLightBleedingReductionAmount(GET_CONFIG_VALUE("defaultSpotLightLightBleedingReductionAmount", 0.2f));
	const Real defaultSpotLightMinVariance(GET_CONFIG_VALUE("defaultSpotLightMinVariance", 0.00002f));
	spotLightNodes = new GameNode* [spotLightCount];
	for (int i = 0; i < spotLightCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		std::string spotLightIndexStr = ss.str();

		spotLightNodes[i] = new GameNode();
		
		Real xPos = GET_CONFIG_VALUE("spotLightPosX_" + spotLightIndexStr, defaultSpotLightPos.GetX());
		Real yPos = GET_CONFIG_VALUE("spotLightPosY_" + spotLightIndexStr, defaultSpotLightPos.GetY());
		Real zPos = GET_CONFIG_VALUE("spotLightPosZ_" + spotLightIndexStr, defaultSpotLightPos.GetZ());
		spotLightNodes[i]->GetTransform().SetPos(xPos, yPos, zPos);
		
		Angle angleX(GET_CONFIG_VALUE("spotLightAngleX_" + spotLightIndexStr, defaultSpotLightRotationX.GetAngleInDegrees()));
		Angle angleY(GET_CONFIG_VALUE("spotLightAngleY_" + spotLightIndexStr, defaultSpotLightRotationY.GetAngleInDegrees()));
		Angle angleZ(GET_CONFIG_VALUE("spotLightAngleZ_" + spotLightIndexStr, defaultSpotLightRotationZ.GetAngleInDegrees()));
		Matrix4D rotMatrix = Matrix4D::RotationEuler(angleX, angleY, angleZ);
		Quaternion rot(rotMatrix);
		spotLightNodes[i]->GetTransform().SetRot(rot);

		Real red = GET_CONFIG_VALUE("spotLightColorRed_" + spotLightIndexStr, defaultSpotLightColor.GetRed());
		Real green = GET_CONFIG_VALUE("spotLightColorGreen_" + spotLightIndexStr, defaultSpotLightColor.GetGreen());
		Real blue = GET_CONFIG_VALUE("spotLightColorBlue_" + spotLightIndexStr, defaultSpotLightColor.GetBlue());
		Color color(red, green, blue);
		
		Real intensity = GET_CONFIG_VALUE("spotLightIntensity_" + spotLightIndexStr, defaultSpotLightIntensity);
		
		Real constant = GET_CONFIG_VALUE("spotLightAttenuationConstant_" + spotLightIndexStr, defaultSpotLightAttenuation.GetConstant());
		Real linear = GET_CONFIG_VALUE("spotLightAttenuationLinear_" + spotLightIndexStr, defaultSpotLightAttenuation.GetLinear());
		Real exponent = GET_CONFIG_VALUE("spotLightAttenuationExponent_" + spotLightIndexStr, defaultSpotLightAttenuation.GetExponent());
		Attenuation attenuation(constant, linear, exponent);

		Angle viewAngle(GET_CONFIG_VALUE("spotLightViewAngle_" + spotLightIndexStr, defaultSpotLightViewAngle.GetAngleInRadians()), Unit::RADIAN);
		
		int shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("spotLightShadowMapSizeAsPowerOf2_" + spotLightIndexStr, 10); // 2 ^ 10 = 1024
		Real shadowSoftness = GET_CONFIG_VALUE("spotLightShadowSoftness_" + spotLightIndexStr, defaultSpotLightShadowSoftness);
		Real lightBleedingReductionAmount = GET_CONFIG_VALUE("spotLightLightBleedingReductionAmount_" + spotLightIndexStr, defaultSpotLightLightBleedingReductionAmount);
		Real minVariance = GET_CONFIG_VALUE("spotLightMinVariance_" + spotLightIndexStr, defaultSpotLightMinVariance);

		spotLightNodes[i]->AddComponent(new SpotLight(color, intensity, attenuation, viewAngle, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance));
		
		// Rendering a small box around spot light node position to let the user see the source
#ifdef RENDER_LIGHT_MESHES
		spotLightNodes[i]->AddComponent(new MeshRenderer(
			new Mesh("..\\Models\\SpotLight.obj"),
			new Material(new Texture("..\\Textures\\SpotLight.png"), 1.0f, 8.0f)));
		spotLightNodes[i]->GetTransform().SetScale(0.1f);
#endif
		
		AddToSceneRoot(spotLightNodes[i]);
	}
}

void TestGameManager::AddCameras()
{
	if (cameraCount < 1)
	{
		LOG(Error, LOGPLACE, "No cameras defined.");
		exit(EXIT_FAILURE);
	}
	
	LOG(Notice, LOGPLACE, "Creating %d camera(-s)...", cameraCount);

	const Real defaultFoV = GET_CONFIG_VALUE("defaultCameraFoV", 70.0f);
	const Real defaultAspectRatio = GET_CONFIG_VALUE("defaultCameraAspectRatio", static_cast<Real>(800) / 600);
	const Real defaultNearPlane = GET_CONFIG_VALUE("defaultCameraNearPlane", 0.1f);
	const Real defaultFarPlane = GET_CONFIG_VALUE("defaultCameraFarPlane", 1000.0f);
	
	const Vector3D defaultCameraPos(GET_CONFIG_VALUE("defaultCameraPosX", 0.0f), GET_CONFIG_VALUE("defaultCameraPosY", 0.0f), GET_CONFIG_VALUE("defaultCameraPosZ", 0.0f));
	const Angle defaultCameraRotationX(GET_CONFIG_VALUE("defaultCameraAngleX", -45.0f));
	const Angle defaultCameraRotationY(GET_CONFIG_VALUE("defaultCameraAngleY", 0.0f));
	const Angle defaultCameraRotationZ(GET_CONFIG_VALUE("defaultCameraAngleZ", 0.0f));

	cameraNodes = new GameNode* [cameraCount];
	for (int i = 0; i < cameraCount; ++i)
	{
		std::stringstream ss("");
		ss << (i + 1);
		std::string cameraIndexStr = ss.str();

		cameraNodes[i] = new GameNode();
		Real xPos = GET_CONFIG_VALUE("cameraPosX_" + cameraIndexStr, defaultCameraPos.GetX());
		Real yPos = GET_CONFIG_VALUE("cameraPosY_" + cameraIndexStr, defaultCameraPos.GetY());
		Real zPos = GET_CONFIG_VALUE("cameraPosZ_" + cameraIndexStr, defaultCameraPos.GetZ());
		cameraNodes[i]->GetTransform().SetPos(xPos, yPos, zPos);
		
		Angle angleX(GET_CONFIG_VALUE("cameraAngleX_" + cameraIndexStr, defaultCameraRotationX.GetAngleInDegrees()));
		Angle angleY(GET_CONFIG_VALUE("cameraAngleY_" + cameraIndexStr, defaultCameraRotationY.GetAngleInDegrees()));
		Angle angleZ(GET_CONFIG_VALUE("cameraAngleZ_" + cameraIndexStr, defaultCameraRotationZ.GetAngleInDegrees()));
		Matrix4D rotMatrix = Matrix4D::RotationEuler(angleX, angleY, angleZ);
		LOG(Delocust, LOGPLACE, "angleX=%.1f, angleY=%.1f, angleZ=%.1f", angleX.GetAngleInDegrees(), angleY.GetAngleInDegrees(), angleZ.GetAngleInDegrees());
		Quaternion rot(rotMatrix);
		Quaternion rot2(Vector3D(1, 0, 0), angleX);
		LOG(Delocust, LOGPLACE, "rotMatrix =\n%s\n rot =\n%s\n rot.ToRotationMatrix() =\n%s\n rot2.ToRotationMatrix() = \n%s",
			rotMatrix.ToString().c_str(),
			rot.ToString().c_str(),
			rot.ToRotationMatrix().ToString().c_str(),
			rot2.ToRotationMatrix().ToString().c_str());
		cameraNodes[i]->GetTransform().SetRot(rot);

		Angle fov(GET_CONFIG_VALUE("cameraFoV_" + cameraIndexStr, defaultFoV), Unit::DEGREE);
		Real aspectRatio = GET_CONFIG_VALUE("cameraAspectRatio_" + cameraIndexStr, defaultAspectRatio);
		Real zNearPlane = GET_CONFIG_VALUE("cameraNearPlane_" + cameraIndexStr, defaultNearPlane);
		Real zFarPlane = GET_CONFIG_VALUE("cameraFarPlane_" + cameraIndexStr, defaultFarPlane);
		cameraNodes[i]->AddComponent(new CameraComponent(fov, aspectRatio, zNearPlane, zFarPlane));
		//testMesh2->AddChild(cameraNodes[i]);
		AddToSceneRoot(cameraNodes[i]);
	}
	LOG(Debug, LOGPLACE, "%d camera(-s) created", cameraCount);
}

//
//void Game::CleanUp()
//{
//	stdlog(Info, LOGPLACE, "The game is being cleaned up");
//}

void TestGameManager::Input(Real delta)
{
	m_gameStateManager->Input(delta);
}

// TODO: Remove in the future
Real temp = 0.0;

Vector3D velocity;
Real maxSpeed = 0.02f;
bool isMouseLocked = false;

void TestGameManager::Update(Real delta)
{
	m_gameStateManager->Update(delta);
}

void TestGameManager::WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	GameManager::WindowResizeEvent(window, width, height);
}

/**
 * @param key the keyboard key that was pressed or released
 * @param scancode the system-specific scancode of the key
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods Bit field describing which modifier keys were held down
 */
void TestGameManager::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GameManager::KeyEvent(window, key, scancode, action, mods);
	m_gameStateManager->KeyEvent(key, scancode, action, mods);
}

/**
 * GLFW_MOUSE_BUTTON_1 = left mouse button
 * GLFW_MOUSE_BUTTON_2 = right mouse button
 * GLFW_MOUSE_BUTTON_3 = middle mouse button
 */
void TestGameManager::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	// TODO: Pass the event to the Input function in the current game state.
	// TODO: Create additional functions for mouse, keyboard events (see IInputable class)
	// I would expect here something as follows:
	// currentGameState->MouseInput(...)
	
	// LOG(Debug, LOGPLACE, "Mouse event: button=%d\t action=%d\t mods=%d", button, action, mods);

	//GameManager::MouseButtonEvent(window, button, action, mods);
	m_gameStateManager->MouseButtonEvent(button, action, mods);
}

void TestGameManager::MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{
	m_gameStateManager->MousePosEvent(xPos, yPos);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void TestGameManager::InitializeTweakBars()
{
#ifdef GAME_PROPERTIES_TWEAK_BAR
	if (!m_isGameLoaded)
	{
		LOG(Warning, LOGPLACE, "Cannot initialize game's tweak bars. The game has not been loaded yet.");
		return;
	}
	LOG(Info, LOGPLACE, "Initializing game's tweak bars");
	// TODO: GAME_PROPERTIES_TWEAK_BAR gives some errors. Investigate why and fix that!

	TwBar* testGamePropertiesBar = TwNewBar("TestGamePropertiesBar");
	TwAddVarRW(testGamePropertiesBar, "heightMapCalculationEnabled", TW_TYPE_BOOLCPP, &m_heightMapCalculationEnabled, " label='Heightmap calculation enabled' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainSpecularIntensity", TW_TYPE_FLOAT, &terrainSpecularIntensity, " label='Terrain specular intensity' group='Terrain' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainSpecularPower", TW_TYPE_FLOAT, &terrainSpecularPower, " label='Terrain specular power' group='Terrain' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainDisplacementScale", TW_TYPE_FLOAT, &terrainDisplacementScale, " label='Terrain displacement scale' group='Terrain' ");
	//TwAddVarRW(testGamePropertiesBar, "terrainDisplacementOffset", TW_TYPE_FLOAT, &terrainDisplacementOffset, " label='Terrain displacement offset' group='Terrain' ");
	if (terrainMaterial == NULL)
	{
		LOG(Error, LOGPLACE, "Cannot add terrain material information to tweak bar. The terrain material is NULL.");
		return;
	}

	//TwAddVarRO(testGamePropertiesBar, "temp1", TW_TYPE_INT32, &humanCount, " label='Human count' group='Terrain' ");
	//TwAddVarRO(testGamePropertiesBar, "temp2", TW_TYPE_INT32, &pointLightCount, " label='Human count' group='Box' ");

	terrainMaterial->SetVector3D("Vec1", Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO));
	terrainMaterial->InitializeTweakBar(testGamePropertiesBar, "Terrain");
	//boxMaterial->SetVector3D("Vec1", Math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ONE));
	//boxMaterial->InitializeTweakBar(testGamePropertiesBar, "Box");

	//TwRemoveVar(testGamePropertiesBar, "temp1");
	//TwRemoveVar(testGamePropertiesBar, "temp2");

	TwSetParam(testGamePropertiesBar, NULL, "visible", TW_PARAM_CSTRING, 1, "true"); // Hide the bar at startup
	LOG(Info, LOGPLACE, "Initializing game's tweak bars finished");
#endif
}
#endif
