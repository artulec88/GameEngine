#include "stdafx.h"
#include "Uniform.h"
#include "Material.h"
#include "Renderer.h"

Rendering::Uniforms::UniformType Rendering::Uniforms::ConvertStringToUniformType(const std::string& uniformTypeStr)
{
	if (uniformTypeStr == "int") { return Uniforms::INT; }
	else if (uniformTypeStr == "float") { return Uniforms::REAL; }
	else if (uniformTypeStr == "vec2") { return Uniforms::VEC_2D; }
	else if (uniformTypeStr == "vec3") { return Uniforms::VEC_3D; }
	else if (uniformTypeStr == "vec4") { return Uniforms::VEC_4D; }
	else if (uniformTypeStr == "mat4") { return Uniforms::MATRIX_4x4; }
	else if (uniformTypeStr == "sampler2D") { return Uniforms::SAMPLER_2D; }
	else if (uniformTypeStr == "samplerCube") { return Uniforms::SAMPLER_CUBE; }
	else if (uniformTypeStr == "BaseLight") { return Uniforms::BASE_LIGHT; }
	else if (uniformTypeStr == "DirectionalLight") { return Uniforms::DIRECTIONAL_LIGHT; }
	else if (uniformTypeStr == "PointLight") { return Uniforms::POINT_LIGHT; }
	else if (uniformTypeStr == "SpotLight") { return Uniforms::SPOT_LIGHT; }
	else if (uniformTypeStr == "Attenuation") { return Uniforms::ATTENUATION; }
	else
	{
		EMERGENCY_LOG_RENDERING("Uniform type \"", uniformTypeStr, "\" not supported by the rendering engine.");
		exit(EXIT_FAILURE);
	}
}

/* ==================== UniformBase class implementation begin ==================== */
Rendering::Uniforms::UniformBase::UniformBase(const std::string& name, Uniforms::UniformType type) :
	m_name(name),
	m_prefix(m_name.substr(0, 2)),
	m_unprefixedName(m_name.substr(2, m_name.length())),
	m_type(type)
{
}

Rendering::Uniforms::UniformBase::~UniformBase()
{
}
/* ==================== UniformBase class implementation end ==================== */


/* ==================== SimpleUniform class implementation begin ==================== */
Rendering::Uniforms::SimpleUniform::SimpleUniform(const std::string& name, Uniforms::UniformType type, GLint location) :
	UniformBase(name, type),
	m_location(location)
{
}

Rendering::Uniforms::SimpleUniform::~SimpleUniform()
{
}
/* ==================== SimpleUniform class implementation end ==================== */


/* ==================== IntUniform class implementation begin ==================== */
Rendering::Uniforms::IntUniform::IntUniform(const std::string& name, GLint location) :
	SimpleUniform(name, INT, location)
{
}

Rendering::Uniforms::IntUniform::~IntUniform()
{
}

void Rendering::Uniforms::IntUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	//glUniform1i(GetLocation(), renderer->GetInt());
}

/* ==================== IntUniform class implementation end ==================== */


/* ==================== TextureUniform class implementation begin ==================== */
Rendering::Uniforms::TextureUniform::TextureUniform(const std::string& name, GLint location) :
	SimpleUniform(name, SAMPLER_2D, location)
{
}

Rendering::Uniforms::TextureUniform::~TextureUniform()
{
}

void Rendering::Uniforms::TextureUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	auto samplerSlot = renderer->GetSamplerSlot((GetPrefix() == "R_") ? GetUnprefixedName() : GetName());
	//CRITICAL_LOG_RENDERING("Binding texture \"", unprefixedName, "\" in sampler slot ", samplerSlot);
	unsigned int multitextureIndex = 0; // used only by the multitextures
	const Texture* texture = (GetPrefix() == "R_") ? renderer->GetTexture(GetUnprefixedName(), &multitextureIndex) : material->GetTexture(GetName());
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(texture != NULL, Utility::Logging::CRITICAL, "Updating uniforms operation failed. Rendering engine texture \"", GetUnprefixedName(), "\" is NULL.");
	texture->Bind(samplerSlot, multitextureIndex);
	glUniform1i(GetLocation(), samplerSlot);
}

/* ==================== TextureUniform class implementation end ==================== */


