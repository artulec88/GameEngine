#include "StdAfx.h"
#include "Shader.h"

#include "CoreEngine.h"
#include "Utility\Utility.h"
#include "Utility\ILogger.h"
#include "Utility\StringUtility.h"

#include <fstream>

using namespace Rendering;
using namespace Utility;
using namespace Math;

Uniforms::UniformType Uniforms::ConvertStringToUniformType(const std::string& uniformTypeStr)
{
	if (uniformTypeStr == "float") { return Uniforms::REAL; }
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
		EMERGENCY_LOG("Uniform type \"%s\" not supported by the rendering engine.", uniformTypeStr.c_str());
		exit(EXIT_FAILURE);
	}
}

std::string Uniforms::ConvertUniformTypeToString(UniformType uniformType)
{
	switch (uniformType)
	{
	case Rendering::Uniforms::VEC_2D:
		return "vec2";
	case Rendering::Uniforms::VEC_3D:
		return "vec3";
	case Rendering::Uniforms::VEC_4D:
		return "vec4";
	case Rendering::Uniforms::MATRIX_4x4:
		return "mat4";
	case Rendering::Uniforms::REAL:
		return "float";
	case Rendering::Uniforms::SAMPLER_2D:
		return "sampler2D";
	case Rendering::Uniforms::SAMPLER_CUBE:
		return "samplerCube";
	case Rendering::Uniforms::BASE_LIGHT:
		return "BaseLight";
	case Rendering::Uniforms::DIRECTIONAL_LIGHT:
		return "DirectionalLight";
	case Rendering::Uniforms::POINT_LIGHT:
		return "PointLight";
	case Rendering::Uniforms::SPOT_LIGHT:
		return "SpotLight";
	case Rendering::Uniforms::ATTENUATION:
		return "Attenuation";
	default:
		EMERGENCY_LOG("Uniform type %d not supported by the rendering engine.", uniformType);
		exit(EXIT_FAILURE);
	}
}

/* static */ const std::string ShaderData::UNIFORM_KEYWORD = "uniform";
/* static */ const std::string ShaderData::ATTRIBUTE_KEYWORD = "attribute";
/* static */ const std::string ShaderData::SINGLE_LINE_COMMENT = "//";
/* static */ const std::string ShaderData::MULTI_LINE_COMMENT_BEGIN = "/*";
/* static */ const std::string ShaderData::MULTI_LINE_COMMENT_END = "*/";

/* static */ std::map<std::string, ShaderData*> Shader::shaderResourceMap;

ShaderData::ShaderData(const std::string& fileName) :
	m_programID(glCreateProgram())
{
	if (m_programID == 0)
	{
		CRITICAL_LOG("Error while creating shader program");
		// TODO: Throw an exception
		exit(EXIT_FAILURE);
	}

	std::string shaderText = LoadShaderData(fileName + ".glsl");
	bool geometryShaderPresent = (shaderText.find("defined(GS_BUILD)") != std::string::npos); // geometry shader found

	/**
	 * TODO: Vertex shader text should only contain the shader file content in the #if defined(VS_BUILD) block.
	 * Analogically, the fragment shader text should only contain the content in the #if defined(FS_BUILD) block.
	 * The same should apply to the geometry shader text.
	 */
	std::string vertexShaderText = "#version " + glslVersion + "\n#define VS_BUILD\n#define GLSL_VERSION " + glslVersion + "\n" + shaderText;
	std::string geometryShaderText = "#version " + glslVersion + "\n#define GS_BUILD\n#define GLSL_VERSION " + glslVersion + "\n" + shaderText;
	std::string fragmentShaderText = "#version " + glslVersion + "\n#define FS_BUILD\n#define GLSL_VERSION " + glslVersion + "\n" + shaderText;

	AddVertexShader(vertexShaderText);
	if (geometryShaderPresent)
	{
		DEBUG_LOG("Geometry shader found in file \"%s\"", fileName.c_str());
		AddGeometryShader(geometryShaderText);
	}
	AddFragmentShader(fragmentShaderText);

	AddAllAttributes(vertexShaderText);

	if (! Compile())
	{
		CRITICAL_LOG("Error while compiling shader program %d", m_programID);
		exit(EXIT_FAILURE);
	}
	AddShaderUniforms(shaderText);
	//ERROR_LOG("Vertex shader text = %s", vertexShaderText.c_str());
	//AddShaderUniforms(vertexShaderText);
	//if (geometryShaderPresent)
	//{
	//	ERROR_LOG("Geometry shader text = %s", geometryShaderText.c_str());
	//	AddShaderUniforms(geometryShaderText);
	//}
	//ERROR_LOG("Fragment shader text = %s", fragmentShaderText.c_str());
	//AddShaderUniforms(fragmentShaderText);
}

