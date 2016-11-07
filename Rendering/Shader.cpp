#include "StdAfx.h"
#include "Shader.h"
#include "Renderer.h"

//#include "CoreEngine.h"
#include "Utility\Utility.h"
#include "Utility\ILogger.h"
#include "Utility\StringUtility.h"

#include <fstream>

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
	//for (std::vector<Uniforms::UniformStruct>::const_iterator structUniformItr = m_structUniforms.begin(); structUniformItr != m_structUniforms.end(); ++structUniformItr)
	//{
	//	DEBUG_LOG_RENDERING("Struct uniform \"", structUniformItr->name, "\".");
	//	for (std::vector<Uniforms::Uniform>::const_iterator structUniformUniformsItr = structUniformItr->uniforms.begin(); structUniformUniformsItr != structUniformItr->uniforms.end(); ++structUniformUniformsItr)
	//	{
	//		DELOCUST_LOG_RENDERING("Uniform \"", structUniformUniformsItr->name, "\" with type: ", structUniformUniformsItr->type, " has location: ", structUniformUniformsItr->location);
	//	}
	//}
	for (std::vector<std::unique_ptr<Uniforms::UniformBase>>::const_iterator uniformItr = m_uniforms.begin(); uniformItr != m_uniforms.end(); ++uniformItr)
	{
		DEBUG_LOG_RENDERING("Uniform: \"", (*uniformItr)->GetName(), "\" with type: ", (*uniformItr)->GetType());
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
	m_rendererUniforms(std::move(shaderData.m_rendererUniforms)),
	m_uniformNameToLocationMap(std::move(shaderData.m_uniformNameToLocationMap))
	//m_structUniforms(std::move(shaderData.m_structUniforms))
{
	DELOCUST_LOG_RENDERING("ShaderData move constructor called for program: ", m_programID, ". ");
	shaderData.m_programID = 0;
	shaderData.m_shaders.clear();
	shaderData.m_uniforms.clear();
	shaderData.m_rendererUniforms.clear();
	shaderData.m_uniformNameToLocationMap.clear();
	//shaderData.m_structUniforms.clear();
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
			DELOCUST_LOG_RENDERING("Shader line \"", line, "\"");
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

	DELOCUST_LOG_RENDERING("Shader \"", fileName, "\" text loaded");
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

	size_t uniformKeywordLocation = shaderText.find(UNIFORM_KEYWORD);
	while (uniformKeywordLocation != std::string::npos)
	{
		size_t begin = uniformKeywordLocation + std::string(UNIFORM_KEYWORD).length();
		size_t end = shaderText.find(";", begin);
		std::string uniformLine = shaderText.substr(begin + 1, end - begin - 1);

		begin = uniformLine.find(" ");

		const std::string uniformName = uniformLine.substr(begin + 1);
		const Uniforms::UniformType uniformType = Uniforms::ConvertStringToUniformType(uniformLine.substr(0, begin).c_str());
		switch (uniformType)
		{
		case Uniforms::VEC_2D:
			AddUniform<Uniforms::Vector2DUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case Uniforms::VEC_3D:
			AddUniform<Uniforms::Vector3DUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case Uniforms::VEC_4D:
			AddUniform<Uniforms::Vector4DUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case Uniforms::MATRIX_4x4:
			AddUniform<Uniforms::MatrixUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case Uniforms::INT:
			AddUniform<Uniforms::IntUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case Uniforms::REAL:
			AddUniform<Uniforms::RealUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case Uniforms::SAMPLER_2D:
			AddUniform<Uniforms::TextureUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case Uniforms::SAMPLER_CUBE:
			AddUniform<Uniforms::CubeTextureUniform>(uniformName, FindUniformLocation(uniformName));
			break;
			//case BASE_LIGHT:
		case Uniforms::DIRECTIONAL_LIGHT:
			AddUniform<Uniforms::DirectionalLightUniform>(uniformName, FindUniformLocation(uniformName + ".base.color"),
				FindUniformLocation(uniformName + ".base.intensity"), FindUniformLocation(uniformName + ".direction"));
			break;
		case Uniforms::POINT_LIGHT:
			AddUniform<Uniforms::PointLightUniform>(uniformName, FindUniformLocation(uniformName + ".base.color"),
				FindUniformLocation(uniformName + ".base.intensity"), FindUniformLocation(uniformName + ".attenuation.constant"),
				FindUniformLocation(uniformName + ".attenuation.linear"), FindUniformLocation(uniformName + ".attenuation.exponent"),
				FindUniformLocation(uniformName + ".position"), FindUniformLocation(uniformName + ".range"));
			break;
		case Uniforms::SPOT_LIGHT:
			AddUniform<Uniforms::SpotLightUniform>(uniformName, FindUniformLocation(uniformName + ".pointLight.base.color"),
				FindUniformLocation(uniformName + ".pointLight.base.intensity"), FindUniformLocation(uniformName + ".pointLight.attenuation.constant"),
				FindUniformLocation(uniformName + ".pointLight.attenuation.linear"), FindUniformLocation(uniformName + ".pointLight.attenuation.exponent"),
				FindUniformLocation(uniformName + ".pointLight.position"), FindUniformLocation(uniformName + ".pointLight.range"),
				FindUniformLocation(uniformName + ".direction"), FindUniformLocation(uniformName + ".cutoff"));
			break;
			//case ATTENUATION:
		default:
			ERROR_LOG_RENDERING("Cannot add uniform with name \"", uniformName, "\". Incorrect uniform type: ", uniformType, " specified.");
		}
		uniformKeywordLocation = shaderText.find(UNIFORM_KEYWORD, uniformKeywordLocation + std::string(UNIFORM_KEYWORD).length());
	}
	//for (std::vector<Uniforms::Uniform>::const_iterator uniformItr = m_uniforms.begin(); uniformItr != m_uniforms.end(); ++uniformItr)
	//{
	//	ERROR_LOG_RENDERING("Uniform \"", uniformItr->name, "\" of type ", uniformItr->type, " has location ", uniformItr->location);
	//}
}

template <class T, typename... Args>
void Rendering::ShaderData::AddUniform(const std::string& uniformName, Args&&... args)
{
	if (uniformName.substr(0, 2) == "R_")
	{
		m_rendererUniforms.emplace_back(std::make_unique<T>(uniformName, std::forward<Args>(args)...));
	}
	else
	{
		m_uniforms.emplace_back(std::make_unique<T>(uniformName, std::forward<Args>(args)...));
	}
	//m_uniforms.emplace_back(std::make_unique<T>(uniformName, std::forward<Args>(args)...));
}

GLint Rendering::ShaderData::FindUniformLocation(const std::string& uniformName)
{
	GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
	CHECK_CONDITION_RENDERING(location != Uniforms::UniformBase::INVALID_LOCATION, Utility::Logging::ERR, "Invalid location for the uniform \"", uniformName, "\".");
	m_uniformNameToLocationMap[uniformName] = location;
	return location;
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

void Rendering::Shader::UpdateRendererUniforms(const Renderer* renderer) const
{
	START_PROFILING_RENDERING(false, "");
	CHECK_CONDITION_EXIT_RENDERING(renderer != NULL, Utility::Logging::CRITICAL, "Cannot update uniforms. Rendering engine is NULL.");
	for (auto uniformItr = m_shaderData.GetRendererUniforms().begin(); uniformItr != m_shaderData.GetRendererUniforms().end(); ++uniformItr)
	{
		(*uniformItr)->Update(renderer, NULL, NULL);
	}
	STOP_PROFILING_RENDERING("");
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
	for (auto uniformItr = m_shaderData.GetUniforms().begin(); uniformItr != m_shaderData.GetUniforms().end(); ++uniformItr)
	{
		(*uniformItr)->Update(renderer, &transform, material);
	}
	STOP_PROFILING_RENDERING("");
}

//void Shader::AddUniform(const std::string& uniform)
//{
//	//INFO_LOG_RENDERING("Adding uniform location \"", uniform, "\".");
//	unsigned int uniformLocation = glGetUniformLocation(program, uniform.c_str());
//	CHECK_CONDITION_EXIT_RENDERING(uniformLocation != Uniforms::Uniform::INVALID_LOCATION, Utility::ERROR, "Could not find uniform \"", uniform, "\"");
//
//	uniforms.insert(std::pair<std::string, unsigned int>(uniform, uniformLocation));
//}

void Rendering::Shader::SetUniformi(const std::string& name, int value) const
{
	START_PROFILING_RENDERING(false, "");
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform integer \"", name, "\":", value);
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
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform float \"", name, "\":", value);
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
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 2D \"", name, "\":", vector);
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
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 3D \"", name, "\":", vector);
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
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 4D \"", name, "\":", vector);
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
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 4D \"", name, "\": [", x, "; ", y, "; ", z, "; ", w, "].");
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
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform color \"", uniformName, "\":\n", color.ToString());
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
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform matrix \"", name, "\":\n", matrix);
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniformMatrix4fv(itr->second, 1, GL_FALSE, matrix.Data());
	}
	STOP_PROFILING_RENDERING("");
}

