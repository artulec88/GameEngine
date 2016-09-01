#include "StdAfx.h"
#include "Shader.h"
#include "Renderer.h"

//#include "CoreEngine.h"
#include "Utility\Utility.h"
#include "Utility\ILogger.h"
#include "Utility\StringUtility.h"

#include <fstream>

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


///* static */ std::map<std::string, std::shared_ptr<Rendering::ShaderData>> Rendering::Shader::shaderResourceMap;

Rendering::ShaderData::ShaderData(const std::string& fileName) :
	m_programID(glCreateProgram())
{
	DELOCUST_LOG_RENDERING("ShaderData constructor called for file name: \"", fileName, "\". ");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_programID != 0, Utility::Logging::CRITICAL, "Error while creating shader program. Program ID is still 0.");

	std::string shaderText = LoadShaderData(fileName);
	//CRITICAL_LOG_RENDERING("Shader text for shader file \"", fileName, "\" is:\n", shaderText);
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
		DEBUG_LOG_RENDERING("Geometry shader found in file \"", fileName, "\"");
		AddGeometryShader(geometryShaderText);
	}
	AddFragmentShader(fragmentShaderText);

	AddAllAttributes(vertexShaderText);

	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(Compile(), Utility::Logging::CRITICAL, "Error while compiling shader program ", m_programID, " for shader file \"", fileName, "\"");
	DEBUG_LOG_RENDERING("Shader program ", m_programID, " for shader file \"", fileName, "\" has been successfully compiled.");
	AddShaderUniforms(shaderText);
	//ERROR_LOG_RENDERING("Vertex shader text = ", vertexShaderText);
	//AddShaderUniforms(vertexShaderText);
	//if (geometryShaderPresent)
	//{
	//	ERROR_LOG_RENDERING("Geometry shader text = ", geometryShaderText);
	//	AddShaderUniforms(geometryShaderText);
	//}
	//ERROR_LOG_RENDERING("Fragment shader text = ", fragmentShaderText);
	//AddShaderUniforms(fragmentShaderText);

#ifdef DEBUG_LOGGING_ENABLED
	for (std::vector<Uniforms::UniformStruct>::const_iterator structUniformItr = m_structUniforms.begin(); structUniformItr != m_structUniforms.end(); ++structUniformItr)
	{
		DEBUG_LOG_RENDERING("Struct uniform \"", structUniformItr->name, "\".");
		for (std::vector<Uniforms::Uniform>::const_iterator structUniformUniformsItr = structUniformItr->uniforms.begin(); structUniformUniformsItr != structUniformItr->uniforms.end(); ++structUniformUniformsItr)
		{
			DELOCUST_LOG_RENDERING("Uniform \"", structUniformUniformsItr->name, "\" with type: ", structUniformUniformsItr->type, " has location: ", structUniformUniformsItr->location);
		}
	}
	for (std::vector<Uniforms::Uniform>::const_iterator uniformItr = m_uniforms.begin(); uniformItr != m_uniforms.end(); ++uniformItr)
	{
		DEBUG_LOG_RENDERING("Uniform: \"", uniformItr->name, "\" with type: ", uniformItr->type, " has location: ", uniformItr->location);
	}
#endif
}

Rendering::ShaderData::~ShaderData()
{
	DELOCUST_LOG_RENDERING("ShaderData destructor called for program: ", m_programID, ". ");
	DEBUG_LOG_RENDERING("Destroying shader data for shader program: ", m_programID);
	for (std::vector<GLuint>::iterator shaderItr = m_shaders.begin(); shaderItr != m_shaders.end(); ++shaderItr)
	{
		glDetachShader(m_programID, *shaderItr);
		glDeleteShader(*shaderItr);
	}
	if (m_programID != 0)
	{
		glDeleteProgram(m_programID);
	}
}

Rendering::ShaderData::ShaderData(ShaderData&& shaderData) :
	m_programID(std::move(shaderData.m_programID)),
	m_shaders(std::move(shaderData.m_shaders)),
	m_uniforms(std::move(shaderData.m_uniforms)),
	m_uniformNameToLocationMap(std::move(shaderData.m_uniformNameToLocationMap)),
	m_structUniforms(std::move(shaderData.m_structUniforms))
{
	DELOCUST_LOG_RENDERING("ShaderData move constructor called for program: ", m_programID, ". ");
	shaderData.m_programID = 0;
	shaderData.m_shaders.clear();
	shaderData.m_uniforms.clear();
	shaderData.m_uniformNameToLocationMap.clear();
	shaderData.m_structUniforms.clear();
}

