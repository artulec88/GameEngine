#ifndef __ENGINE_PARTICLE_GENERATOR_H__
#define __ENGINE_PARTICLE_GENERATOR_H__

#include "Engine.h"
#include "Math\Vector.h"
#include "Rendering\Texture.h"
#include "Rendering\Particle.h"

namespace Engine
{

	class ParticleGenerator
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ParticleGenerator(Rendering::ParticleTexture* particleTexture, Math::Real particlesPerSecondCount,
			Math::Real particleSpeed, Math::Real particleGravityComplient, Math::Real particleLifeSpanLimit);
		ENGINE_API virtual ~ParticleGenerator(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API const Rendering::ParticleTexture* GetTexture() const { return m_particleTexture; }
		ENGINE_API virtual void Update(Math::Real deltaTime);
		ENGINE_API virtual void GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime);
		ENGINE_API virtual inline void EmitParticle(const Math::Vector3D& initialPosition)
		{
			// Something about random number generation: http://stackoverflow.com/questions/7560114/random-number-c-in-some-range
			Math::Real dirX = ((static_cast<Math::Real>(rand() % 20001) / 20000.0f) * 2.0f) - 1.0f;
			Math::Real dirZ = ((static_cast<Math::Real>(rand() % 20001) / 20000.0f) * 2.0f) - 1.0f;
			Math::Vector3D velocity(dirX, 1.0f, dirZ);
			velocity.Normalize();
			velocity *= m_particleSpeed;
			m_particles.push_back(Rendering::Particle(initialPosition, velocity, m_particleGravityComplient, m_particleLifeSpanLimit, Math::Angle(0.0f), 0.022f));
		}
		ENGINE_API const std::vector<Rendering::Particle>& GetParticles() const { return m_particles; }
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		Rendering::ParticleTexture* m_particleTexture;
		std::vector<Rendering::Particle> m_particles;
		Math::Real m_particleSpeed;
		Math::Real m_particleGravityComplient;
		Math::Real m_particleLifeSpanLimit;
		
		mutable Math::Real m_currentTimer; // TODO: Replace with timespan object
		Math::Real m_timeForGeneratingOneParticle; // TODO: Replace with timespan object
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticleGenerator */

} /* end namespace Engine */

#endif // __ENGINE_PARTICLE_GENERATOR_H__
