#include "StdAfx.h"
#include "Shader.h"

#include "Utility\Utility.h"
#include "Utility\ILogger.h"

#include <fstream>

using namespace Rendering;
using namespace Utility;
using namespace Math;
using namespace std;

/* static */ int ShaderData::supportedOpenGLLevel = 0;
/* static */ std::string ShaderData::glslVersion = "";
/* static */ std::map<std::string, ShaderData*> Shader::shaderResourceMap;

ShaderData::ShaderData(const std::string& fileName) :
	programID(0)
{
	programID = glCreateProgram();

	if (programID == 0)
	{
		LOG(Error, LOGPLACE, "Error while creating shader program");
		// TODO: Throw an exception
		exit(EXIT_FAILURE);
	}

    if(supportedOpenGLLevel == 0)
    {
		int majorVersion;
		int minorVersion;
		
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion); 
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
		
		supportedOpenGLLevel = majorVersion * 100 + minorVersion * 10;
		
		if(supportedOpenGLLevel >= 330)
		{
			std::ostringstream convert;
			convert << supportedOpenGLLevel;
		
			glslVersion = convert.str();
		}
		else if(supportedOpenGLLevel >= 320)
		{
			glslVersion = "150";
		}
		else if(supportedOpenGLLevel >= 310)
		{
			glslVersion = "140";
		}
		else if(supportedOpenGLLevel >= 300)
		{
			glslVersion = "130";
		}
		else if(supportedOpenGLLevel >= 210)
		{
			glslVersion = "120";
		}
		else if(supportedOpenGLLevel >= 200)
		{
			glslVersion = "110";
		}
		else
		{
			LOG(Emergency, LOGPLACE, "OpenGL Version %d.%d does not support shaders.\n", majorVersion, minorVersion);
			exit(EXIT_FAILURE);
		}
	}

	std::string shaderText = LoadShaderData(fileName + ".glsl");

	//std::string vertexShaderText = LoadShaderData(fileName + ".vshader");
	//std::string fragmentShaderText = LoadShaderData(fileName + ".fshader");

	std::string vertexShaderText = "#version " + glslVersion + "\n#define VS_BUILD\n#define GLSL_VERSION " + glslVersion + "\n" + shaderText;
	std::string fragmentShaderText = "#version " + glslVersion + "\n#define FS_BUILD\n#define GLSL_VERSION " + glslVersion + "\n" + shaderText;

	AddVertexShader(vertexShaderText);
	AddFragmentShader(fragmentShaderText);

	std::string attributeKeyword = "attribute"; // TODO: make it const
	AddAllAttributes(vertexShaderText, attributeKeyword);

	if (! Compile())
	{
		LOG(Error, LOGPLACE, "Error while compiling shader program %d", programID);
		exit(EXIT_FAILURE);
	}

	AddShaderUniforms(vertexShaderText);
	AddShaderUniforms(fragmentShaderText);
}

ShaderData::~ShaderData()
{
	LOG(Debug, LOGPLACE, "Destroying shader data for shader program %d", programID);
	for (std::vector<int>::iterator itr = shaders.begin(); itr != shaders.end(); ++itr)
	{
		glDetachShader(programID, *itr);
		glDeleteShader(*itr);
	}
	glDeleteProgram(programID);
}