std::string Rendering::ShaderData::LoadShaderData(const std::string& fileName) const
{
	std::string name = fileName;
	const char *tmp = strrchr(name.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	DEBUG_LOG_RENDERING("Loading shader data from file \"", name, "\"");

	//std::ifstream file(fileName);
	std::ifstream file(("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Shaders\\" + fileName));
	CHECK_CONDITION_EXIT_RENDERING(file.is_open(), Utility::Logging::ERR, "Unable to open shader file \"", name, "\". Check the path.");

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
			//DEBUG_LOG_RENDERING("#Include directive found in Line = \"", line, "\"");

			std::vector<std::string> tokens;
			DEBUG_LOG_RENDERING("Shader line \"", line, "\"");
			Utility::StringUtility::CutToTokens(line, tokens, ' ');
			//for (int i = 0; i < tokens.size(); ++i)
			//{
			//	std::cout << i << "):\t" << tokens[i] << std::endl;
			//}
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING(tokens.size() > 1, Utility::Logging::ERR, "Error while reading #include directive in the shader file \"", name, "\"");
			std::string includeFileName = tokens[1];
			//DEBUG_LOG_RENDERING("Tokens[1] = \"", tokens[1], "\". IncludeFileName=\"", includeFileName, "\"");
			//includeFileName = includeFileName.substr(1, includeFileName.length() - 2);
			//DEBUG_LOG_RENDERING("Loading an include shader file \"", includeFileName, "\"");

			std::string fragmentToAppend = LoadShaderData(includeFileName);
			output.append(fragmentToAppend + "\n");
		}
	}
	file.close();

	DEBUG_LOG_RENDERING("Shader \"", fileName, "\" text loaded");
	/* ==================== Removing comments from the shader code begin ==================== */
	size_t commentBegin = output.find(MULTI_LINE_COMMENT_BEGIN);
	while (commentBegin != std::string::npos)
	{
		size_t commentEnd = output.find(MULTI_LINE_COMMENT_END, commentBegin);
		output.erase(commentBegin, commentEnd - commentBegin + std::string(MULTI_LINE_COMMENT_END).length());
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

bool Rendering::ShaderData::Compile()
{
	bool compileSuccess = true;

	glLinkProgram(m_programID);
	int infoLogLength;
	if (CheckForErrors(m_programID, GL_LINK_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage;
		errorMessage.reserve(infoLogLength + 1);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &errorMessage[0]);
		ERROR_LOG_RENDERING("Error linking shader program ", m_programID, ":\n", &errorMessage[0], "\r");
	}

	glValidateProgram(m_programID);
	if (CheckForErrors(m_programID, GL_VALIDATE_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage;
		errorMessage.reserve(infoLogLength + 1);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &errorMessage[0]);
		ERROR_LOG_RENDERING("Error validating shader program ", m_programID, ":\n", &errorMessage[0], "\r");
	}

	if (!compileSuccess)
	{
		ERROR_LOG_RENDERING("Shader program ", m_programID, " compilation error occurred. Investigate the problem.");
	}
	else
	{
		DEBUG_LOG_RENDERING("Shader program ", m_programID, " compiled successfully");
	}
	return compileSuccess;
}

bool Rendering::ShaderData::CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength)
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

void Rendering::ShaderData::AddVertexShader(const std::string& vertexShaderText)
{
	AddProgram(vertexShaderText, GL_VERTEX_SHADER);
}

void Rendering::ShaderData::AddGeometryShader(const std::string& geometryShaderText)
{
	AddProgram(geometryShaderText, GL_GEOMETRY_SHADER);
}

void Rendering::ShaderData::AddFragmentShader(const std::string& fragmentShaderText)
{
	AddProgram(fragmentShaderText, GL_FRAGMENT_SHADER);
}

//void ShaderData::AddGeometryShaderFromFile(const std::string& fileName)
//{
//	std::string shaderText = LoadShader(fileName);
//	AddProgram(shaderText, GL_GEOMETRY_SHADER);
//}

void Rendering::ShaderData::AddProgram(const std::string& shaderText, GLenum type)
{
	GLuint shader = glCreateShader(type);

	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(shader != 0, Utility::Logging::EMERGENCY, "Error creating shader type: ", type);

	const GLchar* p[1];
	p[0] = shaderText.c_str();
	GLint lengths[1] = { static_cast<GLint>(shaderText.length()) }; // TODO: warning C4838: conversion from 'unsigned int' to 'GLint' requires a narrowing conversion.
																	//lengths[0] = shaderText.length(); // strlen(text.c_str());

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	int infoLogLength;
	if (CheckForErrors(shader, GL_COMPILE_STATUS, false, infoLogLength))
	{
		std::vector<char> errorMessage;
		errorMessage.reserve(infoLogLength + 1);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &errorMessage[0]);
		ERROR_LOG_RENDERING("Error linking shader program: \"", &errorMessage[0], "\"");
		//return;
	}

	glAttachShader(m_programID, shader);
	m_shaders.push_back(shader);
}

void Rendering::ShaderData::AddAllAttributes(const std::string& vertexShaderText)
{
	int currentAttribLocation = 0;
	size_t attributeLocation = vertexShaderText.find(ATTRIBUTE_KEYWORD);
	while (attributeLocation != std::string::npos)
	{
		bool isCommented = false;
		size_t lastLineEnd = vertexShaderText.rfind("\n", attributeLocation);

		if (lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = vertexShaderText.substr(lastLineEnd, attributeLocation - lastLineEnd);

			//Potential false positives are both in comments, and in macros.
			isCommented = potentialCommentSection.find("//") != std::string::npos || potentialCommentSection.find("#") != std::string::npos;
		}

		if (!isCommented)
		{
			size_t locationLocation = vertexShaderText.find(LOCATION_KEYWORD, lastLineEnd);
			if (locationLocation != std::string::npos)
			{
				size_t equalLocation = vertexShaderText.find("=", locationLocation);
				size_t begin = equalLocation + 1;
				size_t end = vertexShaderText.find(";", begin);

				std::string attributeLine = vertexShaderText.substr(begin, end - begin);
				Utility::StringUtility::LeftTrim(attributeLine);

				end = attributeLine.find(")");
				std::string locationNumber = attributeLine.substr(0, end);
				currentAttribLocation = Utility::StringUtility::ToInt(locationNumber);
			}
			size_t begin = attributeLocation + std::string(ATTRIBUTE_KEYWORD).length() + 1;
			size_t end = vertexShaderText.find(";", begin);

			std::string attributeLine = vertexShaderText.substr(begin, end - begin);

			begin = attributeLine.find(" ");
			std::string attributeName = attributeLine.substr(begin + 1);

			glBindAttribLocation(m_programID, currentAttribLocation, attributeName.c_str());
			++currentAttribLocation;
		}
		attributeLocation = vertexShaderText.find(ATTRIBUTE_KEYWORD, attributeLocation + std::string(ATTRIBUTE_KEYWORD).length());
	}
}

