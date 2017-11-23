#ifndef __RENDERING_SHADER_H__
#define __RENDERING_SHADER_H__

#include "Rendering.h"
#include "Material.h"
#include "Uniform.h"
#include "Color.h"

#include "Math/Transform.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#ifdef PROFILING_RENDERING_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

#include <map>
#include <vector>
#include <string>

namespace rendering
{
	class Renderer;

	class ShaderData
	{
		/* ==================== Static variables and functions begin ==================== */
		static constexpr char* ATTRIBUTE_KEYWORD = "attribute";
		static constexpr char* LOCATION_KEYWORD = "location";
		static constexpr char* UNIFORM_KEYWORD = "uniform";
		static constexpr char* STRUCT_KEY = "struct";
		static constexpr char* SINGLE_LINE_COMMENT = "//";
		static constexpr char* MULTI_LINE_COMMENT_BEGIN = "/*";
		static constexpr char* MULTI_LINE_COMMENT_END = "*/";
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Shader data constructor. </summary>
		/// <param name="directoryPath"> The system directory where the shader file is located. </param>
		/// <param name="fileName">
		/// The name of the file located in the system directory <paramref name="directoryPath"/>
		/// and containing the GLSL shader code.
		/// </param>
		ShaderData(const std::string& directoryPath, const std::string& fileName);

		/// <summary> Shader data destructor. </summary>
		~ShaderData();

		/// <summary> Shader data copy constructor. </summary>
		/// <param name="shaderData"> The reference to shader data to copy construct from. </param>
		ShaderData(const ShaderData& shaderData) = delete;

		/// <summary> Shader data move constructor. </summary>
		/// <param name="shaderData"> The r-value reference shader data to move construct from. </param>
		ShaderData(ShaderData&& shaderData) noexcept;

		/// <summary> Shader data copy assignment operator. </summary>
		/// <param name="shaderData"> The reference to shader data to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned shader data. </returns>
		ShaderData& operator=(const ShaderData& shaderData) = delete;

		/// <summary> Shader data move assignment operator. </summary>
		/// <param name="shaderData"> The r-value reference to shader data to move assign from. </param>
		/// <returns> The reference to the newly move-assigned shader data. </returns>
		ShaderData& operator=(ShaderData&& shaderData) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Bind() const
		{
			//Rendering::CheckErrorCode(__FUNCTION__, "Started shader binding");
			//DELOCUST_LOG_RENDERING("Binding shader \"", m_fileName, "\".");
			glUseProgram(m_programID);
			//Rendering::CheckErrorCode(__FUNCTION__, "Finished shader binding");
		}
		bool IsBound() const
		{
			GLint currentProgramID;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgramID);
			return currentProgramID == m_programID;
		}
		const std::vector<std::unique_ptr<uniforms::UniformBase>>& GetUniforms() const { return m_uniforms; }
		const std::vector<std::unique_ptr<uniforms::UniformBase>>& GetRendererUniforms() const { return m_rendererUniforms; }
		bool IsUniformPresent(const std::string& uniformName, std::map<std::string, GLint>::const_iterator& itr) const;
		//const std::vector<uniforms::UniformStruct>& GetStructUniforms() const { return m_structUniforms; }
	private:
		std::string LoadShaderData(const std::string& directoryPath, const std::string& fileName) const;
		void AddVertexShader(const std::string& vertexShaderText);
		void AddGeometryShader(const std::string& geometryShaderText);
		void AddFragmentShader(const std::string& fragmentShaderText);
		void AddProgram(const std::string& shaderText, GLenum type);

		void AddAllAttributes(const std::string& vertexShaderText);
		void AddShaderUniforms(const std::string& shaderText);
		//void AddStructuralUniform(const std::string& uniformName, uniforms::UniformType uniformType, const std::vector<uniforms::UniformStructInfo>& structs);

		std::vector<uniforms::UniformStructInfo> ShaderData::FindUniformStructInfos(const std::string& shaderText) const;
		std::string FindUniformStructName(const std::string& structStartToOpeningBrace) const;
		std::vector<uniforms::UniformInfo> FindUniformStructComponents(const std::string& openingBraceToClosingBrace, const std::vector<uniforms::UniformStructInfo>& structUniforms) const;
		void AddUniformInfos(const std::vector<uniforms::UniformStructInfo>& structUniformInfos, std::vector<uniforms::UniformInfo>& uniformInfos,
			const std::string& uniformName, const std::string& uniformTypeStr) const;

