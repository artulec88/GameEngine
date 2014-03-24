#include "StdAfx.h"
#include "Shader.h"

#include "Utility\Utility.h"
#include "Utility\Log.h"

#include <fstream>

using namespace Rendering;
using namespace Utility;
using namespace std;

Shader::Shader(void)
{
	program = glCreateProgram();

	if (program == 0)
	{
		stdlog(Error, LOGPLACE, "Error while creating shader program");
	}
}


Shader::~Shader(void)
{
	stdlog(Debug, LOGPLACE, "Destroying shader instance");
	for (vector<int>::iterator it = shaders.begin(); it != shaders.end(); ++it)
	{
		glDetachShader(program, *it);
		glDeleteShader(*it);
	}

	glDeleteProgram(program);
}

void Shader::Bind()
{
	glUseProgram(program);
}

bool Shader::Compile()
{
	bool compileSuccess = true;

	glLinkProgram(program);
	int infoLogLength;
	if (CheckForErrors(program, GL_LINK_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(program, infoLogLength, NULL, &errorMessage[0]);
		stdlog(Error, LOGPLACE, "Error linking shader program: \"%s\"", &errorMessage[0]);
	}

	glValidateProgram(program);
	if (CheckForErrors(program, GL_VALIDATE_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(program, infoLogLength, NULL, &errorMessage[0]);
		stdlog(Error, LOGPLACE, "Error linking shader program: \"%s\"", &errorMessage[0]);
	}

	if (!compileSuccess)
	{
		stdlog(Warning, LOGPLACE, "Shader program %d compilation error occurred. Investigate the problem.", program);
	}
	else
	{
		stdlog(Notice, LOGPLACE, "Shader program %d compiled successfully", program);
	}
	return compileSuccess;
}

bool Shader::CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength)
{
	GLint success = GL_FALSE;
	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	}

	return (success == GL_FALSE); // means that an error has occurred
}

/* static */ string Shader::LoadShader(const std::string& fileName) const
{
	ifstream file(fileName.c_str());
	if (!file.is_open())
	{
		stdlog(Error, LOGPLACE, "Unable to open shader file \"%s\". Check the path.", fileName.c_str());
		return ""; // TODO: Double-check it in the future. It's better to just throw an error I guess.
	}

	string output;
	string line;

	while (file.good())
	{
		getline(file, line);
		output.append(line + "\n");
	}

	ASSERT(file.is_open());
	file.close();

	return output;
}

void Shader::AddUniform(const std::string& uniform)
{
	//stdlog(Info, LOGPLACE, "Adding uniform location \"%s\"", uniform.c_str());
	unsigned int uniformLocation = glGetUniformLocation(program, uniform.c_str());
	ASSERT(uniformLocation != INVALID_VALUE);

	if (uniformLocation == INVALID_VALUE)
	{
		stdlog(Error, LOGPLACE, "Could not find uniform \"%s\"", uniform.c_str());
		return; // TODO: Throw an exception?
	}

	uniforms.insert(std::pair<std::string, unsigned int>(uniform, uniformLocation));
}

void Shader::SetUniformi(const std::string& name, int value)
{
	glUniform1i(uniforms.at(name), value);
}

void Shader::SetUniformf(const std::string& name, float value)
{
	glUniform1f(uniforms.at(name), value);
}

void Shader::SetUniform(const std::string& name, const Math::Vector3D& vector)
{
	glUniform3f(uniforms.at(name), vector.GetX(), vector.GetY(), vector.GetZ());
}

void Shader::SetUniform(const std::string& name, const Math::Matrix4D& matrix)
{
	//stdlog(Warning, LOGPLACE, "This function does not work for the moment");
	glUniformMatrix4fv(uniforms.at(name), 1, GL_FALSE, &(matrix[0][0]));
}

void Shader::UpdateUniforms(const Math::Matrix4D& worldMatrix, const Math::Matrix4D& projectedMatrix, const Material& material)
{
	stdlog(Warning, LOGPLACE, "The function is not implemented");
}

void Shader::AddVertexShaderFromFile(const std::string& fileName)
{
	std::string shaderText = LoadShader(fileName);
	AddProgram(shaderText, GL_VERTEX_SHADER);
}

void Shader::AddFragmentShaderFromFile(const std::string& fileName)
{
	std::string shaderText = LoadShader(fileName);
	AddProgram(shaderText, GL_FRAGMENT_SHADER);
}

//void Shader::AddGeometryShaderFromFile(const std::string& fileName)
//{
//	std::string shaderText = LoadShader(fileName);
//	AddProgram(shaderText, GL_GEOMETRY_SHADER);
//}

void Shader::AddProgram(const std::string& shaderText, GLenum type)
{
	int shader = glCreateShader(type);

	if (shader == 0)
	{
		stdlog(Emergency, LOGPLACE, "Error creating shader type %d", type);
		return;
	}

	const GLchar* p[1];
	p[0] = shaderText.c_str();
	GLint lengths[1];
	lengths[0] = shaderText.length(); // strlen(text.c_str());

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	int infoLogLength;
	if (CheckForErrors(shader, GL_COMPILE_STATUS, false, infoLogLength))
	{
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &errorMessage[0]);
		stdlog(Error, LOGPLACE, "Error linking shader program: \"%s\"", &errorMessage[0]);
		//return;
	}

	glAttachShader(program, shader);
	shaders.push_back(shader);
}