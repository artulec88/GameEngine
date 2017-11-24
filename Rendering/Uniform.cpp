#include "stdafx.h"
#include "Uniform.h"
#include "Material.h"
#include "Renderer.h"
#include "SpotLight.h"

rendering::uniforms::UniformType rendering::uniforms::ConvertStringToUniformType(const std::string& uniformTypeStr)
{
	if (uniformTypeStr == "int") { return INT; }
	if (uniformTypeStr == "float") { return REAL; }
	if (uniformTypeStr == "vec2") { return VEC_2; }
	if (uniformTypeStr == "vec3") { return VEC_3; }
	if (uniformTypeStr == "vec4") { return VEC_4; }
	if (uniformTypeStr == "mat4") { return MATRIX_4x4; }
	if (uniformTypeStr == "sampler2D") { return SAMPLER_2D; }
	if (uniformTypeStr == "samplerCube") { return SAMPLER_CUBE; }
	if (uniformTypeStr == "BaseLight") { return BASE_LIGHT; }
	if (uniformTypeStr == "DirectionalLight") { return DIRECTIONAL_LIGHT; }
	if (uniformTypeStr == "PointLight") { return POINT_LIGHT; }
	if (uniformTypeStr == "SpotLight") { return SPOT_LIGHT; }
	if (uniformTypeStr == "Attenuation") { return ATTENUATION; }
	
	EMERGENCY_LOG_RENDERING("Uniform type \"", uniformTypeStr, "\" not supported by the rendering engine.");
	exit(EXIT_FAILURE);
}

/* ==================== UniformBase class implementation begin ==================== */
rendering::uniforms::UniformBase::UniformBase(const std::string& name, UniformType type) :
	m_name(name),
	m_prefix(m_name.substr(0, 2)),
	m_unprefixedName(m_name.substr(2, m_name.length())),
	m_type(type)
{
}

rendering::uniforms::UniformBase::~UniformBase()
{
}
/* ==================== UniformBase class implementation end ==================== */


/* ==================== SimpleUniform class implementation begin ==================== */
rendering::uniforms::SimpleUniform::SimpleUniform(const std::string& name, UniformType type, GLint location) :
	UniformBase(name, type),
	m_location(location)
{
}

rendering::uniforms::SimpleUniform::~SimpleUniform()
{
}
/* ==================== SimpleUniform class implementation end ==================== */


/* ==================== IntUniform class implementation begin ==================== */
rendering::uniforms::IntUniform::IntUniform(const std::string& name, GLint location) :
	SimpleUniform(name, INT, location)
{
}

rendering::uniforms::IntUniform::~IntUniform()
{
}

void rendering::uniforms::IntUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	//glUniform1i(GetLocation(), renderer->GetInt());
}

/* ==================== IntUniform class implementation end ==================== */


/* ==================== TextureUniform class implementation begin ==================== */
rendering::uniforms::TextureUniform::TextureUniform(const std::string& name, GLint location) :
	SimpleUniform(name, SAMPLER_2D, location)
{
}

rendering::uniforms::TextureUniform::~TextureUniform()
{
}

void rendering::uniforms::TextureUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	const auto samplerSlot = renderer->GetSamplerSlot(GetPrefix() == "R_" ? GetUnprefixedName() : GetName());
	//CRITICAL_LOG_RENDERING("Binding texture \"", unprefixedName, "\" in sampler slot ", samplerSlot);
	unsigned int multitextureIndex = 0; // used only by the multitextures
	const auto texture = GetPrefix() == "R_" ? renderer->GetTexture(GetUnprefixedName(), &multitextureIndex) : material->GetTexture(GetName());
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(texture != NULL, utility::logging::CRITICAL, "Updating uniforms operation failed. Rendering engine texture \"", GetUnprefixedName(), "\" is NULL.");
	texture->Bind(samplerSlot, multitextureIndex);
	glUniform1i(GetLocation(), samplerSlot);
}

/* ==================== TextureUniform class implementation end ==================== */


