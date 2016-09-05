#ifndef __RENDERING_SHADER_H__
#define __RENDERING_SHADER_H__

#include "Rendering.h"
#include "Material.h"
#include "Uniform.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Color.h"
#include "Camera.h"

#include "Math\Transform.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"
#ifdef PROFILING_RENDERING_MODULE_ENABLED
#include "Math\IStatisticsStorage.h"
#include "Math\Statistics.h"
#endif

#include "Utility\ReferenceCounter.h"
#include <map>
#include <vector>
#include <string>

namespace Rendering
{
	class Renderer;

	class ShaderData
	{
		/* ==================== Static variables and functions begin ==================== */
	private:
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
		ShaderData(const std::string& fileName);
		~ShaderData(void);
		ShaderData(const ShaderData& shaderData) = delete;
		ShaderData(ShaderData&& shaderData);
		ShaderData& operator=(const ShaderData& shaderData) = delete;
		ShaderData& operator=(ShaderData&& shaderData) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		void Bind() const;
		bool IsBound() const;
		const std::vector<std::unique_ptr<Uniforms::UniformBase>>& GetUniforms() const { return m_uniforms; }
		bool IsUniformPresent(const std::string& uniformName, std::map<std::string, GLint>::const_iterator& itr) const;
		//const std::vector<Uniforms::UniformStruct>& GetStructUniforms() const { return m_structUniforms; }
	private:
		std::string LoadShaderData(const std::string& fileName) const;
		void AddVertexShader(const std::string& vertexShaderText);
		void AddGeometryShader(const std::string& geometryShaderText);
		void AddFragmentShader(const std::string& fragmentShaderText);
		void AddProgram(const std::string& shaderText, GLenum type);

		void AddAllAttributes(const std::string& vertexShaderText);
		void AddShaderUniforms(const std::string& shaderText);
		//void AddStructuralUniform(const std::string& uniformName, Uniforms::UniformType uniformType, const std::vector<Uniforms::UniformStructInfo>& structs);

		std::vector<Uniforms::UniformStructInfo> ShaderData::FindUniformStructInfos(const std::string& shaderText) const;
		std::string FindUniformStructName(const std::string& structStartToOpeningBrace) const;
		std::vector<Uniforms::UniformInfo> FindUniformStructComponents(const std::string& openingBraceToClosingBrace, const std::vector<Uniforms::UniformStructInfo>& structUniforms) const;
		void AddUniformInfos(const std::vector<Uniforms::UniformStructInfo>& structUniformInfos, std::vector<Uniforms::UniformInfo>& uniformInfos,
			const std::string& uniformName, const std::string& uniformTypeStr) const;

		bool Compile();
		bool CheckForErrors(int shader, int flag, bool isProgram, int& infoLogLength);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		GLuint m_programID;
		std::vector<GLuint> m_shaders;
		std::vector<std::unique_ptr<Uniforms::UniformBase>> m_uniforms;
		std::map<std::string, GLint> m_uniformNameToLocationMap;
		//std::vector<Uniforms::UniformStruct> m_structUniforms;
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
	private:
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Shader constructor. </summary>
		/// <param name="fileName"> The file name which contains the shader's code. </param>
		RENDERING_API Shader(const std::string& fileName);
		/// <summary> Shader destructor. </summary>
		RENDERING_API ~Shader(void);

		/// <summary> Shader copy constructor. </summary>
		Shader(const Shader& shader) = delete;
		/// <summary> Shader move constructor. </summary>
		RENDERING_API Shader(Shader&& shader);
		/// <summary> Shader copy assignment operator. </summary>
		Shader& operator=(const Shader& shader) = delete;
		/// <summary> Shader move assignment operator. </summary>
		Shader& operator=(Shader&& shader) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary> Binds the shader's program. </summary>
		inline void Bind() const { m_shaderData.Bind(); }
		/// <summary> Checks whether the shader's program is currently bound. </summary>
		/// <returns> <code>true</code> if shader's program is bound and <code>false</code> otherwise. </returns>
		inline bool IsBound() const { return m_shaderData.IsBound(); }
		/// <summary> Unbinds the shader's program. </summary>
		inline void Unbind() const { glUseProgram(0); }
		/// <summary> Updates the uniforms which come from the rendering engine itself. </summary>
		/// <param name="renderer"> The rendering engine that defines the values for the uniforms. </param>
		void UpdateRendererUniforms(const Renderer* renderer) const;
		/// <summary> Updates the uniforms which come from the material. </summary>
		/// <param name="material"> The material that defines the values for the uniforms. </param>
		void UpdateMaterialUniforms(const Material* material) const;
		/// <summary> Updates the uniforms which come from the transform object. </summary>
		/// <param name="transform"> The transform that defines the values for the uniforms. </param>
		void UpdateTransformUniforms(const Math::Transform& transform) const;
		/// <summary> Updates all shader's uniforms. </summary>
		/// <param name="transform"> The transform that defines the values for the uniforms. </param>
		/// <param name="material"> The material that defines the values for the uniforms. </param>
		/// <param name="renderer"> The rendering engine that defines the values for the uniforms. </param>
		void UpdateUniforms(const Math::Transform& transform, const Material* material, const Renderer* renderer) const;

		/// <summary> Updates the integer uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="value"> The new integer value for the uniform. </param>
		void SetUniformi(const std::string& uniformName, int value) const;
		/// <summary> Updates the floating-point uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="value"> The new floating-point value for the uniform. </param>
		void SetUniformf(const std::string& uniformName, Math::Real value) const;
		/// <summary> Updates the <code>Vector2D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="vector"> The new <code>Vector2D</code> value for the uniform. </param>
		void SetUniformVector2D(const std::string& uniformName, const Math::Vector2D& vector) const;
		/// <summary> Updates the <code>Vector3D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="vector"> The new <code>Vector3D</code> value for the uniform. </param>
		void SetUniformVector3D(const std::string& uniformName, const Math::Vector3D& vector) const;
		/// <summary> Updates the <code>Vector4D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="vector"> The new <code>Vector4D</code> value for the uniform. </param>
		void SetUniformVector4D(const std::string& uniformName, const Math::Vector4D& vector) const;
		/// <summary> Updates the <code>Vector4D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="x"> The new floating-point value for the uniform X component of the <code>Vector4D</code> uniform. </param>
		/// <param name="y"> The new floating-point value for the uniform Y component of the <code>Vector4D</code> uniform. </param>
		/// <param name="z"> The new floating-point value for the uniform Z component of the <code>Vector4D</code> uniform. </param>
		/// <param name="w"> The new floating-point value for the uniform W component of the <code>Vector4D</code> uniform. </param>
		void SetUniformVector4D(const std::string& uniformName, Math::Real x, Math::Real y, Math::Real z, Math::Real w) const;
		/// <summary> Updates the <code>Color</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="color"> The new <code>Color</code> value for the uniform. </param>
		void SetUniformColor(const std::string& uniformName, const Color& color) const;
		/// <summary> Updates the <code>Matrix4D</code> uniform with the new value. </summary>
		/// <param name="uniformName"> The name of the uniform. </param>
		/// <param name="matrix"> The new <code>Matrix4D</code> value for the uniform. </param>
		void SetUniformMatrix(const std::string& uniformName, const Math::Matrix4D& matrix) const;
	private:
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		ShaderData m_shaderData;
		std::string m_fileName;
#ifdef PROFILING_RENDERING_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Shader */

} /* end namespace Rendering */

#endif // __RENDERING_SHADER_H__