void Rendering::ShaderData::AddShaderUniforms(const std::string& shaderText)
{
	std::vector<Uniforms::UniformStructInfo> structInfos = FindUniformStructInfos(shaderText);
#ifdef DEBUG_LOGGING_ENABLED
	for (std::vector<Uniforms::UniformStructInfo>::const_iterator itr = structInfos.begin(); itr != structInfos.end(); ++itr)
	{
		DEBUG_LOG_RENDERING("struct.name = \"", itr->name, "\"");
#ifdef DELOCUST_LOGGING_ENABLED
		for (std::vector<Uniforms::UniformInfo>::const_iterator innerItr = itr->uniformInfos.begin(); innerItr != itr->uniformInfos.end(); ++innerItr)
		{
			DELOCUST_LOG_RENDERING("\t .memberName.name = \"", innerItr->name, "\"\t .memberName.uniformType = ", innerItr->type);
		}
#endif
	}
#endif

	size_t uniformLocation = shaderText.find(UNIFORM_KEYWORD);
	while (uniformLocation != std::string::npos)
	{
		size_t begin = uniformLocation + std::string(UNIFORM_KEYWORD).length();
		size_t end = shaderText.find(";", begin);
		std::string uniformLine = shaderText.substr(begin + 1, end - begin - 1);

		begin = uniformLine.find(" ");

		const std::string uniformName = uniformLine.substr(begin + 1);
		const Uniforms::UniformType uniformType = Uniforms::ConvertStringToUniformType(uniformLine.substr(0, begin).c_str());
		if (Uniforms::IsPrimitiveUniformType(uniformType))
		{
			GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
			CHECK_CONDITION_EXIT_RENDERING(location != INVALID_VALUE, Utility::Logging::EMERGENCY, "Invalid value of the location (",
				location, ") for the uniform \"", uniformName, "\"");
			m_uniforms.push_back(Uniforms::Uniform(uniformName, uniformType, location));
			m_uniformNameToLocationMap.insert(std::make_pair(uniformName, location));
		}
		else
		{
			//CRITICAL_LOG_RENDERING("Structural uniform \"", uniformName, "\" of type ", uniformType);
			AddStructuralUniform(uniformName, uniformType, structInfos);
		}
		uniformLocation = shaderText.find(UNIFORM_KEYWORD, uniformLocation + std::string(UNIFORM_KEYWORD).length());
	}
	//for (std::vector<Uniforms::Uniform>::const_iterator uniformItr = m_uniforms.begin(); uniformItr != m_uniforms.end(); ++uniformItr)
	//{
	//	ERROR_LOG_RENDERING("Uniform \"", uniformItr->name, "\" of type ", uniformItr->type, " has location ", uniformItr->location);
	//}
}

void Rendering::ShaderData::AddStructuralUniform(const std::string& uniformName, Uniforms::UniformType uniformType, const std::vector<Uniforms::UniformStructInfo>& structInfos)
{
	const std::string uniformTypeStr = Uniforms::ConvertUniformTypeToString(uniformType);
	DEBUG_LOG_RENDERING("Adding uniform \"", uniformName, "\" of type: ", uniformTypeStr);
	for (std::vector<Uniforms::UniformStructInfo>::const_iterator structInfoItr = structInfos.begin(); structInfoItr != structInfos.end(); ++structInfoItr)
	{
		//EMERGENCY_LOG_RENDERING("structInfoItr->name = \"", structInfoItr->name, "\". UniformTypeStr = \"", uniformTypeStr, "\".");
		if (structInfoItr->name == uniformTypeStr)
		{
			for (std::vector<Uniforms::UniformInfo>::const_iterator structMemberNameItr = structInfoItr->uniformInfos.begin(); structMemberNameItr != structInfoItr->uniformInfos.end(); ++structMemberNameItr)
			{
				const std::string name = uniformName + "." + structMemberNameItr->name;
				CHECK_CONDITION_EXIT_RENDERING(Uniforms::IsPrimitiveUniformType(structMemberNameItr->type), Utility::Logging::EMERGENCY,
					"The structural uniform cannot be added. Non-primitive uniform type found in the uniform info for the type \"", uniformTypeStr, "\".");
				GLint location = glGetUniformLocation(m_programID, name.c_str());
				CHECK_CONDITION_EXIT_RENDERING(location != INVALID_VALUE, Utility::Logging::EMERGENCY, "Invalid value of the location (",
					location, ") for the uniform \"", name, "\"");
				m_uniforms.push_back(Uniforms::Uniform(name, structMemberNameItr->type, location));
				m_uniformNameToLocationMap.insert(std::make_pair(name, location));
			}
			break;
		}
	}
}

