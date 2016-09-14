#ifndef __RENDERING_PARTICLE_PROPERTY_GENERATOR_H__
#define __RENDERING_PARTICLE_PROPERTY_GENERATOR_H__

#include "Rendering.h"
#include "ParticlesContainer.h"

#include "Math\RandomGenerator.h"
#include "Math\AABB.h"
#include "Math\Ellipsoid.h"

namespace Rendering
{
	namespace Particles
	{
		/// <summary>
		/// Particle property generator. Particles property generator can generate one or more different attributes for a particle.
		/// A generator just takes a range of particles and sets new values for some specific attributes of the particles.
		/// </summary>
		class ParticlePropertyGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			ParticlePropertyGenerator();
			virtual ~ParticlePropertyGenerator();

			///// <summary> Particle generator copy constructor. </summary>
			//ParticlePropertyGenerator(const ParticlePropertyGenerator& particlePropertyGenerator) = delete;
			///// <summary> Particle generator move constructor. </summary>
			//ParticlePropertyGenerator(ParticlePropertyGenerator&& particlePropertyGenerator) = delete;
			///// <summary> Particle generator copy assignment operator. </summary>
			//ParticlePropertyGenerator& operator=(const ParticlePropertyGenerator& particlePropertyGenerator) = delete;
			///// <summary> Particle generator move assignment operator. </summary>
			//ParticlePropertyGenerator& operator=(ParticlePropertyGenerator&& particlePropertyGenerator) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			virtual void Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId) = 0;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class ParticlePropertyGenerator */

		/// <summary>
		/// Generates position for the particle.
		/// </summary>
		class PositionGenerator : public ParticlePropertyGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			PositionGenerator()
			{
			}
			virtual ~PositionGenerator()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		protected:
			void Set(ParticlesContainer* particleContainer, size_t i, const Math::Vector3D& position)
			{
				particleContainer->SetPosition(i, position);
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			/* ==================== Non-static member variables end ==================== */
		}; /* end class PositionGenerator */

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
		class VelocityGenerator : public ParticlePropertyGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			VelocityGenerator()
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
		/// Generates life span limit for the particle.
		/// </summary>
		class LifeSpanLimitGenerator : public ParticlePropertyGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			LifeSpanLimitGenerator()
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
		/// Generates ID generator for the particle.
		/// </summary>
		class IdGenerator : public ParticlePropertyGenerator
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			IdGenerator()
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
	} /* end namespace Particles */
} /* end namespace Rendering */

#endif /* __RENDERING_PARTICLE_PROPERTY_GENERATOR_H__ */