/* ==================== CubeTextureUniform class implementation begin ==================== */
rendering::uniforms::CubeTextureUniform::CubeTextureUniform(const std::string& name, GLint location) :
	SimpleUniform(name, SAMPLER_CUBE, location)
{
}

rendering::uniforms::CubeTextureUniform::~CubeTextureUniform()
{
}

void rendering::uniforms::CubeTextureUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	const auto samplerSlot = renderer->GetSamplerSlot(GetPrefix() == "R_" ? GetUnprefixedName() : GetName());
	//CRITICAL_LOG_RENDERING("Binding texture \"", unprefixedName, "\" in sampler slot ", samplerSlot);
	if (GetUnprefixedName() == "cubeShadowMap")
	{
		renderer->BindCubeShadowMap(samplerSlot);
	}
	else
	{
		unsigned int multitextureIndex = 0; // used only by the multitextures
		const auto* texture = GetPrefix() == "R_" ? renderer->GetTexture(GetUnprefixedName(), &multitextureIndex) : material->GetTexture(GetName());
		CHECK_CONDITION_EXIT_ALWAYS_RENDERING(texture != NULL, utility::logging::CRITICAL, "Updating uniforms operation failed. Rendering engine texture \"", GetUnprefixedName(), "\" is NULL.");
		texture->Bind(samplerSlot, multitextureIndex);
	}
	glUniform1i(GetLocation(), samplerSlot);
}

/* ==================== CubeTextureUniform class implementation end ==================== */


/* ==================== RealUniform class implementation begin ==================== */
rendering::uniforms::RealUniform::RealUniform(const std::string& name, GLint location) :
	SimpleUniform(name, REAL, location)
{
}

rendering::uniforms::RealUniform::~RealUniform()
{
}

void rendering::uniforms::RealUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	if (GetPrefix() == "R_")
	{
		glUniform1f(GetLocation(), renderer->GetReal(GetUnprefixedName()));
	}
	else if (GetName() == "T_scale")
	{
		glUniform1f(GetLocation(), transform->GetScale());
	}
	else
	{
		glUniform1f(GetLocation(), material->GetReal(GetName()));
	}
}

/* ==================== RealUniform class implementation end ==================== */


/* ==================== Vector2DUniform class implementation begin ==================== */
rendering::uniforms::Vector2DUniform::Vector2DUniform(const std::string& name, GLint location) :
	SimpleUniform(name, VEC_2, location)
{
}

rendering::uniforms::Vector2DUniform::~Vector2DUniform()
{
}

void rendering::uniforms::Vector2DUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	const auto& vector = renderer->GetVec2D(GetUnprefixedName());
	glUniform2f(GetLocation(), vector.x, vector.y);
}
/* ==================== Vector2DUniform class implementation end ==================== */


/* ==================== Vector3DUniform class implementation begin ==================== */
rendering::uniforms::Vector3DUniform::Vector3DUniform(const std::string& name, GLint location) :
	SimpleUniform(name, VEC_3, location)
{
}

rendering::uniforms::Vector3DUniform::~Vector3DUniform()
{
}

void rendering::uniforms::Vector3DUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	if (GetPrefix() == "R_")
	{
		const auto& vector = renderer->GetVec3D(GetUnprefixedName());
		glUniform3f(GetLocation(), vector.x, vector.y, vector.z);
	}
	else if (GetName() == "C_eyePos")
	{
		const auto& vector = renderer->GetCurrentCamera().GetTransform().GetPos();
		glUniform3f(GetLocation(), vector.x, vector.y, vector.z);
	}
	else
	{
		const auto& vector = material->GetVec3D(GetName());
		glUniform3f(GetLocation(), vector.x, vector.y, vector.z);
	}
}
/* ==================== Vector3DUniform class implementation end ==================== */


/* ==================== Vector4DUniform class implementation begin ==================== */
rendering::uniforms::Vector4DUniform::Vector4DUniform(const std::string& name, GLint location) :
	SimpleUniform(name, VEC_4, location)
{
}

rendering::uniforms::Vector4DUniform::~Vector4DUniform()
{
}

