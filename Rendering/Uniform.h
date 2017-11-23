#ifndef __RENDERING_UNIFORM_H__
#define __RENDERING_UNIFORM_H__

#include "Rendering.h"

#include "Math/Transform.h"

#include <string>
#include <vector>

namespace rendering
{
	class Material;
	class Renderer;

	namespace uniforms
	{
		/// <summary>
		/// Uniform types. It defines both:
		/// * the primitive uniform types, directly supported by GLSL,
		/// * the structural uniform types defined by the developer.
		/// </summary> 
		enum UniformType
		{
			VEC_2D = 0,
			VEC_3D,
			VEC_4D,
			MATRIX_4x4,
			INT,
			REAL,
			SAMPLER_2D,
			SAMPLER_CUBE,
			BASE_LIGHT, // structural uniform
			DIRECTIONAL_LIGHT, // structural uniform
			POINT_LIGHT, // structural uniform
			SPOT_LIGHT, // structural uniform
			ATTENUATION, // structural uniform
			UNKNOWN
		}; /* end enum UniformType */

		struct UniformInfo
		{
			UniformInfo(const std::string& _name, uniforms::UniformType _type) :
				name(_name),
				type(_type)
			{
			}

			const std::string name;
			const UniformType type;
		}; /* end struct UniformInfo */

		struct UniformStructInfo
		{
			std::string name;
			std::vector<UniformInfo> uniformInfos;
		}; /* end struct UniformStructInfo */

		class UniformBase
		{
			/* ==================== Static variables and functions begin ==================== */
		public:
			static constexpr GLint INVALID_LOCATION = -1;
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
			/// <summary> Base uniform constructor. </summary>
			/// <param name="name"> The name of the uniform. </param>
			/// <param name="type"> The type of the uniform. </param>
			UniformBase(const std::string& name, UniformType type);

			/// <summary> Base uniform destructor. </summary>
			virtual ~UniformBase();

			/// <summary> Base uniform copy constructor. </summary>
			/// <param name="uniformBase"> The reference to base uniform to copy construct from. </param>
			UniformBase(const UniformBase& uniformBase) = delete;

			/// <summary> Base uniform move constructor. </summary>
			/// <param name="uniformBase"> The r-value reference to base uniform to move construct from. </param>
			UniformBase(UniformBase&& uniformBase) = delete;

			/// <summary> Base uniform copy assignment operator. </summary>
			/// <param name="uniformBase"> The reference to base uniform to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned base uniform. </returns>
			UniformBase& operator=(const UniformBase& uniformBase) = delete;

			/// <summary> Base uniform move assignment operator. </summary>
			/// <param name="uniformBase"> The r-value reference to base uniform to move assign from. </param>
			/// <returns> The reference to the newly move-assigned base uniform. </returns>
			UniformBase& operator=(UniformBase&& uniformBase) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			virtual void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) = 0;

