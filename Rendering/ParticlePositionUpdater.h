#ifndef __RENDERING_PARTICLE_POSITION_UPDATER_H__
#define __RENDERING_PARTICLE_POSITION_UPDATER_H__

#include "Rendering.h"
#include "ParticleAttributeUpdater.h"

#include "Math/RandomGenerator.h"

namespace rendering
{
	namespace particles
	{
		namespace updaters
		{
			namespace position_updater_types
			{
				enum PositionUpdaterType
				{
					EULER = 0
				}; /* end enum PositionUpdaterType */
			} /* end namespace position_updater_types */

			  /// <summary>
			  /// Updates position for the particle.
			  /// </summary>
			class PositionUpdater : public ParticleAttributeUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				PositionUpdater() :
					ParticleAttributeUpdater(attributes::POSITION)
				{
				}
				virtual ~PositionUpdater()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particlesContainer, size_t i, const math::Vector3D& position)
				{
					particlesContainer->SetPosition(i, position);
				}
				void Set(ParticlesContainer* particlesContainer, size_t i, math::Real x, math::Real y, math::Real z)
				{
					particlesContainer->SetPosition(i, x, y, z);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
				/* ==================== Non-static member variables end ==================== */
			}; /* end class PositionUpdater */

			class ConstantMovementParticlesUpdater : public PositionUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API explicit ConstantMovementParticlesUpdater(const math::Vector3D& movement);
				RENDERING_API virtual ~ConstantMovementParticlesUpdater();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Vector3D m_movement;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantMovementParticlesUpdater */

			class EulerParticlesUpdater : public PositionUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API explicit EulerParticlesUpdater(const math::Vector3D& acceleration);
				RENDERING_API virtual ~EulerParticlesUpdater();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Vector3D m_acceleration;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class EulerParticlesUpdater */
		} /* end namespace updaters */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_POSITION_UPDATER_H__ */