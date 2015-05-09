#include "StdAfx.h"
#include "LightBuilder.h"
#include "CoreEngine.h"
#include "Shader.h"
#include "Utility\IConfig.h"

#ifdef RENDER_LIGHT_MESHES
#include "MeshRenderer.h"
#endif

using namespace Rendering;

LightBuilder::LightBuilder(void)
{
}


LightBuilder::~LightBuilder(void)
{
}

/* ==================== DirectionalLightBuilder implementation begin ==================== */
void DirectionalLightBuilder::BuildPart1()
{
	m_directionalLightNode = new GameNode();

	// Setting position
	const Math::Vector3D defaultDirectionalLightPos(GET_CONFIG_VALUE("defaultDirectionalLightPosX", 0.0f), GET_CONFIG_VALUE("defaultDirectionalLightPosY", 0.0f), GET_CONFIG_VALUE("defaultDirectionalLightPosZ", 0.0f));
	Math::Real xPos = GET_CONFIG_VALUE("directionalLightPosX", defaultDirectionalLightPos.GetX());
	Math::Real yPos = GET_CONFIG_VALUE("directionalLightPosY", defaultDirectionalLightPos.GetY());
	Math::Real zPos = GET_CONFIG_VALUE("directionalLightPosZ", defaultDirectionalLightPos.GetZ());
	Math::Vector3D position(xPos, yPos, zPos);
	m_directionalLightNode->GetTransform().SetPos(position);


	// Setting rotation
	const Math::Angle defaultDirectionalLightRotationX(GET_CONFIG_VALUE("defaultDirectionalLightAngleX", -45.0f));
	const Math::Angle defaultDirectionalLightRotationY(GET_CONFIG_VALUE("defaultDirectionalLightAngleY", REAL_ZERO));
	const Math::Angle defaultDirectionalLightRotationZ(GET_CONFIG_VALUE("defaultDirectionalLightAngleZ", REAL_ZERO));
	Math::Angle angleX(GET_CONFIG_VALUE("directionalLightAngleX", defaultDirectionalLightRotationX.GetAngleInDegrees()));
	Math::Angle angleY(GET_CONFIG_VALUE("directionalLightAngleY", defaultDirectionalLightRotationY.GetAngleInDegrees()));
	Math::Angle angleZ(GET_CONFIG_VALUE("directionalLightAngleZ", defaultDirectionalLightRotationZ.GetAngleInDegrees()));
	Math::Matrix4D rotMatrix = Math::Matrix4D::RotationEuler(angleX, angleY, angleZ);
	LOG(Utility::Debug, LOGPLACE, "angleX=%.1f, angleY=%.1f, angleZ=%.1f, rotMatrix =\n%s", angleX.GetAngleInDegrees(), angleY.GetAngleInDegrees(), angleZ.GetAngleInDegrees(), rotMatrix.ToString().c_str());
	Math::Quaternion rot(rotMatrix);
	Math::Quaternion rot2(Math::Vector3D(1, 0, 0), angleX);
	LOG(Utility::Debug, LOGPLACE, "rotMatrix =\n%s\n rot =\n%s\n rot.ToRotationMatrix() =\n%s\n rot2.ToRotationMatrix() = \n%s",
		rotMatrix.ToString().c_str(),
		rot.ToString().c_str(),
		rot.ToRotationMatrix().ToString().c_str(),
		rot2.ToRotationMatrix().ToString().c_str());
	m_directionalLightNode->GetTransform().SetRot(rot);
	//directionalLightNode->GetTransform().SetRot(Quaternion(Vector3D(1, 0, 0), Angle(90.0f)));
	//directionalLightNode->GetTransform().Rotate(Vector3D(0, 1, 0), Angle(45.0f));
}

