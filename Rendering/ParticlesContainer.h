#ifndef __RENDERING_PARTICLES_CONTAINER_H__
#define __RENDERING_PARTICLES_CONTAINER_H__

#include "Rendering.h"
#include "ParticleAttributes.h"
#include "Color.h"

#include "Math\Vector.h"
#include "Math\Angle.h"

#include <memory>

namespace Rendering
{
	namespace Particles
	{
		/// <summary>
		/// Particles container. It allocates and manages memory for a given maximum number of particles.
		/// The container can kill and activate (revive) a particle when necessary.
		/// The container always makes sure that alive particles are in the front of the allocated buffer, stored continuously.
		/// The parameters of the particles are stored in a separate array.
		/// Struct of Arrays style object gives a possibility to create various ParticlesContainer configurations.
		/// The simple idea is to define a mask (<code>Attributes::AttributesMask</code>) and in the constructor memory for only selected attributes are allocated.
		/// </summary>
		/// <remarks>
		/// See http://www.bfilipek.com/2014/04/flexible-particle-system-container-2.html for details.
		/// </remarks>
		class ParticlesContainer
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Particles container default constructor. </summary>
			ParticlesContainer();

			/// <summary> Particles container constructor. </summary>
			/// <param name="maxCount"> The maximum number of particles the system may store. </param>
			/// <param name="attributesMask"> The mask representing which attributes each particle in the container will have. </param>
			ParticlesContainer(size_t maxCount, const Attributes::AttributesMask& attributesMask);

			/// <summary> Particles container destructor. </summary>
			~ParticlesContainer();

			/// <summary> Particles container copy constructor. </summary>
			ParticlesContainer(const ParticlesContainer& particlesContainer) = delete;

			/// <summary> Particles container move constructor. </summary>
			ParticlesContainer(ParticlesContainer&& particlesContainer) = default;

			/// <summary> Particles container copy assignment operator. </summary>
			ParticlesContainer& operator=(const ParticlesContainer& particlesContainer) = delete;

