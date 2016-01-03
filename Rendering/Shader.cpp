#include "StdAfx.h"
#include "Shader.h"

#include "CoreEngine.h"
#include "Utility\Utility.h"
#include "Utility\ILogger.h"

#include <fstream>

using namespace Rendering;
using namespace Utility;
using namespace Math;
using namespace std;

/* static */ std::map<std::string, ShaderData*> Shader::shaderResourceMap;

ShaderData::ShaderData(const std::string& fileName) :
	programID(0)
{
	programID = glCreateProgram();

	if (programID == 0)
	{
		CRITICAL_LOG("Error while creating shader program");
		// TODO: Throw an exception
		exit(EXIT_FAILURE);
	}

	std::string shaderText = LoadShaderData(fileName + ".glsl");
	bool geometryShaderPresent = (shaderText.find("defined(GS_BUILD)") != std::string::npos); // geometry shader found

	INFO_LOG("Shader \"%s\" text loaded", fileName.c_str());

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

	const std::string attributeKeyword = "attribute"; // TODO: What about geometry shader? Check if we should analyze geometry shader too to find all attributes
	AddAllAttributes(vertexShaderText, attributeKeyword);

	if (! Compile())
	{
		CRITICAL_LOG("Error while compiling shader program %d", programID);
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
	DEBUG_LOG("Destroying shader data for shader program %d", programID);
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
	INFO_LOG("Loading shader data from file \"%s\"", name.c_str());

	ifstream file((CoreEngine::GetCoreEngine()->GetShadersDirectory() + fileName).c_str());
	if (!file.is_open())
	{
		ERROR_LOG("Unable to open shader file \"%s\". Check the path.", name.c_str());
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
			//DEBUG_LOG("#Include directive found in Line = \"%s\"", line.c_str());

			std::vector<std::string> tokens;
			CutToTokens(line, tokens, ' ');
			//for (int i = 0; i < tokens.size(); ++i)
			//{
			//	std::cout << i << "):\t" << tokens[i] << std::endl;
			//}
			ASSERT(tokens.size() > 1);
			if (tokens.size() <= 1)
			{
				ERROR_LOG("Error while reading #include directive in the shader file \"%s\"", name.c_str());
				continue;
			}
			std::string includeFileName = tokens[1];
			//DEBUG_LOG("Tokens[1] = \"%s\". IncludeFileName=\"%s\"", tokens[1].c_str(), includeFileName.c_str());
			includeFileName = includeFileName.substr(1, includeFileName.length() - 2);
			//DEBUG_LOG("Loading an include shader file \"%s\"", includeFileName.c_str());

			string fragmentToAppend = LoadShaderData(includeFileName);
			output.append(fragmentToAppend + "\n");
		}
	}
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
		ERROR_LOG("Error linking shader program %d:\n%s\r", programID, &errorMessage[0]);
	}

	glValidateProgram(programID);
	if (CheckForErrors(programID, GL_VALIDATE_STATUS, true, infoLogLength))
	{
		compileSuccess = false;
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
		ERROR_LOG("Error validating shader program %d:\n%s\r", programID, &errorMessage[0]);
	}

	if (!compileSuccess)
	{
		ERROR_LOG("Shader program %d compilation error occurred. Investigate the problem.", programID);
	}
	else
	{
		NOTICE_LOG("Shader program %d compiled successfully", programID);
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
	int shader = glCreateShader(type);

	if (shader == 0)
	{
		EMERGENCY_LOG("Error creating shader type %d", type);
		return;
	}

	const GLchar* p[1];
	p[0] = shaderText.c_str();
	GLint lengths[1] = { shaderText.length() };
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
		DEBUG_LOG("structs[%d].name = \"%s\"", temp, itr->name.c_str());
		int innerTemp = 0;
		for (std::vector<TypedData>::const_iterator innerItr = itr->memberNames.begin(); innerItr != itr->memberNames.end(); ++innerItr)
		{
			DELOCUST_LOG("\t .memberName[%d].name = \"%s\"\t .memberName[%d].type = \"%s\"", innerTemp, innerItr->name.c_str(), innerTemp, innerItr->type.c_str());
			++innerTemp;
		}
		++temp;
	}
	
	size_t uniformLocation = shaderText.find(UNIFORM_KEY);
	while(uniformLocation != std::string::npos)
	{
		/**
		 * TODO: When uniform is declared as follows:
		 * "uniform someType uniformName; // this is a comment for uniform"
		 * then errors with ignoring successive uniforms occurs. This is critical to check and fix!!!
		 * When the comment is removed all comes back to normal.
		 * It seems there is a problem with the isCommented variable in this function.
		 */
		bool isCommented = false;
		size_t lastLineEnd = shaderText.rfind(";", uniformLocation);
		//INFO_LOG("Uniform location in shader text = %d; lastLineEnd = %d; std::string::npos = %d", uniformLocation, lastLineEnd, std::string::npos);
		if(lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = shaderText.substr(lastLineEnd, uniformLocation - lastLineEnd);
			size_t commentFind = potentialCommentSection.find("//");
			isCommented = (commentFind != std::string::npos);
			//INFO_LOG("potentialCommentSection = \"%s\"; find("") = %d", potentialCommentSection.c_str(), commentFind);
		}
		if(!isCommented)
		{
			size_t begin = uniformLocation + UNIFORM_KEY.length();
			size_t end = shaderText.find(";", begin);
			std::string uniformLine = shaderText.substr(begin + 1, end-begin - 1);
			
			begin = uniformLine.find(" ");
			std::string uniformName = uniformLine.substr(begin + 1);
			std::string uniformType = uniformLine.substr(0, begin);
			
			uniformNames.push_back(uniformName);
			uniformTypes.push_back(uniformType);
			AddUniform(uniformName, uniformType, structs);
		}
		else
		{
			DEBUG_LOG("Uniform is commented out");
		}
		uniformLocation = shaderText.find(UNIFORM_KEY, uniformLocation + UNIFORM_KEY.length());
	}
	for (std::map<std::string, unsigned int>::const_iterator it = uniformMap.begin(); it != uniformMap.end(); ++it)
	{
		DEBUG_LOG("Uniform map <\"%s\", %d>", (it->first).c_str(), it->second);
	}
}

