#ifndef __ENGINE_PARTICLE_GENERATOR_H__
#define __ENGINE_PARTICLE_GENERATOR_H__

#include "Engine.h"
#include "GameManager.h"
#include "Math\Vector.h"

namespace Engine
{

	class ParticleGenerator
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API ParticleGenerator(GameManager* gameManager, Math::Real particlesPerSecondCount, Math::Real speed, Math::Real gravityComplient, Math::Real lifeSpanLimit);
		ENGINE_API virtual ~ParticleGenerator(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void GenerateParticles(const Math::Vector3D& initialPosition, Math::Real deltaTime) const;
		ENGINE_API virtual inline void EmitParticle(const Math::Vector3D& initialPosition) const
		{
			// Something about random number generation: http://stackoverflow.com/questions/7560114/random-number-c-in-some-range
			Math::Real dirX = ((static_cast<Math::Real>(rand() % 20001) / 20000.0f) * 2.0f) - 1.0f;
			Math::Real dirZ = ((static_cast<Math::Real>(rand() % 20001) / 20000.0f) * 2.0f) - 1.0f;
			Math::Vector3D velocity(dirX, 1.0f, dirZ);
			velocity.Normalize();
			velocity *= m_speed;
			//velocity.Set(0.0f, 0.0f, 0.0f); // temporary code
			m_gameManager->AddParticle(initialPosition, velocity, m_gravityComplient, m_lifeSpanLimit, Math::Angle(0.0f), 0.002f);
		}
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		GameManager* m_gameManager;
		Math::Real m_speed;
		Math::Real m_gravityComplient;
		Math::Real m_lifeSpanLimit;
		
		mutable Math::Real m_currentTimer; // TODO: Replace with timespan object
		Math::Real m_timeForGeneratingOneParticle; // TODO: Replace with timespan object
	/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticleGenerator */

} /* end namespace Engine */

#endif // __ENGINE_PARTICLE_GENERATOR_H__