string ShaderData::LoadShaderData(const std::string& fileName) const
{
	string name = fileName;
	const char *tmp = strrchr(name.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	LOG(Info, LOGPLACE, "Loading shader data from file \"%s\"", name.c_str());

	ifstream file(("..\\Shaders\\" + fileName).c_str());
	if (!file.is_open())
	{
		LOG(Error, LOGPLACE, "Unable to open shader file \"%s\". Check the path.", name.c_str());
		exit(EXIT_FAILURE); // TODO: Double-check it in the future. It's better to just throw an error I guess.
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
			//LOG(Debug, LOGPLACE, "#Include directive found in Line = \"%s\"", line.c_str());

			std::vector<std::string> tokens;
			CutToTokens(line, tokens, ' ');
			//for (int i = 0; i < tokens.size(); ++i)
			//{
			//	std::cout << i << "):\t" << tokens[i] << std::endl;
			//}
			ASSERT(tokens.size() > 1);
			if (tokens.size() <= 1)
			{
				LOG(Error, LOGPLACE, "Error while reading #include directive in the shader file \"%s\"", name.c_str());
				continue;
			}
			std::string includeFileName = tokens[1];
			//LOG(Debug, LOGPLACE, "Tokens[1] = \"%s\". IncludeFileName=\"%s\"", tokens[1].c_str(), includeFileName.c_str());
			includeFileName = includeFileName.substr(1, includeFileName.length() - 2);
			//LOG(Debug, LOGPLACE, "Loading an include shader file \"%s\"", includeFileName.c_str());

			string fragmentToAppend = LoadShaderData(includeFileName);
			output.append(fragmentToAppend + "\n");
		}
	}

	ASSERT(file.is_open());
	file.close();

	return output;
}

