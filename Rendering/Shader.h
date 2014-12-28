#pragma once

#include "Rendering.h"
#include "Material.h"
#include "Renderer.h"
#include "Transform.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"
#include "Color.h"
#include "Utility\ReferenceCounter.h"
#include <map>
#include <vector>
#include <string>

namespace Rendering
{

struct TypedData
{
	std::string name;
	std::string type;
};

struct UniformStruct
{
	std::string name;
	std::vector<TypedData> memberNames;
};

class ShaderData : public Utility::ReferenceCounter
{
/* ==================== Static variables begin ==================== */
private:
	static int supportedOpenGLLevel;
	static std::string glslVersion;
/* ==================== Static variables end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	int programID;
	std::vector<int> shaders;
	std::vector<std::string> uniformNames;
	std::vector<std::string> uniformTypes;
	std::map<std::string, unsigned int> uniformMap;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	ShaderData(const std::string& fileName);
	virtual ~ShaderData(void);
private:
	ShaderData(const ShaderData& shaderData) {} // don't implement
	void operator=(const ShaderData& shaderData) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	int GetProgram() const { return programID; }
	std::vector<int>& GetShaders() { return shaders; }
	std::vector<std::string>& GetUniformNames() { return uniformNames; }
	std::vector<std::string>& GetUniformTypes() { return uniformTypes; }
	std::map<std::string, unsigned int>& GetUniformMap() { return uniformMap; }
	bool IsUniformPresent(const std::string& uniformName, std::map<std::string, unsigned int>::const_iterator& itr) const;
private:
	std::string LoadShaderData(const std::string& fileName) const;
	void AddVertexShader(const std::string& vertexShaderText);
	void AddFragmentShader(const std::string& fragmentShaderText);
	//void AddGeometryShader(const std::string& geometryShaderText);
	void AddProgram(const std::string& shaderText, GLenum type);

	void AddAllAttributes(const std::string& vertexShaderText, const std::string& attributeKeyword);
	void AddShaderUniforms(const std::string& shaderText);
	void AddUniform(const std::string& uniformName, const std::string& uniformType, const std::vector<UniformStruct>& structs);

	std::vector<UniformStruct> ShaderData::FindUniformStructs(const std::string& shaderText) const;
	std::string ShaderData::FindUniformStructName(const std::string& structStartToOpeningBrace) const;
	std::vector<TypedData> ShaderData::FindUniformStructComponents(const std::string& openingBraceToClosingBrace) const;
	
	bool Compile();
	bool CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength);
/* ==================== Non-static member functions end ==================== */
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
	RENDERING_API virtual ~Shader(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void Bind() const;
	RENDERING_API void Unbind() const;
	RENDERING_API virtual void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer) const;

	RENDERING_API void SetUniformi(const std::string& uniformName, int value) const;
	RENDERING_API void SetUniformf(const std::string& uniformName, Math::Real value) const;
	RENDERING_API void SetUniformVector3D(const std::string& uniformName, const Math::Vector3D& vector) const;
	RENDERING_API void SetUniformColor(const std::string& uniformName, const Color& color) const;
	RENDERING_API void SetUniformMatrix(const std::string& uniformName, const Math::Matrix4D& matrix) const;

	//void AddAllUniforms(const std::string& shaderText);
	//void AddUniform(const std::string& uniform);
	//void SetUniformi(const std::string& name, int value);
	//void SetUniformf(const std::string& name, Math::Real value);
	//void SetUniform(const std::string& name, const Math::Vector3D& vector);
	//void SetUniform(const std::string& name, const Math::Matrix4D& matrix);

	//virtual void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer);

	//void AddVertexShader(const std::string& vertexShaderText);
	//void AddFragmentShader(const std::string& fragmentShaderText);
	//void AddVertexShaderFromFile(const std::string& fileName);
	//void AddFragmentShaderFromFile(const std::string& fileName);
	////void AddGeometryShaderFromFile(const std::string& fileName);
	

protected:
	//std::string LoadShader(const std::string& fileName) const; // TODO: Consider making this function static
	//void AddProgram(const std::string& text, GLenum type);
private:
	void SetUniformDirectionalLight(const std::string& uniformName, const DirectionalLight& directionalLight) const;
	void SetUniformPointLight(const std::string& uniformName, const PointLight& pointLight) const;
	void SetUniformSpotLight(const std::string& uniformName, const SpotLight& spotLight) const;

	//void AddUniform(const std::string& uniformName, const std::string& uniformType, const std::vector<UniformStruct>& structs);
	//std::vector<UniformStruct> FindUniformStructs(const std::string& shaderText) const;
	//std::string FindUniformStructName(const std::string& structStartToOpeningBrace) const;
	//std::vector<TypedData> FindUniformStructComponents(const std::string& openingBraceToClosingBrace) const;
	//bool IsUniformPresent(const std::string& uniformName, std::map<std::string, unsigned int>::const_iterator& itr) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	ShaderData* shaderData;
	std::string fileName;
/* ==================== Non-static member variables end ==================== */
}; /* end class Shader */

} /* end namespace Rendering */