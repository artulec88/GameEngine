#ifndef __RENDERING_PARTICLES_UPDATER_H__
#define __RENDERING_PARTICLES_UPDATER_H__

#include "Rendering.h"
#include "ParticleContainer.h"

#include "Math\Vector.h"

namespace Rendering
{
	/// <summary>
	/// Particles updater.
	/// </summary>
	class ParticlesUpdater
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API ParticlesUpdater();
		RENDERING_API virtual ~ParticlesUpdater();

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
		RENDERING_API virtual void Update(Math::Real deltaTime, ParticleContainer* particlesContainer) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ParticlesUpdater */

	class EulerParticlesUpdater : public ParticlesUpdater
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		EulerParticlesUpdater(const Math::Vector3D& acceleration);
		virtual ~EulerParticlesUpdater();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void Update(Math::Real deltaTime, ParticleContainer* particlesContainer);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Math::Vector3D m_acceleration;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class EulerParticlesUpdater */

	class LifeSpanParticlesUpdater : public ParticlesUpdater
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		LifeSpanParticlesUpdater();
		virtual ~LifeSpanParticlesUpdater();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void Update(Math::Real deltaTime, ParticleContainer* particlesContainer);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LifeSpanParticlesUpdater */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLES_UPDATER_H__ */