#pragma once

#include "Rendering.h"
#include "Material.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"
#include <map>
#include <vector>
#include <string>

namespace Rendering
{

class Shader
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
	void AddUniform(const std::string& uniform);
	void SetUniformi(const std::string& name, int value);
	void SetUniformf(const std::string& name, Math::Real value);
	void SetUniform(const std::string& name, const Math::Vector3D& vector);
	void SetUniform(const std::string& name, const Math::Matrix4D& matrix);

	virtual void UpdateUniforms(const Math::Matrix4D& worldMatrix, const Math::Matrix4D& projectedMatrix, const Material& material);

	void AddVertexShaderFromFile(const std::string& fileName);
	void AddFragmentShaderFromFile(const std::string& fileName);
	//void AddGeometryShaderFromFile(const std::string& fileName);

	bool Compile();
	bool CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength);
	void Bind();

protected:
	std::string LoadShader(const std::string& fileName) const; // TODO: Consider making this function static
	void AddProgram(const std::string& text, GLenum type);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	int program;
	std::map<std::string, unsigned int> uniforms;
	std::vector<int> shaders;
/* ==================== Non-static member variables end ==================== */
}; /* end class Shader */

} /* end namespace Rendering */