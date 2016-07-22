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

std::string Rendering::Uniforms::ConvertUniformTypeToString(UniformType uniformType)
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
	case Rendering::Uniforms::INT:
		return "int";
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
		EMERGENCY_LOG_RENDERING("Uniform type ", uniformType, " not supported by the rendering engine.");
		exit(EXIT_FAILURE);
	}
}

/* static */ const std::string Rendering::ShaderData::UNIFORM_KEYWORD = "uniform";
/* static */ const std::string Rendering::ShaderData::ATTRIBUTE_KEYWORD = "attribute";
/* static */ const std::string Rendering::ShaderData::LOCATION_KEYWORD = "location";
/* static */ const std::string Rendering::ShaderData::SINGLE_LINE_COMMENT = "//";
/* static */ const std::string Rendering::ShaderData::MULTI_LINE_COMMENT_BEGIN = "/*";
/* static */ const std::string Rendering::ShaderData::MULTI_LINE_COMMENT_END = "*/";

/* static */ std::map<std::string, std::shared_ptr<Rendering::ShaderData>> Rendering::Shader::shaderResourceMap;

Rendering::ShaderData::ShaderData(const std::string& fileName) :
	m_programID(glCreateProgram())
{
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_programID != 0, Utility::Logging::CRITICAL, "Error while creating shader program. Program ID is still 0.");

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
		DEBUG_LOG_RENDERING("Geometry shader found in file \"", fileName, "\"");
		AddGeometryShader(geometryShaderText);
	}
	AddFragmentShader(fragmentShaderText);

	AddAllAttributes(vertexShaderText);

	if (!Compile())
	{
		CRITICAL_LOG_RENDERING("Error while compiling shader program ", m_programID, " for shader file \"", fileName, "\"");
		exit(EXIT_FAILURE);
	}
	else
	{
		NOTICE_LOG_RENDERING("Shader program ", m_programID, " for shader file \"", fileName, "\" has been successfully compiled.");
	}
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
}

Rendering::ShaderData::~ShaderData()
{
	DEBUG_LOG_RENDERING("Destroying shader data for shader program: ", m_programID);
	for (std::vector<GLuint>::iterator shaderItr = m_shaders.begin(); shaderItr != m_shaders.end(); ++shaderItr)
	{
		glDetachShader(m_programID, *shaderItr);
		glDeleteShader(*shaderItr);
	}
	glDeleteProgram(m_programID);
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
			size_t begin = attributeLocation + ATTRIBUTE_KEYWORD.length() + 1;
			size_t end = vertexShaderText.find(";", begin);

			std::string attributeLine = vertexShaderText.substr(begin, end - begin);

			begin = attributeLine.find(" ");
			std::string attributeName = attributeLine.substr(begin + 1);

			glBindAttribLocation(m_programID, currentAttribLocation, attributeName.c_str());
			++currentAttribLocation;
		}
		attributeLocation = vertexShaderText.find(ATTRIBUTE_KEYWORD, attributeLocation + ATTRIBUTE_KEYWORD.length());
	}
}

void Rendering::ShaderData::AddShaderUniforms(const std::string& shaderText)
{
	std::vector<Uniforms::UniformStruct> structs = FindUniformStructs(shaderText);
#ifdef DEBUG_LOGGING_ENABLED
	for (std::vector<Uniforms::UniformStruct>::const_iterator itr = structs.begin(); itr != structs.end(); ++itr)
	{
		DEBUG_LOG_RENDERING("struct.name = \"", itr->name, "\"");
#ifdef DELOCUST_LOGGING_ENABLED
		for (std::vector<Uniforms::Uniform>::const_iterator innerItr = itr->memberNames.begin(); innerItr != itr->memberNames.end(); ++innerItr)
		{
			DELOCUST_LOG_RENDERING("\t .memberName.name = \"", innerItr->name, "\"\t .memberName.uniformType = ", innerItr->uniformType);
		}
#endif
	}
#endif

	size_t uniformLocation = shaderText.find(UNIFORM_KEYWORD);
	while (uniformLocation != std::string::npos)
	{
		size_t begin = uniformLocation + UNIFORM_KEYWORD.length();
		size_t end = shaderText.find(";", begin);
		std::string uniformLine = shaderText.substr(begin + 1, end - begin - 1);

		begin = uniformLine.find(" ");

		const std::string uniformName = uniformLine.substr(begin + 1);
		const Uniforms::UniformType uniformType = Uniforms::ConvertStringToUniformType(uniformLine.substr(0, begin));
		m_uniforms.push_back(Uniforms::Uniform(uniformName, uniformType));
		AddUniform(uniformName, uniformType, structs);
		uniformLocation = shaderText.find(UNIFORM_KEYWORD, uniformLocation + UNIFORM_KEYWORD.length());
	}
#ifdef DEBUG_LOGGING_ENABLED
	for (std::map<std::string, GLint>::const_iterator it = m_uniformMap.begin(); it != m_uniformMap.end(); ++it)
	{
		DEBUG_LOG_RENDERING("Uniform map <\"", it->first, "\", ", it->second, ">");
	}
#endif
}

