#include "stdafx.h"
#include "LightBuilder.h"
#include "ShaderIDs.h"

#include "Utility/IConfig.h"

/* ==================== DirectionalLightBuilder implementation begin ==================== */
rendering::lighting::DirectionalLightBuilder::DirectionalLightBuilder() :
	LightBuilder(math::Vector3D(GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightPosX", 0.0f),
		GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightPosY", 0.0f), GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightPosZ", 0.0f)),
		math::Quaternion(math::Matrix4D(math::Angle(GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightAngleX", -45.0f)),
			math::Angle(GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightAngleY", 0.0f)), math::Angle(GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightAngleZ", 0.0f)))),
		Color(GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightColorRed", 1.0f), GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightColorGreen", 1.0f),
			GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightColorBlue", 1.0f), GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightColorAlpha", 1.0f)),
		GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightIntensity", 1.0f), GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightShadowInfoFlipFacesEnabled", true),
		GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightShadowInfoShadowMapSizeAsPowerOf2", 9), GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightShadowInfoShadowSoftness", 1.0f),
		GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightShadowInfoLightBleedingReductionFactor", 0.2f), GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightShadowInfoMinVariance", 0.002f),
		shader_ids::DIRECTIONAL_LIGHT, shader_ids::DIRECTIONAL_LIGHT_TERRAIN, shader_ids::DIRECTIONAL_LIGHT_NO_SHADOWS, shader_ids::DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS),
	m_defaultHalfShadowArea(GET_CONFIG_VALUE_RENDERING("defaultDirectionalLightHalfShadowArea", 40.0f)),
	m_halfShadowArea(m_defaultHalfShadowArea)
{
}

rendering::lighting::DirectionalLightBuilder::~DirectionalLightBuilder()
{
}

//void Rendering::lighting::DirectionalLightBuilder::BuildTransform()
//{
	//// Determining position
	//m_pos.Set(GET_CONFIG_VALUE_GAME("directionalLightPosX", GET_CONFIG_VALUE_GAME("defaultDirectionalLightPosX", 0.0f)),
	//	GET_CONFIG_VALUE_GAME("directionalLightPosY", GET_CONFIG_VALUE_GAME("defaultDirectionalLightPosY", 0.0f)),
	//	GET_CONFIG_VALUE_GAME("directionalLightPosZ", GET_CONFIG_VALUE_GAME("defaultDirectionalLightPosZ", 0.0f)));

	//// Determining rotation
	//const math::Angle defaultDirectionalLightRotationX(GET_CONFIG_VALUE_GAME("defaultDirectionalLightAngleX", -45.0f), math::units::DEGREE);
	//const math::Angle defaultDirectionalLightRotationY(GET_CONFIG_VALUE_GAME("defaultDirectionalLightAngleY", REAL_ZERO), math::units::DEGREE);
	//const math::Angle defaultDirectionalLightRotationZ(GET_CONFIG_VALUE_GAME("defaultDirectionalLightAngleZ", REAL_ZERO), math::units::DEGREE);
	//math::Angle angleX(GET_CONFIG_VALUE_GAME("directionalLightAngleX", defaultDirectionalLightRotationX.Get(math::units::DEGREE)), math::units::DEGREE);
	//math::Angle angleY(GET_CONFIG_VALUE_GAME("directionalLightAngleY", defaultDirectionalLightRotationY.Get(math::units::DEGREE)), math::units::DEGREE);
	//math::Angle angleZ(GET_CONFIG_VALUE_GAME("directionalLightAngleZ", defaultDirectionalLightRotationZ.Get(math::units::DEGREE)), math::units::DEGREE);
	//DEBUG_LOG_GAME("angleX=", angleX, ", angleY=", angleY, ", angleZ=", angleZ);
	//m_rot = math::Quaternion(math::Matrix4D(angleX, angleY, angleZ));
//}