std::vector<Rendering::Uniforms::UniformStructInfo> Rendering::ShaderData::FindUniformStructInfos(const std::string& shaderText) const
{
	std::vector<Uniforms::UniformStructInfo> result;

	size_t structLocation = shaderText.find(STRUCT_KEY);
	DELOCUST_LOG_RENDERING("structLocation = ", structLocation);
	while (structLocation != std::string::npos)
	{
		structLocation += std::string(STRUCT_KEY).length() + 1; //Ignore the struct keyword and space

		size_t braceOpening = shaderText.find("{", structLocation);
		size_t braceClosing = shaderText.find("}", braceOpening);

		Uniforms::UniformStructInfo newStruct;
		newStruct.name = FindUniformStructName(shaderText.substr(structLocation, braceOpening - structLocation));
		newStruct.uniformInfos = FindUniformStructComponents(shaderText.substr(braceOpening, braceClosing - braceOpening), result);

		result.push_back(newStruct);
		structLocation = shaderText.find(STRUCT_KEY, structLocation);
	}
	return result;
}

std::string Rendering::ShaderData::FindUniformStructName(const std::string& structStartToOpeningBrace) const
{
	std::vector<std::string> tokens;
	Utility::StringUtility::CutToTokens(structStartToOpeningBrace, tokens, ' ');
	Utility::StringUtility::RightTrim(tokens[0]);
	//DELOCUST_LOG_RENDERING("tokens[0] = \"", tokens[0], "\"");
	return tokens[0];

	//return Util::Split(Util::Split(structStartToOpeningBrace, ' ')[0], '\n')[0];
}

std::vector<Rendering::Uniforms::UniformInfo> Rendering::ShaderData::FindUniformStructComponents(const std::string& openingBraceToClosingBrace, const std::vector<Uniforms::UniformStructInfo>& structUniformInfos) const
{
	constexpr int CHARS_TO_IGNORE_COUNT = 4;
	constexpr std::array<char, CHARS_TO_IGNORE_COUNT> charsToIgnore = { ' ', '\n', '\t', '{' };
	constexpr char delimChars[] = { '\n', ';' };
	constexpr size_t UNSIGNED_NEG_ONE = (size_t)-1;

	std::vector<Uniforms::UniformInfo> result;
	std::vector<std::string> structLines;
	Utility::StringUtility::CutToTokens(openingBraceToClosingBrace, structLines, delimChars, 2);

	for (std::vector<std::string>::const_iterator structLinesItr = structLines.begin(); structLinesItr != structLines.end(); ++structLinesItr)
	{
		//DEBUG_LOG_RENDERING("structLines[", i, "] = \"", structLines[i], "\"");
		if (structLinesItr->substr(0, 2) == SINGLE_LINE_COMMENT)
		{
			continue;
		}

		size_t nameBegin = UNSIGNED_NEG_ONE;
		size_t nameEnd = UNSIGNED_NEG_ONE;
		for (unsigned int j = 0; j < structLinesItr->length(); ++j)
		{
			bool isIgnoreableCharacter = false;
			for (size_t k = 0; k < charsToIgnore.size(); ++k)
			{
				if ((*structLinesItr)[j] == charsToIgnore[k])
				{
					isIgnoreableCharacter = true;
					break;
				}
			}
			if (nameBegin == UNSIGNED_NEG_ONE && isIgnoreableCharacter == false)
			{
				nameBegin = j;
			}
			else if (nameBegin != UNSIGNED_NEG_ONE && isIgnoreableCharacter)
			{
				nameEnd = j;
				break;
			}
		}
		if (nameBegin == UNSIGNED_NEG_ONE || nameEnd == UNSIGNED_NEG_ONE)
			continue;

		AddUniformInfos(structUniformInfos, result, structLinesItr->substr(nameEnd + 1), structLinesItr->substr(nameBegin, nameEnd - nameBegin));
	}
	return result;
}

void Rendering::ShaderData::AddUniformInfos(const std::vector<Uniforms::UniformStructInfo>& structUniformInfos, std::vector<Uniforms::UniformInfo>& uniformInfos,
	const std::string& uniformName, const std::string& uniformTypeStr) const
{
	const Uniforms::UniformType uniformType = Uniforms::ConvertStringToUniformType(uniformTypeStr.c_str());
	DELOCUST_LOG_RENDERING("Uniform type = \"", uniformTypeStr, "\". Uniform type = ", uniformType);
	bool structFound = false;
	for (std::vector<Uniforms::UniformStructInfo>::const_iterator structUniformInfoItr = structUniformInfos.begin(); structUniformInfoItr != structUniformInfos.end(); ++structUniformInfoItr)
	{
		if (structUniformInfoItr->name == uniformTypeStr)
		{
			for (std::vector<Uniforms::UniformInfo>::const_iterator uniformInfoItr = structUniformInfoItr->uniformInfos.begin(); uniformInfoItr != structUniformInfoItr->uniformInfos.end(); ++uniformInfoItr)
			{
				AddUniformInfos(structUniformInfos, uniformInfos, uniformName + "." + uniformInfoItr->name, Uniforms::ConvertUniformTypeToString(uniformInfoItr->type));
			}
			structFound = true;
		}
	}
	if (!structFound)
	{
		uniformInfos.emplace_back(uniformName, uniformType);
	}
}