void Rendering::ShaderData::AddUniform(const std::string& uniformName, Uniforms::UniformType uniformType, const std::vector<Uniforms::UniformStruct>& structs)
{
	DEBUG_LOG_RENDERING("Adding uniform \"", uniformName, "\" of type: ", uniformType);

	const std::string uniformTypeStr = Uniforms::ConvertUniformTypeToString(uniformType);
	for (std::vector<Uniforms::UniformStruct>::const_iterator structItr = structs.begin(); structItr != structs.end(); ++structItr)
	{
		if (structItr->name == uniformTypeStr)
		{
			for (std::vector<Uniforms::Uniform>::const_iterator structMemberNameItr = structItr->memberNames.begin(); structMemberNameItr != structItr->memberNames.end(); ++structMemberNameItr)
			{
				AddUniform(uniformName + "." + structMemberNameItr->name, structMemberNameItr->uniformType, structs);
			}
			return;
		}
	}

	GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
	CHECK_CONDITION_EXIT_RENDERING(location != INVALID_VALUE, EMERGENCY, "Invalid value of the location (", location, ") for the uniform \"", uniformName, "\"");
	DELOCUST_LOG_RENDERING("Uniform \"", uniformName, "\" has a location value: ", location);
	m_uniformMap.insert(std::pair<std::string, GLint>(uniformName, location));
}

std::vector<Rendering::Uniforms::UniformStruct> Rendering::ShaderData::FindUniformStructs(const std::string& shaderText) const
{
	const std::string STRUCT_KEY = "struct";
	std::vector<Uniforms::UniformStruct> result;

	size_t structLocation = shaderText.find(STRUCT_KEY);
	DELOCUST_LOG_RENDERING("structLocation = ", structLocation, "; std::string::npos = ", std::string::npos);
	while (structLocation != std::string::npos)
	{
		structLocation += STRUCT_KEY.length() + 1; //Ignore the struct keyword and space

		size_t braceOpening = shaderText.find("{", structLocation);
		size_t braceClosing = shaderText.find("}", braceOpening);

		Uniforms::UniformStruct newStruct;
		newStruct.name = FindUniformStructName(shaderText.substr(structLocation, braceOpening - structLocation));
		newStruct.memberNames = FindUniformStructComponents(shaderText.substr(braceOpening, braceClosing - braceOpening));

		result.push_back(newStruct);
		structLocation = shaderText.find(STRUCT_KEY, structLocation);
	}
	return result;
}

std::string Rendering::ShaderData::FindUniformStructName(const std::string& structStartToOpeningBrace) const
{
	std::vector<std::string> tokens;
	Utility::StringUtility::CutToTokens(structStartToOpeningBrace, tokens, ' ');
	std::string result;
	Utility::StringUtility::RightTrim(tokens[0]);
	//DELOCUST_LOG_RENDERING("tokens[0] = \"", tokens[0], "\"");
	return tokens[0];

	//return Util::Split(Util::Split(structStartToOpeningBrace, ' ')[0], '\n')[0];
}