			const std::string& GetName() const { return m_name; }
			const std::string& GetPrefix() const { return m_prefix; }
			const std::string& GetUnprefixedName() const { return m_unprefixedName; }
			UniformType GetType() const { return m_type; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			const std::string m_name;
			const std::string m_prefix;
			const std::string m_unprefixedName;
			const UniformType m_type;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class UniformBase */

		class SimpleUniform : public UniformBase
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			SimpleUniform(const std::string& name, uniforms::UniformType type, GLint location);
			virtual ~SimpleUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			GLint GetLocation() const { return m_location; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			const GLint m_location;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class SimpleUniform */

		class IntUniform : public SimpleUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			IntUniform(const std::string& name, GLint location);
			virtual ~IntUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class IntUniform */

		class TextureUniform : public SimpleUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			TextureUniform(const std::string& name, GLint location);
			virtual ~TextureUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class TextureUniform */

		class CubeTextureUniform : public SimpleUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			CubeTextureUniform(const std::string& name, GLint location);
			virtual ~CubeTextureUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class CubeTextureUniform */

		class RealUniform : public SimpleUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RealUniform(const std::string& name, GLint location);
			virtual ~RealUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class RealUniform */

		class Vector2DUniform : public SimpleUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			Vector2DUniform(const std::string& name, GLint location);
			virtual ~Vector2DUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Vector2DUniform */

		class Vector3DUniform : public SimpleUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			Vector3DUniform(const std::string& name, GLint location);
			virtual ~Vector3DUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Vector3DUniform */

		class Vector4DUniform : public SimpleUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			Vector4DUniform(const std::string& name, GLint location);
			virtual ~Vector4DUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Vector4DUniform */

		class MatrixUniform : public SimpleUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			MatrixUniform(const std::string& name, GLint location);
			virtual ~MatrixUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			/* ==================== Non-static member variables end ==================== */
		}; /* end class MatrixUniform */

		class BaseLightUniform : public UniformBase
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			BaseLightUniform(const std::string& name, UniformType type, GLint colorLocation, GLint intensityLocation);
			virtual ~BaseLightUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			GLint GetColorLocation() const { return m_colorLocation; }
			GLint GetIntensityLocation() const { return m_intensityLocation; }
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			GLint m_colorLocation;
			GLint m_intensityLocation;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BaseLightUniform */

		class DirectionalLightUniform : public BaseLightUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			DirectionalLightUniform(const std::string& name, GLint colorLocation, GLint intensityLocation, GLint directionLocation);
			virtual ~DirectionalLightUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Updates the directional light uniform. Although the <paramref name="directionalLight"/> is specified as the <code>BaseLight</code> instance
			/// it holds all the information necessary for the directional light. Color and intensity are directly stored in the <code>BaseLight</code> object
			/// and the direction can be easily retrieved from the transformation.
			/// </summary>
			/// <param name="renderer"> Pointer to rendering engine. </param>
			/// <param name="transform"> Pointer to transform of the object we want to update uniforms for. </param>
			/// <param name="material"> Pointer to material of the object we want to update uniforms for. </param>
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			GLint m_directionLocation;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class DirectionalLightUniform */

		class PointLightUniform : public BaseLightUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			PointLightUniform(const std::string& name, GLint colorLocation, GLint intensityLocation, GLint constantFactorLocation, GLint linearFactorLocation,
				GLint exponentFactorLocation, GLint positionLocation, GLint rangeLocation);
			virtual ~PointLightUniform();
		protected:
			PointLightUniform(const std::string& name, UniformType type, GLint colorLocation, GLint intensityLocation, GLint constantFactorLocation,
				GLint linearFactorLocation, GLint exponentFactorLocation, GLint positionLocation, GLint rangeLocation);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Sets the point light uniform.
			/// </summary>
			/// <param name="renderer"> Pointer to rendering engine. </param>
			/// <param name="transform"> Pointer to transform of the object we want to update uniforms for. </param>
			/// <param name="material"> Pointer to material of the object we want to update uniforms for. </param>
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		protected:
			GLint m_constantFactorLocation;
			GLint m_linearFactorLocation;
			GLint m_exponentFactorLocation;
			GLint m_positionLocation;
			GLint m_rangeLocation;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class PointLightUniform */

		class SpotLightUniform : public PointLightUniform
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			SpotLightUniform(const std::string& name, GLint colorLocation, GLint intensityLocation, GLint constantFactorLocation, GLint linearFactorLocation,
				GLint exponentFactorLocation, GLint positionLocation, GLint rangeLocation, GLint directionLocation, GLint cutoffLocation);
			virtual ~SpotLightUniform();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Sets the spot light uniform.
			/// </summary>
			/// <param name="renderer"> Pointer to rendering engine. </param>
			/// <param name="transform"> Pointer to transform of the object we want to update uniforms for. </param>
			/// <param name="material"> Pointer to material of the object we want to update uniforms for. </param>
			void Update(const Renderer* renderer, const math::Transform* transform, const Material* material) override;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			GLint m_directionLocation;
			GLint m_cutoffLocation;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class SpotLightUniform */

		   //struct UniformStruct
		   //{
		   //	std::string name;
		   //	std::vector<Uniform> uniforms;
		   //};

		constexpr bool IsPrimitiveUniformType(UniformType uniformType)
		{
			return ((uniformType == VEC_2D) || (uniformType == VEC_3D) || (uniformType == VEC_4D) || (uniformType == MATRIX_4x4) ||
				(uniformType == INT) || (uniformType == REAL) || (uniformType == SAMPLER_2D) || (uniformType == SAMPLER_CUBE)) ? true : false;
		}

		UniformType ConvertStringToUniformType(const std::string& uniformTypeStr);
		constexpr char* ConvertUniformTypeToString(UniformType uniformType)
		{
			return (uniformType == VEC_2D) ? "vec2" :
				((uniformType == VEC_3D) ? "vec3" :
					((uniformType == VEC_4D) ? "vec4" :
						((uniformType == MATRIX_4x4) ? "mat4" :
							((uniformType == INT) ? "int" :
								((uniformType == REAL) ? "float" :
									((uniformType == SAMPLER_2D) ? "sampler2D" :
										((uniformType == SAMPLER_CUBE) ? "samplerCube" :
											((uniformType == BASE_LIGHT) ? "BaseLight" :
												((uniformType == DIRECTIONAL_LIGHT) ? "DirectionalLight" :
													((uniformType == POINT_LIGHT) ? "PointLight" :
														((uniformType == SPOT_LIGHT) ? "SpotLight" :
															((uniformType == ATTENUATION) ? "Attenuation" :
																"Unknown"))))))))))));
		}
	} /* end namespace uniforms */

} /* end namespace rendering */

#endif /* __RENDERING_UNIFORM_H__ */