ShaderData::~ShaderData()
{
	DEBUG_LOG("Destroying shader data for shader program %d", m_programID);
	for (std::vector<GLuint>::iterator shaderItr = m_shaders.begin(); shaderItr != m_shaders.end(); ++shaderItr)
	{
		glDetachShader(m_programID, *shaderItr);
		glDeleteShader(*shaderItr);
	}
	glDeleteProgram(m_programID);
}

std::string ShaderData::LoadShaderData(const std::string& fileName) const
{
	std::string name = fileName;
	const char *tmp = strrchr(name.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	DEBUG_LOG("Loading shader data from file \"%s\"", name.c_str());

	std::ifstream file((CoreEngine::GetCoreEngine()->GetShadersDirectory() + fileName).c_str());
	if (!file.is_open())
	{
		ERROR_LOG("Unable to open shader file \"%s\". Check the path.", name.c_str());
		exit(EXIT_FAILURE); // TODO: Double-check it in the future. It's better to just throw an error I guess.
	}

	std::string output;
	std::string line;

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
			//DEBUG_LOG("#Include directive found in Line = \"%s\"", line.c_str());

			std::vector<std::string> tokens;
			CutToTokens(line, tokens, ' ');
			//for (int i = 0; i < tokens.size(); ++i)
			//{
			//	std::cout << i << "):\t" << tokens[i] << std::endl;
			//}
			CHECK_CONDITION_EXIT_ALWAYS(tokens.size() > 1, Utility::Error, "Error while reading #include directive in the shader file \"%s\"", name.c_str());
			std::string includeFileName = tokens[1];
			//DEBUG_LOG("Tokens[1] = \"%s\". IncludeFileName=\"%s\"", tokens[1].c_str(), includeFileName.c_str());
			includeFileName = includeFileName.substr(1, includeFileName.length() - 2);
			//DEBUG_LOG("Loading an include shader file \"%s\"", includeFileName.c_str());

			std::string fragmentToAppend = LoadShaderData(includeFileName);
			output.append(fragmentToAppend + "\n");
		}
	}
	file.close();
	
	DEBUG_LOG("Shader \"%s\" text loaded", fileName.c_str());
	/* ==================== Removing comments from the shader code begin ==================== */
	size_t commentBegin = output.find(MULTI_LINE_COMMENT_BEGIN);
	while (commentBegin != std::string::npos)
	{
		size_t commentEnd = output.find(MULTI_LINE_COMMENT_END, commentBegin);
		output.erase(commentBegin, commentEnd - commentBegin + MULTI_LINE_COMMENT_END.length());
		commentBegin = output.find(MULTI_LINE_COMMENT_BEGIN);
	}
	commentBegin = output.find(SINGLE_LINE_COMMENT);
	while (commentBegin != std::string::npos)
	{
		size_t lineEnd = output.find("\n", commentBegin);
		output.erase(commentBegin, lineEnd - commentBegin);
		commentBegin = output.find(SINGLE_LINE_COMMENT);
	}
	/* ==================== Removing comments from the shader code end ==================== */

	return output;
}

