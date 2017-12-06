#ifndef __RENDERING_PARTICLE_LIFE_SPAN_UPDATER_H__
#define __RENDERING_PARTICLE_LIFE_SPAN_UPDATER_H__

#include "Rendering.h"
#include "ParticleAttributeUpdater.h"

#include "Math/RandomGenerator.h"

namespace rendering
{
	namespace particles
	{
		namespace updaters
		{
			namespace life_span_updater_types
			{
				enum LifeSpanUpdaterType
				{
					CONSTANT = 0
				}; /* end enum LifeSpanUpdaterType */
			} /* end namespace life_span_updater_types */

			  /// <summary>
			  /// Updates the life span for the particle.
			  /// </summary>
			class LifeSpanUpdater : public ParticleAttributeUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				LifeSpanUpdater() :
					ParticleAttributeUpdater(attributes::LIFE_SPAN)
				{
				}
				virtual ~LifeSpanUpdater()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particlesContainer, size_t i, math::Real lifeSpan) const
				{
					particlesContainer->IncreaseLifeSpan(i, lifeSpan);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
				/* ==================== Non-static member variables end ==================== */
			}; /* end class LifeSpanUpdater */

			class ConstantLifeSpanUpdater : public LifeSpanUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API explicit ConstantLifeSpanUpdater(const math::Real lifeSpanIncrement);
				RENDERING_API virtual ~ConstantLifeSpanUpdater();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_lifeSpanIncrement;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantLifeSpanUpdater */
		} /* end namespace updaters */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_LIFE_SPAN_UPDATER_H__ */