std::vector<Rendering::Uniforms::Uniform> Rendering::ShaderData::FindUniformStructComponents(const std::string& openingBraceToClosingBrace) const
{
	const int CHARS_TO_IGNORE_COUNT = 4;
	const char charsToIgnore[CHARS_TO_IGNORE_COUNT] = { ' ', '\n', '\t', '{' };
	const size_t UNSIGNED_NEG_ONE = (size_t)-1;

	std::vector<Uniforms::Uniform> result;
	std::vector<std::string> structLines;
	const char delimChars[] = { '\n', ';' };
	Utility::StringUtility::CutToTokens(openingBraceToClosingBrace, structLines, delimChars, 2);

	for (std::vector<std::string>::const_iterator structLinesItr = structLines.begin(); structLinesItr != structLines.end(); ++structLinesItr)
	{
		//DEBUG_LOG_RENDERING("structLines[", i, "] = \"", structLines[i], "\"");
		if (structLinesItr->substr(0, 2) == "//")
		{
			continue;
		}

		size_t nameBegin = UNSIGNED_NEG_ONE;
		size_t nameEnd = UNSIGNED_NEG_ONE;
		for (unsigned int j = 0; j < structLinesItr->length(); ++j)
		{
			bool isIgnoreableCharacter = false;
			for (unsigned int k = 0; k < CHARS_TO_IGNORE_COUNT; ++k)
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

		result.emplace_back(structLinesItr->substr(nameEnd + 1), Uniforms::ConvertStringToUniformType(structLinesItr->substr(nameBegin, nameEnd - nameBegin)));
	}
	return result;
}

bool Rendering::ShaderData::IsUniformPresent(const std::string& uniformName, std::map<std::string, GLint>::const_iterator& itr) const
{
	itr = m_uniformMap.find(uniformName);
	CHECK_CONDITION_RENDERING(itr != uniformMap.end(), ERR, "Uniform \"", uniformName, "\" has not been found.");
	return (itr != m_uniformMap.end());
}

/* ==================== Shader class begin ==================== */
Rendering::Shader::Shader(const std::string& fileName) :
	m_shaderData(nullptr),
	m_fileName(fileName)
{
	DEBUG_LOG_RENDERING("Shader constructed based on filename \"", fileName, "\"");
	std::map<std::string, std::shared_ptr<ShaderData>>::const_iterator itr = shaderResourceMap.find(fileName);
	if (itr == shaderResourceMap.end())
	{
		m_shaderData = std::make_shared<ShaderData>(fileName);
		shaderResourceMap.insert(std::make_pair(fileName, m_shaderData));
	}
	else
	{
		INFO_LOG_RENDERING("Shader data \"", fileName, "\" already present in the resource manager");
		m_shaderData = itr->second;
	}
}


Rendering::Shader::~Shader(void)
{
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
{
	//shader.m_shaderData = nullptr;
	//shader.m_fileName.clear();
}

void Rendering::Shader::Bind() const
{
	CHECK_CONDITION_EXIT_RENDERING(m_shaderData != NULL, Utility::CRITICAL, "Cannot bind the shader. Shader data is NULL.");
	glUseProgram(m_shaderData->GetProgram());
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
	CHECK_CONDITION_EXIT_RENDERING(renderer != NULL, CRITICAL, "Cannot update uniforms. Rendering engine is NULL.");
	CHECK_CONDITION_EXIT_RENDERING(m_shaderData != NULL, CRITICAL, "Cannot update uniforms. Shader data is NULL.");
	CHECK_CONDITION_EXIT_RENDERING(m_shaderData->GetUniformNames().size() == m_shaderData->GetUniformTypes().size(), ERR,
		"Shader data is incorrect. There are ", m_shaderData->GetUniformNames().size(), " uniform names and ", m_shaderData->GetUniformTypes().size(), " uniform types");

	Math::Matrix4D worldMatrix = transform.GetTransformation();
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
	for (std::vector<Uniforms::Uniform>::const_iterator uniformItr = m_shaderData->GetUniforms().begin(); uniformItr != m_shaderData->GetUniforms().end(); ++uniformItr)
	{
		const std::string& uniformName = uniformItr->name;
		const Uniforms::UniformType& uniformType = uniformItr->uniformType;
		DEBUG_LOG_RENDERING("Updating uniform with name = \"", uniformName, "\" and type = ", uniformType, ".");

		const std::string uniformNamePrefix = uniformName.substr(0, 2);

		if (uniformNamePrefix == "R_")
		{
			std::string unprefixedName = uniformName.substr(2, uniformName.length());
			if (unprefixedName == "lightMatrix")
			{
				//CRITICAL_LOG_RENDERING("Renderer->GetLightMatrix() = \"", renderer->GetLightMatrix().ToString(), "\"");
				//CRITICAL_LOG_RENDERING("WorldMatrix = \"", worldMatrix.ToString(), "\"");
				SetUniformMatrix(uniformName, renderer->GetLightMatrix() * worldMatrix); // FIXME: Check matrix multiplication
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
					CHECK_CONDITION_EXIT_RENDERING(texture != NULL, CRITICAL, "Updating uniforms operation failed. Rendering engine texture \"", unprefixedName, "\" is NULL.");
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
			else if (uniformType == Uniforms::VEC_2D)
			{
				SetUniformVector2D(uniformName, renderer->GetVec2D(unprefixedName));
			}
			else if (uniformType == Uniforms::REAL)
			{
				SetUniformf(uniformName, renderer->GetReal(unprefixedName));
			}
			else if (uniformType == Uniforms::DIRECTIONAL_LIGHT)
			{
				// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
				//Lighting::DirectionalLight* directionalLight = dynamic_cast<Lighting::DirectionalLight*>(renderer->GetCurrentLight());
				const Lighting::BaseLight* directionalLight = renderer->GetCurrentLight();
				CHECK_CONDITION_EXIT_RENDERING(directionalLight != NULL, ERR, "Cannot update directional light uniform. Directional light instance is NULL.");
				SetUniformDirectionalLight(uniformName, *directionalLight);
			}
			else if (uniformType == Uniforms::POINT_LIGHT)
			{
				// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
				//Lighting::PointLight* pointLight = dynamic_cast<Lighting::PointLight*>(renderer->GetCurrentLight());
				const Lighting::PointLight* pointLight = renderer->GetCurrentPointLight();
				CHECK_CONDITION_EXIT_RENDERING(pointLight != NULL, ERR, "Cannot update point light uniform. Point light instance is NULL.");
				SetUniformPointLight(uniformName, *pointLight);
			}
			else if (uniformType == Uniforms::SPOT_LIGHT)
			{
				// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
				const Lighting::SpotLight* spotLight = dynamic_cast<const Lighting::SpotLight*>(renderer->GetCurrentLight());
				//const Lighting::SpotLight* spotLight = renderer->GetSpotLight();
				CHECK_CONDITION_EXIT_RENDERING(spotLight != NULL, ERR, "Cannot update spot light uniform. Spot light instance is NULL.");
				SetUniformSpotLight(uniformName, *spotLight);
			}
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
			CHECK_CONDITION_EXIT_RENDERING(texture != NULL, CRITICAL, "Updating uniforms operation failed. Material texture \"", uniformName, "\" is NULL.");
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
			else if (uniformName == "T_scale")
			{
				SetUniformf(uniformName, transform.GetScale());
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
				SetUniformVector3D(uniformName, renderer->GetCurrentCamera().GetTransform().GetTransformedPos());
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
				ERROR_LOG_RENDERING("The uniform \"", uniformName, "\" of type ", uniformType, " is not supported by the Material class");
			}
		}
	}
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
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform1i(itr->second, value);
	}
	else
	{
		EMERGENCY_LOG_RENDERING("Uniform \"", name, "\" is not defined in the shader \"", m_fileName, "\"");
	}
}

void Rendering::Shader::SetUniformf(const std::string& name, Math::Real value) const
{
	//for (std::map<std::string, unsigned int>::const_iterator it = m_shaderData->GetUniformMap().begin(); it != m_shaderData->GetUniformMap().end(); ++it)
	//{
	//	DEBUG_LOG_RENDERING("Uniform map <\"", it->first, "\", ", it->second, ">");
	//}
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform1f(itr->second, value);
	}
}

void Rendering::Shader::SetUniformVector2D(const std::string& name, const Math::Vector2D& vector) const
{
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform2f(itr->second, vector.GetX(), vector.GetY());
		//glUniform3fv(itr->second, 1, vector.At());
		// TODO: Check whether glUniform3fv(itr->second, 3, vector) is faster.
	}
}

