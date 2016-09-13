#ifndef __RENDERING_PARTICLE_PROPERTY_GENERATOR_H__
#define __RENDERING_PARTICLE_PROPERTY_GENERATOR_H__

#include "Rendering.h"
#include "ParticleContainer.h"

#include "Math\RandomGenerator.h"

namespace Rendering
{
	/// <summary>
	/// Particle property generator.
	/// </summary>
	class ParticlePropertyGenerator
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API ParticlePropertyGenerator();
		RENDERING_API virtual ~ParticlePropertyGenerator();

		///// <summary> Particle generator copy constructor. </summary>
		//ParticlePropertyGenerator(const ParticlePropertyGenerator& particlePropertyGenerator) = delete;
		///// <summary> Particle generator move constructor. </summary>
		//ParticlePropertyGenerator(ParticlePropertyGenerator&& particlePropertyGenerator) = delete;
		///// <summary> Particle generator copy assignment operator. </summary>
		//ParticlePropertyGenerator& operator=(const ParticlePropertyGenerator& particlePropertyGenerator) = delete;
		///// <summary> Particle generator move assignment operator. </summary>
		//ParticlePropertyGenerator& operator=(ParticlePropertyGenerator&& particlePropertyGenerator) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void Generate(Math::Real deltaTime, ParticleContainer* particleContainer, size_t startId, size_t endId) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticlePropertyGenerator */

	class BoxPositionGenerator : public ParticlePropertyGenerator
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		BoxPositionGenerator(Math::Real minX, Math::Real maxX, Math::Real minY, Math::Real maxY, Math::Real minZ, Math::Real maxZ);
		virtual ~BoxPositionGenerator();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void Generate(Math::Real deltaTime, ParticleContainer* particleContainer, size_t startId, size_t endId);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Math::Real m_minX;
		Math::Real m_maxX;
		Math::Real m_minY;
		Math::Real m_maxY;
		Math::Real m_minZ;
		Math::Real m_maxZ;
		const Math::Random::RandomGenerator& m_randomGenerator;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class BoxPositionGenerator */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_PROPERTY_GENERATOR_H__ */