bool Rendering::ShaderData::IsUniformPresent(const std::string& uniformName, std::map<std::string, GLint>::const_iterator& itr) const
{
	itr = m_uniformNameToLocationMap.find(uniformName);
	CHECK_CONDITION_RENDERING(itr != m_uniformMap.end(), Utility::Logging::ERR, "Uniform \"", uniformName, "\" has not been found.");
	return (itr != m_uniformNameToLocationMap.end());
}

/* ==================== Shader class begin ==================== */
Rendering::Shader::Shader(const std::string& fileName) :
	m_shaderData(fileName),
	m_fileName(fileName)
#ifdef PROFILING_RENDERING_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Shader"))
#endif
{
	//DELOCUST_LOG_RENDERING("Shader constructor called for file name: \"", m_fileName, "\". ");
	DEBUG_LOG_RENDERING("Shader constructed based on filename \"", fileName, "\"");
}


Rendering::Shader::~Shader(void)
{
	DELOCUST_LOG_RENDERING("Shader destructor called for file name: \"", m_fileName, "\". ");
}

//Rendering::Shader::Shader(const Shader& shader) :
//	m_shaderData(shader.m_shaderData),
//	m_fileName(shader.m_fileName)
//{
//
//}

Rendering::Shader::Shader(Shader&& shader) :
	m_shaderData(std::move(shader.m_shaderData)),
	m_fileName(std::move(shader.m_fileName))
#ifdef PROFILING_RENDERING_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Shader"))
#endif
{
	DELOCUST_LOG_RENDERING("Shader move constructor called for file name: \"", m_fileName, "\". ");
	//shader.m_shaderData = nullptr;
	//shader.m_fileName.clear();
}

void Rendering::Shader::Bind() const
{
	//Rendering::CheckErrorCode(__FUNCTION__, "Started shader binding");
	//DELOCUST_LOG_RENDERING("Binding shader \"", m_fileName, "\".");
	glUseProgram(m_shaderData.GetProgram());
	//Rendering::CheckErrorCode(__FUNCTION__, "Finished shader binding");
}

bool Rendering::Shader::IsBound() const
{
	GLint currentProgramID;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgramID);
	return currentProgramID == GetProgramID();
}

void Rendering::Shader::Unbind() const
{
	DELOCUST_LOG_RENDERING("The shader is being unbound");
	glUseProgram(0);
}

void Rendering::Shader::UpdateRendererUniforms(const Renderer* renderer) const
{
}

void Rendering::Shader::UpdateMaterialUniforms(const Material* material) const
{
}

void Rendering::Shader::UpdateTransformUniforms(const Math::Transform& transform) const
{
}

