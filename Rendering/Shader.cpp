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
	//stdlog(Debug, LOGPLACE, "Destroying shader instance");
	//for (vector<int>::iterator it = shaders.begin(); it != shaders.end(); ++it)
	//{
	//	glDetachShader(program, *it);
	//	glDeleteShader(*it);
	//}

	//glDeleteProgram(program);
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

string Shader::LoadShader(const std::string& fileName) const
{
	string name = fileName;
	const char *tmp = strrchr(name.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	stdlog(Info, LOGPLACE, "Loading shader from file \"%s\"", name.c_str());

	ifstream file(("..\\Shaders\\" + fileName).c_str());
	if (!file.is_open())
	{
		stdlog(Error, LOGPLACE, "Unable to open shader file \"%s\". Check the path.", name.c_str());
		return ""; // TODO: Double-check it in the future. It's better to just throw an error I guess.
	}

	string output;
	string line;

	while (file.good())
	{
		getline(file, line);
		if (line.find("#include") == std::string::npos)
		{
			output.append(line + "\n");
		}
		else
		{
			// TODO: This will not work with circular includes (i.e. File1 includes File2 and File2 includes File1)
			//stdlog(Debug, LOGPLACE, "#Include directive found in Line = \"%s\"", line.c_str());

			std::vector<std::string> tokens;
			CutToTokens(line, tokens, ' ');
			for (int i = 0; i < tokens.size(); ++i)
			{
				std::cout << i << "):\t" << tokens[i] << std::endl;
			}
			ASSERT(tokens.size() > 1);
			if (tokens.size() <= 1)
			{
				stdlog(Error, LOGPLACE, "Error while reading #include directive in the shader file \"%s\"", name.c_str());
				continue;
			}
			std::string includeFileName = tokens[1];
			//stdlog(Debug, LOGPLACE, "Tokens[1] = \"%s\". IncludeFileName=\"%s\"", tokens[1].c_str(), includeFileName.c_str());
			includeFileName = includeFileName.substr(1, includeFileName.length() - 2);
			//stdlog(Debug, LOGPLACE, "Loading an include shader file \"%s\"", includeFileName.c_str());

			string fragmentToAppend = LoadShader(includeFileName);
			output.append(fragmentToAppend + "\n");
		}
	}

	ASSERT(file.is_open());
	file.close();

	return output;
}

void Shader::AddAllUniforms(const std::string& shaderText)
{
	const std::string UNIFORM_KEY = "uniform";
	std::vector<UniformStruct> structs = FindUniformStructs(shaderText);
	
	unsigned int uniformLocation = shaderText.find(UNIFORM_KEY);
	while(uniformLocation != std::string::npos)
	{
		bool isCommented = false;
		unsigned int lastLineEnd = shaderText.rfind(";", uniformLocation);
		if(lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = shaderText.substr(lastLineEnd,uniformLocation - lastLineEnd);
			isCommented = potentialCommentSection.find("//") != std::string::npos;
		}
		if(!isCommented)
		{
			unsigned int begin = uniformLocation + UNIFORM_KEY.length();
			unsigned int end = shaderText.find(";", begin);
			std::string uniformLine = shaderText.substr(begin + 1, end-begin - 1);
			
			begin = uniformLine.find(" ");
			std::string uniformName = uniformLine.substr(begin + 1);
			std::string uniformType = uniformLine.substr(0, begin);
			
			uniformNames.push_back(uniformName);
			uniformTypes.push_back(uniformType);
			AddUniform(uniformName, uniformType, structs);
		}
		uniformLocation = shaderText.find(UNIFORM_KEY, uniformLocation + UNIFORM_KEY.length());
	}
}

std::vector<UniformStruct> Shader::FindUniformStructs(const std::string& shaderText) const
{
	const std::string STRUCT_KEY = "struct";
	std::vector<UniformStruct> result;
	
	unsigned int structLocation = shaderText.find(STRUCT_KEY);
	while(structLocation != std::string::npos)
	{
		structLocation += STRUCT_KEY.length() + 1; //Ignore the struct keyword and space
		
		unsigned int braceOpening = shaderText.find("{", structLocation);
		unsigned int braceClosing = shaderText.find("}", braceOpening);
		
		UniformStruct newStruct;
		newStruct.name = FindUniformStructName(shaderText.substr(structLocation, braceOpening - structLocation));
		newStruct.memberNames = FindUniformStructComponents(shaderText.substr(braceOpening, braceClosing - braceOpening));
		
		result.push_back(newStruct);
		structLocation = shaderText.find(STRUCT_KEY, structLocation);
	}
	return result;
}

std::string Shader::FindUniformStructName(const std::string& structStartToOpeningBrace) const
{
	std::vector<std::string> tokens;
	Utility::CutToTokens(structStartToOpeningBrace, tokens, ' ');
	return tokens[0];

	//return Util::Split(Util::Split(structStartToOpeningBrace, ' ')[0], '\n')[0];
}

std::vector<TypedData> Shader::FindUniformStructComponents(const std::string& openingBraceToClosingBrace) const
{
	const char charsToIgnore[] = {' ', '\n', '\t', '{'};
	const size_t UNSIGNED_NEG_ONE = (size_t)-1;
	
	std::vector<TypedData> result;
	std::vector<std::string> structLines;
	Utility::CutToTokens(openingBraceToClosingBrace, structLines, ';');
	
	for(unsigned int i = 0; i < structLines.size(); ++i)
	{
		size_t nameBegin = UNSIGNED_NEG_ONE;
		size_t nameEnd = UNSIGNED_NEG_ONE;
		for(unsigned int j = 0; j < structLines[i].length(); ++j)
		{
			bool isIgnoreableCharacter = false;
			for(unsigned int k = 0; k < sizeof(charsToIgnore) / sizeof(char); ++k)
			{
				if(structLines[i][j] == charsToIgnore[k])
				{
					isIgnoreableCharacter = true;
					break;
				}
			}
			if(nameBegin == UNSIGNED_NEG_ONE && isIgnoreableCharacter == false)
			{
				nameBegin = j;
			}
			else if(nameBegin != UNSIGNED_NEG_ONE && isIgnoreableCharacter)
			{
				nameEnd = j;
				break;
			}
		}
		if(nameBegin == UNSIGNED_NEG_ONE || nameEnd == UNSIGNED_NEG_ONE)
			continue;
		
		TypedData newData;
		newData.type = structLines[i].substr(nameBegin, nameEnd - nameBegin);
		newData.name = structLines[i].substr(nameEnd + 1);
		
		result.push_back(newData);
	}
	return result;
}

void Shader::AddUniform(const std::string& uniformName, const std::string& uniformType, const std::vector<UniformStruct>& structs)
{
	bool addThis = true;
	for(unsigned int i = 0; i < structs.size(); ++i)
	{
		if(structs[i].name.compare(uniformType) == 0)
		{
			addThis = false;
			for(unsigned int j = 0; j < structs[i].memberNames.size(); ++j)
			{
				AddUniform(uniformName + "." + structs[i].memberNames[j].name, structs[i].memberNames[j].type, structs);
			}
		}
	}
	if(! addThis)
	{
		return;
	}
	
	unsigned int location = glGetUniformLocation(program, uniformName.c_str());
	ASSERT(location != INVALID_VALUE);
	uniforms.insert(std::pair<std::string, unsigned int>(uniformName, location));
}

void Shader::AddUniform(const std::string& uniform)
{
	//stdlog(Info, LOGPLACE, "Adding uniform location \"%s\"", uniform.c_str());
	unsigned int uniformLocation = glGetUniformLocation(program, uniform.c_str());
	ASSERT(uniformLocation != INVALID_VALUE);

	if (uniformLocation == INVALID_VALUE)
	{
		stdlog(Error, LOGPLACE, "Could not find uniform \"%s\"", uniform.c_str());
		exit(EXIT_FAILURE); // TODO: Throw an exception?
	}

	uniforms.insert(std::pair<std::string, unsigned int>(uniform, uniformLocation));
}

bool Shader::IsUniformPresent(const std::string& uniformName, std::map<std::string, unsigned int>::const_iterator& itr) const
{
	itr = uniforms.find(uniformName);
	if (itr == uniforms.end())
	{
		stdlog(Error, LOGPLACE, "Uniform \"%s\" has not been found", uniformName.c_str());
		return false;
	}
	return true;
}

void Shader::SetUniformi(const std::string& name, int value)
{
	std::map<std::string, unsigned int>::const_iterator itr;
	if (IsUniformPresent(name, itr))
	{
		glUniform1i(itr->second, value);
	}
}

void Shader::SetUniformf(const std::string& name, float value)
{
	std::map<std::string, unsigned int>::const_iterator itr;
	if (IsUniformPresent(name, itr))
	{
		glUniform1f(itr->second, value);
	}
}

void Shader::SetUniform(const std::string& name, const Math::Vector3D& vector)
{
	std::map<std::string, unsigned int>::const_iterator itr;
	if (IsUniformPresent(name, itr))
	{
		glUniform3f(itr->second, vector.GetX(), vector.GetY(), vector.GetZ());
	}
}

void Shader::SetUniform(const std::string& name, const Math::Matrix4D& matrix)
{
	std::map<std::string, unsigned int>::const_iterator itr;
	if (IsUniformPresent(name, itr))
	{
		glUniformMatrix4fv(itr->second, 1, GL_FALSE, &(matrix[0][0]));
	}
}

void Shader::UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer)
{
	stdlog(Warning, LOGPLACE, "The function is not implemented");
}

void Shader::AddVertexShader(const std::string& vertexShaderText)
{
	AddProgram(vertexShaderText, GL_VERTEX_SHADER);
}

void Shader::AddFragmentShader(const std::string& fragmentShaderText)
{
	AddProgram(fragmentShaderText, GL_FRAGMENT_SHADER);
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