//void Rendering::lighting::DirectionalLightBuilder::BuildLightParams()
//{
	//// Determining color
	//m_color.Set(GET_CONFIG_VALUE_GAME("directionalLightColorRed", GET_CONFIG_VALUE_GAME("defaultDirectionalLightColorRed", REAL_ONE)),
	//	GET_CONFIG_VALUE_GAME("directionalLightColorGreen", GET_CONFIG_VALUE_GAME("defaultDirectionalLightColorGreen", REAL_ONE)),
	//	GET_CONFIG_VALUE_GAME("directionalLightColorBlue", GET_CONFIG_VALUE_GAME("defaultDirectionalLightColorBlue", REAL_ONE)),
	//	REAL_ONE /* TODO: Alpha? */);

	//// Determining intensity
	//m_intensity = GET_CONFIG_VALUE_GAME("directionalLightIntensity", GET_CONFIG_VALUE_GAME("defaultDirectionalLightIntensity", REAL_ONE));

	//// Determining shadow info
	//m_halfShadowArea = GET_CONFIG_VALUE_GAME("directionalLightShadowArea", GET_CONFIG_VALUE_GAME("defaultDirectionalLightShadowArea", 80.0f)) / 2.0f;
	//m_shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE_GAME("directionalLightShadowMapSizeAsPowerOf2", GET_CONFIG_VALUE_GAME("defaultDirectionalLightShadowMapSizeAsPowerOf2", 10));
	//m_shadowSoftness = GET_CONFIG_VALUE_GAME("directionalLightShadowSoftness", GET_CONFIG_VALUE_GAME("defaultDirectionalLightShadowSoftness", 1.0f));
	//m_lightBleedingReductionFactor = GET_CONFIG_VALUE_GAME("directionalLightLightBleedingReductionAmount", GET_CONFIG_VALUE_GAME("defaultDirectionalLightLightBleedingReductionAmount", 0.2f));
	//m_minVariance = GET_CONFIG_VALUE_GAME("directionalLightMinVariance", GET_CONFIG_VALUE_GAME("defaultDirectionalLightMinVariance", 0.00002f));

	//// Setting additional directional light information
	////math::Real maxIntensity = directionalLight->GetIntensity();
	////const Rendering::Color sunlightDaytimeColor(directionalLight->GetColor());
	////const Rendering::Color sunNearHorizonColor(Rendering::Color(GET_CONFIG_VALUE_GAME("directionalLightNearHorizonColorRed", REAL_ONE), GET_CONFIG_VALUE_GAME("directionalLightNearHorizonColorGreen", 0.2f), GET_CONFIG_VALUE_GAME("directionalLightNearHorizonColorBlue", REAL_ZERO)));
	////const Rendering::Color sunlightNighttimeColor(Rendering::Color(GET_CONFIG_VALUE_GAME("directionalLightNighttimeColorRed", REAL_ZERO), GET_CONFIG_VALUE_GAME("directionalLightNighttimeColorGreen", REAL_ZERO), GET_CONFIG_VALUE_GAME("directionalLightNighttimeColorBlue", REAL_ZERO)));

	////m_object->AddComponent(new DirectionalLightComponent(directionalLight, maxIntensity, sunlightDaytimeColor, sunNearHorizonColor, sunlightNighttimeColor,
	////	math::Angle(GET_CONFIG_VALUE_GAME("latitude", 52.0f)), math::Angle(GET_CONFIG_VALUE_GAME("longitude", -16.0f)),
	////	math::Angle(GET_CONFIG_VALUE_GAME("sunlightFirstElevationLevel", -REAL_ONE)), math::Angle(GET_CONFIG_VALUE_GAME("sunlightSecondElevationLevel", REAL_ZERO)),
	////	math::Angle(GET_CONFIG_VALUE_GAME("sunlightThirdElevationLevel", REAL_ONE))));
//}

void rendering::lighting::DirectionalLightBuilder::SetDefault()
{
	LightBuilder<DirectionalLight>::SetDefault();
	m_halfShadowArea = m_defaultHalfShadowArea;
}

#ifdef BUILD_MESH_RENDERER
void Rendering::lighting::DirectionalLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	//Material directionalLightMaterial("directionalLight_material", Texture("DirectionalLight.png"), 1, 8);
	//Material directionalLightLineMaterial("directionalLightLine_material", Texture("DirectionalLight.png"), 1, 8);

	m_gameNode->GetTransform().SetScale(0.4f); /* TODO: Don't use hardcoded values! Ever! */
	m_gameNode->AddComponent(new MeshRendererComponent(m_gameManager->GetMesh("DirectionalLight.obj"), new Rendering::Material(m_textureFactory.GetTexture(TextureIDs::DIRECTIONAL_LIGHT), 1.0f, 8.0f)));

	math::Vector3D forwardVec = m_gameNode->GetTransform().GetTransformedRot().GetForward().Normalized();
	math::Vector3D rayEndPosition = forwardVec * 2.0f;
	//DELOCUST_LOG_ENGINE("light position = ", m_gameNode->GetTransform().GetTransformedPos(), ";\t light rotation = ",
	//	m_gameNode->GetTransform().GetTransformedRot(), ";\t light forward vector = ", forwardVec, ";\t light end pos = ",
	//	(m_gameNode->GetTransform().GetTransformedPos() + rayEndPosition));

	//Vertex vertices [] = { Vertex(math::Vector3D()), Vertex(rayEndPosition) };
	//int indices [] = { 0, 1 };
	//m_gameNode->AddComponent(new MeshRenderer(
	//	new Mesh(vertices, 2, indices, 2, false, GL_LINES),
	//	new Material(m_gameManager->GetTextureFactory().GetTexture(TextureIDs::DIRECTIONAL_LIGHT), 1.0f, 8.0f)));
}
#endif
/* ==================== DirectionalLightBuilder implementation end ==================== */

