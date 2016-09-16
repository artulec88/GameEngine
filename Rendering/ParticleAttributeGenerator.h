#ifndef __RENDERING_PARTICLE_PROPERTY_GENERATOR_H__
#define __RENDERING_PARTICLE_PROPERTY_GENERATOR_H__

#include "Rendering.h"
#include "ParticlesContainer.h"
#include "ParticleAttributes.h"

#include "Math\RandomGenerator.h"
#include "Math\AABB.h"
#include "Math\Ellipsoid.h"
#include "Math\Plane.h"

namespace Rendering
{
	namespace Particles
	{
		/// <summary>
		/// Particle attribute generator. Particles attributes generator can generate one or more different attributes for a particle.
		/// A generator just takes a range of particles and sets new values for some specific attributes of the particles.
		/// </summary>
		class ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			ParticleAttributeGenerator(Attributes::AttributesMask attributesMask) :
				m_attributesMask(attributesMask)
			{
			}
			virtual ~ParticleAttributeGenerator()
			{
			}

			///// <summary> Particle generator copy constructor. </summary>
			//ParticleAttributeGenerator(const ParticleAttributeGenerator& particleAttributeGenerator) = delete;
			///// <summary> Particle generator move constructor. </summary>
			//ParticleAttributeGenerator(ParticleAttributeGenerator&& particleAttributeGenerator) = delete;
			///// <summary> Particle generator copy assignment operator. </summary>
			//ParticleAttributeGenerator& operator=(const ParticleAttributeGenerator& particleAttributeGenerator) = delete;
			///// <summary> Particle generator move assignment operator. </summary>
			//ParticleAttributeGenerator& operator=(ParticleAttributeGenerator&& particleAttributeGenerator) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) = 0;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Attributes::AttributesMask m_attributesMask;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticleAttributeGenerator */

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
		/// Generates position for the particle. The position is chosen randomly from the given plane.
		/// </summary>
		class PlanePositionGenerator : public PositionGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API PlanePositionGenerator(const Math::Plane& plane);
			RENDERING_API virtual ~PlanePositionGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Plane m_plane;
			const Math::Random::RandomGenerator& m_randomGenerator;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class PlanePositionGenerator */

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