/* ==================== CubeTextureUniform class implementation begin ==================== */
Rendering::Uniforms::CubeTextureUniform::CubeTextureUniform(const std::string& name, GLint location) :
	SimpleUniform(name, SAMPLER_CUBE, location)
{
}

Rendering::Uniforms::CubeTextureUniform::~CubeTextureUniform()
{
}

void Rendering::Uniforms::CubeTextureUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	auto samplerSlot = renderer->GetSamplerSlot((GetPrefix() == "R_") ? GetUnprefixedName() : GetName());
	//CRITICAL_LOG_RENDERING("Binding texture \"", unprefixedName, "\" in sampler slot ", samplerSlot);
	if (GetUnprefixedName() == "cubeShadowMap")
	{
		renderer->BindCubeShadowMap(samplerSlot);
	}
	else
	{
		unsigned int multitextureIndex = 0; // used only by the multitextures
		const Texture* texture = (GetPrefix() == "R_") ? renderer->GetTexture(GetUnprefixedName(), &multitextureIndex) : material->GetTexture(GetName());
		CHECK_CONDITION_EXIT_ALWAYS_RENDERING(texture != NULL, Utility::Logging::CRITICAL, "Updating uniforms operation failed. Rendering engine texture \"", GetUnprefixedName(), "\" is NULL.");
		texture->Bind(samplerSlot, multitextureIndex);
	}
	glUniform1i(GetLocation(), samplerSlot);
}

/* ==================== CubeTextureUniform class implementation end ==================== */


/* ==================== RealUniform class implementation begin ==================== */
Rendering::Uniforms::RealUniform::RealUniform(const std::string& name, GLint location) :
	SimpleUniform(name, REAL, location)
{
}

Rendering::Uniforms::RealUniform::~RealUniform()
{
}

void Rendering::Uniforms::RealUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
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
Rendering::Uniforms::Vector2DUniform::Vector2DUniform(const std::string& name, GLint location) :
	SimpleUniform(name, VEC_2D, location)
{
}

Rendering::Uniforms::Vector2DUniform::~Vector2DUniform()
{
}

void Rendering::Uniforms::Vector2DUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	const Math::Vector2D& vector = renderer->GetVec2D(GetUnprefixedName());
	glUniform2f(GetLocation(), vector.GetX(), vector.GetY());
}
/* ==================== Vector2DUniform class implementation end ==================== */


/* ==================== Vector3DUniform class implementation begin ==================== */
Rendering::Uniforms::Vector3DUniform::Vector3DUniform(const std::string& name, GLint location) :
	SimpleUniform(name, VEC_3D, location)
{
}

Rendering::Uniforms::Vector3DUniform::~Vector3DUniform()
{
}

void Rendering::Uniforms::Vector3DUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	if (GetPrefix() == "R_")
	{
		const Math::Vector3D& vector = renderer->GetVec3D(GetUnprefixedName());
		glUniform3f(GetLocation(), vector.GetX(), vector.GetY(), vector.GetZ());
	}
	else if (GetName() == "C_eyePos")
	{
		const Math::Vector3D& vector = renderer->GetCurrentCamera().GetPos();
		glUniform3f(GetLocation(), vector.GetX(), vector.GetY(), vector.GetZ());
	}
	else
	{
		const Math::Vector3D& vector = material->GetVec3D(GetName());
		glUniform3f(GetLocation(), vector.GetX(), vector.GetY(), vector.GetZ());
	}
}
/* ==================== Vector3DUniform class implementation end ==================== */


/* ==================== Vector4DUniform class implementation begin ==================== */
Rendering::Uniforms::Vector4DUniform::Vector4DUniform(const std::string& name, GLint location) :
	SimpleUniform(name, VEC_4D, location)
{
}

Rendering::Uniforms::Vector4DUniform::~Vector4DUniform()
{
}

void Rendering::Uniforms::Vector4DUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	if (GetPrefix() == "R_")
	{
		const Math::Vector4D& vector = renderer->GetVec4D(GetUnprefixedName());
		glUniform4f(GetLocation(), vector.GetX(), vector.GetY(), vector.GetZ(), vector.GetW());
	}
	else
	{
		const Math::Vector4D& vector = material->GetVec4D(GetName());
		glUniform4f(GetLocation(), vector.GetX(), vector.GetY(), vector.GetZ(), vector.GetW());
	}
}
/* ==================== Vector4DUniform class implementation end ==================== */


