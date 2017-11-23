#include "StdAfx.h"
#include "Shader.h"
#include "Renderer.h"

#include "Utility/Utility.h"
#include "Utility/ILogger.h"
#include "Utility/StringUtility.h"

#include <fstream>

///* static */ std::map<std::string, std::shared_ptr<Rendering::ShaderData>> Rendering::Shader::shaderResourceMap;

rendering::ShaderData::ShaderData(const std::string& directoryPath, const std::string& fileName) :
	m_programId(glCreateProgram())
{
	DELOCUST_LOG_RENDERING("ShaderData constructor called for file name: \"", fileName, "\". ");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_programId != 0, utility::logging::CRITICAL, "Error while creating shader program. Program ID is still 0.");

	std::string shaderText = LoadShaderData(directoryPath, fileName);
	//CRITICAL_LOG_RENDERING("Shader text for shader file \"", fileName, "\" is:\n", shaderText);
	const auto geometryShaderPresent = (shaderText.find("defined(GS_BUILD)") != std::string::npos); // geometry shader found

	/**
	* TODO: Vertex shader text should only contain the shader file content in the #if defined(VS_BUILD) block.
	* Analogically, the fragment shader text should only contain the content in the #if defined(FS_BUILD) block.
	* The same should apply to the geometry shader text.
	*/
	const auto vertexShaderText = "#version " + glslVersion + "\n#define VS_BUILD\n#define GLSL_VERSION " + glslVersion + "\n" + shaderText;
	const auto geometryShaderText = "#version " + glslVersion + "\n#define GS_BUILD\n#define GLSL_VERSION " + glslVersion + "\n" + shaderText;
	const auto fragmentShaderText = "#version " + glslVersion + "\n#define FS_BUILD\n#define GLSL_VERSION " + glslVersion + "\n" + shaderText;

	AddVertexShader(vertexShaderText);
	if (geometryShaderPresent)
	{
		DELOCUST_LOG_RENDERING("Geometry shader found in file \"", fileName, "\"");
		AddGeometryShader(geometryShaderText);
	}
	AddFragmentShader(fragmentShaderText);

	AddAllAttributes(vertexShaderText);

	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(Compile(), utility::logging::CRITICAL, "Error while compiling shader program ", m_programId, " for shader file \"", fileName, "\"");
	DEBUG_LOG_RENDERING("Shader program ", m_programId, " for shader file \"", fileName, "\" has been successfully compiled.");
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

#ifdef DELOCUST_LOGGING_ENABLED
	//for (std::vector<uniforms::UniformStruct>::const_iterator structUniformItr = m_structUniforms.begin(); structUniformItr != m_structUniforms.end(); ++structUniformItr)
	//{
	//	DELOCUST_LOG_RENDERING("Struct uniform \"", structUniformItr->name, "\".");
	//	for (std::vector<uniforms::Uniform>::const_iterator structUniformUniformsItr = structUniformItr->uniforms.begin(); structUniformUniformsItr != structUniformItr->uniforms.end(); ++structUniformUniformsItr)
	//	{
	//		DELOCUST_LOG_RENDERING("Uniform \"", structUniformUniformsItr->name, "\" with type: ", structUniformUniformsItr->type, " has location: ", structUniformUniformsItr->location);
	//	}
	//}
	for (std::vector<std::unique_ptr<uniforms::UniformBase>>::const_iterator uniformItr = m_uniforms.begin(); uniformItr != m_uniforms.end(); ++uniformItr)
	{
		DELOCUST_LOG_RENDERING("Uniform: \"", (*uniformItr)->GetName(), "\" with type: ", (*uniformItr)->GetType());
	}
#endif
}

rendering::ShaderData::~ShaderData()
{
	DELOCUST_LOG_RENDERING("Destroying shader data for shader program: ", m_programId, ".");
	for (std::vector<GLuint>::iterator shaderItr = m_shaders.begin(); shaderItr != m_shaders.end(); ++shaderItr)
	{
		glDetachShader(m_programId, *shaderItr);
		glDeleteShader(*shaderItr);
	}
	if (m_programId != 0)
	{
		glDeleteProgram(m_programId);
	}
}