bool ShaderData::Compile()
{
	bool compileSuccess = true;

	glLinkProgram(m_programID);
	int infoLogLength;
	if (CheckForErrors(m_programID, GL_LINK_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &errorMessage[0]);
		ERROR_LOG("Error linking shader program %d:\n%s\r", m_programID, &errorMessage[0]);
	}

	glValidateProgram(m_programID);
	if (CheckForErrors(m_programID, GL_VALIDATE_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &errorMessage[0]);
		ERROR_LOG("Error validating shader program %d:\n%s\r", m_programID, &errorMessage[0]);
	}

	if (!compileSuccess)
	{
		ERROR_LOG("Shader program %d compilation error occurred. Investigate the problem.", m_programID);
	}
	else
	{
		DEBUG_LOG("Shader program %d compiled successfully", m_programID);
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

void ShaderData::AddGeometryShader(const std::string& geometryShaderText)
{
	AddProgram(geometryShaderText, GL_GEOMETRY_SHADER);
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
	GLuint shader = glCreateShader(type);

	if (shader == 0)
	{
		EMERGENCY_LOG("Error creating shader type %d", type);
		return;
	}

	const GLchar* p[1];
	p[0] = shaderText.c_str();
	GLint lengths[1] = { static_cast<GLint>(shaderText.length()) }; // TODO: warning C4838: conversion from 'unsigned int' to 'GLint' requires a narrowing conversion.
	//lengths[0] = shaderText.length(); // strlen(text.c_str());

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	int infoLogLength;
	if (CheckForErrors(shader, GL_COMPILE_STATUS, false, infoLogLength))
	{
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &errorMessage[0]);
		ERROR_LOG("Error linking shader program: \"%s\"", &errorMessage[0]);
		//return;
	}

	glAttachShader(m_programID, shader);
	m_shaders.push_back(shader);
}

void ShaderData::AddAllAttributes(const std::string& vertexShaderText)
{
	int currentAttribLocation = 0;
	size_t attributeLocation = vertexShaderText.find(ATTRIBUTE_KEYWORD);
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
			size_t begin = attributeLocation + ATTRIBUTE_KEYWORD.length();
			size_t end = vertexShaderText.find(";", begin);
			
			std::string attributeLine = vertexShaderText.substr(begin + 1, end-begin - 1);
			
			begin = attributeLine.find(" ");
			std::string attributeName = attributeLine.substr(begin + 1);
				
			glBindAttribLocation(m_programID, currentAttribLocation, attributeName.c_str());
			currentAttribLocation++;
		}
		attributeLocation = vertexShaderText.find(ATTRIBUTE_KEYWORD, attributeLocation + ATTRIBUTE_KEYWORD.length());
	}
}

void ShaderData::AddShaderUniforms(const std::string& shaderText)
{
	std::vector<UniformStruct> structs = FindUniformStructs(shaderText);
#ifdef DEBUG_LOGGING_ENABLED
	for (std::vector<UniformStruct>::const_iterator itr = structs.begin(); itr != structs.end(); ++itr)
	{
		DEBUG_LOG("struct.name = \"%s\"", itr->name.c_str());
#ifdef DELOCUST_LOGGING_ENABLED
		for (std::vector<TypedData>::const_iterator innerItr = itr->memberNames.begin(); innerItr != itr->memberNames.end(); ++innerItr)
		{
			DELOCUST_LOG("\t .memberName.name = \"%s\"\t .memberName.uniformType = %d", innerItr->name.c_str(), innerItr->uniformType);
		}
#endif
	}
#endif
	
	size_t uniformLocation = shaderText.find(UNIFORM_KEYWORD);
	while(uniformLocation != std::string::npos)
	{
		size_t begin = uniformLocation + UNIFORM_KEYWORD.length();
		size_t end = shaderText.find(";", begin);
		std::string uniformLine = shaderText.substr(begin + 1, end - begin - 1);

		begin = uniformLine.find(" ");

		const std::string uniformName = uniformLine.substr(begin + 1);
		const Uniforms::UniformType uniformType = Uniforms::ConvertStringToUniformType(uniformLine.substr(0, begin));
		m_uniformNames.push_back(uniformName);
		m_uniformTypes.push_back(uniformType);
		AddUniform(uniformName, uniformType, structs);
		uniformLocation = shaderText.find(UNIFORM_KEYWORD, uniformLocation + UNIFORM_KEYWORD.length());
	}
#ifdef DEBUG_LOGGING_ENABLED
	for (std::map<std::string, GLint>::const_iterator it = m_uniformMap.begin(); it != m_uniformMap.end(); ++it)
	{
		DEBUG_LOG("Uniform map <\"%s\", %d>", (it->first).c_str(), it->second);
	}
#endif
}

