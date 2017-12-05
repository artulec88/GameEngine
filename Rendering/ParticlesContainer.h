#ifndef __RENDERING_PARTICLES_CONTAINER_H__
#define __RENDERING_PARTICLES_CONTAINER_H__

#include "Rendering.h"
#include "ParticleAttributes.h"
#include "Color.h"

#include "Math/Vector.h"
#include "Math/Angle.h"

#include <memory>

namespace rendering
{
	namespace particles
	{
		/// <summary>
		/// Particles container. It allocates and manages memory for a given maximum number of particles.
		/// The container can kill and activate (revive) a particle when necessary.
		/// The container always makes sure that alive particles are in the front of the allocated buffer, stored continuously.
		/// The parameters of the particles are stored each in a separate array.
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
			ParticlesContainer(size_t maxCount, const attributes::AttributesMask& attributesMask);

			/// <summary> Particles container destructor. </summary>
			~ParticlesContainer();

			/// <summary> Particles container copy constructor. </summary>
			/// <param name="particlesContainer"> The particles container new particles container will be copy-constructed from. </param>
			ParticlesContainer(const ParticlesContainer& particlesContainer) = delete;

			/// <summary> Particles container move constructor. </summary>
			/// <param name="particlesContainer"> The particles container new particles container will be move-constructed from. </param>
			ParticlesContainer(ParticlesContainer&& particlesContainer) = default;

			/// <summary> Particles container copy assignment operator. </summary>
			/// <param name="particlesContainer"> The particles container current particles container will be copy-assigned from. </param>
			ParticlesContainer& operator=(const ParticlesContainer& particlesContainer) = delete;

			/// <summary> Particles container move assignment operator. </summary>
			/// <param name="particlesContainer"> The particles container current particles container will be move-assigned from. </param>
			ParticlesContainer& operator=(ParticlesContainer&& particlesContainer) = default;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			/// <summary>
			/// Resets the particles container by killing all particles instantly.
			/// The killing procedure is simply done by resetting the number of alive particles to <code>0</code>.
			/// </summary>
			void Reset() { m_countAlive = 0; }

			/// <summary> Returns the maximum number of particles that the container is able to maintain. </summary>
			/// <returns> The maximum number of particles that the container is able to maintain. </returns>
			size_t GetMaxCount() const { return m_maxCount; }

			/// <summary> Returns the number of alive particles currently maintained by the container. </summary>
			/// <returns> The number of alive particles currently maintained by the container. </returns>
			size_t GetAliveCount() const { return m_countAlive; }

			bool IsAlive(size_t i) const { return m_lifeSpans[i] < m_lifeSpanLimits[i]; }
			const math::Vector3D& GetPosition(size_t i) const { return m_positions[i]; }
			const math::Vector3D& GetVelocity(size_t i) const { return m_velocities[i]; }
			const math::Vector3D& GetAcceleration(size_t i) const { return m_accelerations[i]; }
			math::Real GetGravityEffectFactor(size_t i) const { return m_gravityEffectFactors[i]; }
			math::Real GetLifeSpanLimit(size_t i) const { return m_lifeSpanLimits[i]; }
			math::Real GetLifeSpan(size_t i) const { return m_lifeSpans[i]; }
			const math::Angle& GetRotation(size_t i) const { return m_rotations[i]; }
			math::Real GetScale(size_t i) const { return m_scales[i]; }
			math::Real* GetScalePtr(size_t i) const { return &m_scales[i]; }
			void SetPosition(size_t i, const math::Vector3D& newPosition) const { m_positions[i] = newPosition; }
			void SetPosition(size_t i, math::Real x, math::Real y, math::Real z) const
			{
				m_positions[i].x = x;
				m_positions[i].y = y;
				m_positions[i].z = z;
			}
			void SetVelocity(size_t i, const math::Vector3D& newVelocity) const { m_velocities[i] = newVelocity; }
			void SetVelocity(size_t i, math::Real x, math::Real y, math::Real z) const
			{
				m_velocities[i].x = x;
				m_velocities[i].y = y;
				m_velocities[i].z = z;
			}
			void SetAcceleration(size_t i, const math::Vector3D& newAcceleration) const { m_accelerations[i] = newAcceleration; }
			void SetAcceleration(size_t i, math::Real x, math::Real y, math::Real z) const
			{
				m_accelerations[i].x = x;
				m_accelerations[i].y = y;
				m_accelerations[i].z = z;
			}
			void SetGravityEffectFactor(size_t i, math::Real newGravityEffectFactor) const { m_gravityEffectFactors[i] = newGravityEffectFactor; }
			void SetLifeSpanLimit(size_t i, math::Real newLifeSpanLimit) const { m_lifeSpanLimits[i] = newLifeSpanLimit; }
			void SetLifeSpan(size_t i, math::Real newLifeSpan) const { m_lifeSpans[i] = newLifeSpan; }
			void SetRotation(size_t i, const math::Angle& newRotation) const { m_rotations[i] = newRotation; }
			void SetRotation(size_t i, math::Real angle, math::units::UnitType angleUnit = math::units::RADIAN) const { m_rotations[i].Set(angle, angleUnit); }
			void SetScale(size_t i, math::Real newScale) const { m_scales[i] = newScale; }
			void SetTextureOffset(size_t i, const math::Vector2D& newTextureOffset) const { m_textureOffsets[i] = newTextureOffset; }
			void SetColor(size_t i, const Color& newColor) const { m_colors[i] = newColor; }
			void SetId(size_t i, int id) const { m_ids[i] = id; }
			void IncreasePosition(size_t i, const math::Vector3D& positionIncrease) const { m_positions[i] += positionIncrease; }
			void IncreaseVelocity(size_t i, const math::Vector3D& velocityIncrease) const { m_velocities[i] += velocityIncrease; }
			void IncreaseAcceleration(size_t i, const math::Vector3D& accelerationIncrease) const
			{
				m_accelerations[i] += accelerationIncrease;
			}
			void IncreaseGravityEffectFactor(size_t i, math::Real gravityEffectFactorIncrease) const { m_gravityEffectFactors[i] += gravityEffectFactorIncrease; }
			void IncreaseLifeSpanLimit(size_t i, math::Real lifeSpanLimitIncrease) const { m_lifeSpanLimits[i] += lifeSpanLimitIncrease; }
			void IncreaseLifeSpan(size_t i, math::Real lifeSpanIncrease) const { m_lifeSpans[i] += lifeSpanIncrease; }
			void IncreaseRotation(size_t i, const math::Angle& rotationIncrease) const { m_rotations[i] += rotationIncrease; }
			void IncreaseScale(size_t i, math::Real scaleIncrease) const { m_scales[i] += scaleIncrease; }

