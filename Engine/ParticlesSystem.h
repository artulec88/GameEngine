#ifndef __ENGINE_PARTICLES_SYSTEM_H__
#define __ENGINE_PARTICLES_SYSTEM_H__

#include "Engine.h"

#include "Rendering/Texture.h"
#include "Rendering/Particle.h"
//#include "Rendering/ParticleContainer.h"

#include "Math/Vector.h"
#include "Math/RandomGeneratorFactory.h"

#include <array>

namespace engine
{
	class ParticlesSystem
	{
	/* ==================== Static variables and functions begin ==================== */
	protected:
		static constexpr int MAX_PARTICLES_COUNT = 10000;
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ParticlesSystem(rendering::particles::ParticleTexture* particleTexture, math::Real particlesPerSecondCount, math::Real particleLifeSpanLimit);
		ENGINE_API virtual ~ParticlesSystem();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API const rendering::particles::ParticleTexture* GetTexture() const { return m_particleTexture; }
		ENGINE_API const rendering::particles::Particle* GetParticles() const { return m_particles.data(); }
		ENGINE_API int GetAliveParticlesCount() const { return m_aliveParticlesCount; }
		/// <summary>
		/// Sorts particles according to the distance to the origin point.
		/// </summary>
		/// <returns>
		/// The number of particles that are alive.
		/// </returns>
		ENGINE_API void SortParticles(const math::Vector3D& originPosition /* cameraPosition */);

		ENGINE_API void Update(math::Real deltaTime);
		ENGINE_API virtual void GenerateParticles(const math::Vector3D& initialPosition, math::Real deltaTime) = 0;
		ENGINE_API virtual inline void EmitParticle(const math::Vector3D& initialPosition) = 0;
	protected:
		int FindDeadParticleIndex() const
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
		rendering::particles::ParticleTexture* m_particleTexture;
		std::array<rendering::particles::Particle, MAX_PARTICLES_COUNT> m_particles;
		//Rendering::ParticleContainer m_particles;
		math::Real m_particleLifeSpanLimit;

		mutable math::Real m_currentTimer; // TODO: Replace with timespan object
		math::Real m_timeForGeneratingOneParticle; // TODO: Replace with timespan object
		int m_aliveParticlesCount;
		mutable int m_lastRevivedParticleIndex;

		const math::random::RandomGenerator& m_randomGenerator;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticlesSystem */

	class FireParticlesSystem : public ParticlesSystem
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API FireParticlesSystem(rendering::particles::ParticleTexture* particleTexture, math::Real particlesPerSecondCount, math::Real particleLifeSpanLimit,
			math::Real particleSpeed, math::Real particleGravityComplient, const math::Angle& particleRotation, math::Real particleScale);
		ENGINE_API virtual ~FireParticlesSystem();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void GenerateParticles(const math::Vector3D& initialPosition, math::Real deltaTime) override;
		ENGINE_API void EmitParticle(const math::Vector3D& initialPosition) override
		{
			// Something about random number generation: http://stackoverflow.com/questions/7560114/random-number-c-in-some-range
			const auto dirX = m_randomGenerator.NextFloat(-1.0f, 1.0f);
			const auto dirY = 1.0f; //m_randomGenerator.NextFloat(0.0f, 1.0f);;
			const auto dirZ = m_randomGenerator.NextFloat(-1.0f, 1.0f);
			math::Vector3D velocity(dirX, dirY, dirZ);
			velocity.Normalize();
			velocity *= m_particleSpeed;
			m_particles[FindDeadParticleIndex()].Revive(initialPosition, velocity, m_particleGravityComplient, m_particleLifeSpanLimit, m_particleRotation, m_particleScale);
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Real m_particleSpeed;
		math::Real m_particleGravityComplient;
		math::Angle m_particleRotation;
		math::Real m_particleScale;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class FireParticlesSystem */

	class FreeFallParticlesSystem : public ParticlesSystem
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API FreeFallParticlesSystem(rendering::particles::ParticleTexture* particleTexture, math::Real particlesPerSecondCount, math::Real particleLifeSpanLimit,
			math::Real particleSpeed, math::Real particleGravityComplient, const math::Angle& particleRotation, math::Real particleScale);
		ENGINE_API virtual ~FreeFallParticlesSystem();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		ENGINE_API void GenerateParticles(const math::Vector3D& initialPosition, math::Real deltaTime) override;
		ENGINE_API void EmitParticle(const math::Vector3D& initialPosition) override
		{
			// Something about random number generation: http://stackoverflow.com/questions/7560114/random-number-c-in-some-range
			math::Real dirX = m_randomGenerator.NextFloat(-1.0f, 1.0f);
			math::Real dirY = 1.0f; //m_randomGenerator.NextFloat(0.0f, 1.0f);;
			math::Real dirZ = m_randomGenerator.NextFloat(-1.0f, 1.0f);
			math::Vector3D velocity(dirX, dirY, dirZ);
			velocity.Normalize();
			velocity *= m_particleSpeed;
			m_particles[FindDeadParticleIndex()].Revive(initialPosition, velocity, m_particleGravityComplient, m_particleLifeSpanLimit, m_particleRotation, m_particleScale);
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Real m_particleSpeed;
		math::Real m_particleGravityComplient;
		math::Angle m_particleRotation;
		math::Real m_particleScale;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class FreeFallParticlesSystem */

} /* end namespace engine */

#endif // __ENGINE_PARTICLES_SYSTEM_H__