void ShaderData::AddUniform(const std::string& uniformName, Uniforms::UniformType uniformType, const std::vector<UniformStruct>& structs)
{
	DEBUG_LOG("Adding uniform \"%s\" of type %d", uniformName.c_str(), uniformType);

	const std::string uniformTypeStr = Uniforms::ConvertUniformTypeToString(uniformType);
	for (std::vector<UniformStruct>::const_iterator structItr = structs.begin(); structItr != structs.end(); ++structItr)
	{
		if (structItr->name == uniformTypeStr)
		{
			for (std::vector<TypedData>::const_iterator structMemberNameItr = structItr->memberNames.begin(); structMemberNameItr != structItr->memberNames.end(); ++structMemberNameItr)
			{
				AddUniform(uniformName + "." + structMemberNameItr->name, structMemberNameItr->uniformType, structs);
			}
			return;
		}
	}

	GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
	CHECK_CONDITION_EXIT(location != INVALID_VALUE, Emergency, "Invalid value of the location (%d) for the uniform \"%s\"", location, uniformName.c_str());
	DELOCUST_LOG("Uniform \"%s\" has a location value of %d", uniformName.c_str(), location);
	m_uniformMap.insert(std::pair<std::string, GLint>(uniformName, location));
}

std::vector<UniformStruct> ShaderData::FindUniformStructs(const std::string& shaderText) const
{
	const std::string STRUCT_KEY = "struct";
	std::vector<UniformStruct> result;
	
	size_t structLocation = shaderText.find(STRUCT_KEY);
	DELOCUST_LOG("structLocation = %d; std::string::npos = %d", structLocation, std::string::npos);
	while(structLocation != std::string::npos)
	{
		structLocation += STRUCT_KEY.length() + 1; //Ignore the struct keyword and space
		
		size_t braceOpening = shaderText.find("{", structLocation);
		size_t braceClosing = shaderText.find("}", braceOpening);
		
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
	std::string result;
	Utility::StringUtility::RightTrim(tokens[0]);
	//DELOCUST_LOG("tokens[0] = \"%s\"", tokens[0].c_str());
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
		//DEBUG_LOG("structLines[%d] = \"%s\"", i, structLines[i].c_str());
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

		result.push_back(TypedData(structLines[i].substr(nameEnd + 1), Uniforms::ConvertStringToUniformType(structLines[i].substr(nameBegin, nameEnd - nameBegin))));
	}
	return result;
}

bool ShaderData::IsUniformPresent(const std::string& uniformName, std::map<std::string, GLint>::const_iterator& itr) const
{
	itr = m_uniformMap.find(uniformName);
	CHECK_CONDITION(itr != uniformMap.end(), Error, "Uniform \"%s\" has not been found.", uniformName.c_str());
	return (itr != m_uniformMap.end());
}

/* ==================== Shader class begin ==================== */
Shader::Shader(const std::string& fileName) :
	m_shaderData(NULL),
	m_fileName(fileName)
{
	DEBUG_LOG("Shader constructed based on filename \"%s\"", fileName.c_str());
	std::map<std::string, ShaderData*>::const_iterator itr = shaderResourceMap.find(fileName);
	if (itr == shaderResourceMap.end())
	{
		m_shaderData = new ShaderData(fileName);
		shaderResourceMap.insert(std::pair<std::string, ShaderData*>(fileName, m_shaderData));
	}
	else
	{
		INFO_LOG("Shader data \"%s\" already present in the resource manager", fileName.c_str());
		m_shaderData = itr->second;
		m_shaderData->AddReference();
	}
}