		bool Compile();
		bool CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength);

		GLint FindUniformLocation(const std::string& uniformName);

		template <class T, typename... Args>
		void AddUniform(const std::string& uniformName, Args&&... args);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		GLuint m_programID;
		std::vector<GLuint> m_shaders;
		std::vector<std::unique_ptr<uniforms::UniformBase>> m_uniforms;
		std::vector<std::unique_ptr<uniforms::UniformBase>> m_rendererUniforms;
		std::map<std::string, GLint> m_uniformNameToLocationMap;
		//std::vector<uniforms::UniformStruct> m_structUniforms;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ShaderData */

	/// <summary>
	/// The class representing the shaders.
	/// </summary>
	/// <remarks>
	/// See https://en.wikipedia.org/wiki/Shader.
	/// </remarks>
	class Shader
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Shader constructor. </summary>
		/// <param name="directoryPath"> The path to the system directory where the shader's file with name <paramref name="fileName"/> should be located in. </param>
		/// <param name="fileName"> The file name which contains the shader's code. </param>
		RENDERING_API Shader(const std::string& directoryPath, const std::string& fileName);

		/// <summary> Shader destructor. </summary>
		RENDERING_API ~Shader();

		/// <summary> Shader copy constructor. </summary>
		/// <param name="shader"> The reference to shader to copy construct from. </param>
		Shader(const Shader& shader) = delete;

		/// <summary> Shader move constructor. </summary>
		/// <param name="shader"> The r-value reference to shader to move construct from. </param>
		RENDERING_API Shader(Shader&& shader) noexcept;

		/// <summary> Shader copy assignment operator. </summary>
		/// <param name="shader"> The reference to shader to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned shader. </returns>
		Shader& operator=(const Shader& shader) = delete;

		/// <summary> Shader move assignment operator. </summary>
		/// <param name="shader"> The r-value reference to shader to move assign from. </param>
		/// <returns> The reference to the newly move-assigned shader. </returns>
		Shader& operator=(Shader&& shader) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Binds the shader's program. </summary>
		void Bind() const { m_shaderData.Bind(); }

		/// <summary> Checks whether the shader's program is currently bound. </summary>
		/// <returns> <code>true</code> if shader's program is bound and <code>false</code> otherwise. </returns>
		bool IsBound() const { return m_shaderData.IsBound(); }
		
		/// <summary> Unbinds the shader's program. </summary>
		void Unbind() const { glUseProgram(0); }

		/// <summary> Updates the uniforms which come from the rendering engine itself. </summary>
		/// <param name="renderer"> The rendering engine that defines the values for the uniforms. </param>
		void UpdateRendererUniforms(const Renderer* renderer) const;

		/// <summary> Updates the uniforms which come from the material. </summary>
		/// <param name="material"> The material that defines the values for the uniforms. </param>
		void UpdateMaterialUniforms(const Material* material) const;

		/// <summary> Updates the uniforms which come from the transform object. </summary>
		/// <param name="transform"> The transform that defines the values for the uniforms. </param>
		void UpdateTransformUniforms(const math::Transform& transform) const;

		/// <summary> Updates all shader's uniforms. </summary>
		/// <param name="transform"> The transform that defines the values for the uniforms. </param>
		/// <param name="material"> The material that defines the values for the uniforms. </param>
		/// <param name="renderer"> The rendering engine that defines the values for the uniforms. </param>
		void UpdateUniforms(const math::Transform& transform, const Material* material, const Renderer* renderer) const;

		/// <summary> Updates the integer uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="value"> The new integer value for the uniform. </param>
		void SetUniformi(const std::string& uniformName, int value) const;

		/// <summary> Updates the floating-point uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="value"> The new floating-point value for the uniform. </param>
		void SetUniformf(const std::string& uniformName, math::Real value) const;

		/// <summary> Updates the <code>Vector2D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="vector"> The new <code>Vector2D</code> value for the uniform. </param>
		void SetUniformVector2D(const std::string& uniformName, const math::Vector2D& vector) const;

		/// <summary> Updates the <code>Vector3D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="vector"> The new <code>Vector3D</code> value for the uniform. </param>
		void SetUniformVector3D(const std::string& uniformName, const math::Vector3D& vector) const;

		/// <summary> Updates the <code>Vector4D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="vector"> The new <code>Vector4D</code> value for the uniform. </param>
		void SetUniformVector4D(const std::string& uniformName, const math::Vector4D& vector) const;

		/// <summary> Updates the <code>Vector4D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="x"> The new floating-point value for the uniform X component of the <code>Vector4D</code> uniform. </param>
		/// <param name="y"> The new floating-point value for the uniform Y component of the <code>Vector4D</code> uniform. </param>
		/// <param name="z"> The new floating-point value for the uniform Z component of the <code>Vector4D</code> uniform. </param>
		/// <param name="w"> The new floating-point value for the uniform W component of the <code>Vector4D</code> uniform. </param>
		void SetUniformVector4D(const std::string& uniformName, math::Real x, math::Real y, math::Real z, math::Real w) const;

		/// <summary> Updates the <code>Color</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="color"> The new <code>Color</code> value for the uniform. </param>
		void SetUniformColor(const std::string& uniformName, const Color& color) const;

		/// <summary> Updates the <code>Matrix4D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="matrix"> The new <code>Matrix4D</code> value for the uniform. </param>
		void SetUniformMatrix(const std::string& uniformName, const math::Matrix4D& matrix) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		ShaderData m_shaderData;
		std::string m_fileName;
#ifdef PROFILING_RENDERING_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Shader */

} /* end namespace rendering */

#endif // __RENDERING_SHADER_H__