			/// <summary>
			/// Kills the particle stored under the specified index <paramref name="id"/>.
			/// The function does nothing when no alive particles exist.
			/// </summary>
			/// <param name="id"> The index of the particle requested to be killed. </param>
			void Kill(size_t id);

			/// <summary>
			/// Revives the particle stored under the specified index <paramref name="id"/>.
			/// The function does nothing if the number of alive particles already reached the maximum number of alive particles.
			/// </summary>
			/// <param name="id"> The index of the particle requested to be revived. </param>
			void Revive(size_t id);
			void SwapData(size_t a, size_t b);
			math::Real CalculateLifeStageFactor(size_t i) const { return m_lifeSpans[i] / m_lifeSpanLimits[i]; }
			bool IsAttributeEnabled(attributes::Attribute attribute) const { return m_attributesMask.IsAttributeEnabled(attribute); }
			void SetMaxParticlesCount(size_t maxCount) { m_maxCount = maxCount; }
			void SetAttributesMask(attributes::AttributesMask attributesMask);

			friend std::ostream& operator<<(std::ostream& out, const ParticlesContainer& particlesContainer)
			{
				out << "Particles container: " << std::endl << "Attributes mask: " << particlesContainer.m_attributesMask <<
					"; max count: " << particlesContainer.m_maxCount << "; alive: " << particlesContainer.m_countAlive;
				for (auto i = 0; i < particlesContainer.m_countAlive; ++i)
				{
					if (particlesContainer.IsAttributeEnabled(attributes::POSITION))
					{
						out << std::endl << "Pos[" << i << "] = " << particlesContainer.m_positions[i];
					}
					if (particlesContainer.IsAttributeEnabled(attributes::COLOR))
					{
						out << " Color[" << i << "] = " << particlesContainer.m_colors[i];
					}
				}
				return out;
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			attributes::AttributesMask m_attributesMask;
			std::unique_ptr<math::Vector3D[]> m_positions;
			std::unique_ptr<math::Vector3D[]> m_velocities;
			std::unique_ptr<math::Vector3D[]> m_accelerations;
			std::unique_ptr<math::Real[]> m_gravityEffectFactors;
			std::unique_ptr<math::Real[]> m_lifeSpanLimits;
			std::unique_ptr<math::Real[]> m_lifeSpans;
			std::unique_ptr<math::Angle[]> m_rotations;
			std::unique_ptr<math::Real[]>  m_scales;
			std::unique_ptr<math::Vector2D[]> m_textureOffsets;
			std::unique_ptr<Color[]> m_colors;
			std::unique_ptr<math::Real[]> m_masses;
			std::unique_ptr<bool[]> m_aliveFlags;
			std::unique_ptr<int[]> m_ids;

			size_t m_maxCount;
			size_t m_countAlive;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlesContainer */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLES_CONTAINER_H__ */