void rendering::uniforms::Vector4DUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	if (GetPrefix() == "R_")
	{
		const auto& vector = renderer->GetVec4D(GetUnprefixedName());
		glUniform4f(GetLocation(), vector.x, vector.y, vector.z, vector.w);
	}
	else
	{
		const auto& vector = material->GetVec4D(GetName());
		glUniform4f(GetLocation(), vector.x, vector.y, vector.z, vector.w);
	}
}
/* ==================== Vector4DUniform class implementation end ==================== */


/* ==================== MatrixUniform class implementation begin ==================== */
rendering::uniforms::MatrixUniform::MatrixUniform(const std::string& name, GLint location) :
	SimpleUniform(name, MATRIX_4x4, location)
{
}

rendering::uniforms::MatrixUniform::~MatrixUniform()
{
}

void rendering::uniforms::MatrixUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	if (GetName() == "T_lightMatrix")
	{
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, (renderer->GetLightMatrix() * transform->GetTransformation()).Data());
	}
	else if (GetName() == "T_MVP")
	{
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, (renderer->GetCurrentCamera().GetViewProjection() * transform->GetTransformation()).Data());
	}
	else if (GetName() == "T_VP")
	{
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, renderer->GetCurrentCamera().GetViewProjection().Data());
	}
	else if (GetName() == "T_model")
	{
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, transform->GetTransformation().Data());
	}
}
/* ==================== MatrixUniform class implementation end ==================== */

/* ==================== BaseLightUniform class implementation begin ==================== */
rendering::uniforms::BaseLightUniform::BaseLightUniform(const std::string& name, UniformType type, GLint colorLocation, GLint intensityLocation) :
	UniformBase(name, BASE_LIGHT),
	m_colorLocation(colorLocation),
	m_intensityLocation(intensityLocation)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(colorLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create base light uniform. The color location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(intensityLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create base light uniform. The intensity location is invalid.");
}

rendering::uniforms::BaseLightUniform::~BaseLightUniform()
{
}
/* ==================== BaseLightUniform class implementation end ==================== */


/* ==================== DirectionalLightUniform class implementation begin ==================== */
rendering::uniforms::DirectionalLightUniform::DirectionalLightUniform(const std::string& name, GLint colorLocation, GLint intensityLocation, GLint directionLocation) :
	BaseLightUniform(name, DIRECTIONAL_LIGHT, colorLocation, intensityLocation),
	m_directionLocation(directionLocation)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(directionLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create directional light uniform. The direction location is invalid.");
}

rendering::uniforms::DirectionalLightUniform::~DirectionalLightUniform()
{
}

void rendering::uniforms::DirectionalLightUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	const auto currentLight = renderer->GetCurrentLight();
	const auto& forwardVector = currentLight->GetTransform().GetTransformedRot().GetForward();
	glUniform3f(m_directionLocation, forwardVector.x, forwardVector.y, forwardVector.z);
	glUniform4f(GetColorLocation(), currentLight->GetColor().GetRed(), currentLight->GetColor().GetGreen(), currentLight->GetColor().GetBlue(), currentLight->GetColor().GetAlpha());
	glUniform1f(GetIntensityLocation(), currentLight->GetIntensity());
}
/* ==================== DirectionalLightUniform class implementation end ==================== */


/* ==================== PointLightUniform class implementation begin ==================== */
rendering::uniforms::PointLightUniform::PointLightUniform(const std::string& name, GLint colorLocation, GLint intensityLocation, GLint constantFactorLocation, GLint linearFactorLocation,
	GLint exponentFactorLocation, GLint positionLocation, GLint rangeLocation) :
	PointLightUniform(name, POINT_LIGHT, colorLocation, intensityLocation, constantFactorLocation, linearFactorLocation, exponentFactorLocation, positionLocation, rangeLocation)
{
}

