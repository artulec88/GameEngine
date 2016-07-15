#ifndef __ENGINE_PARTICLE_GENERATOR_H__
#define __ENGINE_PARTICLE_GENERATOR_H__

#include "Engine.h"
#include "Math\Vector.h"
#include "Math\RandomGeneratorFactory.h"
#include "Rendering\Texture.h"
#include "Rendering\Particle.h"
#include <list>

#define MAX_PARTICLES_COUNT 10000

namespace Engine
{
	class ParticleGenerator
	{
		//typedef std::vector<Rendering::Particle> ParticleContainer;
		//typedef Rendering::Particle ParticleContainer;
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ParticleGenerator(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit);
		ENGINE_API virtual ~ParticleGenerator(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API const Rendering::ParticleTexture* GetTexture() const { return m_particleTexture; }
		ENGINE_API const Rendering::Particle* GetParticles() const { return &m_particles[0]; }
		ENGINE_API int GetAliveParticlesCount() const { return m_aliveParticlesCount; }
		/// <summary>
		/// Sorts particles according to the distance to the origin point.
		/// </summary>
		/// <returns>
		/// The number of particles that are alive.
		/// </returns>
		ENGINE_API void SortParticles(const Math::Vector3D& originPosition /* cameraPosition */);

		ENGINE_API virtual void Update(Math::Real deltaTime) = 0;
		ENGINE_API virtual void GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime) = 0;
		ENGINE_API virtual inline void EmitParticle(const Math::Vector3D& initialPosition) = 0;
	protected:
		inline int FindDeadParticleIndex() const
		{
			for (int i = m_lastRevivedParticleIndex; i < MAX_PARTICLES_COUNT; ++i)
			{
				if (!m_particles[i].IsAlive())
				{
					m_lastRevivedParticleIndex = i;
					//EMERGENCY_LOG_ENGINE("Dead particle index = ", m_lastRevivedParticleIndex);
					return i;
				}
			}
			for (int i = 0; i < m_lastRevivedParticleIndex; ++i)
			{
				if (!m_particles[i].IsAlive())
				{
					m_lastRevivedParticleIndex = i;
					//ERROR_LOG_ENGINE("Dead particle index = ", m_lastRevivedParticleIndex);
					return i;
				}
			}
			//WARNING_LOG_ENGINE("Dead particle index = ", m_lastRevivedParticleIndex);
			return 0; // all particles are alive, override the first one
		}
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::ParticleTexture* m_particleTexture;
		Rendering::Particle m_particles[MAX_PARTICLES_COUNT];
		Math::Real m_particleLifeSpanLimit;

		mutable Math::Real m_currentTimer; // TODO: Replace with timespan object
		Math::Real m_timeForGeneratingOneParticle; // TODO: Replace with timespan object
		int m_aliveParticlesCount;
		mutable int m_lastRevivedParticleIndex;

		const Math::Random::RandomGenerator& m_randomGenerator;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticleGenerator */

	class FireParticleGenerator : public ParticleGenerator
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API FireParticleGenerator(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit,
			Math::Real particleSpeed, Math::Real particleGravityComplient, const Math::Angle& particleRotation, Math::Real particleScale);
		ENGINE_API virtual ~FireParticleGenerator(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
		ENGINE_API virtual void GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime);
		ENGINE_API virtual inline void EmitParticle(const Math::Vector3D& initialPosition)
		{
			// Something about random number generation: http://stackoverflow.com/questions/7560114/random-number-c-in-some-range
			Math::Real dirX = m_randomGenerator.NextFloat(-1.0f, 1.0f);
			Math::Real dirY = 1.0f; //m_randomGenerator.NextFloat(0.0f, 1.0f);;
			Math::Real dirZ = m_randomGenerator.NextFloat(-1.0f, 1.0f);
			Math::Vector3D velocity(dirX, dirY, dirZ);
			velocity.Normalize();
			velocity *= m_particleSpeed;
			m_particles[FindDeadParticleIndex()].Revive(initialPosition, velocity, m_particleGravityComplient, m_particleLifeSpanLimit, m_particleRotation, m_particleScale);
		}
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Real m_particleSpeed;
		Math::Real m_particleGravityComplient;
		Math::Angle m_particleRotation;
		Math::Real m_particleScale;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class FireParticleGenerator */

	class FreeFallParticleGenerator : public ParticleGenerator
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API FreeFallParticleGenerator(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit,
			Math::Real particleSpeed, Math::Real particleGravityComplient, const Math::Angle& particleRotation, Math::Real particleScale);
		ENGINE_API virtual ~FreeFallParticleGenerator(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Update(Math::Real deltaTime);
		ENGINE_API virtual void GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime);
		ENGINE_API virtual inline void EmitParticle(const Math::Vector3D& initialPosition)
		{
			// Something about random number generation: http://stackoverflow.com/questions/7560114/random-number-c-in-some-range
			Math::Real dirX = m_randomGenerator.NextFloat(-1.0f, 1.0f);
			Math::Real dirY = 1.0f; //m_randomGenerator.NextFloat(0.0f, 1.0f);;
			Math::Real dirZ = m_randomGenerator.NextFloat(-1.0f, 1.0f);
			Math::Vector3D velocity(dirX, dirY, dirZ);
			velocity.Normalize();
			velocity *= m_particleSpeed;
			m_particles[FindDeadParticleIndex()].Revive(initialPosition, velocity, m_particleGravityComplient, m_particleLifeSpanLimit, m_particleRotation, m_particleScale);
		}
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Real m_particleSpeed;
		Math::Real m_particleGravityComplient;
		Math::Angle m_particleRotation;
		Math::Real m_particleScale;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class FreeFallParticleGenerator */

} /* end namespace Engine */

#endif // __ENGINE_PARTICLE_GENERATOR_H__