void Rendering::Shader::SetUniformVector3D(const std::string& name, const Math::Vector3D& vector) const
{
	std::map<std::string, GLint>::const_iterator itr;
	//if (name.compare("R_directionalLight.direction") == 0)
	//{
	//	CRITICAL_LOG_RENDERING("Directional light direction = \"", vector.ToString(), "\"");
	//}
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform3f(itr->second, vector.GetX(), vector.GetY(), vector.GetZ());
		//glUniform3fv(itr->second, 1, vector.At());
		// TODO: Check whether glUniform3fv(itr->second, 3, vector) is faster.
	}
}

void Rendering::Shader::SetUniformVector4D(const std::string& name, const Math::Vector4D& vector) const
{
	std::map<std::string, GLint>::const_iterator itr;
	//if (name.compare("R_directionalLight.direction") == 0)
	//{
	//	CRITICAL_LOG_RENDERING("Directional light direction = \"", vector.ToString(), "\"");
	//}
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform4f(itr->second, vector.GetX(), vector.GetY(), vector.GetZ(), vector.GetW());
		//glUniform4fv(itr->second, 4, vector.At());
		// TODO: Check whether glUniform4fv(itr->second, 4, vector) is faster.
	}
}

void Rendering::Shader::SetUniformVector4D(const std::string& name, Math::Real x, Math::Real y, Math::Real z, Math::Real w) const
{
	std::map<std::string, GLint>::const_iterator itr;
	//if (name.compare("R_directionalLight.direction") == 0)
	//{
	//	CRITICAL_LOG_RENDERING("Directional light direction = \"", vector.ToString(), "\"");
	//}
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform4f(itr->second, x, y, z, w);
	}
}