Shader::~Shader(void)
{
	ASSERT(m_shaderData != NULL);
	if (m_shaderData == NULL)
	{
		WARNING_LOG("Shader data is already NULL.");
	}
	
	m_shaderData->RemoveReference();
	if (! m_shaderData->IsReferenced())
	{
		if (m_fileName.length() > 0)
		{
			shaderResourceMap.erase(m_fileName);
		}
		SAFE_DELETE(m_shaderData);
	}
}

void Shader::Bind() const
{
	CHECK_CONDITION_EXIT(m_shaderData != NULL, Utility::Critical, "Cannot bind the shader. Shader data is NULL.");
	glUseProgram(m_shaderData->GetProgram());
}

bool Shader::IsBound() const
{
	GLint currentProgramID;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgramID);
	return currentProgramID == GetProgramID();
}

void Shader::Unbind() const
{
	DELOCUST_LOG("The shader is being unbound");
	glUseProgram(0);
}

void Shader::UpdateUniforms(const Transform& transform, const Material* material, Renderer* renderer) const
{
	CHECK_CONDITION_EXIT(renderer != NULL, Critical, "Cannot update uniforms. Rendering engine is NULL.");
	CHECK_CONDITION_EXIT(m_shaderData != NULL, Critical, "Cannot update uniforms. Shader data is NULL.");
	CHECK_CONDITION_EXIT(m_shaderData->GetUniformNames().size() == m_shaderData->GetUniformTypes().size(), Error, "Shader data is incorrect. There are %d uniform names and %d uniform types", m_shaderData->GetUniformNames().size(), m_shaderData->GetUniformTypes().size());

	Matrix4D worldMatrix = transform.GetTransformation();
	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc.
	/* ==================== SOLUTION #1 begin ==================== */
	//Matrix4D projectedMatrix(renderer->GetCurrentCamera().GetViewProjection()); // TODO: Pass camera object as parameter instead of using GetCurrentCamera() function.
	//projectedMatrix *= worldMatrix;
	/* ==================== SOLUTION #1 end ==================== */
	/* ==================== SOLUTION #2 begin ==================== */
	Matrix4D projectedMatrix(renderer->GetCurrentCamera().GetViewProjection() * worldMatrix); // TODO: Pass camera object as parameter instead of using GetCurrentCamera() function.
																							  // FIXME: Check matrix multiplication
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	//Matrix4D projectedMatrix = renderer->GetCurrentCamera().GetViewProjection() * worldMatrix; // TODO: Pass camera object as parameter instead of using GetCurrentCamera() function.
	/* ==================== SOLUTION #3 end ==================== */
	/* ==================== SOLUTION #4 begin ==================== */
	//Matrix4D projectedMatrix;
	//renderer->GetCurrentCamera().GetViewProjection(projectedMatrix); // TODO: Pass camera object as parameter instead of using GetCurrentCamera() function.
	//projectedMatrix *= worldMatrix;
	/* ==================== SOLUTION #4 end ==================== */
	std::vector<Uniforms::UniformType>::const_iterator uniformTypeItr = m_shaderData->GetUniformTypes().begin();
	for (std::vector<std::string>::const_iterator uniformItr = m_shaderData->GetUniformNames().begin(); uniformItr != m_shaderData->GetUniformNames().end(); ++uniformItr, ++uniformTypeItr)
	{
		const std::string uniformName = *uniformItr;
		const Uniforms::UniformType uniformType = *uniformTypeItr;
		DEBUG_LOG("Updating uniform with name = \"%s\" and type = %d.", uniformName.c_str(), uniformType);

		const std::string uniformNamePrefix = uniformName.substr(0, 2);

		if (uniformNamePrefix == "R_")
		{
			std::string unprefixedName = uniformName.substr(2, uniformName.length());
			if (unprefixedName == "lightMatrix")
			{
				//CRITICAL_LOG("Renderer->GetLightMatrix() = \"%s\"", renderer->GetLightMatrix().ToString().c_str());
				//CRITICAL_LOG("WorldMatrix = \"%s\"", worldMatrix.ToString().c_str());
				SetUniformMatrix(uniformName, renderer->GetLightMatrix() * worldMatrix); // FIXME: Check matrix multiplication
			}
			else if ((uniformType == Uniforms::SAMPLER_2D) || (uniformType == Uniforms::SAMPLER_CUBE))
			{
				unsigned int samplerSlot = renderer->GetSamplerSlot(unprefixedName);
				//CRITICAL_LOG("Binding texture \"%s\" in sampler slot %d", unprefixedName.c_str(), samplerSlot);
				if (unprefixedName == "cubeShadowMap")
				{
					renderer->BindCubeShadowMap(samplerSlot);
				}
				else
				{
					unsigned int multitextureIndex = 0; // used only by the multitextures
					Texture* texture = renderer->GetTexture(unprefixedName, &multitextureIndex);
					CHECK_CONDITION_EXIT(texture != NULL, Critical, "Updating uniforms operation failed. Rendering engine texture \"%s\" is NULL.", unprefixedName.c_str());
					texture->Bind(samplerSlot, multitextureIndex);
				}
				SetUniformi(uniformName, samplerSlot);
			}
			//else if (uniformType == "samplerCubeShadow")
			//{
			//	unsigned int samplerSlot = renderer->GetSamplerSlot(unprefixedName);
			//	renderer->BindCubeShadowMap(samplerSlot);
			//	SetUniformi(uniformName, samplerSlot);
			//}
			else if (uniformType == Uniforms::VEC_3D)
			{
				SetUniformVector3D(uniformName, renderer->GetVec3D(unprefixedName));
			}
			else if (uniformType == Uniforms::VEC_4D)
			{
				SetUniformVector4D(uniformName, renderer->GetVec4D(unprefixedName));
			}
			else if(uniformType == Uniforms::REAL)
			{
				SetUniformf(uniformName, renderer->GetReal(unprefixedName));
			}
			else if(uniformType == Uniforms::DIRECTIONAL_LIGHT)
			{
				// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
				//Lighting::DirectionalLight* directionalLight = dynamic_cast<Lighting::DirectionalLight*>(renderer->GetCurrentLight());
				const Lighting::BaseLight* directionalLight = renderer->GetCurrentLight();
				CHECK_CONDITION_EXIT(directionalLight != NULL, Error, "Cannot update directional light uniform. Directional light instance is NULL.");
				SetUniformDirectionalLight(uniformName, *directionalLight);
			}
			else if(uniformType == Uniforms::POINT_LIGHT)
			{
				// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
				//Lighting::PointLight* pointLight = dynamic_cast<Lighting::PointLight*>(renderer->GetCurrentLight());
				const Lighting::PointLight* pointLight = renderer->GetPointLight();
				CHECK_CONDITION_EXIT(pointLight != NULL, Error, "Cannot update point light uniform. Point light instance is NULL.");
				SetUniformPointLight(uniformName, *pointLight);
			}
			else if(uniformType == Uniforms::SPOT_LIGHT)
			{
				// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
				const Lighting::SpotLight* spotLight = dynamic_cast<const Lighting::SpotLight*>(renderer->GetCurrentLight());
				//const Lighting::SpotLight* spotLight = renderer->GetSpotLight();
				CHECK_CONDITION_EXIT(spotLight != NULL, Error, "Cannot update spot light uniform. Spot light instance is NULL.");
				SetUniformSpotLight(uniformName, *spotLight);
			}
			else
			{
				//renderer->UpdateUniformStruct(transform, *material, *this, uniformName, uniformType);
				ERROR_LOG("Uniform name \"%s\" of type %d is not supported by the rendering engine", uniformName.c_str(), uniformType);
			}
		}
		else if ((uniformType == Uniforms::SAMPLER_2D) || (uniformType == Uniforms::SAMPLER_CUBE))
		{
			unsigned int samplerSlot = renderer->GetSamplerSlot(uniformName);
			Texture* texture = material->GetTexture(uniformName);
			CHECK_CONDITION_EXIT(texture != NULL, Critical, "Updating uniforms operation failed. Material texture \"%s\" is NULL.", uniformName.c_str());
			texture->Bind(samplerSlot);
			SetUniformi(uniformName, samplerSlot);
		}
		else if (uniformNamePrefix == "T_") // tranform uniform
		{
			if (uniformName == "T_MVP")
			{
				SetUniformMatrix(uniformName, projectedMatrix);
			}
			else if (uniformName == "T_VP")
			{
				SetUniformMatrix(uniformName, renderer->GetCurrentCamera().GetViewProjection());
			}
			else if (uniformName == "T_model")
			{
				SetUniformMatrix(uniformName, worldMatrix);
			}
			else
			{
				//throw "Invalid Transform Uniform: " + uniformName;
				ERROR_LOG("Invalid transform uniform \"%s\"", uniformName.c_str());
			}
		}
		else if (uniformNamePrefix == "C_")
		{
			if (uniformName == "C_eyePos")
			{
				SetUniformVector3D(uniformName, renderer->GetCurrentCamera().GetTransform().GetTransformedPos());
			}
			else
			{
				//throw "Invalid Transform Uniform: " + uniformName;
				ERROR_LOG("Invalid camera uniform \"%s\"", uniformName.c_str());
			}
		}
		else
		{
			if (uniformType == Uniforms::VEC_3D)
			{
				SetUniformVector3D(uniformName, material->GetVec3D(uniformName));
			}
			else if (uniformType == Uniforms::VEC_4D)
			{
				SetUniformVector4D(uniformName, material->GetVec4D(uniformName));
			}
			else if (uniformType == Uniforms::REAL)
			{
				SetUniformf(uniformName, material->GetReal(uniformName));
			}
			else
			{
				//throw "Invalid Transform Uniform: " + uniformName;
				ERROR_LOG("The uniform \"%s\" of type %d is not supported by the Material class", uniformName.c_str(), uniformType);
			}
		}
	}
}

