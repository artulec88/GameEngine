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
		/// Constructs a dead particle (but ready to be revived- see function "Revive").
		/// </summary>
		RENDERING_API Particle();

		/// <summary>
		/// Constructs the particle.
		/// </summary>
		/// <param name="position"> The initial position of the particle. </param>
		/// <param name="velocity"> The initial velocity of the particle. </param>
		/// <param name="gravityEffectFactor"> Determines how much gravity affects the particle.. </param>
		/// <param name="lifespanLimit"> The time span (in seconds) representing how long the particle will stay alive for. </param>
		/// <param name="rotation"> The initial rotation of the particle. </param>
		/// <param name="scale"> The initial scale of the particle. </param>
		RENDERING_API Particle(const Math::Vector3D& position, const Math::Vector3D& velocity,
			Math::Real gravityEffectFactor, Math::Real lifespanLimit, const Math::Angle& rotation, Math::Real scale);
		RENDERING_API virtual ~Particle(void);
		//public:
		//	Particle(const Particle& particle) :
		//		m_position(particle.m_position),
		//		m_velocity(particle.m_velocity),
		//		m_gravityEffectFactor(particle.m_gravityEffectFactor),
		//		m_lifeSpanLimit(particle.m_lifeSpanLimit),
		//		m_lifeSpan(particle.m_lifeSpan),
		//		m_rotation(particle.m_rotation),
		//		m_scale(particle.m_scale)
		//	{
		//	}
		//	Particle(Particle&& particle)
		//	{
		//		std::swap(m_position, particle.m_position);
		//		std::swap(m_velocity, particle.m_velocity);
		//		std::swap(m_gravityEffectFactor, particle.m_gravityEffectFactor);
		//		std::swap(m_lifeSpanLimit, particle.m_lifeSpanLimit);
		//		std::swap(m_lifeSpan, particle.m_lifeSpan);
		//		std::swap(m_rotation, particle.m_rotation);
		//		std::swap(m_scale, particle.m_scale);
		//	}
		//	Particle& operator=(Particle particle)
		//	{
		//		std::swap(m_position, particle.m_position);
		//		std::swap(m_velocity, particle.m_velocity);
		//		std::swap(m_gravityEffectFactor, particle.m_gravityEffectFactor);
		//		std::swap(m_lifeSpanLimit, particle.m_lifeSpanLimit);
		//		std::swap(m_lifeSpan, particle.m_lifeSpan);
		//		std::swap(m_rotation, particle.m_rotation);
		//		std::swap(m_scale, particle.m_scale);
		//		return *this;
		//	}
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		const Math::Vector3D& GetPosition() const { return m_position; }
		const Math::Angle& GetRotation() const { return m_rotation; }
		Math::Real GetScale() const { return m_scale; }
		bool IsAlive() const { return m_lifeSpan < m_lifeSpanLimit; }
		//Math::Real GetLifeSpanFactor() const { return m_lifeSpan / m_lifeSpanLimit; }

		Math::Real CalculateLifeStageFactor() const
		{
			return m_lifeSpan / m_lifeSpanLimit;
		}
		void CalculateTextureAtlasInfo(int textureAtlasRowsCount, Math::Vector2D& textureOffset0, Math::Vector2D& textureOffset1, Math::Real& textureAtlasBlendFactor) const;
		//const Math::Vector2D& GetTextureOffsetCurrentState() const { return m_textureOffsetCurrentState; }
		//const Math::Vector2D& GetTextureOffsetNextState() const { return m_textureOffsetNextState; }
		//Math::Real GetTextureStateBlendFactor() const { return m_textureStateBlendFactor; }

		/// <summary>
		/// Instead of removing the particle when it is dead we simply mark it as dead and when time comes to create new particle the dead particle comes back to life
		/// (instead of creating a completely new particle instance).
		/// </summary>
		RENDERING_API void Revive(const Math::Vector3D& position, const Math::Vector3D& velocity,
			Math::Real gravityEffectFactor, Math::Real lifespanLimit, const Math::Angle& rotation, Math::Real scale);

		/// <summary>
		/// Updates the particle's state.
		/// </summary>
		/// <returns> <code>true</code> if the particle is still alive and <code>false</code> if particle's lifespan exceeded its lifespan limit (particle is dead).
		RENDERING_API bool Update(Math::Real deltaTime);
	private:
		inline void SetTextureOffset(Math::Vector2D& textureOffset, int index, int textureAtlasRowsCount) const
		{
			int column = index % textureAtlasRowsCount;
			int row = index / textureAtlasRowsCount;
			textureOffset.SetX(static_cast<Math::Real>(column) / textureAtlasRowsCount);
			textureOffset.SetY(static_cast<Math::Real>(row) / textureAtlasRowsCount);
		}
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

		/// <summary> The texture coordinates offset for the current state the particle is in </summary>
		//Math::Vector2D m_textureOffsetCurrentState;

		/// <summary> The texture coordinates offset for the next state the particle will be in.
		/// If the particle is currently in the last stage before dying then this offset should be the same as offset for the current state. </summary>
		//Math::Vector2D m_textureOffsetNextState;

		/// <summary>
		/// The blend factor. The value of <code>0</code> means the particle has just recently switched the state to the current one and
		/// the closer this value is to <code>1</code> the closer the particle is to switch the state to the next one.
		/// </summary>
		//Math::Real m_textureStateBlendFactor;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Particle */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_H__ */