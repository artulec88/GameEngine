#ifndef __RENDERING_SHADER_H__
#define __RENDERING_SHADER_H__

#include "Rendering.h"
#include "Material.h"
#include "Renderer.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Color.h"

#include "Math\Transform.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"

#include "Utility\ReferenceCounter.h"
#include <map>
#include <vector>
#include <string>

namespace Rendering
{

namespace Uniforms
{
	enum UniformType
	{
		VEC_2D = 0,
		VEC_3D,
		VEC_4D,
		MATRIX_4x4,
		REAL,
		SAMPLER_2D,
		SAMPLER_CUBE,
		BASE_LIGHT,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
		ATTENUATION
	};

	UniformType ConvertStringToUniformType(const std::string& uniformTypeStr);
	std::string ConvertUniformTypeToString(UniformType uniformType);
} /* end namespace Uniforms */

struct TypedData
{
	TypedData(const std::string& _name, Uniforms::UniformType _uniformType) :
		name(_name),
		uniformType(_uniformType)
	{
	}

	const std::string name;
	const Uniforms::UniformType uniformType;
};

struct UniformStruct
{
	std::string name;
	std::vector<TypedData> memberNames;
};

class ShaderData : public Utility::ReferenceCounter
{
/* ==================== Static variables and functions begin ==================== */
private:
	static const std::string ATTRIBUTE_KEYWORD;
	static const std::string UNIFORM_KEYWORD;
	static const std::string SINGLE_LINE_COMMENT;
	static const std::string MULTI_LINE_COMMENT_BEGIN;
	static const std::string MULTI_LINE_COMMENT_END;
	static const char* UniformTypeNames[];
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ShaderData(const std::string& fileName);
	~ShaderData(void);
private:
	ShaderData(const ShaderData& shaderData) {} // don't implement
	void operator=(const ShaderData& shaderData) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	GLuint GetProgram() const { return m_programID; }
	std::vector<std::string>& GetUniformNames() { return m_uniformNames; }
	std::vector<Uniforms::UniformType>& GetUniformTypes() { return m_uniformTypes; }
	std::map<std::string, GLint>& GetUniformMap() { return m_uniformMap; }
	bool IsUniformPresent(const std::string& uniformName, std::map<std::string, GLint>::const_iterator& itr) const;
private:
	std::string LoadShaderData(const std::string& fileName) const;
	void AddVertexShader(const std::string& vertexShaderText);
	void AddGeometryShader(const std::string& geometryShaderText);
	void AddFragmentShader(const std::string& fragmentShaderText);
	void AddProgram(const std::string& shaderText, GLenum type);

	void AddAllAttributes(const std::string& vertexShaderText);
	void AddShaderUniforms(const std::string& shaderText);
	void AddUniform(const std::string& uniformName, Uniforms::UniformType uniformType, const std::vector<UniformStruct>& structs);

	std::vector<UniformStruct> ShaderData::FindUniformStructs(const std::string& shaderText) const;
	std::string ShaderData::FindUniformStructName(const std::string& structStartToOpeningBrace) const;
	std::vector<TypedData> ShaderData::FindUniformStructComponents(const std::string& openingBraceToClosingBrace) const;
	
	bool Compile();
	bool CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	GLuint m_programID;
	std::vector<GLuint> m_shaders;
	std::vector<std::string> m_uniformNames;
	std::vector<Uniforms::UniformType> m_uniformTypes;
	std::map<std::string, GLint> m_uniformMap;
	/* ==================== Non-static member variables end ==================== */
}; /* end class ShaderData */

class Shader
{
/* ==================== Static variables begin ==================== */
private:
	static std::map<std::string, ShaderData*> shaderResourceMap;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Shader(const std::string& fileName);
	RENDERING_API ~Shader(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	GLuint GetProgramID() const { return m_shaderData->GetProgram(); }
	void Bind() const;
	bool IsBound() const;
	void Unbind() const;
	void UpdateUniforms(const Math::Transform& transform, const Material* material, const Renderer* renderer) const;

	void SetUniformi(const std::string& uniformName, int value) const;
	void SetUniformf(const std::string& uniformName, Math::Real value) const;
	void SetUniformVector3D(const std::string& uniformName, const Math::Vector3D& vector) const;
	void SetUniformVector4D(const std::string& uniformName, const Math::Vector4D& vector) const;
	void SetUniformVector4D(const std::string& uniformName, Math::Real x, Math::Real y, Math::Real z, Math::Real w) const;
	void SetUniformColor(const std::string& uniformName, const Color& color) const;
	void SetUniformMatrix(const std::string& uniformName, const Math::Matrix4D& matrix) const;
protected:
private:
	/// <summary>
	/// Sets the directional light uniform. Although the <paramref name="directionalLight"/> is specified as the BaseLight instance
	/// it holds all the information necessary for the directional light. Color and intensity are directly stored in the BaseLight object
	/// and the direction can be easily retrieved from the transformation.
	/// </summary>
	void SetUniformDirectionalLight(const std::string& uniformName, const Lighting::BaseLight& directionalLight) const;
	/// <summary>
	/// Sets the point light uniform.
	/// </summary>
	void SetUniformPointLight(const std::string& uniformName, const Lighting::PointLight& pointLight) const;
	/// <summary>
	/// Sets the spot light uniform.
	/// </summary>
	void SetUniformSpotLight(const std::string& uniformName, const Lighting::SpotLight& spotLight) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	ShaderData* m_shaderData;
	std::string m_fileName;
/* ==================== Non-static member variables end ==================== */
}; /* end class Shader */

} /* end namespace Rendering */

#endif // __RENDERING_SHADER_H__