//void Shader::AddUniform(const std::string& uniform)
//{
//	//INFO_LOG("Adding uniform location \"%s\"", uniform.c_str());
//	unsigned int uniformLocation = glGetUniformLocation(program, uniform.c_str());
//	ASSERT(uniformLocation != INVALID_VALUE);
//
//	if (uniformLocation == INVALID_VALUE)
//	{
//		ERROR_LOG("Could not find uniform \"%s\"", uniform.c_str());
//		exit(EXIT_FAILURE); // TODO: Throw an exception?
//	}
//
//	uniforms.insert(std::pair<std::string, unsigned int>(uniform, uniformLocation));
//}

void Shader::SetUniformi(const std::string& name, int value) const
{
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform1i(itr->second, value);
	}
}

void Shader::SetUniformf(const std::string& name, Math::Real value) const
{
	//for (std::map<std::string, unsigned int>::const_iterator it = m_shaderData->GetUniformMap().begin(); it != m_shaderData->GetUniformMap().end(); ++it)
	//{
	//	DEBUG_LOG("Uniform map <\"%s\", %d>", (it->first).c_str(), it->second);
	//}
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform1f(itr->second, value);
	}
}

void Shader::SetUniformVector3D(const std::string& name, const Math::Vector3D& vector) const
{
	std::map<std::string, GLint>::const_iterator itr;
	//if (name.compare("R_directionalLight.direction") == 0)
	//{
	//	CRITICAL_LOG("Directional light direction = \"%s\"", vector.ToString().c_str());
	//}
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform3f(itr->second, vector.GetX(), vector.GetY(), vector.GetZ());
		//glUniform3fv(itr->second, 1, vector.At());
		// TODO: Check whether glUniform3fv(itr->second, 3, vector) is faster.
	}
}

