#ifndef __ENGINE_PARTICLE_SYSTEM_H__
#define __ENGINE_PARTICLE_SYSTEM_H__

#include "Engine.h"

#include "Rendering\Texture.h"
#include "Rendering\Particle.h"
//#include "Rendering\ParticleContainer.h"

#include "Math\Vector.h"
#include "Math\RandomGeneratorFactory.h"

#include <list>
#include <array>

namespace Engine
{
	class ParticleSystem
	{
	/* ==================== Static variables and functions begin ==================== */
	protected:
		static constexpr int MAX_PARTICLES_COUNT = 10000;
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ParticleSystem(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit);
		ENGINE_API virtual ~ParticleSystem(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API const Rendering::ParticleTexture* GetTexture() const { return m_particleTexture; }
		ENGINE_API const Rendering::Particle* GetParticles() const { return m_particles.data(); }
		ENGINE_API int GetAliveParticlesCount() const { return m_aliveParticlesCount; }
		/// <summary>
		/// Sorts particles according to the distance to the origin point.
		/// </summary>
		/// <returns>
		/// The number of particles that are alive.
		/// </returns>
		ENGINE_API void SortParticles(const Math::Vector3D& originPosition /* cameraPosition */);

		ENGINE_API void Update(Math::Real deltaTime);
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
			m_lastRevivedParticleIndex = 0;
			return 0; // all particles are alive, override the first one
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::ParticleTexture* m_particleTexture;
		std::array<Rendering::Particle, MAX_PARTICLES_COUNT> m_particles;
		//Rendering::ParticleContainer m_particles;
		Math::Real m_particleLifeSpanLimit;

		mutable Math::Real m_currentTimer; // TODO: Replace with timespan object
		Math::Real m_timeForGeneratingOneParticle; // TODO: Replace with timespan object
		int m_aliveParticlesCount;
		mutable int m_lastRevivedParticleIndex;

		const Math::Random::RandomGenerator& m_randomGenerator;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticleSystem */

	class FireParticleSystem : public ParticleSystem
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API FireParticleSystem(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit,
			Math::Real particleSpeed, Math::Real particleGravityComplient, const Math::Angle& particleRotation, Math::Real particleScale);
		ENGINE_API virtual ~FireParticleSystem(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
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
	}; /* end class FireParticleSystem */

	class FreeFallParticleSystem : public ParticleSystem
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API FreeFallParticleSystem(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount, Math::Real particleLifeSpanLimit,
			Math::Real particleSpeed, Math::Real particleGravityComplient, const Math::Angle& particleRotation, Math::Real particleScale);
		ENGINE_API virtual ~FreeFallParticleSystem(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
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
	}; /* end class FreeFallParticleSystem */

} /* end namespace Engine */

#endif // __ENGINE_PARTICLE_SYSTEM_H__
