#include "TestGame.h"

#include "Rendering\CoreEngine.h"
#include "Rendering\Camera.h"
#include "Rendering\MeshRenderer.h"
#include "Rendering\LookAtComponent.h"
#include "Rendering\Color.h"

#include "Math\FloatingPoint.h"
#include "Math\Quaternion.h"
//#include "Math\Vector.h"

//#include "Utility\FileNotFoundException.h" // TODO: Remove in the future when not needed
#include "Utility\ILogger.h"
#include "Utility\IConfig.h"

#include <sstream>

using namespace Utility;
using namespace Math;
using namespace Rendering;

#define RENDER_LIGHT_MESHES
//#define SIMULATE_SUN_BEHAVIOR

TestGame::TestGame() :
	Game(),
	planeNode(NULL),
	boxNode(NULL),
#ifdef ANT_TWEAK_BAR_ENABLED
	planeMaterial(NULL),
	boxMaterial(NULL),
	planeSpecularIntensity(GET_CONFIG_VALUE("defaultSpecularIntensity", 1.0f)),
	planeSpecularPower(GET_CONFIG_VALUE("defaultSpecularPower", 8.0f)),
	planeDisplacementScale(GET_CONFIG_VALUE("defaultDisplacementScale", 0.02f)),
	planeDisplacementOffset(GET_CONFIG_VALUE("defaultDisplacementOffset", -0.5f)),
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
	skyboxNode(NULL)
{
	LOG(Debug, LOGPLACE, "TestGame is being constructed");
}


TestGame::~TestGame(void)
{
	SAFE_DELETE_JUST_TABLE(humanNodes);
	SAFE_DELETE_JUST_TABLE(pointLightNodes);
	SAFE_DELETE_JUST_TABLE(spotLightNodes);
	SAFE_DELETE_JUST_TABLE(cameraNodes);
	//SAFE_DELETE(planeNode);
}

void TestGame::Init()
{
	LOG(Notice, LOGPLACE, "Initalizing test game");
	Game::Init();

	//Material bricks(new Texture("..\\Textures\\bricks.jpg"), specularIntensity, specularPower, Texture("..\\Textures\\bricks_normal.jpg"), Texture("..\\Textures\\bricks_disp.png"), 0.03f, -0.5f);
	//Material bricks2("bricks2_material", Texture("..\\Textures\\bricks2.jpg"), 0.0f, 0, Texture("..\\Textures\\bricks2_normal.jpg"), Texture("..\\Textures\\bricks2_disp.jpg"), 0.04f, -1.0f);
	//Material humanMaterial("human_material", Texture("..\\Textures\\HumanSkin.jpg"), 2, 32);

	planeNode = new GameNode();
#ifdef ANT_TWEAK_BAR_ENABLED
	//planeMaterial = new Material(new Texture("..\\Textures\\grass.jpg"), planeSpecularIntensity, planeSpecularPower,
	//	new Texture("..\\Textures\\bricks_normal.jpg"), new Texture("..\\Textures\\bricks_disp.png"), planeDisplacementScale, planeDisplacementOffset);
	//planeNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\terrain02.obj"), planeMaterial));
	planeMaterial = new Material(new Texture("..\\Textures\\grass.jpg"), planeSpecularIntensity, planeSpecularPower,
		new Texture("..\\Textures\\grass_normal.jpg"), new Texture("..\\Textures\\bricks_disp.png"), planeDisplacementScale, planeDisplacementOffset);
	planeNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\terrain02.obj"), planeMaterial));
#else
	Math::Real planeSpecularIntensity = GET_CONFIG_VALUE("defaultSpecularIntensity", 1.0f);
	Math::Real planeSpecularPower = GET_CONFIG_VALUE("defaultSpecularPower", 8.0f);
	Math::Real planeDisplacementScale = GET_CONFIG_VALUE("defaultDisplacementScale", 0.02f);
	Math::Real planeDisplacementOffset = GET_CONFIG_VALUE("defaultDisplacementOffset", -0.5f);
	planeNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\terrain02.obj"),
		new Material(new Texture("..\\Textures\\grass.jpg"), planeSpecularIntensity, planeSpecularPower,
		new Texture("..\\Textures\\bricks_normal.jpg"), new Texture("..\\Textures\\bricks_disp.png"), planeDisplacementScale, planeDisplacementOffset)));