void Rendering::Shader::UpdateUniforms(const Math::Transform& transform, const Material* material, const Renderer* renderer) const
{
	START_PROFILING_RENDERING(false, "");
	CHECK_CONDITION_EXIT_RENDERING(renderer != NULL, Utility::Logging::CRITICAL, "Cannot update uniforms. Rendering engine is NULL.");

	Math::Matrix4D worldMatrix(transform.GetTransformation());
	// TODO: Check which one is the fastest: SOLUTION #1, SOLUTION #2, etc.
	/* ==================== SOLUTION #1 begin ==================== */
	//Matrix4D projectedMatrix(renderer->GetCurrentCamera().GetViewProjection()); // TODO: Pass camera object as parameter instead of using GetCurrentCamera() function.
	//projectedMatrix *= worldMatrix;
	/* ==================== SOLUTION #1 end ==================== */
	/* ==================== SOLUTION #2 begin ==================== */
	Math::Matrix4D projectedMatrix(renderer->GetCurrentCamera().GetViewProjection() * worldMatrix); // TODO: Pass camera object as parameter instead of using GetCurrentCamera() function.
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
	for (std::vector<Uniforms::Uniform>::const_iterator uniformItr = m_shaderData.GetUniforms().begin(); uniformItr != m_shaderData.GetUniforms().end(); ++uniformItr)
	{
		const std::string& uniformName = uniformItr->name;
		const Uniforms::UniformType& uniformType = uniformItr->type;
		DELOCUST_LOG_RENDERING("Updating uniform with name = \"", uniformName, "\" and type = ", Uniforms::ConvertUniformTypeToString(uniformType), ".");

		const std::string uniformNamePrefix = uniformName.substr(0, 2);

		if (uniformNamePrefix == "R_")
		{
			std::string unprefixedName = uniformName.substr(2, uniformName.length());
			if (unprefixedName == "lightMatrix")
			{
				//CRITICAL_LOG_RENDERING("Renderer->GetLightMatrix() = \"", renderer->GetLightMatrix().ToString(), "\"");
				//CRITICAL_LOG_RENDERING("WorldMatrix = \"", worldMatrix.ToString(), "\"");
#ifdef MATRIX_MODE_TWO_DIMENSIONS
				glUniformMatrix4fv(uniformItr->location, 1, GL_FALSE, &((renderer->GetLightMatrix() * worldMatrix)[0][0]));
#else
				glUniformMatrix4fv(uniformItr->location, 1, GL_FALSE, (renderer->GetLightMatrix() * worldMatrix).At(0));
#endif
			}
			else if ((uniformType == Uniforms::SAMPLER_2D) || (uniformType == Uniforms::SAMPLER_CUBE))
			{
				unsigned int samplerSlot = renderer->GetSamplerSlot(unprefixedName);
				//CRITICAL_LOG_RENDERING("Binding texture \"", unprefixedName, "\" in sampler slot ", samplerSlot);
				if (unprefixedName == "cubeShadowMap")
				{
					renderer->BindCubeShadowMap(samplerSlot);
				}
				else
				{
					unsigned int multitextureIndex = 0; // used only by the multitextures
					const Texture* texture = renderer->GetTexture(unprefixedName, &multitextureIndex);
					CHECK_CONDITION_EXIT_RENDERING(texture != NULL, Utility::Logging::CRITICAL, "Updating uniforms operation failed. Rendering engine texture \"", unprefixedName, "\" is NULL.");
					texture->Bind(samplerSlot, multitextureIndex);
				}
				glUniform1i(uniformItr->location, samplerSlot);
			}
			//else if (uniformType == "samplerCubeShadow")
			//{
			//	unsigned int samplerSlot = renderer->GetSamplerSlot(unprefixedName);
			//	renderer->BindCubeShadowMap(samplerSlot);
			//	glUniform1i(uniformItr->location, samplerSlot);
			//}
			else if (uniformType == Uniforms::VEC_2D)
			{
				const Math::Vector2D& vector = renderer->GetVec2D(unprefixedName);
				glUniform2f(uniformItr->location, vector.GetX(), vector.GetY());
			}
			else if (uniformType == Uniforms::VEC_3D)
			{
				const Math::Vector3D& vector = renderer->GetVec3D(unprefixedName);
				glUniform3f(uniformItr->location, vector.GetX(), vector.GetY(), vector.GetZ());
			}
			else if (uniformType == Uniforms::VEC_4D)
			{
				const Math::Vector4D& vector = renderer->GetVec4D(unprefixedName);
				glUniform4f(uniformItr->location, vector.GetX(), vector.GetY(), vector.GetZ(), vector.GetW());
			}
			else if (uniformType == Uniforms::REAL)
			{
				glUniform1f(uniformItr->location, renderer->GetReal(unprefixedName));
			}
			//else if (uniformType == Uniforms::DIRECTIONAL_LIGHT)
			//{
			//	// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
			//	//Lighting::DirectionalLight* directionalLight = dynamic_cast<Lighting::DirectionalLight*>(renderer->GetCurrentLight());
			//	const Lighting::BaseLight* directionalLight = renderer->GetCurrentLight();
			//	CHECK_CONDITION_EXIT_RENDERING(directionalLight != NULL, Utility::Logging::ERR, "Cannot update directional light uniform. Directional light instance is NULL.");
			//	SetUniformDirectionalLight(uniformName, *directionalLight);
			//}
			//else if (uniformType == Uniforms::POINT_LIGHT)
			//{
			//	// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
			//	//Lighting::PointLight* pointLight = dynamic_cast<Lighting::PointLight*>(renderer->GetCurrentLight());
			//	const Lighting::PointLight* pointLight = renderer->GetCurrentPointLight();
			//	CHECK_CONDITION_EXIT_RENDERING(pointLight != NULL, Utility::Logging::ERR, "Cannot update point light uniform. Point light instance is NULL.");
			//	SetUniformPointLight(uniformName, *pointLight);
			//}
			//else if (uniformType == Uniforms::SPOT_LIGHT)
			//{
			//	// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
			//	const Lighting::SpotLight* spotLight = dynamic_cast<const Lighting::SpotLight*>(renderer->GetCurrentLight());
			//	//const Lighting::SpotLight* spotLight = renderer->GetSpotLight();
			//	CHECK_CONDITION_EXIT_RENDERING(spotLight != NULL, Utility::Logging::ERR, "Cannot update spot light uniform. Spot light instance is NULL.");
			//	SetUniformSpotLight(uniformName, *spotLight);
			//}
			else
			{
				//renderer->UpdateUniformStruct(transform, *material, *this, uniformName, uniformType);
				ERROR_LOG_RENDERING("Uniform name \"", uniformName, "\" of type ", uniformType, " is not supported by the rendering engine");
			}
		}
		else if ((uniformType == Uniforms::SAMPLER_2D) || (uniformType == Uniforms::SAMPLER_CUBE))
		{
			unsigned int samplerSlot = renderer->GetSamplerSlot(uniformName);
			const Texture* texture = material->GetTexture(uniformName);
			CHECK_CONDITION_EXIT_RENDERING(texture != NULL, Utility::Logging::CRITICAL, "Updating uniforms operation failed. Material texture \"", uniformName, "\" is NULL.");
			texture->Bind(samplerSlot);
			glUniform1i(uniformItr->location, samplerSlot);
		}
		else if (uniformNamePrefix == "T_") // tranform uniform
		{
			if (uniformName == "T_MVP")
			{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
				glUniformMatrix4fv(uniformItr->location, 1, GL_FALSE, &(projectedMatrix[0][0]));
#else
				glUniformMatrix4fv(uniformItr->location, 1, GL_FALSE, projectedMatrix.At(0));
#endif
			}
			else if (uniformName == "T_VP")
			{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
				glUniformMatrix4fv(uniformItr->location, 1, GL_FALSE, &(renderer->GetCurrentCamera().GetViewProjection()[0][0]));
#else
				glUniformMatrix4fv(uniformItr->location, 1, GL_FALSE, renderer->GetCurrentCamera().GetViewProjection().At(0));
#endif
			}
			else if (uniformName == "T_model")
			{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
				glUniformMatrix4fv(uniformItr->location, 1, GL_FALSE, &(worldMatrix[0][0]));
#else
				glUniformMatrix4fv(uniformItr->location, 1, GL_FALSE, worldMatrix.At(0));
#endif
			}
			else if (uniformName == "T_scale")
			{
				glUniform1f(uniformItr->location, transform.GetScale());
			}
			else
			{
				//throw "Invalid Transform Uniform: " + uniformName;
				ERROR_LOG_RENDERING("Invalid transform uniform \"", uniformName, "\"");
			}
		}
		else if (uniformNamePrefix == "C_")
		{
			if (uniformName == "C_eyePos")
			{
				const Math::Vector3D& vector = renderer->GetCurrentCamera().GetPos();
				glUniform3f(uniformItr->location, vector.GetX(), vector.GetY(), vector.GetZ());
			}
			else
			{
				//throw "Invalid Transform Uniform: " + uniformName;
				ERROR_LOG_RENDERING("Invalid camera uniform \"", uniformName, "\"");
			}
		}
		else
		{
			if (uniformType == Uniforms::VEC_3D)
			{
				const Math::Vector3D& vector = material->GetVec3D(uniformName);
				glUniform3f(uniformItr->location, vector.GetX(), vector.GetY(), vector.GetZ());
			}
			else if (uniformType == Uniforms::VEC_4D)
			{
				const Math::Vector4D& vector = material->GetVec4D(uniformName);
				glUniform4f(uniformItr->location, vector.GetX(), vector.GetY(), vector.GetZ(), vector.GetW());
			}
			else if (uniformType == Uniforms::REAL)
			{
				glUniform1f(uniformItr->location, material->GetReal(uniformName));
			}
			else
			{
				//throw "Invalid Transform Uniform: " + uniformName;
				ERROR_LOG_RENDERING("The uniform \"", uniformName, "\" of type ", uniformType, " is not supported by the Material class");
			}
		}
	}
	STOP_PROFILING_RENDERING("");
}

