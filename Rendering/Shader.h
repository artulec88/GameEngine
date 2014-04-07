#pragma once

#include "Rendering.h"
#include "Material.h"
#include "Renderer.h"
#include "Transform.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"
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

class RENDERING_API Shader
{
/* ==================== Static functions begin ==================== */
//protected:
//	static std::string LoadShader(const std::string& fileName); // TODO: Consider making this function non-static
/* ==================== Static functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Shader(void);
	virtual ~Shader(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void AddAllUniforms(const std::string& shaderText);
	void AddUniform(const std::string& uniform);
	void SetUniformi(const std::string& name, int value);
	void SetUniformf(const std::string& name, Math::Real value);
	void SetUniform(const std::string& name, const Math::Vector3D& vector);
	void SetUniform(const std::string& name, const Math::Matrix4D& matrix);

	virtual void UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer);

	void AddVertexShader(const std::string& vertexShaderText);
	void AddFragmentShader(const std::string& fragmentShaderText);
	void AddVertexShaderFromFile(const std::string& fileName);
	void AddFragmentShaderFromFile(const std::string& fileName);
	//void AddGeometryShaderFromFile(const std::string& fileName);

	bool Compile();
	bool CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength);
	void Bind();

protected:
	std::string LoadShader(const std::string& fileName) const; // TODO: Consider making this function static
	void AddProgram(const std::string& text, GLenum type);
private:
	void AddUniform(const std::string& uniformName, const std::string& uniformType, const std::vector<UniformStruct>& structs);
	std::vector<UniformStruct> FindUniformStructs(const std::string& shaderText) const;
	std::string FindUniformStructName(const std::string& structStartToOpeningBrace) const;
	std::vector<TypedData> FindUniformStructComponents(const std::string& openingBraceToClosingBrace) const;
	bool IsUniformPresent(const std::string& uniformName, std::map<std::string, unsigned int>::const_iterator& itr) const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	int program;
	std::vector<std::string> uniformNames;
	std::vector<std::string> uniformTypes;
	std::map<std::string, unsigned int> uniforms;
	std::vector<int> shaders;
/* ==================== Non-static member variables end ==================== */
}; /* end class Shader */

} /* end namespace Rendering */