#endif
	planeNode->GetTransform().SetPos(0.0f, 0.0f, 5.0f);
	planeNode->GetTransform().SetScale(15.0f);
	AddToSceneRoot(planeNode);

	boxNode = new GameNode();
#ifdef ANT_TWEAK_BAR_ENABLED
	boxMaterial = new Material(new Texture("..\\Textures\\crateBox2.jpg"), 1.0f, 2.0f);
	boxNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\SimpleCrate\\CrateModel.obj"), boxMaterial));
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
	boxNode2->GetTransform().SetPos(8.0f, 0.5f, 5.0f);
	boxNode2->GetTransform().Rotate(Quaternion(Vector3D(0.0f, 1.0f, 0.0f), Angle(30.0f)));
	boxNode2->GetTransform().SetScale(0.5f);
	AddToSceneRoot(boxNode2);

	GameNode* testMesh1 = new GameNode();
	testMesh1->GetTransform().SetPos(-2.0f, 2.5f, 2.0f);
	testMesh1->GetTransform().SetScale(0.5f);
	GameNode* testMesh2 = new GameNode();
	testMesh2->GetTransform().SetPos(9.0f, 0.0f, 0.0f);
	//testMesh2->GetTransform().SetScale(0.5f);
	testMesh1->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane.obj"),
		new Material(new Texture("..\\Textures\\bricks2.jpg"), 0.0f, 0, new Texture("..\\Textures\\bricks2_normal.jpg"), new Texture("..\\Textures\\bricks2_disp.jpg"), 0.04f, -1.0f)));
	testMesh2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\plane.obj"),
		new Material(new Texture("..\\Textures\\bricks2.jpg"), 0.0f, 0)));
	AddToSceneRoot(testMesh1);
	//AddToSceneRoot(testMesh2);
	testMesh1->AddChild(testMesh2);

	GameNode* monkeyNode1 = new GameNode();
	monkeyNode1->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\monkey3.obj"),
		new Material(new Texture("..\\Textures\\chessboard.jpg", GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR), 1.0f, 8.0f)));
	monkeyNode1->GetTransform().SetPos(-5.0f, 3.75f, 4.0f);
	//monkeyNode1->GetTransform().SetRotation(Quaternion(Vector3D(0, 1, 0), Angle(-45)));
	//stdlog(Info, LOGPLACE, "MonkeyNode1 has ID=%d", monkeyNode1->GetID());
	//monkeyNode1->AddComponent(new LookAtComponent());
	AddToSceneRoot(monkeyNode1);

	GameNode* monkeyNode2 = new GameNode();
	monkeyNode2->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\monkey3.obj"),
		new Material(new Texture("..\\Textures\\bricks.jpg"), 2.0f, 32.0f)));
	monkeyNode2->GetTransform().SetPos(5.0, 3.0, 15.0);
	//monkeyNode2->AddComponent(new LookAtComponent());
	AddToSceneRoot(monkeyNode2);

	srand((unsigned int)time(NULL));

	humanNodes = new GameNode* [humanCount];
	for (int i = 0; i < humanCount; ++i)
	{
		humanNodes[i] = new GameNode();
		humanNodes[i]->AddComponent(new MeshRenderer(new Mesh("..\\Models\\BodyMesh.obj"), new Material(new Texture("..\\Textures\\HumanSkin.jpg"), 2.0f, 32.0f)));
		humanNodes[i]->GetTransform().SetPos(static_cast<Real>(rand() % 20), 1.7f, static_cast<Real>(rand() % 20));
		AddToSceneRoot(humanNodes[i]);
	}

	//castleNode = new GameNode();
	//castleNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\castle.obj"), new Material(new Texture("..\\Textures\\HumanSkin.jpg"), 0.5f, 8.0f)));
	//castleNode->GetTransform().SetPos(static_cast<Real>(rand() % 50), 1.0f, static_cast<Real>(rand() % 50));
	//AddToSceneRoot(castleNode);

	GameNode* teapotNode = new GameNode();
	teapotNode->GetTransform().SetPos(3.0f, 4.0f, REAL_ONE);
	teapotNode->AddComponent(new MeshRenderer(new Mesh("..\\Models\\teapot.obj"), new Material(new Texture("..\\Textures\\chessboard3.jpg"))));
	AddToSceneRoot(teapotNode);

	AddLights(); // Adding all kinds of light (directional, point, spot)

	///* ==================== Adding cameras begin ==================== */
	if (cameraCount > 0)
	{
		LOG(Notice, LOGPLACE, "Creating %d camera(-s)...", cameraCount);
		AddCameras();
		LOG(Debug, LOGPLACE, "%d camera(-s) created", cameraCount);
	}
	else
	{
		LOG(Error, LOGPLACE, "No cameras defined.");
		exit(EXIT_FAILURE);
	}
	///* ==================== Adding cameras end ==================== */

	LOG(Notice, LOGPLACE, "Initalizing test game finished");
}