//void Shader::AddUniform(const std::string& uniform)
//{
//	//INFO_LOG_RENDERING("Adding uniform location \"", uniform, "\".");
//	unsigned int uniformLocation = glGetUniformLocation(program, uniform.c_str());
//	CHECK_CONDITION_EXIT_RENDERING(uniformLocation != INVALID_VALUE, Utility::ERROR, "Could not find uniform \"", uniform, "\"");
//
//	uniforms.insert(std::pair<std::string, unsigned int>(uniform, uniformLocation));
//}

void Rendering::Shader::SetUniformi(const std::string& name, int value) const
{
	START_PROFILING_RENDERING(false, "");
	DEBUG_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform integer \"", name, "\":", value);
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform1i(itr->second, value);
	}
	else
	{
		EMERGENCY_LOG_RENDERING("Uniform \"", name, "\" is not defined in the shader \"", m_fileName, "\"");
	}
	STOP_PROFILING_RENDERING("");
}

void Rendering::Shader::SetUniformf(const std::string& name, Math::Real value) const
{
	START_PROFILING_RENDERING(false, "");
	DEBUG_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform float \"", name, "\":", value);
	//for (std::map<std::string, unsigned int>::const_iterator it = m_shaderData->GetUniformMap().begin(); it != m_shaderData->GetUniformMap().end(); ++it)
	//{
	//	DEBUG_LOG_RENDERING("Uniform map <\"", it->first, "\", ", it->second, ">");
	//}
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform1f(itr->second, value);
	}
	STOP_PROFILING_RENDERING("");
}

void Rendering::Shader::SetUniformVector2D(const std::string& name, const Math::Vector2D& vector) const
{
	START_PROFILING_RENDERING(false, "");
	DEBUG_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 2D \"", name, "\":", vector.ToString());
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform2f(itr->second, vector.GetX(), vector.GetY());
		//glUniform3fv(itr->second, 1, vector.At());
		// TODO: Check whether glUniform3fv(itr->second, 3, vector) is faster.
	}
	STOP_PROFILING_RENDERING("");
}

void Rendering::Shader::SetUniformVector3D(const std::string& name, const Math::Vector3D& vector) const
{
	START_PROFILING_RENDERING(false, "");
	DEBUG_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 3D \"", name, "\":", vector.ToString());
	std::map<std::string, GLint>::const_iterator itr;
	//if (name.compare("R_directionalLight.direction") == 0)
	//{
	//	CRITICAL_LOG_RENDERING("Directional light direction = \"", vector.ToString(), "\"");
	//}
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform3f(itr->second, vector.GetX(), vector.GetY(), vector.GetZ());
		//glUniform3fv(itr->second, 1, vector.At());
		// TODO: Check whether glUniform3fv(itr->second, 3, vector) is faster.
	}
	STOP_PROFILING_RENDERING("");
}