/* ==================== MatrixUniform class implementation begin ==================== */
Rendering::Uniforms::MatrixUniform::MatrixUniform(const std::string& name, GLint location) :
	SimpleUniform(name, MATRIX_4x4, location)
{
}

Rendering::Uniforms::MatrixUniform::~MatrixUniform()
{
}

void Rendering::Uniforms::MatrixUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	if (GetName() == "T_lightMatrix")
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, &((renderer->GetLightMatrix() * transform->GetTransformation())[0][0]));
#else
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, (renderer->GetLightMatrix() * transform->GetTransformation()).At(0));
#endif
	}
	else if (GetName() == "T_MVP")
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, &(renderer->GetCurrentCamera().GetViewProjection() * transform->GetTransformation())[0][0]);
#else
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, (renderer->GetCurrentCamera().GetViewProjection() * transform->GetTransformation()).At(0));
#endif
	}
	else if (GetName() == "T_VP")
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, &(renderer->GetCurrentCamera().GetViewProjection()[0][0]));
#else
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, renderer->GetCurrentCamera().GetViewProjection().At(0));
#endif
	}
	else if (GetName() == "T_model")
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, &(transform->GetTransformation()[0][0]));
#else
		glUniformMatrix4fv(GetLocation(), 1, GL_FALSE, transform->GetTransformation().At(0));
#endif
	}
}
/* ==================== MatrixUniform class implementation end ==================== */

/* ==================== BaseLightUniform class implementation begin ==================== */
Rendering::Uniforms::BaseLightUniform::BaseLightUniform(const std::string& name, UniformType type, GLint colorLocation, GLint intensityLocation) :
	UniformBase(name, BASE_LIGHT),
	m_colorLocation(colorLocation),
	m_intensityLocation(intensityLocation)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(colorLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create base light uniform. The color location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(intensityLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create base light uniform. The intensity location is invalid.");
}

Rendering::Uniforms::BaseLightUniform::~BaseLightUniform()
{
}
/* ==================== BaseLightUniform class implementation end ==================== */


/* ==================== DirectionalLightUniform class implementation begin ==================== */
Rendering::Uniforms::DirectionalLightUniform::DirectionalLightUniform(const std::string& name, GLint colorLocation, GLint intensityLocation, GLint directionLocation) :
	BaseLightUniform(name, DIRECTIONAL_LIGHT, colorLocation, intensityLocation),
	m_directionLocation(directionLocation)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(directionLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create directional light uniform. The direction location is invalid.");
}

Rendering::Uniforms::DirectionalLightUniform::~DirectionalLightUniform()
{
}

void Rendering::Uniforms::DirectionalLightUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	const Lighting::BaseLight* currentLight = renderer->GetCurrentLight();
	const Math::Vector3D& forwardVector = currentLight->GetTransform().GetTransformedRot().GetForward();
	glUniform3f(m_directionLocation, forwardVector.GetX(), forwardVector.GetY(), forwardVector.GetZ());
	glUniform4f(GetColorLocation(), currentLight->GetColor().GetRed(), currentLight->GetColor().GetGreen(), currentLight->GetColor().GetBlue(), currentLight->GetColor().GetAlpha());
	glUniform1f(GetIntensityLocation(), currentLight->GetIntensity());
}
/* ==================== DirectionalLightUniform class implementation end ==================== */


/* ==================== PointLightUniform class implementation begin ==================== */
Rendering::Uniforms::PointLightUniform::PointLightUniform(const std::string& name, GLint colorLocation, GLint intensityLocation, GLint constantFactorLocation, GLint linearFactorLocation,
	GLint exponentFactorLocation, GLint positionLocation, GLint rangeLocation) :
	PointLightUniform(name, POINT_LIGHT, colorLocation, intensityLocation, constantFactorLocation, linearFactorLocation, exponentFactorLocation, positionLocation, rangeLocation)
{
}