rendering::uniforms::PointLightUniform::PointLightUniform(const std::string& name, UniformType type, GLint colorLocation, GLint intensityLocation, GLint constantFactorLocation,
	GLint linearFactorLocation, GLint exponentFactorLocation, GLint positionLocation, GLint rangeLocation) :
	BaseLightUniform(name, type, colorLocation, intensityLocation),
	m_constantFactorLocation(constantFactorLocation),
	m_linearFactorLocation(linearFactorLocation),
	m_exponentFactorLocation(exponentFactorLocation),
	m_positionLocation(positionLocation),
	m_rangeLocation(rangeLocation)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(constantFactorLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create point light uniform. The attenuation constant factor location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(linearFactorLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create point light uniform. The attenuation linear factor location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(exponentFactorLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create point light uniform. The attenuation exponent factor location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(positionLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create point light uniform. The position location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(rangeLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create point light uniform. The range location is invalid.");
}

rendering::uniforms::PointLightUniform::~PointLightUniform()
{
}

void rendering::uniforms::PointLightUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	const auto pointLight = renderer->GetCurrentPointLight();
	glUniform4f(GetColorLocation(), pointLight->GetColor().GetRed(), pointLight->GetColor().GetGreen(), pointLight->GetColor().GetBlue(), pointLight->GetColor().GetAlpha());
	glUniform1f(GetIntensityLocation(), pointLight->GetIntensity());
	glUniform1f(m_constantFactorLocation, pointLight->GetAttenuation().GetConstant());
	glUniform1f(m_linearFactorLocation, pointLight->GetAttenuation().GetLinear());
	glUniform1f(m_exponentFactorLocation, pointLight->GetAttenuation().GetExponent());
	const auto& posVector = pointLight->GetTransform().GetTransformedPos();
	glUniform3f(m_positionLocation, posVector.x, posVector.y, posVector.z);
	glUniform1f(m_rangeLocation, pointLight->GetRange());
}
/* ==================== PointLightUniform class implementation end ==================== */


/* ==================== SpotLightUniform class implementation begin ==================== */
rendering::uniforms::SpotLightUniform::SpotLightUniform(const std::string& name, GLint colorLocation, GLint intensityLocation, GLint constantFactorLocation, GLint linearFactorLocation,
	GLint exponentFactorLocation, GLint positionLocation, GLint rangeLocation, GLint directionLocation, GLint cutoffLocation) :
	PointLightUniform(name, SPOT_LIGHT, colorLocation, intensityLocation, constantFactorLocation, linearFactorLocation, exponentFactorLocation, positionLocation, rangeLocation),
	m_directionLocation(directionLocation),
	m_cutoffLocation(cutoffLocation)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_directionLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create spot light uniform. The direction location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_cutoffLocation != INVALID_LOCATION, utility::logging::EMERGENCY,
		"Cannot create spot light uniform. The cutoff location is invalid.");
}

rendering::uniforms::SpotLightUniform::~SpotLightUniform()
{
}

void rendering::uniforms::SpotLightUniform::Update(const Renderer* renderer, const math::Transform* transform, const Material* material)
{
	const auto spotLight = dynamic_cast<const lighting::SpotLight*>(renderer->GetCurrentLight());
	glUniform4f(GetColorLocation(), spotLight->GetColor().GetRed(), spotLight->GetColor().GetGreen(), spotLight->GetColor().GetBlue(), spotLight->GetColor().GetAlpha());
	glUniform1f(GetIntensityLocation(), spotLight->GetIntensity());
	glUniform1f(m_constantFactorLocation, spotLight->GetAttenuation().GetConstant());
	glUniform1f(m_linearFactorLocation, spotLight->GetAttenuation().GetLinear());
	glUniform1f(m_exponentFactorLocation, spotLight->GetAttenuation().GetExponent());
	const auto& posVector = spotLight->GetTransform().GetTransformedPos();
	glUniform3f(m_positionLocation, posVector.x, posVector.y, posVector.z);
	glUniform1f(m_rangeLocation, spotLight->GetRange());
	const auto& forwardVector = spotLight->GetTransform().GetTransformedRot().GetForward();
	glUniform3f(m_directionLocation, forwardVector.x, forwardVector.y, forwardVector.z);
	glUniform1f(m_cutoffLocation, spotLight->GetCutoff());
}
/* ==================== SpotLightUniform class implementation end ==================== */