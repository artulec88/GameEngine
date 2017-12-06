#ifndef __RENDERING_PARTICLE_COLOR_UPDATER_H__
#define __RENDERING_PARTICLE_COLOR_UPDATER_H__

#include "Rendering.h"
#include "ParticleAttributeUpdater.h"

#include "Math/RandomGenerator.h"

namespace rendering
{
	namespace particles
	{
		namespace updaters
		{
			namespace color_updater_types
			{
				enum ColorUpdaterType
				{
					CONSTANT = 0
				}; /* end enum ColorUpdaterType */
			} /* end namespace color_updater_types */

			  /// <summary>
			  /// Updates color for the particle.
			  /// </summary>
			class ColorUpdater : public ParticleAttributeUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				ColorUpdater() :
					ParticleAttributeUpdater(attributes::COLOR)
				{
				}
				virtual ~ColorUpdater()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				void Set(ParticlesContainer* particlesContainer, size_t i, const Color& color)
				{
					particlesContainer->SetColor(i, color);
				}
				void Set(ParticlesContainer* particlesContainer, size_t i, math::Real red, math::Real green, math::Real blue, math::Real alpha)
				{
					particlesContainer->SetColor(i, red, green, blue, alpha);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ColorUpdater */

			class ConstantColorUpdater : public ColorUpdater
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API explicit ConstantColorUpdater(const Color& color);
				RENDERING_API virtual ~ConstantColorUpdater();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Update(math::Real deltaTime, ParticlesContainer* particlesContainer) const override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Color m_color;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantColorUpdater */
		} /* end namespace updaters */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_COLOR_UPDATER_H__ */