void DirectionalLightBuilder::BuildPart2()
{
	// Setting light
	const Color defaultDirectionalLightColor(GET_CONFIG_VALUE("defaultDirectionalLightColorRed", REAL_ONE), GET_CONFIG_VALUE("defaultDirectionalLightColorGreen", REAL_ONE), GET_CONFIG_VALUE("defaultDirectionalLightColorBlue", REAL_ONE));
	const Math::Real defaultDirectionalLightIntensity(GET_CONFIG_VALUE("defaultDirectionalLightIntensity", REAL_ONE));

	Math::Real red = GET_CONFIG_VALUE("directionalLightColorRed", defaultDirectionalLightColor.GetRed());
	Math::Real green = GET_CONFIG_VALUE("directionalLightColorGreen", defaultDirectionalLightColor.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE("directionalLightColorBlue", defaultDirectionalLightColor.GetBlue());
	Color color(red, green, blue);

	Math::Real intensity = GET_CONFIG_VALUE("directionalLightIntensity", defaultDirectionalLightIntensity);
	DirectionalLight* directionalLight = new DirectionalLight(color, intensity);

	// Setting shaders
	directionalLight->SetShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShader", "forward-directional")));
	directionalLight->SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("directionalLightShaderTerrain", "forward-directional-terrain")));

	// Setting shadow info
	const Math::Real defaultDirectionalLightShadowArea = GET_CONFIG_VALUE("defaultDirectionalLightShadowArea", 80.0f);
	const int defaultDirectionalLightShadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("defaultDirectionalLightShadowMapSizeAsPowerOf2", 10);
	const Math::Real defaultDirectionalLightShadowSoftness = GET_CONFIG_VALUE("defaultDirectionalLightShadowSoftness", 1.0f);
	const Math::Real defaultDirectionalLightLightBleedingReductionAmount = GET_CONFIG_VALUE("defaultDirectionalLightLightBleedingReductionAmount", 0.2f);
	const Math::Real defaultDirectionalLightMinVariance = GET_CONFIG_VALUE("defaultDirectionalLightMinVariance", 0.00002f);

	Math::Real halfShadowArea = GET_CONFIG_VALUE("directionalLightShadowArea", defaultDirectionalLightShadowArea) / 2.0f;
	int shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE("directionalLightShadowMapSizeAsPowerOf2", defaultDirectionalLightShadowMapSizeAsPowerOf2);
	Math::Real shadowSoftness = GET_CONFIG_VALUE("directionalLightShadowSoftness", defaultDirectionalLightShadowSoftness);
	Math::Real lightBleedingReductionAmount = GET_CONFIG_VALUE("directionalLightLightBleedingReductionAmount", defaultDirectionalLightLightBleedingReductionAmount);
	Math::Real minVariance = GET_CONFIG_VALUE("directionalLightMinVariance", defaultDirectionalLightMinVariance);
	bool isShadowingEnabled = (shadowMapSizeAsPowerOf2 != 0); // shadowMapSizeAsPowerOf2 == 0 means the light doesn't cast shadows
	if (isShadowingEnabled)
	{
		Math::Matrix4D ortoMatrix = Math::Matrix4D::OrtographicProjection(-halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea, -halfShadowArea, halfShadowArea);
		directionalLight->SetShadowInfo(new ShadowInfo(ortoMatrix, true, shadowMapSizeAsPowerOf2, shadowSoftness, lightBleedingReductionAmount, minVariance));
	}

	// Setting additional directional light information
	directionalLight->SetMaxIntensity(directionalLight->GetIntensity());
	directionalLight->SetSunlightDaytimeColor(directionalLight->GetColor());
	directionalLight->SetSunNearHorizonColor(Color(GET_CONFIG_VALUE("directionalLightNearHorizonColorRed", REAL_ONE), GET_CONFIG_VALUE("directionalLightNearHorizonColorGreen", 0.2f), GET_CONFIG_VALUE("directionalLightNearHorizonColorBlue", REAL_ZERO)));
	directionalLight->SetSunlightNighttimeColor(Color(GET_CONFIG_VALUE("directionalLightNighttimeColorRed", REAL_ZERO), GET_CONFIG_VALUE("directionalLightNighttimeColorGreen", REAL_ZERO), GET_CONFIG_VALUE("directionalLightNighttimeColorBlue", REAL_ZERO)));

	m_directionalLightNode->AddComponent(directionalLight);

	CoreEngine::GetCoreEngine()->GetRenderer()->AddLight(directionalLight);
}

#ifdef RENDER_LIGHT_MESHES
void DirectionalLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	//Material directionalLightMaterial("directionalLight_material", Texture("..\\Textures\\DirectionalLight.png"), 1, 8);
	//Material directionalLightLineMaterial("directionalLightLine_material", Texture("..\\Textures\\DirectionalLight.png"), 1, 8);

	m_directionalLightNode->GetTransform().SetScale(0.4f);
	m_directionalLightNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\DirectionalLight.obj"),
		new Material(new Texture("..\\Textures\\DirectionalLight.png"), 1.0f, 8.0f)));
		
	Math::Vector3D forwardVec = m_directionalLightNode->GetTransform().GetTransformedRot().GetForward().Normalized();
	Math::Vector3D rayEndPosition = forwardVec * 2.0f;
	//LOG(Delocust, LOGPLACE, "light position = %s;\t light rotation = %s;\t light forward vector = %s;\t light end pos = %s",
	//	position.ToString().c_str(),
	//	directionalLightNode->GetTransform().GetTransformedRot().ToString().c_str(),
	//	forwardVec.ToString().c_str(),
	//	(position + rayEndPosition).ToString().c_str());
	Vertex vertices [] = { Vertex(Math::Vector3D()), Vertex(rayEndPosition) };
	int indices [] = { 0, 1 };
	m_directionalLightNode->AddComponent(new MeshRenderer(
		new Mesh(vertices, 2, indices, 2, false, GL_LINES),
		new Material(new Texture("..\\Textures\\DirectionalLight.png"), 1.0f, 8.0f)));
}
#endif
/* ==================== DirectionalLightBuilder implementation end ==================== */

