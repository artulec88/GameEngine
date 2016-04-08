#ifndef __RENDERING_PARTICLE_H__
#define __RENDERING_PARTICLE_H__

#include "Rendering.h"
#include "Math\Vector.h"
#include "Math\Angle.h"

namespace Rendering
{
	/// <summary>
	/// The representation of the single particle in the rendering engine.
	/// </summary>
	class Particle
	{
	/* ==================== Static variables begin ==================== */
	static const Math::Real GRAVITY_ACCELERATION; // TODO: The same is defined in the GravityComponent class.
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Constructs the particle.
		/// </summary>
		/// <param name="position"> The initial position of the particle. </param>
		/// <param name="velocity"> The initial velocity of the particle. </param>
		/// <param name="gravityEffectFactor"> Determines how much gravity affects the particle.. </param>
		/// <param name="lifespanLimit"> The time span (in seconds) representing how long the particle will stay alive for. </param>
		/// <param name="rotation"> The initial rotation of the particle. </param>
		/// <param name="scale"> The initial scale of the particle. </param>
		RENDERING_API Particle(const Math::Vector3D& position, const Math::Vector3D& velocity, Math::Real gravityEffectFactor, Math::Real lifespanLimit, const Math::Angle& rotation, Math::Real scale);
		RENDERING_API virtual ~Particle(void);
	//private:
	//	Particle(const Particle& particle);
	//	void operator=(const Particle& particle);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		const Math::Vector3D& GetPosition() const { return m_position; }
		const Math::Angle& GetRotation() const { return m_rotation; }
		Math::Real GetScale() const { return m_scale; }
		bool IsAlive() const { return m_lifeSpan < m_lifeSpanLimit; }

		/// <summary>
		/// Updates the particle's state.
		/// </summary>
		/// <returns> <code>true</code> if the particle is still alive and <code>false</code> if particle's lifespan exceeded its lifespan limit (particle is dead).
		RENDERING_API bool Update(Math::Real deltaTime);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		/// </summary> The vector representing the position of the particle. </summary>
		Math::Vector3D m_position;

		/// </summary> Represents current velocity of the particle. </summary>
		Math::Vector3D m_velocity;

		/// </summary>
		/// The value represents how much the particle is affected by gravity.
		/// The value <code>1.0f</code> means the particle is affected normally by gravity,
		/// whereas the value <code>0.0f</code> would mean the particle is not affected by gravity at all.
		/// </summary>
		Math::Real m_gravityEffectFactor;

		/// </summary>
		/// Information about how long the particle should stay alive for.
		/// </summary>
		Math::Real m_lifeSpanLimit;

		/// </summary>
		/// Information about how long the particle is alive for.
		/// </summary>
		Math::Real m_lifeSpan;

		/// <summary> The rotation of the particle. </summary>
		Math::Angle m_rotation;

		/// <summary> The scale of the particle. </summary>
		Math::Real m_scale;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Particle */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_H__ */