/* ==================== PointLightBuilder implementation begin ==================== */
rendering::lighting::PointLightBuilder::PointLightBuilder() :
	LightBuilder(math::Vector3D(GET_CONFIG_VALUE_RENDERING("defaultPointLightPosX", 0.0f),
		GET_CONFIG_VALUE_RENDERING("defaultPointLightPosY", 0.0f), GET_CONFIG_VALUE_RENDERING("defaultPointLightPosZ", 0.0f)),
		math::Quaternion(math::Matrix4D(math::Angle(GET_CONFIG_VALUE_RENDERING("defaultPointLightAngleX", -45.0f)),
			math::Angle(GET_CONFIG_VALUE_RENDERING("defaultPointLightAngleY", 0.0f)), math::Angle(GET_CONFIG_VALUE_RENDERING("defaultPointLightAngleZ", 0.0f)))),
		Color(GET_CONFIG_VALUE_RENDERING("defaultPointLightColorRed", 1.0f), GET_CONFIG_VALUE_RENDERING("defaultPointLightColorGreen", 1.0f),
			GET_CONFIG_VALUE_RENDERING("defaultPointLightColorBlue", 1.0f), GET_CONFIG_VALUE_RENDERING("defaultPointLightColorAlpha", 1.0f)),
		GET_CONFIG_VALUE_RENDERING("defaultPointLightIntensity", 1.0f), false, 0, REAL_ONE, REAL_ZERO, REAL_ZERO,
		shader_ids::POINT_LIGHT, shader_ids::POINT_LIGHT_TERRAIN, shader_ids::POINT_LIGHT_NO_SHADOWS, shader_ids::POINT_LIGHT_TERRAIN_NO_SHADOWS),
	m_defaultPointLightAttenuation(GET_CONFIG_VALUE_RENDERING("defaultPointLightAttenuationConstant", REAL_ZERO),
		GET_CONFIG_VALUE_RENDERING("defaultPointLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE_RENDERING("defaultPointLightAttenuationExponent", REAL_ZERO)),
	m_attenuation(m_defaultPointLightAttenuation)
{
}

//void Rendering::lighting::PointLightBuilder::BuildTransform()
//{
	//// Determining position
	//m_pos.Set(GET_CONFIG_VALUE_GAME("pointLightPosX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetX()),
	//	GET_CONFIG_VALUE_GAME("pointLightPosY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetY()),
	//	GET_CONFIG_VALUE_GAME("pointLightPosZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_POS.GetZ()));

	//// Determining rotation
	//math::Angle angleX(GET_CONFIG_VALUE_GAME("pointLightAngleX_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_X.Get(math::units::DEGREE)));
	//math::Angle angleY(GET_CONFIG_VALUE_GAME("pointLightAngleY_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Y.Get(math::units::DEGREE)));
	//math::Angle angleZ(GET_CONFIG_VALUE_GAME("pointLightAngleZ_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ROTATION_ANGLE_Z.Get(math::units::DEGREE)));
	//DEBUG_LOG_GAME("angleX=", angleX, ", angleY=", angleY, ", angleZ=", angleZ);
	//m_rot = math::Quaternion(math::Matrix4D(angleX, angleY, angleZ));
//}

//void Rendering::lighting::PointLightBuilder::BuildLightParams()
//{
	//// Determining color
	//m_color.Set(GET_CONFIG_VALUE_GAME("pointLightColorRed_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetRed()),
	//	GET_CONFIG_VALUE_GAME("pointLightColorGreen_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetGreen()),
	//	GET_CONFIG_VALUE_GAME("pointLightColorBlue_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_COLOR.GetBlue()),
	//	REAL_ONE /* TODO: Alpha? */);

	//// Determining intensity
	//m_intensity = GET_CONFIG_VALUE_GAME("pointLightIntensity_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_INTENSITY);

	//// Determining attenuation
	//math::Real constant = GET_CONFIG_VALUE_GAME("pointLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetConstant());
	//math::Real linear = GET_CONFIG_VALUE_GAME("pointLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetLinear());
	//math::Real exponent = GET_CONFIG_VALUE_GAME("pointLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_POINT_LIGHT_ATTENUATION.GetExponent());
	//m_attenuation = std::move(Rendering::Attenuation(constant, linear, exponent));

	//// TODO: Setting shadow info
	//// TODO: Setting additional point light information
//}

void rendering::lighting::PointLightBuilder::SetDefault()
{
	LightBuilder<PointLight>::SetDefault();
	m_attenuation = m_defaultPointLightAttenuation;
}

#ifdef BUILD_MESH_RENDERER
void Rendering::lighting::PointLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around point light node position to let the user see the source
	m_gameNode->AddComponent(new MeshRendererComponent(/* new Mesh("Bulb\\Bulb.obj") */ m_gameManager->GetMesh("PointLight.obj"), new Rendering::Material(m_textureFactory.GetTexture(TextureIDs::POINT_LIGHT), 1.0f, 8.0f)));
	m_gameNode->GetTransform().SetScale(0.005f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif
/* ==================== PointLightBuilder implementation end ==================== */

/* ==================== SpotLightBuilder implementation begin ==================== */
rendering::lighting::SpotLightBuilder::SpotLightBuilder() :
	LightBuilder(math::Vector3D(GET_CONFIG_VALUE_RENDERING("defaultSpotLightPosX", 0.0f),
		GET_CONFIG_VALUE_RENDERING("defaultSpotLightPosY", 0.0f), GET_CONFIG_VALUE_RENDERING("defaultSpotLightPosZ", 0.0f)),
		math::Quaternion(math::Matrix4D(math::Angle(GET_CONFIG_VALUE_RENDERING("defaultSpotLightAngleX", -45.0f)),
			math::Angle(GET_CONFIG_VALUE_RENDERING("defaultSpotLightAngleY", 0.0f)), math::Angle(GET_CONFIG_VALUE_RENDERING("defaultSpotLightAngleZ", 0.0f)))),
		Color(GET_CONFIG_VALUE_RENDERING("defaultSpotLightColorRed", 1.0f), GET_CONFIG_VALUE_RENDERING("defaultSpotLightColorGreen", 1.0f),
			GET_CONFIG_VALUE_RENDERING("defaultSpotLightColorBlue", 1.0f), GET_CONFIG_VALUE_RENDERING("defaultSpotLightColorAlpha", 1.0f)),
		GET_CONFIG_VALUE_RENDERING("defaultSpotLightIntensity", 1.0f), GET_CONFIG_VALUE_RENDERING("defaultSpotLightShadowInfoFlipFacesEnabled", false),
		GET_CONFIG_VALUE_RENDERING("defaultSpotLightShadowInfoShadowMapSizeAsPowerOf2", 10), GET_CONFIG_VALUE_RENDERING("defaultSpotLightShadowInfoShadowSoftness", REAL_ONE),
		GET_CONFIG_VALUE_RENDERING("defaultSpotLightShadowInfoLightBleedingReductionFactor", 0.2f), GET_CONFIG_VALUE_RENDERING("defaultSpotLightMinVariance", 0.00002f),
		shader_ids::SPOT_LIGHT, shader_ids::SPOT_LIGHT_TERRAIN, shader_ids::SPOT_LIGHT_NO_SHADOWS, shader_ids::SPOT_LIGHT_TERRAIN_NO_SHADOWS),
	m_defaultSpotLightAttenuation(GET_CONFIG_VALUE_RENDERING("defaultSpotLightAttenuationConstant", 0.5f), GET_CONFIG_VALUE_RENDERING("defaultSpotLightAttenuationLinear", 0.1f), GET_CONFIG_VALUE_RENDERING("defaultSpotLightAttenuationExponent", 0.05f)),
	m_defaultSpotLightViewAngle(GET_CONFIG_VALUE_RENDERING("defaultSpotLightViewAngle", 120.0f), math::units::DEGREE),
	m_defaultSpotLightShadowInfoProjectionNearPlane(GET_CONFIG_VALUE_RENDERING("defaultSpotLightShadowInfoProjectionNearPlane", 0.1f)),
	m_attenuation(m_defaultSpotLightAttenuation),
	m_viewAngle(m_defaultSpotLightViewAngle),
	m_shadowInfoProjectionNearPlane(m_defaultSpotLightShadowInfoProjectionNearPlane)
{
}

//void Rendering::lighting::SpotLightBuilder::BuildTransform()
//{
	//// Determining position
	//m_pos.Set(GET_CONFIG_VALUE_GAME("spotLightPosX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetX()),
	//	GET_CONFIG_VALUE_GAME("spotLightPosY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetY()),
	//	GET_CONFIG_VALUE_GAME("spotLightPosZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_POS.GetZ()));

	//// Determining rotation
	//math::Angle angleX(GET_CONFIG_VALUE_GAME("spotLightAngleX_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_X.Get(math::units::DEGREE)));
	//math::Angle angleY(GET_CONFIG_VALUE_GAME("spotLightAngleY_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Y.Get(math::units::DEGREE)));
	//math::Angle angleZ(GET_CONFIG_VALUE_GAME("spotLightAngleZ_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ROTATION_ANGLE_Z.Get(math::units::DEGREE)));
	//DEBUG_LOG_GAME("angleX=", angleX, ", angleY=", angleY, ", angleZ=", angleZ);
	//m_rot = math::Quaternion(math::Matrix4D(angleX, angleY, angleZ));
//}

//void Rendering::lighting::SpotLightBuilder::BuildLightParams()
//{
	//// Determining color
	//m_color.Set(GET_CONFIG_VALUE_GAME("spotLightColorRed_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetRed()),
	//	GET_CONFIG_VALUE_GAME("spotLightColorGreen_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetGreen()),
	//	GET_CONFIG_VALUE_GAME("spotLightColorBlue_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_COLOR.GetBlue()),
	//	REAL_ONE /* TODO: Alpha channel? */);

	//// Setting intensity
	//m_intensity = GET_CONFIG_VALUE_GAME("spotLightIntensity_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_INTENSITY);

	//// Setting attenuation
	//math::Real constant = GET_CONFIG_VALUE_GAME("spotLightAttenuationConstant_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetConstant());
	//math::Real linear = GET_CONFIG_VALUE_GAME("spotLightAttenuationLinear_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetLinear());
	//math::Real exponent = GET_CONFIG_VALUE_GAME("spotLightAttenuationExponent_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_ATTENUATION.GetExponent());
	//m_attenuation = Rendering::Attenuation(constant, linear, exponent);

	//// Setting shadow info
	//m_viewAngle.Set(GET_CONFIG_VALUE_GAME("spotLightViewAngle_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_VIEW_ANGLE.Get(math::units::DEGREE)), math::units::DEGREE);
	//m_shadowMapSizeAsPowerOf2 = GET_CONFIG_VALUE_GAME("spotLightShadowMapSizeAsPowerOf2_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_MAP_SIZE_AS_POWER_OF_2);
	//m_nearPlane = GET_CONFIG_VALUE_GAME("spotLightProjectionNearPlane", 0.1f);
	//m_shadowSoftness = GET_CONFIG_VALUE_GAME("spotLightShadowSoftness_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_SHADOW_SOFTNESS);
	//m_lightBleedingReductionFactor = GET_CONFIG_VALUE_GAME("spotLightLightBleedingReductionAmount_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_LIGHT_BLEEDING_REDUCTION_AMOUNT);
	//m_minVariance = GET_CONFIG_VALUE_GAME("spotLightMinVariance_" + m_lightIndexStr, M_DEFAULT_SPOT_LIGHT_MIN_VARIANCE);

	//// TODO: Setting additional spot light information
//}

void rendering::lighting::SpotLightBuilder::SetDefault()
{
	LightBuilder<SpotLight>::SetDefault();
	m_attenuation = m_defaultSpotLightAttenuation;
	m_viewAngle = m_defaultSpotLightViewAngle;
	m_shadowInfoProjectionNearPlane = m_defaultSpotLightShadowInfoProjectionNearPlane;
}


#ifdef BUILD_MESH_RENDERER
void Rendering::lighting::SpotLightBuilder::BuildMeshRenderer()
{
	// Rendering a small box around spot light node position to let the user see the source
	m_gameNode->AddComponent(new MeshRendererComponent(m_gameManager->GetMesh("SpotLight.obj"), new Rendering::Material(m_textureFactory.GetTexture(TextureIDs::SPOT_LIGHT), 1.0f, 8.0f)));
	m_gameNode->GetTransform().SetScale(0.1f); /* TODO: Don't use hard-coded values! Ever! */
}
#endif
/* ==================== SpotLightBuilder implementation end ==================== */