rendering::ShaderData::ShaderData(ShaderData&& shaderData) noexcept :
m_programId(std::move(shaderData.m_programId)),
	m_shaders(std::move(shaderData.m_shaders)),
	m_uniforms(std::move(shaderData.m_uniforms)),
	m_rendererUniforms(std::move(shaderData.m_rendererUniforms)),
	m_uniformNameToLocationMap(std::move(shaderData.m_uniformNameToLocationMap))
	//m_structUniforms(std::move(shaderData.m_structUniforms))
{
	DELOCUST_LOG_RENDERING("ShaderData move constructor called for program: ", m_programId, ". ");
	shaderData.m_programId = 0;
	shaderData.m_shaders.clear();
	shaderData.m_uniforms.clear();
	shaderData.m_rendererUniforms.clear();
	shaderData.m_uniformNameToLocationMap.clear();
	//shaderData.m_structUniforms.clear();
}

std::string rendering::ShaderData::LoadShaderData(const std::string& directoryPath, const std::string& fileName) const
{
	DELOCUST_LOG_RENDERING("Loading shader data from file \"", fileName, "\"");

	std::ifstream file(directoryPath + fileName);
	CHECK_CONDITION_EXIT_RENDERING(file.is_open(), Utility::Logging::ERR, "Unable to open shader file \"", fileName, "\". Check the path.");

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
			utility::string_utility::CutToTokens(line, tokens, ' ');
			//for (int i = 0; i < tokens.size(); ++i)
			//{
			//	std::cout << i << "):\t" << tokens[i] << std::endl;
			//}
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING(tokens.size() > 1, utility::logging::ERR, "Error while reading #include directive in the shader file \"", fileName, "\"");
			const auto includeFileName = tokens[1];
			//DEBUG_LOG_RENDERING("Tokens[1] = \"", tokens[1], "\". IncludeFileName=\"", includeFileName, "\"");
			//includeFileName = includeFileName.substr(1, includeFileName.length() - 2);
			//DEBUG_LOG_RENDERING("Loading an include shader file \"", includeFileName, "\"");

			const auto fragmentToAppend = LoadShaderData(directoryPath, includeFileName);
			output.append(fragmentToAppend + "\n");
		}
	}
	file.close();

	DELOCUST_LOG_RENDERING("Shader \"", fileName, "\" text loaded");
	/* ==================== Removing comments from the shader code begin ==================== */
	auto commentBegin = output.find(MULTI_LINE_COMMENT_BEGIN);
	while (commentBegin != std::string::npos)
	{
		const auto commentEnd = output.find(MULTI_LINE_COMMENT_END, commentBegin);
		output.erase(commentBegin, commentEnd - commentBegin + std::string(MULTI_LINE_COMMENT_END).length());
		commentBegin = output.find(MULTI_LINE_COMMENT_BEGIN);
	}
	commentBegin = output.find(SINGLE_LINE_COMMENT);
	while (commentBegin != std::string::npos)
	{
		const auto lineEnd = output.find("\n", commentBegin);
		output.erase(commentBegin, lineEnd - commentBegin);
		commentBegin = output.find(SINGLE_LINE_COMMENT);
	}
	/* ==================== Removing comments from the shader code end ==================== */

	return output;
}

