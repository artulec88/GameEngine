#ifndef __RENDERING_PARTICLE_POSITION_GENERATOR_H__
#define __RENDERING_PARTICLE_POSITION_GENERATOR_H__

#include "Rendering.h"
#include "ParticleAttributeGenerator.h"

#include "Math\RandomGenerator.h"
#include "Math\AABB.h"
#include "Math\Ellipsoid.h"
#include "Math\Plane.h"

namespace Rendering
{
	namespace Particles
	{
		namespace Generators
		{
			namespace PositionGeneratorTypes
			{
				enum PositionGeneratorType
				{
					CONSTANT = 0,
					BOX,
					PLANE,
					ELLIPSOID,
				}; /* end enum PositionGeneratorType */
			} /* end namespace PositionGeneratorTypes */

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
					ParticleAttributeGenerator(Attributes::POSITION)
				{
				}
				virtual ~PositionGenerator()
				{
				}
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			protected:
				inline void Set(ParticlesContainer* particlesContainer, size_t i, const Math::Vector3D& position)
				{
					particlesContainer->SetPosition(i, position);
				}
				inline void Set(ParticlesContainer* particlesContainer, size_t i, Math::Real x, Math::Real y, Math::Real z)
				{
					particlesContainer->SetPosition(i, x, y, z);
				}
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
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
				RENDERING_API ConstantPositionGenerator(Math::Real xPos, Math::Real yPos, Math::Real zPos);
				RENDERING_API ConstantPositionGenerator(const Math::Vector3D& position);
				RENDERING_API virtual ~ConstantPositionGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Vector3D m_position;
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
				RENDERING_API BoxPositionGenerator(const Math::AABB& aabb);
				RENDERING_API BoxPositionGenerator(const Math::Vector3D& centerPoint, Math::Real xOffset, Math::Real yOffset, Math::Real zOffset);
				RENDERING_API BoxPositionGenerator(Math::Real minX, Math::Real maxX, Math::Real minY, Math::Real maxY, Math::Real minZ, Math::Real maxZ);
				RENDERING_API virtual ~BoxPositionGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Real m_minX;
				Math::Real m_maxX;
				Math::Real m_minY;
				Math::Real m_maxY;
				Math::Real m_minZ;
				Math::Real m_maxZ;
				const Math::Random::RandomGenerator& m_randomGenerator;
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
				RENDERING_API PlanePositionGenerator(Math::Real normalX, Math::Real normalY, Math::Real normalZ, Math::Real distanceToOrigin, Math::Real radius);
				RENDERING_API PlanePositionGenerator(const Math::Plane& plane, Math::Real radius);
				RENDERING_API virtual ~PlanePositionGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Plane m_plane;
				Math::Real m_radius;
				Math::Vector3D m_translationPoint;
				const Math::Random::RandomGenerator& m_randomGenerator;
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
				RENDERING_API EllipsoidPositionGenerator(const Math::Ellipsoid& ellipsoid);
				RENDERING_API EllipsoidPositionGenerator(const Math::Vector3D& center, Math::Real a, Math::Real b, Math::Real c);
				RENDERING_API virtual ~EllipsoidPositionGenerator();
				/* ==================== Constructors and destructors end ==================== */

				/* ==================== Non-static member functions begin ==================== */
			public:
				RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
				/* ==================== Non-static member functions end ==================== */

				/* ==================== Non-static member variables begin ==================== */
			private:
				Math::Vector3D m_center;
				Math::Real m_a;
				Math::Real m_b;
				Math::Real m_c;
				const Math::Random::RandomGenerator& m_randomGenerator;
				/* ==================== Non-static member variables end ==================== */
			}; /* end class EllipsoidPositionGenerator */
		} /* end namespace PositionGenerators */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_POSITION_GENERATOR_H__ */