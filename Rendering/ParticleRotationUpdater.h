#ifndef __RENDERING_PARTICLE_ROTATION_UPDATER_H__
#define __RENDERING_PARTICLE_ROTATION_UPDATER_H__

#include "Rendering.h"
#include "ParticleAttributeUpdater.h"

#include "Math/RandomGenerator.h"

namespace rendering
{
	namespace particles
	{
		namespace updaters
		{
			namespace rotation_updater_types
			{
				enum RotationUpdaterType
				{
					CONSTANT = 0
				}; /* end enum RotationUpdaterType */
			} /* end namespace rotation_updater_types */

			/// <summary>
			/// Updates rotation for the particle.
			/// </summary>
			class RotationUpdater : public ParticleAttributeUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RotationUpdater() :
					ParticleAttributeUpdater(attributes::ROTATION)
				{
				}
				virtual ~RotationUpdater()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particlesContainer, size_t i, const math::Angle& angle) const
				{
					particlesContainer->SetRotation(i, angle);
				}
				void Increase(ParticlesContainer* particlesContainer, size_t i, const math::Angle& angle) const
				{
					particlesContainer->IncreaseRotation(i, angle);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
				/* ==================== Non-static member variables end ==================== */
			}; /* end class RotationUpdater */

			class ConstantRotationParticlesUpdater : public RotationUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API explicit ConstantRotationParticlesUpdater(const math::Angle& angle);
				RENDERING_API virtual ~ConstantRotationParticlesUpdater();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Angle m_angle;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantRotationParticlesUpdater */
		} /* end namespace updaters */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_ROTATION_UPDATER_H__ */