void Rendering::Shader::SetUniformVector4D(const std::string& name, const Math::Vector4D& vector) const
{
	START_PROFILING_RENDERING(false, "");
	DEBUG_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 4D \"", name, "\":", vector.ToString());
	std::map<std::string, GLint>::const_iterator itr;
	//if (name.compare("R_directionalLight.direction") == 0)
	//{
	//	CRITICAL_LOG_RENDERING("Directional light direction = \"", vector.ToString(), "\"");
	//}
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform4f(itr->second, vector.GetX(), vector.GetY(), vector.GetZ(), vector.GetW());
		//glUniform4fv(itr->second, 4, vector.At());
		// TODO: Check whether glUniform4fv(itr->second, 4, vector) is faster.
	}
	STOP_PROFILING_RENDERING("");
}

void Rendering::Shader::SetUniformVector4D(const std::string& name, Math::Real x, Math::Real y, Math::Real z, Math::Real w) const
{
	START_PROFILING_RENDERING(false, "");
	DEBUG_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 4D \"", name, "\": [", x, "; ", y, "; ", z, "; ", w, "].");
	std::map<std::string, GLint>::const_iterator itr;
	//if (name.compare("R_directionalLight.direction") == 0)
	//{
	//	CRITICAL_LOG_RENDERING("Directional light direction = \"", vector.ToString(), "\"");
	//}
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform4f(itr->second, x, y, z, w);
	}
	STOP_PROFILING_RENDERING("");
}

void Rendering::Shader::SetUniformColor(const std::string& uniformName, const Color& color) const
{
	START_PROFILING_RENDERING(false, "");
	DEBUG_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform color \"", uniformName, "\":\n", color.ToString());
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(uniformName, itr))
	{
		glUniform4f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		//glUniform3f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue());
	}
	STOP_PROFILING_RENDERING("");
}

void Rendering::Shader::SetUniformMatrix(const std::string& name, const Math::Matrix4D& matrix) const
{
	START_PROFILING_RENDERING(false, "");
	DEBUG_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform matrix \"", name, "\":\n", matrix.ToString());
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(name, itr))
	{
#ifdef MATRIX_MODE_TWO_DIMENSIONS
		glUniformMatrix4fv(itr->second, 1, GL_FALSE, &(matrix[0][0]));
#else
		glUniformMatrix4fv(itr->second, 1, GL_FALSE, matrix.At(0));
#endif
	}
	STOP_PROFILING_RENDERING("");
}

/**
* BaseLight object is absolutely enough to get all the information necessary for the directional light.
* Color and intensity are directly stored in the BaseLight object and the direction can be easily retrieved from the transformation.
*/
void Rendering::Shader::SetUniformDirectionalLight(const std::string& uniformName, const Lighting::BaseLight& directionalLight) const
{
	START_PROFILING_RENDERING(false, "");
	DELOCUST_LOG_RENDERING("Directional light:\n\tIntensity = ", directionalLight.GetIntensity(), "\n\tColor = ", directionalLight.GetColor().ToString(),
		"\n\tDirection = ", directionalLight.GetTransform().GetTransformedRot().GetForward().ToString());
	SetUniformVector3D(uniformName + ".direction", directionalLight.GetTransform().GetTransformedRot().GetForward());
	SetUniformColor(uniformName + ".base.color", directionalLight.GetColor());
	SetUniformf(uniformName + ".base.intensity", directionalLight.GetIntensity());
	STOP_PROFILING_RENDERING("");
}

void Rendering::Shader::SetUniformPointLight(const std::string& uniformName, const Lighting::PointLight& pointLight) const
{
	START_PROFILING_RENDERING(false, "");
	SetUniformColor(uniformName + ".base.color", pointLight.GetColor());
	SetUniformf(uniformName + ".base.intensity", pointLight.GetIntensity());
	SetUniformf(uniformName + ".attenuation.constant", pointLight.GetAttenuation().GetConstant());
	SetUniformf(uniformName + ".attenuation.linear", pointLight.GetAttenuation().GetLinear());
	SetUniformf(uniformName + ".attenuation.exponent", pointLight.GetAttenuation().GetExponent());
	SetUniformVector3D(uniformName + ".position", pointLight.GetTransform().GetTransformedPos());
	SetUniformf(uniformName + ".range", pointLight.GetRange());
	STOP_PROFILING_RENDERING("");
}

void Rendering::Shader::SetUniformSpotLight(const std::string& uniformName, const Lighting::SpotLight& spotLight) const
{
	START_PROFILING_RENDERING(false, "");
	SetUniformColor(uniformName + ".pointLight.base.color", spotLight.GetColor());
	SetUniformf(uniformName + ".pointLight.base.intensity", spotLight.GetIntensity());
	SetUniformf(uniformName + ".pointLight.attenuation.constant", spotLight.GetAttenuation().GetConstant());
	SetUniformf(uniformName + ".pointLight.attenuation.linear", spotLight.GetAttenuation().GetLinear());
	SetUniformf(uniformName + ".pointLight.attenuation.exponent", spotLight.GetAttenuation().GetExponent());
	SetUniformVector3D(uniformName + ".pointLight.position", spotLight.GetTransform().GetTransformedPos());
	SetUniformf(uniformName + ".pointLight.range", spotLight.GetRange());
	SetUniformVector3D(uniformName + ".direction", spotLight.GetTransform().GetTransformedRot().GetForward());
	SetUniformf(uniformName + ".cutoff", spotLight.GetCutoff());
	STOP_PROFILING_RENDERING("");
}