bool ShaderData::Compile()
{
	bool compileSuccess = true;

	glLinkProgram(programID);
	int infoLogLength;
	if (CheckForErrors(programID, GL_LINK_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
		LOG(Error, LOGPLACE, "Error linking shader program: \"%s\"", &errorMessage[0]);
	}

	glValidateProgram(programID);
	if (CheckForErrors(programID, GL_VALIDATE_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
		LOG(Error, LOGPLACE, "Error linking shader program: \"%s\"", &errorMessage[0]);
	}

	if (!compileSuccess)
	{
		LOG(Warning, LOGPLACE, "Shader program %d compilation error occurred. Investigate the problem.", programID);
	}
	else
	{
		LOG(Notice, LOGPLACE, "Shader program %d compiled successfully", programID);
	}
	return compileSuccess;
}

bool ShaderData::CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength)
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

void ShaderData::AddVertexShader(const std::string& vertexShaderText)
{
	AddProgram(vertexShaderText, GL_VERTEX_SHADER);
}

void ShaderData::AddFragmentShader(const std::string& fragmentShaderText)
{
	AddProgram(fragmentShaderText, GL_FRAGMENT_SHADER);
}

//void ShaderData::AddGeometryShaderFromFile(const std::string& fileName)
//{
//	std::string shaderText = LoadShader(fileName);
//	AddProgram(shaderText, GL_GEOMETRY_SHADER);
//}

void ShaderData::AddProgram(const std::string& shaderText, GLenum type)
{
	int shader = glCreateShader(type);

	if (shader == 0)
	{
		LOG(Emergency, LOGPLACE, "Error creating shader type %d", type);
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
		LOG(Error, LOGPLACE, "Error linking shader program: \"%s\"", &errorMessage[0]);
		//return;
	}

	glAttachShader(programID, shader);
	shaders.push_back(shader);
}

void ShaderData::AddAllAttributes(const std::string& vertexShaderText, const std::string& attributeKeyword)
{
	int currentAttribLocation = 0;
	size_t attributeLocation = vertexShaderText.find(attributeKeyword);
	while(attributeLocation != std::string::npos)
	{
 		bool isCommented = false;
		size_t lastLineEnd = vertexShaderText.rfind("\n", attributeLocation);
		
		if(lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = vertexShaderText.substr(lastLineEnd,attributeLocation - lastLineEnd);
			
			//Potential false positives are both in comments, and in macros.
			isCommented = potentialCommentSection.find("//") != std::string::npos || potentialCommentSection.find("#") != std::string::npos;
		}
		
		if(!isCommented)
		{
			size_t begin = attributeLocation + attributeKeyword.length();
			size_t end = vertexShaderText.find(";", begin);
			
			std::string attributeLine = vertexShaderText.substr(begin + 1, end-begin - 1);
			
			begin = attributeLine.find(" ");
			std::string attributeName = attributeLine.substr(begin + 1);
				
			glBindAttribLocation(programID, currentAttribLocation, attributeName.c_str());
			currentAttribLocation++;
		}
		attributeLocation = vertexShaderText.find(attributeKeyword, attributeLocation + attributeKeyword.length());
	}
}

void ShaderData::AddShaderUniforms(const std::string& shaderText)
{
	const std::string UNIFORM_KEY = "uniform";
	std::vector<UniformStruct> structs = FindUniformStructs(shaderText);
	int temp = 0;
	for (std::vector<UniformStruct>::const_iterator itr = structs.begin(); itr != structs.end(); ++itr)
	{
		LOG(Delocust, LOGPLACE, "structs[%d].name = \"%s\"", temp, itr->name.c_str());
		int innerTemp = 0;
		for (std::vector<TypedData>::const_iterator innerItr = itr->memberNames.begin(); innerItr != itr->memberNames.end(); ++innerItr)
		{
			LOG(Delocust, LOGPLACE, "\t .memberName[%d].name = \"%s\"\t .memberName[%d].type = \"%s\"", innerTemp, innerItr->name.c_str(), innerTemp, innerItr->type.c_str());
			++innerTemp;
		}
		++temp;
	}
	
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
	for (std::map<std::string, unsigned int>::const_iterator it = uniformMap.begin(); it != uniformMap.end(); ++it)
	{
		LOG(Delocust, LOGPLACE, "Uniform map <\"%s\", %d>", (it->first).c_str(), it->second);
	}
}

void ShaderData::AddUniform(const std::string& uniformName, const std::string& uniformType, const std::vector<UniformStruct>& structs)
{
	bool addThis = true;

	for (unsigned int i = 0; i < structs.size(); ++i)
	{
		if (structs[i].name.compare(uniformType) == 0)
		{
			addThis = false;
			for (unsigned int j = 0; j < structs[i].memberNames.size(); ++j)
			{
				AddUniform(uniformName + "." + structs[i].memberNames[j].name, structs[i].memberNames[j].type, structs);
			}
		}
	}

	if (! addThis)
	{
		//LOG(Info, LOGPLACE, "addThis == false");
		return;
	}

	unsigned int location = glGetUniformLocation(programID, uniformName.c_str());
	ASSERT(location != INVALID_VALUE);
	if (location == INVALID_VALUE)
	{
		LOG(Warning, LOGPLACE, "Invalid value of the location (%d) for the uniform \"%s\"", location, uniformName.c_str());
	}
	LOG(Delocust, LOGPLACE, "Uniform \"%s\" has a location value of %d", uniformName.c_str(), location);
	uniformMap.insert(std::pair<std::string, unsigned int>(uniformName, location));
}

std::vector<UniformStruct> ShaderData::FindUniformStructs(const std::string& shaderText) const
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

std::string ShaderData::FindUniformStructName(const std::string& structStartToOpeningBrace) const
{
	std::vector<std::string> tokens;
	Utility::CutToTokens(structStartToOpeningBrace, tokens, ' ');
	//std::string result;
	//Utility::RightTrim(tokens[0], result);
	//LOG(Delocust, LOGPLACE, "tokens[0] = \"%s\"", tokens[0].c_str());
	return tokens[0];

	//return Util::Split(Util::Split(structStartToOpeningBrace, ' ')[0], '\n')[0];
}

std::vector<TypedData> ShaderData::FindUniformStructComponents(const std::string& openingBraceToClosingBrace) const
{
	const char charsToIgnore[] = {' ', '\n', '\t', '{'};
	const size_t UNSIGNED_NEG_ONE = (size_t)-1;
	
	std::vector<TypedData> result;
	std::vector<std::string> structLines;
	const char delimChars[] = { '\n', ';' };
	Utility::CutToTokens(openingBraceToClosingBrace, structLines, delimChars, 2);
	
	for(unsigned int i = 0; i < structLines.size(); ++i)
	{
		//LOG(Debug, LOGPLACE, "structLines[%d] = \"%s\"", i, structLines[i].c_str());
		if (structLines[i].substr(0, 2) == "//")
		{
			continue;
		}

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

bool ShaderData::IsUniformPresent(const std::string& uniformName, std::map<std::string, unsigned int>::const_iterator& itr) const
{
	itr = uniformMap.find(uniformName);
	if (itr == uniformMap.end())
	{
		LOG(Error, LOGPLACE, "Uniform \"%s\" has not been found", uniformName.c_str());
		exit(EXIT_FAILURE);
		return false;
	}
	return true;
}

/* ==================== Shader class begin ==================== */
Shader::Shader(const std::string& fileName) :
	shaderData(NULL),
	fileName(fileName)
{
	std::map<std::string, ShaderData*>::const_iterator itr = shaderResourceMap.find(fileName);
	if (itr == shaderResourceMap.end())
	{
		shaderData = new ShaderData(fileName);
		shaderResourceMap.insert(std::pair<std::string, ShaderData*>(fileName, shaderData));
	}
	else
	{
		LOG(Info, LOGPLACE, "Shader data \"%s\" already present in the resource manager", fileName.c_str());
		shaderData = itr->second;
		shaderData->AddReference();
	}
}


Shader::~Shader(void)
{
	ASSERT(shaderData != NULL);
	if (shaderData == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Shader data is already NULL");
		return;
	}
	
	shaderData->RemoveReference();
	if (! shaderData->IsReferenced())
	{
		if (fileName.length() > 0)
		{
			shaderResourceMap.erase(fileName);
		}
		delete shaderData;
		shaderData = NULL;
	}
}

void Shader::Bind() const
{
	ASSERT(shaderData != NULL);
	if (shaderData == NULL)
	{
		LOG(Utility::Critical, LOGPLACE, "Cannot bind the shader. Shader data is NULL");
		exit(EXIT_FAILURE);
	}
	glUseProgram(shaderData->GetProgram());
}

void Shader::UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer) const
{
	ASSERT(renderer != NULL);
	if (renderer == NULL)
	{
		LOG(Critical, LOGPLACE, "Cannot update uniforms. Rendering engine is NULL.");
		exit(EXIT_FAILURE);
	}
	ASSERT(shaderData != NULL);
	if (shaderData == NULL)
	{
		LOG(Critical, LOGPLACE, "Cannot update uniforms. Shader data is NULL.");
		exit(EXIT_FAILURE);
	}
	ASSERT(shaderData->GetUniformNames().size() == shaderData->GetUniformTypes().size());
	if (shaderData->GetUniformNames().size() != shaderData->GetUniformTypes().size())
	{
		LOG(Error, LOGPLACE, "Shader data is incorrect. There are %d uniform names and %d uniform types",
			shaderData->GetUniformNames().size(), shaderData->GetUniformTypes().size());
	}

	Matrix4D worldMatrix = transform.GetTransformation();
	Matrix4D projectedMatrix = renderer->GetCurrentCamera().GetViewProjection() * worldMatrix;

	for (unsigned int i = 0; i < shaderData->GetUniformNames().size(); ++i)
	{
		std::string uniformName = shaderData->GetUniformNames()[i];
		std::string uniformType = shaderData->GetUniformTypes()[i];

		const std::string uniformSubstr = uniformName.substr(0, 2);

		if (uniformSubstr == "R_")
		{
			std::string unprefixedName = uniformName.substr(2, uniformName.length());
			if (unprefixedName == "lightMatrix")
			{
				SetUniformMatrix(uniformName, renderer->GetLightMatrix() * worldMatrix);
			}
			else if (uniformType == "sampler2D")
			{
				unsigned int samplerSlot = renderer->GetSamplerSlot(unprefixedName);
				Texture* texture = renderer->GetTexture(unprefixedName);
				ASSERT(texture != NULL);
				if (texture == NULL)
				{
					LOG(Utility::Critical, LOGPLACE, "Updating uniforms operation failed. Rendering engine texture \"%s\" is NULL", unprefixedName.c_str());
					exit(EXIT_FAILURE);
				}
				texture->Bind(samplerSlot);
				SetUniformi(uniformName, samplerSlot);
			}
			else if (uniformType == "vec3")
			{
				SetUniformVector3D(uniformName, renderer->GetVec3D(unprefixedName));
			}
			else if(uniformType == "float")
			{
				SetUniformf(uniformName, renderer->GetReal(unprefixedName));
			}
			else if(uniformType == "DirectionalLight")
			{
				DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(renderer->GetCurrentLight());
				if (directionalLight == NULL)
				{
					LOG(Error, LOGPLACE, "Cannot update directional light uniform. Directional light instance is NULL.");
					continue;
				}
				SetUniformDirectionalLight(uniformName, *directionalLight);
			}
			else if(uniformType == "PointLight")
			{
				PointLight* pointLight = dynamic_cast<PointLight*>(renderer->GetCurrentLight());
				if (pointLight == NULL)
				{
					LOG(Error, LOGPLACE, "Cannot update point light uniform. Point light instance is NULL.");
					continue;
				}
				SetUniformPointLight(uniformName, *pointLight);
			}
			else if(uniformType == "SpotLight")
			{
				SpotLight* spotLight = dynamic_cast<SpotLight*>(renderer->GetCurrentLight());
				if (spotLight == NULL)
				{
					LOG(Error, LOGPLACE, "Cannot update spot light uniform. Spot light instance is NULL.");
					continue;
				}
				SetUniformSpotLight(uniformName, *spotLight);
			}
			else
			{
				renderer->UpdateUniformStruct(transform, material, *this, uniformName, uniformType);
			}
		}
		else if (uniformType == "sampler2D")
		{
			unsigned int samplerSlot = renderer->GetSamplerSlot(uniformName);
			Texture* texture = material.GetTexture(uniformName);
			ASSERT(texture != NULL);
			if (texture == NULL)
			{
				LOG(Utility::Critical, LOGPLACE, "Updating uniforms operation failed. Material texture \"%s\" is NULL", uniformName.c_str());
				exit(EXIT_FAILURE);
			}
			texture->Bind(samplerSlot);
			SetUniformi(uniformName, samplerSlot);
		}
		else if (uniformSubstr == "T_") // tranform uniform
		{
			if (uniformName == "T_MVP")
			{
				SetUniformMatrix(uniformName, projectedMatrix);
			}
			else if (uniformName == "T_model")
			{
				SetUniformMatrix(uniformName, worldMatrix);
			}
			else
			{
				//throw "Invalid Transform Uniform: " + uniformName;
				LOG(Error, LOGPLACE, "Invalid transform uniform \"%s\"", uniformName.c_str());
			}
		}
		else if (uniformSubstr == "C_")
		{
			if (uniformName == "C_eyePos")
			{
				SetUniformVector3D(uniformName, renderer->GetCurrentCamera().GetTransform().GetTransformedPos());
			}
			else
			{
				//throw "Invalid Transform Uniform: " + uniformName;
				LOG(Error, LOGPLACE, "Invalid camera uniform \"%s\"", uniformName.c_str());
			}
		}
		else
		{
			if (uniformType == "vec3")
			{
				SetUniformVector3D(uniformName, material.GetVec3D(uniformName));
			}
			else if (uniformType == "float")
			{
				SetUniformf(uniformName, material.GetReal(uniformName));
			}
			else
			{
				//throw "Invalid Transform Uniform: " + uniformName;
				LOG(Error, LOGPLACE, "The uniform \"%s\" of type \"%s\" is not supported by the Material class", uniformName.c_str(), uniformType.c_str());
			}
		}
	}
}

//void Shader::AddUniform(const std::string& uniform)
//{
//	//LOG(Info, LOGPLACE, "Adding uniform location \"%s\"", uniform.c_str());
//	unsigned int uniformLocation = glGetUniformLocation(program, uniform.c_str());
//	ASSERT(uniformLocation != INVALID_VALUE);
//
//	if (uniformLocation == INVALID_VALUE)
//	{
//		LOG(Error, LOGPLACE, "Could not find uniform \"%s\"", uniform.c_str());
//		exit(EXIT_FAILURE); // TODO: Throw an exception?
//	}
//
//	uniforms.insert(std::pair<std::string, unsigned int>(uniform, uniformLocation));
//}

void Shader::SetUniformi(const std::string& name, int value) const
{
	std::map<std::string, unsigned int>::const_iterator itr;
	if (shaderData->IsUniformPresent(name, itr))
	{
		glUniform1i(itr->second, value);
	}
}

void Shader::SetUniformf(const std::string& name, Math::Real value) const
{
	//for (std::map<std::string, unsigned int>::const_iterator it = shaderData->GetUniformMap().begin(); it != shaderData->GetUniformMap().end(); ++it)
	//{
	//	LOG(Debug, LOGPLACE, "Uniform map <\"%s\", %d>", (it->first).c_str(), it->second);
	//}
	std::map<std::string, unsigned int>::const_iterator itr;
	if (shaderData->IsUniformPresent(name, itr))
	{
		glUniform1f(itr->second, value);
	}
}

void Shader::SetUniformVector3D(const std::string& name, const Math::Vector3D& vector) const
{
	std::map<std::string, unsigned int>::const_iterator itr;
	if (shaderData->IsUniformPresent(name, itr))
	{
		glUniform3f(itr->second, vector.GetX(), vector.GetY(), vector.GetZ());
	}
}

void Shader::SetUniformMatrix(const std::string& name, const Math::Matrix4D& matrix) const
{
	std::map<std::string, unsigned int>::const_iterator itr;
	if (shaderData->IsUniformPresent(name, itr))
	{
		glUniformMatrix4fv(itr->second, 1, GL_FALSE, &(matrix[0][0]));
	}
}

void Shader::SetUniformDirectionalLight(const std::string& uniformName, const DirectionalLight& directionalLight) const
{
	SetUniformVector3D(uniformName + ".direction", directionalLight.GetTransform().GetTransformedRot().GetForward());
	SetUniformVector3D(uniformName + ".base.color", directionalLight.GetColor());
	SetUniformf(uniformName + ".base.intensity", directionalLight.GetIntensity());
}

void Shader::SetUniformPointLight(const std::string& uniformName, const PointLight& pointLight) const
{
	SetUniformVector3D(uniformName + ".base.color", pointLight.GetColor());
	SetUniformf(uniformName + ".base.intensity", pointLight.GetIntensity());
	SetUniformf(uniformName + ".attenuation.constant", pointLight.GetAttenuation().GetConstant());
	SetUniformf(uniformName + ".attenuation.linear", pointLight.GetAttenuation().GetLinear());
	SetUniformf(uniformName + ".attenuation.exponent", pointLight.GetAttenuation().GetExponent());
	SetUniformVector3D(uniformName + ".position", pointLight.GetTransform().GetTransformedPos());
	SetUniformf(uniformName + ".range", pointLight.GetRange());
}

void Shader::SetUniformSpotLight(const std::string& uniformName, const SpotLight& spotLight) const
{
	SetUniformVector3D(uniformName + ".pointLight.base.color", spotLight.GetColor());
	SetUniformf(uniformName + ".pointLight.base.intensity", spotLight.GetIntensity());
	SetUniformf(uniformName + ".pointLight.attenuation.constant", spotLight.GetAttenuation().GetConstant());
	SetUniformf(uniformName + ".pointLight.attenuation.linear", spotLight.GetAttenuation().GetLinear());
	SetUniformf(uniformName + ".pointLight.attenuation.exponent", spotLight.GetAttenuation().GetExponent());
	SetUniformVector3D(uniformName + ".pointLight.position", spotLight.GetTransform().GetTransformedPos());
	SetUniformf(uniformName + ".pointLight.range", spotLight.GetRange());
	SetUniformVector3D(uniformName + ".direction", spotLight.GetTransform().GetTransformedRot().GetForward());
	SetUniformf(uniformName + ".cutoff", spotLight.GetCutoff());
}