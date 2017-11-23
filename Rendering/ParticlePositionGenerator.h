#ifndef __RENDERING_PARTICLE_POSITION_GENERATOR_H__
#define __RENDERING_PARTICLE_POSITION_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math/RandomGenerator.h"
#include "Math/AABB.h"
#include "Math/Ellipsoid.h"
#include "Math/Plane.h"

namespace rendering
{
	namespace particles
	{
		namespace generators
		{
			namespace position_generator_types
			{
				enum PositionGeneratorType
				{
					CONSTANT = 0,
					BOX,
					PLANE,
					ELLIPSOID,
				}; /* end enum PositionGeneratorType */
			} /* end namespace position_generator_types */

			/// <summary>
			/// Generates position for the particle.
			/// </summary>
			class PositionGenerator : public ParticleAttributeGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				PositionGenerator() :
					ParticleAttributeGenerator(attributes::POSITION)
				{
				}
				virtual ~PositionGenerator()
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
			}; /* end class PositionGenerator */

			/// <summary>
			/// Generates position for the particle. The generated position is specified as the member variable.
			/// Thus, in general there is no randomness in how positions are generated.
			/// </summary>
			class ConstantPositionGenerator : public PositionGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API ConstantPositionGenerator(math::Real xPos, math::Real yPos, math::Real zPos);
				RENDERING_API explicit ConstantPositionGenerator(const math::Vector3D& position);
				RENDERING_API virtual ~ConstantPositionGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Vector3D m_position;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class ConstantPositionGenerator */

			/// <summary>
			/// Generates position for the particle. The position is chosen randomly from the given AABB.
			/// </summary>
			class BoxPositionGenerator : public PositionGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API explicit BoxPositionGenerator(const math::Aabb& aabb);
				RENDERING_API BoxPositionGenerator(const math::Vector3D& centerPoint, math::Real xOffset, math::Real yOffset, math::Real zOffset);
				RENDERING_API BoxPositionGenerator(math::Real minX, math::Real maxX, math::Real minY, math::Real maxY, math::Real minZ, math::Real maxZ);
				RENDERING_API virtual ~BoxPositionGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Real m_minX;
				math::Real m_maxX;
				math::Real m_minY;
				math::Real m_maxY;
				math::Real m_minZ;
				math::Real m_maxZ;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class BoxPositionGenerator */

			/// <summary>
			/// Generates position for the particle. The position is chosen randomly from the given plane.
			/// </summary>
			class PlanePositionGenerator : public PositionGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API PlanePositionGenerator(math::Real normalX, math::Real normalY, math::Real normalZ, math::Real distanceToOrigin, math::Real radius);
				RENDERING_API PlanePositionGenerator(const math::Plane& plane, math::Real radius);
				RENDERING_API virtual ~PlanePositionGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Plane m_plane;
				math::Real m_radius;
				math::Vector3D m_translationPoint;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class PlanePositionGenerator */

			/// <summary>
			/// Generates position for the particle. The position is chosen randomly from the specified ellipsoid.
			/// </summary>
			class EllipsoidPositionGenerator : public PositionGenerator
			{
				/* ==================== Static variables and functions begin ==================== */
				/* ==================== Static variables and functions end ==================== */

				/* ==================== Constructors and destructors begin ==================== */
			public:
				RENDERING_API explicit EllipsoidPositionGenerator(const math::Ellipsoid& ellipsoid);
				RENDERING_API EllipsoidPositionGenerator(const math::Vector3D& center, math::Real a, math::Real b, math::Real c);
				RENDERING_API virtual ~EllipsoidPositionGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
				RENDERING_API void Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) override;
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				math::Vector3D m_center;
				math::Real m_a;
				math::Real m_b;
				math::Real m_c;
				const math::random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class EllipsoidPositionGenerator */
		} /* end namespace generators */
	} /* end namespace particles */
} /* end namespace rendering */

#endif /* __RENDERING_PARTICLE_POSITION_GENERATOR_H__ */