bool rendering::ShaderData::Compile()
{
	bool compileSuccess = true;

	glLinkProgram(m_programId);
	int infoLogLength;
	if (CheckForErrors(m_programId, GL_LINK_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage;
		errorMessage.reserve(infoLogLength + 1);
		glGetProgramInfoLog(m_programId, infoLogLength, nullptr, &errorMessage[0]);
		ERROR_LOG_RENDERING("Error linking shader program ", m_programId, ":\n", &errorMessage[0], "\r");
	}

	glValidateProgram(m_programId);
	if (CheckForErrors(m_programId, GL_VALIDATE_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage;
		errorMessage.reserve(infoLogLength + 1);
		glGetProgramInfoLog(m_programId, infoLogLength, nullptr, &errorMessage[0]);
		ERROR_LOG_RENDERING("Error validating shader program ", m_programId, ":\n", &errorMessage[0], "\r");
	}

	CHECK_CONDITION_RENDERING(compileSuccess, Utility::Logging::EMERGENCY,
		"Shader program ", m_programId, " compilation error occurred. Investigate the problem.");
	return compileSuccess;
}

bool rendering::ShaderData::CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength)
{
	auto success = GL_FALSE;
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

void rendering::ShaderData::AddVertexShader(const std::string& vertexShaderText)
{
	AddProgram(vertexShaderText, GL_VERTEX_SHADER);
}

void rendering::ShaderData::AddGeometryShader(const std::string& geometryShaderText)
{
	AddProgram(geometryShaderText, GL_GEOMETRY_SHADER);
}

void rendering::ShaderData::AddFragmentShader(const std::string& fragmentShaderText)
{
	AddProgram(fragmentShaderText, GL_FRAGMENT_SHADER);
}

//void ShaderData::AddGeometryShaderFromFile(const std::string& fileName)
//{
//	std::string shaderText = LoadShader(fileName);
//	AddProgram(shaderText, GL_GEOMETRY_SHADER);
//}

void rendering::ShaderData::AddProgram(const std::string& shaderText, GLenum type)
{
	const auto shader = glCreateShader(type);

	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(shader != 0, utility::logging::EMERGENCY, "Error creating shader type: ", type);

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
		glGetShaderInfoLog(shader, infoLogLength, nullptr, &errorMessage[0]);
		ERROR_LOG_RENDERING("Error linking shader program: \"", &errorMessage[0], "\"");
		//return;
	}

	glAttachShader(m_programId, shader);
	m_shaders.push_back(shader);
}

void rendering::ShaderData::AddAllAttributes(const std::string& vertexShaderText)
{
	auto currentAttribLocation = 0;
	auto attributeLocation = vertexShaderText.find(ATTRIBUTE_KEYWORD);
	while (attributeLocation != std::string::npos)
	{
		const auto lastLineEnd = vertexShaderText.rfind("\n", attributeLocation);
		auto isCommented = false;
		if (lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = vertexShaderText.substr(lastLineEnd, attributeLocation - lastLineEnd);

			//Potential false positives are both in comments, and in macros.
			isCommented = potentialCommentSection.find("//") != std::string::npos || potentialCommentSection.find("#") != std::string::npos;
		}

		if (!isCommented)
		{
			const auto locationLocation = vertexShaderText.find(LOCATION_KEYWORD, lastLineEnd);
			if (locationLocation != std::string::npos)
			{
				const auto equalLocation = vertexShaderText.find("=", locationLocation);
				const auto begin = equalLocation + 1;
				auto end = vertexShaderText.find(";", begin);

				std::string attributeLine = vertexShaderText.substr(begin, end - begin);
				utility::string_utility::LeftTrim(attributeLine);

				end = attributeLine.find(")");
				const auto locationNumber = attributeLine.substr(0, end);
				currentAttribLocation = utility::string_utility::ToInt(locationNumber);
			}
			auto begin = attributeLocation + std::string(ATTRIBUTE_KEYWORD).length() + 1;
			const auto end = vertexShaderText.find(";", begin);

			std::string attributeLine = vertexShaderText.substr(begin, end - begin);

			begin = attributeLine.find(" ");
			std::string attributeName = attributeLine.substr(begin + 1);

			glBindAttribLocation(m_programId, currentAttribLocation, attributeName.c_str());
			++currentAttribLocation;
		}
		attributeLocation = vertexShaderText.find(ATTRIBUTE_KEYWORD, attributeLocation + std::string(ATTRIBUTE_KEYWORD).length());
	}
}

void rendering::ShaderData::AddShaderUniforms(const std::string& shaderText)
{
	std::vector<uniforms::UniformStructInfo> structInfos = FindUniformStructInfos(shaderText);
#ifdef DELOCUST_LOGGING_ENABLED
	for (auto itr = structInfos.begin(); itr != structInfos.end(); ++itr)
	{
		DELOCUST_LOG_RENDERING("struct.name = \"", itr->name, "\"");
		for (auto innerItr = itr->uniformInfos.begin(); innerItr != itr->uniformInfos.end(); ++innerItr)
		{
			DELOCUST_LOG_RENDERING("\t .memberName.name = \"", innerItr->name, "\"\t .memberName.uniformType = ", innerItr->type);
		}
	}
#endif

	auto uniformKeywordLocation = shaderText.find(UNIFORM_KEYWORD);
	while (uniformKeywordLocation != std::string::npos)
	{
		auto begin = uniformKeywordLocation + std::string(UNIFORM_KEYWORD).length();
		const auto end = shaderText.find(";", begin);
		const auto uniformLine = shaderText.substr(begin + 1, end - begin - 1);

		begin = uniformLine.find(" ");

		const std::string uniformName = uniformLine.substr(begin + 1);
		const uniforms::UniformType uniformType = uniforms::ConvertStringToUniformType(uniformLine.substr(0, begin).c_str());
		switch (uniformType)
		{
		case uniforms::VEC_2:
			AddUniform<uniforms::Vector2DUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case uniforms::VEC_3:
			AddUniform<uniforms::Vector3DUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case uniforms::VEC_4:
			AddUniform<uniforms::Vector4DUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case uniforms::MATRIX_4x4:
			AddUniform<uniforms::MatrixUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case uniforms::INT:
			AddUniform<uniforms::IntUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case uniforms::REAL:
			AddUniform<uniforms::RealUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case uniforms::SAMPLER_2D:
			AddUniform<uniforms::TextureUniform>(uniformName, FindUniformLocation(uniformName));
			break;
		case uniforms::SAMPLER_CUBE:
			AddUniform<uniforms::CubeTextureUniform>(uniformName, FindUniformLocation(uniformName));
			break;
			//case BASE_LIGHT:
		case uniforms::DIRECTIONAL_LIGHT:
			AddUniform<uniforms::DirectionalLightUniform>(uniformName, FindUniformLocation(uniformName + ".base.color"),
				FindUniformLocation(uniformName + ".base.intensity"), FindUniformLocation(uniformName + ".direction"));
			break;
		case uniforms::POINT_LIGHT:
			AddUniform<uniforms::PointLightUniform>(uniformName, FindUniformLocation(uniformName + ".base.color"),
				FindUniformLocation(uniformName + ".base.intensity"), FindUniformLocation(uniformName + ".attenuation.constant"),
				FindUniformLocation(uniformName + ".attenuation.linear"), FindUniformLocation(uniformName + ".attenuation.exponent"),
				FindUniformLocation(uniformName + ".position"), FindUniformLocation(uniformName + ".range"));
			break;
		case uniforms::SPOT_LIGHT:
			AddUniform<uniforms::SpotLightUniform>(uniformName, FindUniformLocation(uniformName + ".pointLight.base.color"),
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
	//for (auto uniformItr = m_uniforms.begin(); uniformItr != m_uniforms.end(); ++uniformItr)
	//{
	//	ERROR_LOG_RENDERING("Uniform \"", uniformItr->name, "\" of type ", uniformItr->type, " has location ", uniformItr->location);
	//}
}

template <class T, typename... Args>
void rendering::ShaderData::AddUniform(const std::string& uniformName, Args&&... args)
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

GLint rendering::ShaderData::FindUniformLocation(const std::string& uniformName)
{
	const auto location = glGetUniformLocation(m_programId, uniformName.c_str());
	CHECK_CONDITION_RENDERING(location != uniforms::UniformBase::INVALID_LOCATION, Utility::Logging::ERR, "Invalid location for the uniform \"", uniformName, "\".");
	m_uniformNameToLocationMap[uniformName] = location;
	return location;
}

std::vector<rendering::uniforms::UniformStructInfo> rendering::ShaderData::FindUniformStructInfos(const std::string& shaderText) const
{
	std::vector<uniforms::UniformStructInfo> result;

	auto structLocation = shaderText.find(STRUCT_KEY);
	DELOCUST_LOG_RENDERING("structLocation = ", structLocation);
	while (structLocation != std::string::npos)
	{
		structLocation += std::string(STRUCT_KEY).length() + 1; //Ignore the struct keyword and space

		const auto braceOpening = shaderText.find("{", structLocation);
		const auto braceClosing = shaderText.find("}", braceOpening);

		uniforms::UniformStructInfo newStruct;
		newStruct.name = FindUniformStructName(shaderText.substr(structLocation, braceOpening - structLocation));
		newStruct.uniformInfos = FindUniformStructComponents(shaderText.substr(braceOpening, braceClosing - braceOpening), result);

		result.push_back(newStruct);
		structLocation = shaderText.find(STRUCT_KEY, structLocation);
	}
	return result;
}

std::string rendering::ShaderData::FindUniformStructName(const std::string& structStartToOpeningBrace) const
{
	std::vector<std::string> tokens;
	utility::string_utility::CutToTokens(structStartToOpeningBrace, tokens, ' ');
	utility::string_utility::RightTrim(tokens[0]);
	//DELOCUST_LOG_RENDERING("tokens[0] = \"", tokens[0], "\"");
	return tokens[0];

	//return Util::Split(Util::Split(structStartToOpeningBrace, ' ')[0], '\n')[0];
}

std::vector<rendering::uniforms::UniformInfo> rendering::ShaderData::FindUniformStructComponents(const std::string& openingBraceToClosingBrace, const std::vector<uniforms::UniformStructInfo>& structUniformInfos) const
{
	constexpr auto charsToIgnoreCount = 4;
	constexpr std::array<char, charsToIgnoreCount> charsToIgnore = { ' ', '\n', '\t', '{' };
	constexpr char delimChars[] = { '\n', ';' };
	constexpr auto unsignedNegOne = static_cast<size_t>(-1);

	std::vector<uniforms::UniformInfo> result;
	std::vector<std::string> structLines;
	utility::string_utility::CutToTokens(openingBraceToClosingBrace, structLines, delimChars, 2);

	for (auto structLinesItr = structLines.begin(); structLinesItr != structLines.end(); ++structLinesItr)
	{
		//DEBUG_LOG_RENDERING("structLines[", i, "] = \"", structLines[i], "\"");
		if (structLinesItr->substr(0, 2) == SINGLE_LINE_COMMENT)
		{
			continue;
		}

		auto nameBegin = unsignedNegOne;
		auto nameEnd = unsignedNegOne;
		for (unsigned int j = 0; j < structLinesItr->length(); ++j)
		{
			auto isIgnoreableCharacter = false;
			for (size_t k = 0; k < charsToIgnore.size(); ++k)
			{
				if ((*structLinesItr)[j] == charsToIgnore[k])
				{
					isIgnoreableCharacter = true;
					break;
				}
			}
			if (nameBegin == unsignedNegOne && isIgnoreableCharacter == false)
			{
				nameBegin = j;
			}
			else if (nameBegin != unsignedNegOne && isIgnoreableCharacter)
			{
				nameEnd = j;
				break;
			}
		}
		if (nameBegin == unsignedNegOne || nameEnd == unsignedNegOne)
			continue;

		AddUniformInfos(structUniformInfos, result, structLinesItr->substr(nameEnd + 1), structLinesItr->substr(nameBegin, nameEnd - nameBegin));
	}
	return result;
}

void rendering::ShaderData::AddUniformInfos(const std::vector<uniforms::UniformStructInfo>& structUniformInfos, std::vector<uniforms::UniformInfo>& uniformInfos,
	const std::string& uniformName, const std::string& uniformTypeStr) const
{
	const uniforms::UniformType uniformType = uniforms::ConvertStringToUniformType(uniformTypeStr.c_str());
	DELOCUST_LOG_RENDERING("Uniform type = \"", uniformTypeStr, "\". Uniform type = ", uniformType);
	bool structFound = false;
	for (auto structUniformInfoItr = structUniformInfos.begin(); structUniformInfoItr != structUniformInfos.end(); ++structUniformInfoItr)
	{
		if (structUniformInfoItr->name == uniformTypeStr)
		{
			for (auto uniformInfoItr = structUniformInfoItr->uniformInfos.begin(); uniformInfoItr != structUniformInfoItr->uniformInfos.end(); ++uniformInfoItr)
			{
				AddUniformInfos(structUniformInfos, uniformInfos, uniformName + "." + uniformInfoItr->name, uniforms::ConvertUniformTypeToString(uniformInfoItr->type));
			}
			structFound = true;
		}
	}
	if (!structFound)
	{
		uniformInfos.emplace_back(uniformName, uniformType);
	}
}

bool rendering::ShaderData::IsUniformPresent(const std::string& uniformName, std::map<std::string, GLint>::const_iterator& itr) const
{
	itr = m_uniformNameToLocationMap.find(uniformName);
	CHECK_CONDITION_RENDERING(itr != m_uniformMap.end(), Utility::Logging::ERR, "Uniform \"", uniformName, "\" has not been found.");
	return (itr != m_uniformNameToLocationMap.end());
}

/* ==================== Shader class begin ==================== */
rendering::Shader::Shader(const std::string& directoryPath, const std::string& fileName) :
	m_shaderData(directoryPath, fileName),
	m_fileName(fileName)
#ifdef PROFILING_RENDERING_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Shader"))
#endif
{
	DELOCUST_LOG_RENDERING("Shader constructed based on filename \"", fileName, "\"");
}


rendering::Shader::~Shader(void)
{
	DELOCUST_LOG_RENDERING("Shader destructor called for file name: \"", m_fileName, "\". ");
}

//Rendering::Shader::Shader(const Shader& shader) :
//	m_shaderData(shader.m_shaderData),
//	m_fileName(shader.m_fileName)
//{
//
//}

rendering::Shader::Shader(Shader&& shader) noexcept :
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

void rendering::Shader::UpdateRendererUniforms(const Renderer* renderer) const
{
	START_PROFILING_RENDERING(false, "");
	CHECK_CONDITION_EXIT_RENDERING(renderer != nullptr, Utility::Logging::CRITICAL, "Cannot update uniforms. Rendering engine is NULL.");
	for (auto uniformItr = m_shaderData.GetRendererUniforms().begin(); uniformItr != m_shaderData.GetRendererUniforms().end(); ++uniformItr)
	{
		(*uniformItr)->Update(renderer, nullptr, nullptr);
	}
	STOP_PROFILING_RENDERING("");
}

void rendering::Shader::UpdateMaterialUniforms(const Material* material) const
{
}

void rendering::Shader::UpdateTransformUniforms(const math::Transform& transform) const
{
}

void rendering::Shader::UpdateUniforms(const math::Transform& transform, const Material* material, const Renderer* renderer) const
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
//	CHECK_CONDITION_EXIT_RENDERING(uniformLocation != uniforms::Uniform::INVALID_LOCATION, Utility::ERROR, "Could not find uniform \"", uniform, "\"");
//
//	uniforms.insert(std::pair<std::string, unsigned int>(uniform, uniformLocation));
//}

void rendering::Shader::SetUniformi(const std::string& name, int value) const
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

void rendering::Shader::SetUniformf(const std::string& name, math::Real value) const
{
	START_PROFILING_RENDERING(false, "");
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform float \"", name, "\":", value);
	//for (auto it = m_shaderData->GetUniformMap().begin(); it != m_shaderData->GetUniformMap().end(); ++it)
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

void rendering::Shader::SetUniformVector2D(const std::string& name, const math::Vector2D& vector) const
{
	START_PROFILING_RENDERING(false, "");
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 2D \"", name, "\":", vector);
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform2f(itr->second, vector.x, vector.y);
		//glUniform3fv(itr->second, 1, vector.At());
		// TODO: Check whether glUniform3fv(itr->second, 3, vector) is faster.
	}
	STOP_PROFILING_RENDERING("");
}

void rendering::Shader::SetUniformVector3D(const std::string& name, const math::Vector3D& vector) const
{
	START_PROFILING_RENDERING(false, "");
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 3D \"", name, "\":", vector);
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform3f(itr->second, vector.x, vector.y, vector.z);
		//glUniform3fv(itr->second, 1, vector.At());
		// TODO: Check whether glUniform3fv(itr->second, 3, vector) is faster.
	}
	STOP_PROFILING_RENDERING("");
}