void Shader::SetUniformVector4D(const std::string& name, const Math::Vector4D& vector) const
{
	std::map<std::string, GLint>::const_iterator itr;
	//if (name.compare("R_directionalLight.direction") == 0)
	//{
	//	CRITICAL_LOG("Directional light direction = \"%s\"", vector.ToString().c_str());
	//}
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform4f(itr->second, vector.GetX(), vector.GetY(), vector.GetZ(), vector.GetW());
		//glUniform4fv(itr->second, 4, vector.At());
		// TODO: Check whether glUniform4fv(itr->second, 4, vector) is faster.
	}
}

void Shader::SetUniformVector4D(const std::string& name, Math::Real x, Math::Real y, Math::Real z, Math::Real w) const
{
	std::map<std::string, GLint>::const_iterator itr;
	//if (name.compare("R_directionalLight.direction") == 0)
	//{
	//	CRITICAL_LOG("Directional light direction = \"%s\"", vector.ToString().c_str());
	//}
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform4f(itr->second, x, y, z, w);
	}
}

void Shader::SetUniformColor(const std::string& uniformName, const Color& color) const
{
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(uniformName, itr))
	{
		glUniform4f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		//glUniform3f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue());
	}
}

void Shader::SetUniformMatrix(const std::string& name, const Math::Matrix4D& matrix) const
{
	//DEBUG_LOG("Setting uniform matrix \"%s\":\n%s", name.c_str(), matrix.ToString().c_str());
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(name, itr))
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		glUniformMatrix4fv(itr->second, 1, GL_FALSE, &(matrix[0][0]));
#else
		glUniformMatrix4fv(itr->second, 1, GL_FALSE, matrix.At(0));
