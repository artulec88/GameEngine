#ifndef __RENDERING_PARTICLE_H__
#define __RENDERING_PARTICLE_H__

#include "Rendering.h"

#include "Math/Vector.h"
#include "Math\Angle.h"

#ifdef PROFILING_RENDERING_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

namespace Rendering
{
	namespace Particles
	{
		/// <summary>
		/// The representation of the single particle in the rendering engine.
		/// </summary>
		class Particle
		{
			/* ==================== Static variables begin ==================== */
			static constexpr math::Real GRAVITY_ACCELERATION = -9.8f; // TODO: The same is defined in the GravityComponent class.
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
			RENDERING_API Particle(const math::Vector3D& position, const math::Vector3D& velocity,
				math::Real gravityEffectFactor, math::Real lifespanLimit, const math::Angle& rotation, math::Real scale);

			/// <summary>
			/// Particle destructor.
			/// </summary>
			RENDERING_API ~Particle(void);

			/// <summary>
			/// Particle copy constructor.
			/// </summary>
			RENDERING_API Particle(const Particle& particle);

			/// <summary>
			/// Particle move constructor.
			/// </summary>
			RENDERING_API Particle(Particle&& particle);

			/// <summary> Particle copy assignment operator. </summary>
			RENDERING_API Particle& operator=(const Particle& particle);

			/// <summary> Particle move assignment operator. </summary>
			RENDERING_API Particle& operator=(Particle&& particle);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API const math::Vector3D& GetPosition() const { return m_position; }
			RENDERING_API const math::Angle& GetRotation() const { return m_rotation; }
			RENDERING_API math::Real GetScale() const { return m_scale; }
			RENDERING_API bool IsAlive() const { return m_lifeSpan < m_lifeSpanLimit; }
			//math::Real GetLifeSpanFactor() const { return m_lifeSpan / m_lifeSpanLimit; }

			RENDERING_API math::Real CalculateLifeStageFactor() const
			{
				return m_lifeSpan / m_lifeSpanLimit;
			}
			RENDERING_API void CalculateTextureAtlasInfo(int textureAtlasRowsCount, math::Vector2D& textureOffset0, math::Vector2D& textureOffset1, math::Real& textureAtlasBlendFactor) const;
			//const math::Vector2D& GetTextureOffsetCurrentState() const { return m_textureOffsetCurrentState; }
			//const math::Vector2D& GetTextureOffsetNextState() const { return m_textureOffsetNextState; }
			//math::Real GetTextureStateBlendFactor() const { return m_textureStateBlendFactor; }

			/// <summary>
			/// Instead of removing the particle when it is dead we simply mark it as dead and when time comes to create new particle the dead particle comes back to life
			/// (instead of creating a completely new particle instance).
			/// </summary>
			RENDERING_API void Revive(const math::Vector3D& position, const math::Vector3D& velocity,
				math::Real gravityEffectFactor, math::Real lifespanLimit, const math::Angle& rotation, math::Real scale);

			/// <summary>
			/// Updates the particle's state.
			/// </summary>
			/// <returns> <code>true</code> if the particle is still alive and <code>false</code> if particle's lifespan exceeded its lifespan limit (particle is dead).
			RENDERING_API bool Update(math::Real deltaTime);
		private:
			inline void SetTextureOffset(math::Vector2D& textureOffset, int index, int textureAtlasRowsCount) const
			{
				int column = index % textureAtlasRowsCount;
				int row = index / textureAtlasRowsCount;
				textureOffset.x = static_cast<math::Real>(column) / textureAtlasRowsCount;
				textureOffset.y = static_cast<math::Real>(row) / textureAtlasRowsCount;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/// </summary> The vector representing the position of the particle. </summary>
			math::Vector3D m_position;

			/// </summary> Represents current velocity of the particle. </summary>
			math::Vector3D m_velocity;

			/// </summary>
			/// The value represents how much the particle is affected by gravity.
			/// The value <code>1.0</code> means the particle is affected normally by gravity,
			/// whereas the value <code>0.0</code> would mean the particle is not affected by gravity at all.
			/// </summary>
			math::Real m_gravityEffectFactor;

			/// </summary>
			/// Information about how long the particle should stay alive for.
			/// </summary>
			math::Real m_lifeSpanLimit;

			/// </summary>
			/// Information about how long the particle is alive for.
			/// </summary>
			math::Real m_lifeSpan;

			/// <summary> The rotation of the particle. </summary>
			math::Angle m_rotation;

			/// <summary> The scale of the particle. </summary>
			math::Real m_scale;

			/// <summary> The texture coordinates offset for the current state the particle is in </summary>
			//math::Vector2D m_textureOffsetCurrentState;

			/// <summary> The texture coordinates offset for the next state the particle will be in.
			/// If the particle is currently in the last stage before dying then this offset should be the same as offset for the current state. </summary>
			//math::Vector2D m_textureOffsetNextState;

			/// <summary>
			/// The blend factor. The value of <code>0</code> means the particle has just recently switched the state to the current one and
			/// the closer this value is to <code>1</code> the closer the particle is to switch the state to the next one.
			/// </summary>
			//math::Real m_textureStateBlendFactor;

#ifdef PROFILING_RENDERING_MODULE_ENABLED
			math::statistics::ClassStats& m_classStats;
#endif
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Particle */


		/// <summary>
		/// The <code>Particle</code> objects comparator.
		/// </summary>
		/// <remarks>
		/// It is more efficient to pass a function-object in a function call than pass function pointers.
		/// That is why, instead of creating a Compare(const Mesh& other) method in the <code>Mesh</code> class itself and pass the pointer to this function whenever it is needed,
		/// it is better to create a <code>MeshComparator</code> object and pass its temporary instance. The reason is that function-objects are almost always inlined by the compiler.
		/// See https://en.wikibooks.org/wiki/Optimizing_C%2B%2B/Writing_efficient_code/Performance_improving_features#Function-objects.
		/// </remarks>
		class ParticleComparator
		{
		public:
			ParticleComparator(const math::Vector3D& origin) :
				m_origin(origin)
			{
			}

			bool operator() (const Particle& lhs, const Particle& rhs) const
			{
				if (!rhs.IsAlive())
				{
					return true;
				}
				else if (!lhs.IsAlive())
				{
					return false;
				}
				return (m_origin - lhs.GetPosition()).LengthSquared() < (m_origin - rhs.GetPosition()).LengthSquared();
			}
		private:
			const math::Vector3D& m_origin;
		}; // end class ParticleComparator
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_H__ */