void rendering::Shader::SetUniformVector4D(const std::string& name, const math::Vector4D& vector) const
{
	START_PROFILING_RENDERING(false, "");
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 4D \"", name, "\":", vector);
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform4f(itr->second, vector.x, vector.y, vector.z, vector.w);
		//glUniform4fv(itr->second, 4, vector.At());
		// TODO: Check whether glUniform4fv(itr->second, 4, vector) is faster.
	}
	STOP_PROFILING_RENDERING("");
}

void rendering::Shader::SetUniformVector4D(const std::string& name, math::Real x, math::Real y, math::Real z, math::Real w) const
{
	START_PROFILING_RENDERING(false, "");
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform vector 4D \"", name, "\": [", x, "; ", y, "; ", z, "; ", w, "].");
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(name, itr))
	{
		glUniform4f(itr->second, x, y, z, w);
	}
	STOP_PROFILING_RENDERING("");
}

void rendering::Shader::SetUniformColor(const std::string& uniformName, const Color& color) const
{
	START_PROFILING_RENDERING(false, "");
	DELOCUST_LOG_RENDERING("Shader \"", m_fileName, "\": setting uniform color \"", uniformName, "\":\n", color);
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData.IsUniformPresent(uniformName, itr))
	{
		glUniform4f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		//glUniform3f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue());
	}
	STOP_PROFILING_RENDERING("");
}

void rendering::Shader::SetUniformMatrix(const std::string& name, const math::Matrix4D& matrix) const
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