#endif
	}
}

/**
 * BaseLight object is absolutely enough to get all the information necessary for the directional light.
 * Color and intensity are directly stored in the BaseLight object and the direction can be easily retrieved from the transformation.
 */
void Shader::SetUniformDirectionalLight(const std::string& uniformName, const Lighting::BaseLight& directionalLight) const
{
	SetUniformVector3D(uniformName + ".direction", directionalLight.GetTransform().GetTransformedRot().GetForward());
	SetUniformColor(uniformName + ".base.color", directionalLight.GetColor());
	SetUniformf(uniformName + ".base.intensity", directionalLight.GetIntensity());
}

void Shader::SetUniformPointLight(const std::string& uniformName, const Lighting::PointLight& pointLight) const
{
	SetUniformColor(uniformName + ".base.color", pointLight.GetColor());
	SetUniformf(uniformName + ".base.intensity", pointLight.GetIntensity());
	SetUniformf(uniformName + ".attenuation.constant", pointLight.GetAttenuation().GetConstant());
	SetUniformf(uniformName + ".attenuation.linear", pointLight.GetAttenuation().GetLinear());
	SetUniformf(uniformName + ".attenuation.exponent", pointLight.GetAttenuation().GetExponent());
	SetUniformVector3D(uniformName + ".position", pointLight.GetTransform().GetTransformedPos());
	SetUniformf(uniformName + ".range", pointLight.GetRange());
}

void Shader::SetUniformSpotLight(const std::string& uniformName, const Lighting::SpotLight& spotLight) const
{
	SetUniformColor(uniformName + ".pointLight.base.color", spotLight.GetColor());
	SetUniformf(uniformName + ".pointLight.base.intensity", spotLight.GetIntensity());
	SetUniformf(uniformName + ".pointLight.attenuation.constant", spotLight.GetAttenuation().GetConstant());
	SetUniformf(uniformName + ".pointLight.attenuation.linear", spotLight.GetAttenuation().GetLinear());
	SetUniformf(uniformName + ".pointLight.attenuation.exponent", spotLight.GetAttenuation().GetExponent());
	SetUniformVector3D(uniformName + ".pointLight.position", spotLight.GetTransform().GetTransformedPos());
	SetUniformf(uniformName + ".pointLight.range", spotLight.GetRange());
	SetUniformVector3D(uniformName + ".direction", spotLight.GetTransform().GetTransformedRot().GetForward());
	SetUniformf(uniformName + ".cutoff", spotLight.GetCutoff());
}