		/// <summary>
		/// Generates velocity for the particle.
		/// </summary>
		class VelocityGenerator : public ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			VelocityGenerator() :
				ParticleAttributeGenerator(Attributes::VELOCITY)
			{
			}
			virtual ~VelocityGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			void Set(ParticlesContainer* particleContainer, size_t i, const Math::Vector3D& velocity)
			{
				particleContainer->SetVelocity(i, velocity);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class VelocityGenerator */

		/// <summary>
		/// Generates random velocity from a given range and sets it in the particle.
		/// </summary>
		class BasicVelocityGenerator : public VelocityGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API BasicVelocityGenerator(Math::Real minSpeedX, Math::Real maxSpeedX, Math::Real minSpeedY, Math::Real maxSpeedY, Math::Real minSpeedZ, Math::Real maxSpeedZ);
			RENDERING_API virtual ~BasicVelocityGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_minSpeedX;
			Math::Real m_maxSpeedX;
			Math::Real m_minSpeedY;
			Math::Real m_maxSpeedY;
			Math::Real m_minSpeedZ;
			Math::Real m_maxSpeedZ;
			const Math::Random::RandomGenerator& m_randomGenerator;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BasicVelocityGenerator */

		/// <summary>
		/// Generates acceleration for the particle.
		/// </summary>
		class AccelerationGenerator : public ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			AccelerationGenerator() :
				ParticleAttributeGenerator(Attributes::ACCELERATION)
			{
			}
			virtual ~AccelerationGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			void Set(ParticlesContainer* particleContainer, size_t i, const Math::Vector3D& acceleration)
			{
				particleContainer->SetAcceleration(i, acceleration);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class AccelerationGenerator */

		/// <summary>
		/// Generates constant acceleration and sets it in the particle.
		/// </summary>
		class ConstantAccelerationGenerator : public AccelerationGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API ConstantAccelerationGenerator(Math::Real accelerationX, Math::Real accelerationY, Math::Real accelerationZ);
			RENDERING_API ConstantAccelerationGenerator(const Math::Vector3D& acceleration);
			RENDERING_API virtual ~ConstantAccelerationGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Vector3D m_acceleration;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ConstantAccelerationGenerator */

		/// <summary>
		/// Generates random acceleration from a given range and sets it in the particle.
		/// </summary>
		class RandomAccelerationGenerator : public AccelerationGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API RandomAccelerationGenerator(Math::Real minAccelerationX, Math::Real maxAccelerationX, Math::Real minAccelerationY, Math::Real maxAccelerationY, Math::Real minAccelerationZ, Math::Real maxAccelerationZ);
			RENDERING_API virtual ~RandomAccelerationGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_minAccelerationX;
			Math::Real m_maxAccelerationX;
			Math::Real m_minAccelerationY;
			Math::Real m_maxAccelerationY;
			Math::Real m_minAccelerationZ;
			Math::Real m_maxAccelerationZ;
			const Math::Random::RandomGenerator& m_randomGenerator;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class RandomAccelerationGenerator */

		/// <summary>
		/// Generates life span limit for the particle.
		/// </summary>
		class LifeSpanLimitGenerator : public ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			LifeSpanLimitGenerator() :
				ParticleAttributeGenerator(Attributes::LIFE_SPAN)
			{
			}
			virtual ~LifeSpanLimitGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			inline void Set(ParticlesContainer* particleContainer, size_t i, Math::Real lifeSpanLimit)
			{
				particleContainer->SetLifeSpanLimit(i, lifeSpanLimit);
				particleContainer->SetLifeSpan(i, REAL_ZERO);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class LifeSpanLimitGenerator */

		/// <summary>
		/// Generates random life span limit from a given range and sets it in the particle.
		/// </summary>
		class BasicLifeSpanLimitGenerator : public LifeSpanLimitGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API BasicLifeSpanLimitGenerator(Math::Real minLifeSpanLimit, Math::Real maxLifeSpanLimit);
			RENDERING_API virtual ~BasicLifeSpanLimitGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_minLifeSpanLimit;
			Math::Real m_maxLifeSpanLimit;
			const Math::Random::RandomGenerator& m_randomGenerator;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BasicLifeSpanLimitGenerator */

		/// <summary>
		/// Generates rotation for the particle.
		/// </summary>
		class RotationGenerator : public ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RotationGenerator() :
				ParticleAttributeGenerator(Attributes::ROTATION)
			{
			}
			virtual ~RotationGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			inline void Set(ParticlesContainer* particleContainer, size_t i, const Math::Angle& rotationAngle)
			{
				particleContainer->SetRotation(i, rotationAngle);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class RotationGenerator */

		/// <summary>
		/// Generates random rotation from a given range of angles and sets it in the particle.
		/// </summary>
		class RandomRotationGenerator : public RotationGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API RandomRotationGenerator(const Math::Angle& minAngle, const Math::Angle& maxAngle);
			RENDERING_API virtual ~RandomRotationGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_minAngleInRadians;
			Math::Real m_maxAngleInRadians;
			const Math::Random::RandomGenerator& m_randomGenerator;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class RandomRotationGenerator */

		/// <summary>
		/// Generates ID for the particle.
		/// </summary>
		class IdGenerator : public ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			IdGenerator() :
				ParticleAttributeGenerator(Attributes::ID)
			{
			}
			virtual ~IdGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			inline void Set(ParticlesContainer* particleContainer, size_t i, int ID)
			{
				particleContainer->SetID(i, ID);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class IdGenerator */

		/// <summary>
		/// Generates ID and sets it in the particle.
		/// </summary>
		class BasicIdGenerator : public IdGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API BasicIdGenerator();
			RENDERING_API virtual ~BasicIdGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			int m_id;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class BasicIdGenerator */

		/// <summary>
		/// Generates scale for the particle.
		/// </summary>
		class ScaleGenerator : public ParticleAttributeGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			ScaleGenerator() :
				ParticleAttributeGenerator(Attributes::SCALE)
			{
			}
			virtual ~ScaleGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			inline void Set(ParticlesContainer* particleContainer, size_t i, Math::Real scale)
			{
				particleContainer->SetScale(i, scale);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ScaleGenerator */

		/// <summary>
		/// Generates scale and sets it in the particle.
		/// </summary>
		class ConstantScaleGenerator : public ScaleGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API ConstantScaleGenerator(Math::Real scale);
			RENDERING_API virtual ~ConstantScaleGenerator();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			RENDERING_API virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			Math::Real m_scale;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ConstantScaleGenerator */
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_PROPERTY_GENERATOR_H__ */