Rendering::Uniforms::PointLightUniform::PointLightUniform(const std::string& name, UniformType type, GLint colorLocation, GLint intensityLocation, GLint constantFactorLocation,
	GLint linearFactorLocation, GLint exponentFactorLocation, GLint positionLocation, GLint rangeLocation) :
	BaseLightUniform(name, type, colorLocation, intensityLocation),
	m_constantFactorLocation(constantFactorLocation),
	m_linearFactorLocation(linearFactorLocation),
	m_exponentFactorLocation(exponentFactorLocation),
	m_positionLocation(positionLocation),
	m_rangeLocation(rangeLocation)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(constantFactorLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create point light uniform. The attenuation constant factor location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(linearFactorLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create point light uniform. The attenuation linear factor location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(exponentFactorLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create point light uniform. The attenuation exponent factor location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(positionLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create point light uniform. The position location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(rangeLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create point light uniform. The range location is invalid.");
}

Rendering::Uniforms::PointLightUniform::~PointLightUniform()
{
}

void Rendering::Uniforms::PointLightUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	const Lighting::PointLight* pointLight = renderer->GetCurrentPointLight();
	glUniform4f(GetColorLocation(), pointLight->GetColor().GetRed(), pointLight->GetColor().GetGreen(), pointLight->GetColor().GetBlue(), pointLight->GetColor().GetAlpha());
	glUniform1f(GetIntensityLocation(), pointLight->GetIntensity());
	glUniform1f(m_constantFactorLocation, pointLight->GetAttenuation().GetConstant());
	glUniform1f(m_linearFactorLocation, pointLight->GetAttenuation().GetLinear());
	glUniform1f(m_exponentFactorLocation, pointLight->GetAttenuation().GetExponent());
	const Math::Vector3D& posVector = pointLight->GetTransform().GetTransformedPos();
	glUniform3f(m_positionLocation, posVector.GetX(), posVector.GetY(), posVector.GetZ());
	glUniform1f(m_rangeLocation, pointLight->GetRange());
}
/* ==================== PointLightUniform class implementation end ==================== */


/* ==================== SpotLightUniform class implementation begin ==================== */
Rendering::Uniforms::SpotLightUniform::SpotLightUniform(const std::string& name, GLint colorLocation, GLint intensityLocation, GLint constantFactorLocation, GLint linearFactorLocation,
	GLint exponentFactorLocation, GLint positionLocation, GLint rangeLocation, GLint directionLocation, GLint cutoffLocation) :
	PointLightUniform(name, SPOT_LIGHT, colorLocation, intensityLocation, constantFactorLocation, linearFactorLocation, exponentFactorLocation, positionLocation, rangeLocation),
	m_directionLocation(directionLocation),
	m_cutoffLocation(cutoffLocation)
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_directionLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create spot light uniform. The direction location is invalid.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_cutoffLocation != INVALID_LOCATION, Utility::Logging::EMERGENCY,
		"Cannot create spot light uniform. The cutoff location is invalid.");
}

Rendering::Uniforms::SpotLightUniform::~SpotLightUniform()
{
}

void Rendering::Uniforms::SpotLightUniform::Update(const Renderer* renderer, const Math::Transform* transform, const Material* material)
{
	const Lighting::SpotLight* spotLight = dynamic_cast<const Lighting::SpotLight*>(renderer->GetCurrentLight());
	glUniform4f(GetColorLocation(), spotLight->GetColor().GetRed(), spotLight->GetColor().GetGreen(), spotLight->GetColor().GetBlue(), spotLight->GetColor().GetAlpha());
	glUniform1f(GetIntensityLocation(), spotLight->GetIntensity());
	glUniform1f(m_constantFactorLocation, spotLight->GetAttenuation().GetConstant());
	glUniform1f(m_linearFactorLocation, spotLight->GetAttenuation().GetLinear());
	glUniform1f(m_exponentFactorLocation, spotLight->GetAttenuation().GetExponent());
	const Math::Vector3D& posVector = spotLight->GetTransform().GetTransformedPos();
	glUniform3f(m_positionLocation, posVector.GetX(), posVector.GetY(), posVector.GetZ());
	glUniform1f(m_rangeLocation, spotLight->GetRange());
	const Math::Vector3D& forwardVector = spotLight->GetTransform().GetTransformedRot().GetForward();
	glUniform3f(m_directionLocation, forwardVector.GetX(), forwardVector.GetY(), forwardVector.GetZ());
	glUniform1f(m_cutoffLocation, spotLight->GetCutoff());
}
/* ==================== SpotLightUniform class implementation end ==================== */