void TestGame::AddLights()
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

void TestGame::AddDirectionalLight()
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
	const Color defaultDirectionalLightColor(GET_CONFIG_VALUE("defaultDirectionalLightColorRed", 1.0f), GET_CONFIG_VALUE("defaultDirectionalLightColorGreen", 1.0f), GET_CONFIG_VALUE("defaultDirectionalLightColorBlue", 1.0f));
	const Real defaultDirectionalLightIntensity(GET_CONFIG_VALUE("defaultDirectionalLightIntensity", 1.0f));
	const Angle defaultDirectionalLightRotationX(GET_CONFIG_VALUE("defaultDirectionalLightAngleX", -45.0f));
	const Angle defaultDirectionalLightRotationY(GET_CONFIG_VALUE("defaultDirectionalLightAngleY", 0.0f));
	const Angle defaultDirectionalLightRotationZ(GET_CONFIG_VALUE("defaultDirectionalLightAngleZ", 0.0f));
	const Real defaultDirectionalLightShadowArea = GET_CONFIG_VALUE("defaultDirectionalLightShadowArea", 80.0f);
	const int defaultDirectionalLightShadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("defaultDirectionalLightShadowMapSizeAsPowerOf2", 10);
	const Real defaultDirectionalLightShadowSoftness = GET_CONFIG_VALUE("defaultDirectionalLightShadowSoftness", 1.0f);
	const Real defaultDirectionalLightLightBleedingReductionAmount = GET_CONFIG_VALUE("defaultDirectionalLightLightBleedingReductionAmount", 0.2f);
	const Real defaultDirectionalLightMinVariance = GET_CONFIG_VALUE("defaultDirectionalLightMinVariance", 0.00002f);

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

	Real red = GET_CONFIG_VALUE("directionalLightColorRed", defaultDirectionalLightColor.GetRed());
	Real green = GET_CONFIG_VALUE("directionalLightColorGreen", defaultDirectionalLightColor.GetGreen());
	Real blue = GET_CONFIG_VALUE("directionalLightColorBlue", defaultDirectionalLightColor.GetBlue());
	Color color(red, green, blue);
		
	Real intensity = GET_CONFIG_VALUE("directionalLightIntensity", defaultDirectionalLightIntensity);

	Real shadowArea = GET_CONFIG_VALUE("directionalLightShadowArea", defaultDirectionalLightShadowArea);
	int shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("directionalLightShadowMapSizeAsPowerOf2", defaultDirectionalLightShadowMapSizeAsPowerOf2);
	Real shadowSoftness = GET_CONFIG_VALUE("directionalLightShadowSoftness", defaultDirectionalLightShadowSoftness);
	Real lightBleedingReductionAmount = GET_CONFIG_VALUE("directionalLightLightBleedingReductionAmount", defaultDirectionalLightLightBleedingReductionAmount);
	Real minVariance = GET_CONFIG_VALUE("directionalLightMinVariance", defaultDirectionalLightMinVariance);

	directionalLightNode->AddComponent(new DirectionalLight(color, intensity, shadowMapSizeAsPowerOf2, shadowArea, shadowSoftness, lightBleedingReductionAmount, minVariance));
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

void TestGame::AddPointLights()
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

void TestGame::AddSpotLights()
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
	const Angle defaultSpotLightViewAngle(GET_CONFIG_VALUE("defaultSpotLightViewAngle", 120.0f), Angle::DEGREE);
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

		Angle viewAngle(GET_CONFIG_VALUE("spotLightViewAngle_" + spotLightIndexStr, defaultSpotLightViewAngle.GetAngleInRadians()), Angle::RADIAN);
		
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