void ShaderData::AddUniform(const std::string& uniformName, const std::string& uniformType, const std::vector<UniformStruct>& structs)
{
	DEBUG_LOG("Adding uniform \"%s\" of type \"%s\"", uniformName.c_str(), uniformType.c_str());
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
		//INFO_LOG("addThis == false");
		return;
	}

	unsigned int location = glGetUniformLocation(programID, uniformName.c_str());
	CHECK_CONDITION_EXIT(location != INVALID_VALUE, Emergency, "Invalid value of the location (%d) for the uniform \"%s\"", location, uniformName.c_str());
	DELOCUST_LOG("Uniform \"%s\" has a location value of %d", uniformName.c_str(), location);
	uniformMap.insert(std::pair<std::string, unsigned int>(uniformName, location));
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
	//std::string result;
	//StringUtility::RightTrim(tokens[0], result);
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
	CHECK_CONDITION(itr != uniformMap.end(), Error, "Uniform \"%s\" has not been found.", uniformName.c_str());
	return (itr != uniformMap.end());
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

void Shader::UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer) const
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
	/* ==================== SOLUTION #2 end ==================== */
	/* ==================== SOLUTION #3 begin ==================== */
	//Matrix4D projectedMatrix = renderer->GetCurrentCamera().GetViewProjection() * worldMatrix; // TODO: Pass camera object as parameter instead of using GetCurrentCamera() function.
	/* ==================== SOLUTION #3 end ==================== */
	/* ==================== SOLUTION #4 begin ==================== */
	//Matrix4D projectedMatrix;
	//renderer->GetCurrentCamera().GetViewProjection(projectedMatrix); // TODO: Pass camera object as parameter instead of using GetCurrentCamera() function.
	//projectedMatrix *= worldMatrix;
	/* ==================== SOLUTION #4 end ==================== */
	for (unsigned int i = 0; i < m_shaderData->GetUniformNames().size(); ++i)
	{
		std::string uniformName = m_shaderData->GetUniformNames()[i];
		DEBUG_LOG("uniformName = \"%s\"", uniformName.c_str());
		std::string uniformType = m_shaderData->GetUniformTypes()[i];

		const std::string uniformSubstr = uniformName.substr(0, 2);

		if (uniformSubstr == "R_")
		{
			std::string unprefixedName = uniformName.substr(2, uniformName.length());
			if (unprefixedName == "lightMatrix")
			{
				SetUniformMatrix(uniformName, renderer->GetLightMatrix() * worldMatrix);
			}
			else if ((uniformType == "sampler2D") || (uniformType == "samplerCube"))
			{
				unsigned int samplerSlot = renderer->GetSamplerSlot(unprefixedName);
				if (unprefixedName == "cubeShadowMap")
				{
					renderer->BindCubeShadowMap(samplerSlot);
				}
				else
				{
					Texture* texture = renderer->GetTexture(unprefixedName);
					CHECK_CONDITION_EXIT(texture != NULL, Critical, "Updating uniforms operation failed. Rendering engine texture \"%s\" is NULL.", unprefixedName.c_str());
					texture->Bind(samplerSlot);
				}
				SetUniformi(uniformName, samplerSlot);
			}
			//else if (uniformType == "samplerCubeShadow")
			//{
			//	unsigned int samplerSlot = renderer->GetSamplerSlot(unprefixedName);
			//	renderer->BindCubeShadowMap(samplerSlot);
			//	SetUniformi(uniformName, samplerSlot);
			//}
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
				// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
				//Lighting::DirectionalLight* directionalLight = dynamic_cast<Lighting::DirectionalLight*>(renderer->GetCurrentLight());
				const Lighting::BaseLight* directionalLight = renderer->GetCurrentLight();
				CHECK_CONDITION_EXIT(directionalLight != NULL, Error, "Cannot update directional light uniform. Directional light instance is NULL.");
				SetUniformDirectionalLight(uniformName, *directionalLight);
			}
			else if(uniformType == "PointLight")
			{
				// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
				//Lighting::PointLight* pointLight = dynamic_cast<Lighting::PointLight*>(renderer->GetCurrentLight());
				const Lighting::PointLight* pointLight = renderer->GetPointLight();
				CHECK_CONDITION_EXIT(pointLight != NULL, Error, "Cannot update point light uniform. Point light instance is NULL.");
				SetUniformPointLight(uniformName, *pointLight);
			}
			else if(uniformType == "SpotLight")
			{
				// TODO: Avoid using dynamic_casts in the frequently used code. See e.g. http://www.nerdblog.com/2006/12/how-slow-is-dynamiccast.html
				const Lighting::SpotLight* spotLight = dynamic_cast<const Lighting::SpotLight*>(renderer->GetCurrentLight());
				//const Lighting::SpotLight* spotLight = renderer->GetSpotLight();
				CHECK_CONDITION_EXIT(spotLight != NULL, Error, "Cannot update spot light uniform. Spot light instance is NULL.");
				SetUniformSpotLight(uniformName, *spotLight);
			}
			else
			{
				renderer->UpdateUniformStruct(transform, material, *this, uniformName, uniformType);
			}
		}
		else if ((uniformType == "sampler2D") || (uniformType == "samplerCube"))
		{
			unsigned int samplerSlot = renderer->GetSamplerSlot(uniformName);
			Texture* texture = material.GetTexture(uniformName);
			CHECK_CONDITION_EXIT(texture != NULL, Critical, "Updating uniforms operation failed. Material texture \"%s\" is NULL.", uniformName.c_str());
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
				ERROR_LOG("Invalid transform uniform \"%s\"", uniformName.c_str());
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
				ERROR_LOG("Invalid camera uniform \"%s\"", uniformName.c_str());
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
				ERROR_LOG("The uniform \"%s\" of type \"%s\" is not supported by the Material class", uniformName.c_str(), uniformType.c_str());
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
	std::map<std::string, unsigned int>::const_iterator itr;
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
	std::map<std::string, unsigned int>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform1f(itr->second, value);
	}
}

void Shader::SetUniformVector3D(const std::string& name, const Math::Vector3D& vector) const
{
	std::map<std::string, unsigned int>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(name, itr))
	{
		glUniform3f(itr->second, vector.GetX(), vector.GetY(), vector.GetZ());
		// TODO: Check whether glUniform3fv(itr->second, 3, vector) is faster.
	}
}

void Shader::SetUniformColor(const std::string& uniformName, const Color& color) const
{
	std::map<std::string, unsigned int>::const_iterator itr;
	if (m_shaderData->IsUniformPresent(uniformName, itr))
	{
		glUniform4f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		//glUniform3f(itr->second, color.GetRed(), color.GetGreen(), color.GetBlue());
	}
}

void Shader::SetUniformMatrix(const std::string& name, const Math::Matrix4D& matrix) const
{
	std::map<std::string, unsigned int>::const_iterator itr;
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