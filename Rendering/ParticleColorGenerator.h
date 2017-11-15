#ifndef __RENDERING_PARTICLE_COLOR_GENERATOR_H__
#define __RENDERING_PARTICLE_COLOR_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math\RandomGenerator.h"

namespace Rendering
{
	namespace Particles
	{
		namespace Generators
		{
			namespace ColorGeneratorTypes
			{
				enum ColorGeneratorType
				{
					CONSTANT = 0,
					FROM_SET,
					RANGE
				}; /* end enum ColorGeneratorType */
			} /* end namespace ColorGeneratorTypes */

			  /// <summary>
			  /// Generates color for the particle.
			  /// </summary>
			class ColorGenerator : public ParticleAttributeGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				ColorGenerator() :
					ParticleAttributeGenerator(Attributes::COLOR)
				{
				}
				virtual ~ColorGenerator()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particleContainer, size_t i, const Color& color)
				{
					particleContainer->SetColor(i, color);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ColorGenerator */

			   /// <summary>
			   /// Generates constant color and sets it in the particle.
			   /// </summary>
			class ConstantColorGenerator : public ColorGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API ConstantColorGenerator(math::Real red, math::Real green, math::Real blue, math::Real alpha = REAL_ONE);
				RENDERING_API ConstantColorGenerator(const Color& color);
				RENDERING_API virtual ~ConstantColorGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Color m_color;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantColorGenerator */

			/// <summary>
			/// Color generator that chooses random color from the set and sets it in the particle.
			/// When this set of color has only one color then <code>FromSetColorGenerator</code> works identically as the <code>ConstantColorGenerator</code>.
			/// </summary>
			class FromSetColorGenerator : public ColorGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API FromSetColorGenerator(std::initializer_list<Color> colors);
				RENDERING_API virtual ~FromSetColorGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				std::vector<Color> m_colors;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class FromSetColorGenerator */

			/// <summary>
			/// Generates random color from a given range and sets it in the particle.
			/// </summary>
			class RangeColorGenerator : public ColorGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				/// <summary>
				/// The range color generator constructor.
				/// </summary>
				/// <param name="minColor">
				/// The color representing the minimum values for RGBA components that the generator may use
				/// when setting the color for the particle.
				/// </param>
				/// <param name="maxColor">
				/// The color representing the maximum values for RGBA components that the generator may use
				/// when setting the color for the particle.
				/// </param>
				RENDERING_API RangeColorGenerator(const Color& minColor, const Color& maxColor);
				RENDERING_API virtual ~RangeColorGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_minRed;
				math::Real m_maxRed;
				math::Real m_minGreen;
				math::Real m_maxGreen;
				math::Real m_minBlue;
				math::Real m_maxBlue;
				math::Real m_minAlpha;
				math::Real m_maxAlpha;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RangeColorGenerator */
		} /* end namespace Generators */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_ACCELERATION_GENERATOR_H__ */