void TestGame::AddCameras()
{
	if (cameraCount < 1)
	{
		LOG(Error, LOGPLACE, "No cameras defined.");
		exit(EXIT_FAILURE);
	}

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
		LOG(Debug, LOGPLACE, "angleX=%.1f, angleY=%.1f, angleZ=%.1f", angleX.GetAngleInDegrees(), angleY.GetAngleInDegrees(), angleZ.GetAngleInDegrees());
		Quaternion rot(rotMatrix);
		Quaternion rot2(Vector3D(1, 0, 0), angleX);
		LOG(Debug, LOGPLACE, "rotMatrix =\n%s\n rot =\n%s\n rot.ToRotationMatrix() =\n%s\n rot2.ToRotationMatrix() = \n%s",
			rotMatrix.ToString().c_str(),
			rot.ToString().c_str(),
			rot.ToRotationMatrix().ToString().c_str(),
			rot2.ToRotationMatrix().ToString().c_str());
		cameraNodes[i]->GetTransform().SetRot(rot);

		Angle fov(GET_CONFIG_VALUE("cameraFoV_" + cameraIndexStr, defaultFoV), Angle::DEGREE);
		Real aspectRatio = GET_CONFIG_VALUE("cameraAspectRatio_" + cameraIndexStr, defaultAspectRatio);
		Real zNearPlane = GET_CONFIG_VALUE("cameraNearPlane_" + cameraIndexStr, defaultNearPlane);
		Real zFarPlane = GET_CONFIG_VALUE("cameraFarPlane_" + cameraIndexStr, defaultFarPlane);
		cameraNodes[i]->AddComponent(new Camera(fov, aspectRatio, zNearPlane, zFarPlane));
		//testMesh2->AddChild(cameraNodes[i]);
		AddToSceneRoot(cameraNodes[i]);
	}
}

//
//void Game::CleanUp()
//{
//	stdlog(Info, LOGPLACE, "The game is being cleaned up");
//}

void TestGame::Input(Real delta)
{
	GetRootGameNode().InputAll(delta);
}

// TODO: Remove in the future
Real temp = 0.0;
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;
bool up = false;
bool down = false;
Vector3D velocity;
Real maxSpeed = 0.02f;
bool isMouseLocked = false;

void TestGame::Update(Real delta)
{
	//stdlog(Delocust, LOGPLACE, "Game is being updated");
	
	//planeObject->GetTransform().SetPos(0.0, -1.0, 5.0);

	//rootGameNode->GetTransform().SetPos(0.0, -1.0, 5.0);
	GetRootGameNode().UpdateAll(delta);

	temp += delta;
	if (temp > 20.0 * M_PI)
	{
		temp = 0.0;
	}

#ifdef SIMULATE_SUN_BEHAVIOR
	if (directionalLightNode != NULL)
	{
		Transform& t = directionalLightNode->GetTransform();
		//t.SetPos(t.GetPos() + (Vector3D(sin(temp) / 1000, cos(temp) / 200, cos(temp) / 1000)));
		t.SetRot(t.GetRot() * Quaternion(sin(temp) / 20000.0f, cos(temp) / 10000.0f, 0.0f, 1.0f));
	}
#endif

	for (int i = 0; i < pointLightCount; ++i)
	{
		if (pointLightNodes[i] == NULL)
		{
			LOG(Error, LOGPLACE, "Point light node #%d is NULL while the number of created point lights should be equal to %d.", i, pointLightCount);
			continue;
		}
		Transform& t = pointLightNodes[i]->GetTransform();
		t.SetPos(t.GetPos() + (Vector3D(sin(temp) / 1000, cos(temp) / 2000, cos(temp) / 1000)));
	}

	//for (int i = 0; i < spotLightCount; ++i)
	//{
	//	if (spotLightNodes[i] == NULL)
	//	{
	//		LOG(Error, LOGPLACE, "Spot light node #%d is NULL while the number of created spot lights should be equal to %d.", i, spotLightCount);
	//		continue;
	//	}
	//	Transform& t = spotLightNodes[i]->GetTransform();
	//	t.SetPos(t.GetPos() + (Vector3D(sin(temp) / 1000, sin(temp) / 2000, cos(temp) / 1000)));
	//}

	unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
	Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	const Real sensitivity = static_cast<Real>(Camera::GetSensitivity());
	Vector3D acceleration;
	if (forward)
	{
		acceleration += transform.GetRot().GetForward().Normalized();
	}
	if (backward)
	{
		acceleration -= transform.GetRot().GetForward().Normalized();
	}
	if (left)
	{
		acceleration -= transform.GetRot().GetRight().Normalized();
	}
	if (right)
	{
		acceleration += transform.GetRot().GetRight().Normalized();
	}
	if (up)
	{
		acceleration += transform.GetRot().GetUp().Normalized();
	}
	if (down)
	{
		acceleration -= transform.GetRot().GetUp().Normalized();
	}
	velocity += acceleration * delta * sensitivity * 0.01f;
	const Real step = 0.1f;
	const Real approachedValue = 0.0f; // must be ZERO!
	if (AlmostEqual(acceleration.GetX(), approachedValue))
	{
		velocity.ApproachX(step, approachedValue);
	}
	if (AlmostEqual(acceleration.GetY(), approachedValue))
	{
		velocity.ApproachY(step, approachedValue);
	}
	if (AlmostEqual(acceleration.GetZ(), approachedValue))
	{
		velocity.ApproachZ(step, approachedValue);
	}
	velocity.Threshold(maxSpeed);
	//velocity += acceleration * delta;
	//velocity -= slowDownVec;
	//stdlog(Debug, LOGPLACE, "Acceleration = %s\t Velocity = %s", acceleration.ToString().c_str(), velocity.ToString().c_str());

	transform.SetPos(transform.GetPos() + velocity);
}