void Rendering::Shader::SetUniformColor(const std::string& uniformName, const Color& color) const
{
	std::map<std::string, GLint>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(uniformName, itr))
	{
		glUniform4f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		//glUniform3f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue());
	}
}

void Rendering::Shader::SetUniformMatrix(const std::string& name, const Math::Matrix4D& matrix) const
{
	//DEBUG_LOG_RENDERING("Setting uniform matrix \"", name, "\":\n", matrix.ToString());
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
void Rendering::Shader::SetUniformDirectionalLight(const std::string& uniformName, const Lighting::BaseLight& directionalLight) const
{
	DELOCUST_LOG_RENDERING("Directional light:\n\tIntensity = ", directionalLight.GetIntensity(), "\n\tColor = ", directionalLight.GetColor().ToString(),
		"\n\tDirection = ", directionalLight.GetTransform().GetTransformedRot().GetForward().ToString());
	SetUniformVector3D(uniformName + ".direction", directionalLight.GetTransform().GetTransformedRot().GetForward());
	SetUniformColor(uniformName + ".base.color", directionalLight.GetColor());
	SetUniformf(uniformName + ".base.intensity", directionalLight.GetIntensity());
}

void Rendering::Shader::SetUniformPointLight(const std::string& uniformName, const Lighting::PointLight& pointLight) const
{
	SetUniformColor(uniformName + ".base.color", pointLight.GetColor());
	SetUniformf(uniformName + ".base.intensity", pointLight.GetIntensity());
	SetUniformf(uniformName + ".attenuation.constant", pointLight.GetAttenuation().GetConstant());
	SetUniformf(uniformName + ".attenuation.linear", pointLight.GetAttenuation().GetLinear());
	SetUniformf(uniformName + ".attenuation.exponent", pointLight.GetAttenuation().GetExponent());
	SetUniformVector3D(uniformName + ".position", pointLight.GetTransform().GetTransformedPos());
	SetUniformf(uniformName + ".range", pointLight.GetRange());
}

void Rendering::Shader::SetUniformSpotLight(const std::string& uniformName, const Lighting::SpotLight& spotLight) const
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