			/// <summary> Particles container move assignment operator. </summary>
			ParticlesContainer& operator=(ParticlesContainer&& particlesContainer) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			inline void Reset() { m_countAlive = 0; }
			inline size_t GetCount() const { return m_count; }
			inline size_t GetAliveCount() const { return m_countAlive; }
			inline bool IsAlive(size_t i) const { return m_lifeSpans[i] < m_lifeSpanLimits[i]; }
			inline const Math::Vector3D& GetPosition(size_t i) const { return m_positions[i]; }
			inline const Math::Vector3D& GetVelocity(size_t i) const { return m_velocities[i]; }
			inline const Math::Vector3D& GetAcceleration(size_t i) const { return m_accelerations[i]; }
			inline Math::Real GetGravityEffectFactor(size_t i) const { return m_gravityEffectFactors[i]; }
			inline Math::Real GetLifeSpanLimit(size_t i) const { return m_lifeSpanLimits[i]; }
			inline Math::Real GetLifeSpan(size_t i) const { return m_lifeSpans[i]; }
			inline const Math::Angle& GetRotation(size_t i) const { return m_rotations[i]; }
			inline Math::Real GetScale(size_t i) const { return m_scales[i]; }
			inline Math::Real* GetScalePtr(size_t i) { return &m_scales[i]; }
			inline void SetPosition(size_t i, const Math::Vector3D& newPosition) { m_positions[i] = newPosition; }
			inline void SetPosition(size_t i, Math::Real x, Math::Real y, Math::Real z)
			{
				m_positions[i].x = x;
				m_positions[i].y = y;
				m_positions[i].z = z;
			}
			inline void SetVelocity(size_t i, const Math::Vector3D& newVelocity) { m_velocities[i] = newVelocity; }
			inline void SetVelocity(size_t i, Math::Real x, Math::Real y, Math::Real z)
			{
				m_velocities[i].x = x;
				m_velocities[i].y = y;
				m_velocities[i].z = z;
			}
			inline void SetAcceleration(size_t i, const Math::Vector3D& newAcceleration) { m_accelerations[i] = newAcceleration; }
			inline void SetAcceleration(size_t i, Math::Real x, Math::Real y, Math::Real z)
			{
				m_accelerations[i].x = x;
				m_accelerations[i].y = y;
				m_accelerations[i].z = z;
			}
			inline void SetGravityEffectFactor(size_t i, Math::Real newGravityEffectFactor) { m_gravityEffectFactors[i] = newGravityEffectFactor; }
			inline void SetLifeSpanLimit(size_t i, Math::Real newLifeSpanLimit) { m_lifeSpanLimits[i] = newLifeSpanLimit; }
			inline void SetLifeSpan(size_t i, Math::Real newLifeSpan) { m_lifeSpans[i] = newLifeSpan; }
			inline void SetRotation(size_t i, const Math::Angle& newRotation) { m_rotations[i] = newRotation; }
			inline void SetRotation(size_t i, Math::Real angle, Math::Unit::UnitType angleUnit = Math::Unit::RADIAN) { m_rotations[i].Set(angle, angleUnit); }
			inline void SetScale(size_t i, Math::Real newScale) { m_scales[i] = newScale; }
			inline void SetID(size_t i, int ID) { m_IDs[i] = ID; }
			inline void IncreasePosition(size_t i, const Math::Vector3D& positionIncrease) { m_positions[i] += positionIncrease; }
			inline void IncreaseVelocity(size_t i, const Math::Vector3D& velocityIncrease) { m_velocities[i] += velocityIncrease; }
			inline void IncreaseAcceleration(size_t i, const Math::Vector3D& accelerationIncrease) { m_accelerations[i] += accelerationIncrease; }
			inline void IncreaseGravityEffectFactor(size_t i, Math::Real gravityEffectFactorIncrease) { m_gravityEffectFactors[i] += gravityEffectFactorIncrease; }
			inline void IncreaseLifeSpanLimit(size_t i, Math::Real lifeSpanLimitIncrease) { m_lifeSpanLimits[i] += lifeSpanLimitIncrease; }
			inline void IncreaseLifeSpan(size_t i, Math::Real lifeSpanIncrease) { m_lifeSpans[i] += lifeSpanIncrease; }
			inline void IncreaseRotation(size_t i, const Math::Angle& rotationIncrease) { m_rotations[i] += rotationIncrease; }
			inline void IncreaseScale(size_t i, Math::Real scaleIncrease) { m_scales[i] += scaleIncrease; }
			void Kill(size_t id);
			void Revive(size_t id);
			void SwapData(size_t a, size_t b);
			Math::Real CalculateLifeStageFactor(size_t i) const { return m_lifeSpans[i] / m_lifeSpanLimits[i]; }
			bool IsAttributeEnabled(Attributes::Attribute attribute) const { return m_attributesMask.IsAttributeEnabled(attribute); }
			inline void SetMaxParticlesCount(size_t maxCount) { m_count = maxCount; }
			void SetAttributesMask(Attributes::AttributesMask attributesMask);
			
			friend std::ostream& operator<<(std::ostream& out, const ParticlesContainer& particlesContainer)
			{
				out << "Particles container: " << std::endl << "Attributes mask: " << particlesContainer.m_attributesMask <<
					"; count: " << particlesContainer.m_count << "; alive: " << particlesContainer.m_countAlive;
				return out;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Attributes::AttributesMask m_attributesMask;
			std::unique_ptr<Math::Vector3D[]> m_positions;
			std::unique_ptr<Math::Vector3D[]> m_velocities;
			std::unique_ptr<Math::Vector3D[]> m_accelerations;
			std::unique_ptr<Math::Real[]> m_gravityEffectFactors;
			std::unique_ptr<Math::Real[]> m_lifeSpanLimits;
			std::unique_ptr<Math::Real[]> m_lifeSpans;
			std::unique_ptr<Math::Angle[]> m_rotations;
			std::unique_ptr<Math::Real[]>  m_scales;
			std::unique_ptr<Math::Vector2D[]> m_textureOffsets;
			std::unique_ptr<Color[]> m_colors;
			std::unique_ptr<Math::Real[]> m_masses;
			std::unique_ptr<bool[]> m_aliveFlags;
			std::unique_ptr<int[]> m_IDs;

			size_t m_count;
			size_t m_countAlive;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesContainer */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLES_CONTAINER_H__ */