void TestGame::WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	Game::WindowResizeEvent(window, width, height);
}

/**
 * @param key the keyboard key that was pressed or released
 * @param scancode the system-specific scancode of the key
 * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods Bit field describing which modifier keys were held down
 */
void TestGame::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game::KeyEvent(window, key, scancode, action, mods);

	//ASSERT(camera != NULL);
	//if (camera == NULL)
	//{
	//	LOG(Error, LOGPLACE, "Camera instance is not initialized");
	//	return;
	//}

	// TODO: Set delta to correct value
	unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
	Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
	const Real sensitivity = static_cast<Real>(Camera::GetSensitivity());
	switch (key)
	{
	case GLFW_KEY_C:
		LOG(Debug, LOGPLACE, "transform.GetPos() = %s;\t transform.GetRot().GetForward() = %s", transform.GetPos().ToString().c_str(), transform.GetRot().GetForward().ToString().c_str());
		break;
	case GLFW_KEY_W:
		forward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//LOG(Debug, LOGPLACE, "Forward = %d", forward);

		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_S:
		backward = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//LOG(Debug, LOGPLACE, "Backward = %d", backward);

		//direction -= transform.GetRot().GetForward().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetForward() * sensitivity));
		break;
	case GLFW_KEY_A:
		left = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//LOG(Debug, LOGPLACE, "Left = %d", left);

		//direction -= transform.GetRot().GetRight().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_D:
		right = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//LOG(Debug, LOGPLACE, "Right = %d", right);

		//direction += transform.GetRot().GetRight().Normalized();
		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetRight() * sensitivity));
		break;
	case GLFW_KEY_SPACE: // move up
		up = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//direction += transform.GetRot().GetUp().Normalized();
		//transform.SetPos(transform.GetPos() + (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_LEFT_CONTROL: // move down
		down = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));
		//direction -= transform.GetRot().GetUp().Normalized();
		//transform.SetPos(transform.GetPos() - (transform.GetRot().GetUp() * sensitivity));
		break;
	case GLFW_KEY_UP: // rotation around X axis
		transform.Rotate(transform.GetRot().GetRight(), Angle(-sensitivity));
		break;
	case GLFW_KEY_DOWN: // rotation around X axis
		transform.Rotate(transform.GetRot().GetRight(), Angle(sensitivity));
		break;
	case GLFW_KEY_LEFT: // rotation around Y axis
		transform.Rotate(transform.GetTransformedRot().GetUp() /*Camera::yAxis*/, Angle(-sensitivity));
		break;
	case GLFW_KEY_RIGHT: // rotation around Y axis
		transform.Rotate(transform.GetTransformedRot().GetUp() /*Camera::yAxis*/, Angle(sensitivity));
		break;
	case GLFW_KEY_N: // next camera
		if (action == GLFW_PRESS)
		{
			CoreEngine::GetCoreEngine()->NextCamera();
		}
		break;
	case GLFW_KEY_P: // prev camera
		if (action == GLFW_PRESS)
		{
			CoreEngine::GetCoreEngine()->PrevCamera();
		}
		break;
	}
}