/* ==================== PointLightBuilder implementation begin ==================== */
PointLightBuilder::PointLightBuilder(int pointLightIndex /* = 0 */) :
	M_DEFAULT_POINT_LIGHT_POS(GET_CONFIG_VALUE("defaultPointLightPosX", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightPosY", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightPosZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X(GET_CONFIG_VALUE("defaultPointLightAngleX", -45.0f)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y(GET_CONFIG_VALUE("defaultPointLightAngleY", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z(GET_CONFIG_VALUE("defaultPointLightAngleZ", REAL_ZERO)),
	M_DEFAULT_POINT_LIGHT_COLOR(GET_CONFIG_VALUE("defaultPointLightColorRed", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightColorGreen", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightColorBlue", REAL_ONE)),
	M_DEFAULT_POINT_LIGHT_INTENSITY(GET_CONFIG_VALUE("defaultPointLightIntensity", 10.0f)),
	M_DEFAULT_POINT_LIGHT_ATTENUATION(GET_CONFIG_VALUE("defaultPointLightAttenuationConstant", REAL_ZERO), GET_CONFIG_VALUE("defaultPointLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE("defaultPointLightAttenuationExponent", REAL_ZERO)),
	m_pointLightIndex(pointLightIndex)
{
	std::stringstream ss("");
	ss << (pointLightIndex + 1);
	m_pointLightIndexStr = ss.str();
}

void PointLightBuilder::BuildPart1()
{
	m_pointLightNode = new GameNode();

	// Setting position
	Math::Real xPos = GET_CONFIG_VALUE("pointLightPosX_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetX());
	Math::Real yPos = GET_CONFIG_VALUE("pointLightPosY_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetY());
	Math::Real zPos = GET_CONFIG_VALUE("pointLightPosZ_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetZ());
	m_pointLightNode->GetTransform().SetPos(xPos, yPos, zPos);

	// Setting rotation
	Math::Angle angleX(GET_CONFIG_VALUE("pointLightAngleX_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X.GetAngleInDegrees()));
	Math::Angle angleY(GET_CONFIG_VALUE("pointLightAngleY_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y.GetAngleInDegrees()));
	Math::Angle angleZ(GET_CONFIG_VALUE("pointLightAngleZ_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z.GetAngleInDegrees()));
	Math::Matrix4D rotMatrix = Math::Matrix4D::RotationEuler(angleX, angleY, angleZ);
	Math::Quaternion rot(rotMatrix);
	m_pointLightNode->GetTransform().SetRot(rot);
}

void PointLightBuilder::BuildPart2()
{
	// Setting color
	Math::Real red = GET_CONFIG_VALUE("pointLightColorRed_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetRed());
	Math::Real green = GET_CONFIG_VALUE("pointLightColorGreen_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetGreen());
	Math::Real blue = GET_CONFIG_VALUE("pointLightColorBlue_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetBlue());
	Color color(red, green, blue);

	// Setting intensity
	Math::Real intensity = GET_CONFIG_VALUE("pointLightIntensity_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_INTENSITY);

	// Setting attenuation
	Math::Real constant = GET_CONFIG_VALUE("pointLightAttenuationConstant_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetConstant());
	Math::Real linear = GET_CONFIG_VALUE("pointLightAttenuationLinear_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetLinear());
	Math::Real exponent = GET_CONFIG_VALUE("pointLightAttenuationExponent_" + m_pointLightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetExponent());
	Attenuation attenuation(constant, linear, exponent);

	PointLight* pointLight = new PointLight(color, intensity, attenuation);

	// Setting shaders
	pointLight->SetShader(new Shader(GET_CONFIG_VALUE_STR("pointLightShader", "forward-point")));
	pointLight->SetTerrainShader(new Shader(GET_CONFIG_VALUE_STR("pointLightShaderTerrain", "forward-point-terrain")));

	// Setting shadow info
	// Setting additional directional light information

	m_pointLightNode->AddComponent(pointLight);

	CoreEngine::GetCoreEngine()->GetRenderer()->AddLight(pointLight);
}

#ifdef RENDER_LIGHT_MESHES
void PointLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	m_pointLightNode->AddComponent(new MeshRenderer(
		new Mesh("..\\Models\\Bulb\\Bulb.obj") /* new Mesh("..\\Models\\PointLight.obj") */,
		new Material(new Texture("..\\Textures\\PointLight.png"), 1.0f, 8.0f)));
	m_pointLightNode->GetTransform().SetScale(0.005f);
}
#endif
/* ==================== PointLightBuilder implementation end ==================== */

/* ==================== SpotLightBuilder implementation begin ==================== */
void SpotLightBuilder::BuildPart1()
{
}

void SpotLightBuilder::BuildPart2()
{
}

#ifdef RENDER_LIGHT_MESHES
void SpotLightBuilder::BuildMeshRenderer()
{
}
#endif
/* ==================== SpotLightBuilder implementation end ==================== */