void TestGame::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	LOG(Debug, LOGPLACE, "Mouse event: button=%d\t action=%d\t mods=%d", button, action, mods);

	/**
	 * GLFW_MOUSE_BUTTON_1 = left mouse button
	 * GLFW_MOUSE_BUTTON_2 = right mouse button
	 * GLFW_MOUSE_BUTTON_3 = middle mouse button
	 */

	switch (action)
	{
	case GLFW_PRESS:
		isMouseLocked = ! isMouseLocked;
		LOG(Info, LOGPLACE, "Mouse is locked");
		LOG(Debug, LOGPLACE, "Mouse button pressed: button=%d\t mods=%d", button, mods);
		break;
	case GLFW_RELEASE:
		LOG(Debug, LOGPLACE, "Mouse button released: button=%d\t mods=%d", button, mods);
		break;
	default:
		LOG(Warning, LOGPLACE, "Unknown action performed with the mouse");
	}
}

void TestGame::MousePosEvent(GLFWwindow* window, double xPos, double yPos)
{
	if (! isMouseLocked)
	{
		return;
	}
	//stdlog(Debug, LOGPLACE, "Cursor position = %.2f, %.2f", xPos, yPos);

	int width = CoreEngine::GetCoreEngine()->GetWindowWidth();
	int height = CoreEngine::GetCoreEngine()->GetWindowHeight();
	Vector2D centerPosition(static_cast<Real>(width) / 2, static_cast<Real>(height) / 2);
	Vector2D deltaPosition(static_cast<Real>(xPos), static_cast<Real>(yPos));
	deltaPosition -= centerPosition;
	
	bool rotX = ! AlmostEqual(deltaPosition.GetX(), 0.0f);
	bool rotY = ! AlmostEqual(deltaPosition.GetY(), 0.0f);

	if (rotX || rotY)
	{
		unsigned int currentCameraIndex = CoreEngine::GetCoreEngine()->GetCurrentCameraIndex();
		const Real sensitivity = static_cast<Real>(Camera::GetSensitivity());
		Transform& transform = cameraNodes[currentCameraIndex]->GetTransform();
		if (rotX)
		{
			transform.Rotate(Vector3D(0, 1, 0), Angle(deltaPosition.GetX() * sensitivity));
		}
		if (rotY)
		{
			transform.Rotate(transform.GetRot().GetRight(), Angle(deltaPosition.GetY() * sensitivity));
		}
		CoreEngine::GetCoreEngine()->SetCursorPos(centerPosition.GetX(), centerPosition.GetY());
	}
}

#ifdef ANT_TWEAK_BAR_ENABLED
void TestGame::InitializeTweakBars()
{
#ifdef GAME_PROPERTIES_TWEAK_BAR
	// TODO: GAME_PROPERTIES_TWEAK_BAR gives some errors. Investigate why and fix that!

	TwBar* testGamePropertiesBar = TwNewBar("TestGamePropertiesBar");
	//TwAddVarRW(testGamePropertiesBar, "planeSpecularIntensity", TW_TYPE_FLOAT, &planeSpecularIntensity, " label='Plane specular intensity' group=Plane ");
	//TwAddVarRW(testGamePropertiesBar, "planeSpecularPower", TW_TYPE_FLOAT, &planeSpecularPower, " label='Plane specular power' group=Plane ");
	//TwAddVarRW(testGamePropertiesBar, "planeDisplacementScale", TW_TYPE_FLOAT, &planeDisplacementScale, " label='Plane displacement scale' group=Plane ");
	//TwAddVarRW(testGamePropertiesBar, "planeDisplacementOffset", TW_TYPE_FLOAT, &planeDisplacementOffset, " label='Plane displacement offset' group=Plane ");
	planeMaterial->SetVector3D("Vec1", Math::Vector3D(REAL_ZERO, REAL_ONE, REAL_ZERO));
	planeMaterial->InitializeTweakBar(testGamePropertiesBar, "Plane");
	//boxMaterial->SetVector3D("Vec1", Math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ONE));
	//boxMaterial->InitializeTweakBar(testGamePropertiesBar, "Box");
#endif
}
#endif
