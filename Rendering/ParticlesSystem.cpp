#include "stdafx.h"
#include "ParticlesSystem.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

#include "Math/FloatingPoint.h"

#include "Utility/ILogger.h"

rendering::particles::ParticlesSystem::ParticlesSystem(const ParticlesSystemBuilder& particlesSystemBuilder) :
	m_maxCount(particlesSystemBuilder.GetMaxCount()),
	m_particles(particlesSystemBuilder.GetMaxCount(), particlesSystemBuilder.GetAttributesMask()),
	m_emitters(particlesSystemBuilder.GetEmitters()),
	m_updaters(particlesSystemBuilder.GetUpdaters()),
	m_killer(particlesSystemBuilder.GetKiller()),
	m_meshId(particlesSystemBuilder.GetMeshId()),
	m_textureId(particlesSystemBuilder.GetTextureId()),
	m_textureAtlasRowsCount(particlesSystemBuilder.GetTextureAtlasRowsCount()),
	m_isAdditive(particlesSystemBuilder.IsAdditive()),
	m_shaderId(particlesSystemBuilder.GetShaderId()),
	m_uniformColor(particlesSystemBuilder.GetUniformColor())
{
	// TODO: Determine m_meshId and m_shaderID based on the given attributesMask
}

rendering::particles::ParticlesSystem::~ParticlesSystem()
{
}

void rendering::particles::ParticlesSystem::Update(math::Real deltaTime)
{
	DEBUG_LOG_RENDERING("Killing particles started. Delta time = ", deltaTime);
	if (m_killer != nullptr)
	{
		m_killer->Kill(deltaTime, &m_particles);
	}

	DEBUG_LOG_RENDERING("Updating particles started. Delta time = ", deltaTime);
	//CRITICAL_LOG_RENDERING("Particles after emission, before update = ");
	//ERROR_LOG_RENDERING(m_particles);
	for (auto& updater : m_updaters)
	{
		updater->Update(deltaTime, &m_particles);
	}
	//CRITICAL_LOG_RENDERING("Particles after update = ");
	//ERROR_LOG_RENDERING(m_particles);

	DEBUG_LOG_RENDERING("Emitting new particles started. Delta time = ", deltaTime);
	//CRITICAL_LOG_RENDERING("Particles before emission = ");
	//ERROR_LOG_RENDERING(m_particles);
	for (auto& emitter : m_emitters)
	{
		emitter.Emit(deltaTime, &m_particles);
	}

	//constexpr math::Vector3D zeroVector{ REAL_ZERO, REAL_ZERO, REAL_ZERO };
	//for (size_t i = 0; i < m_count; ++i)
	//{
	//	m_particles.SetAcceleration(i, zeroVector);
	//}
}

void rendering::particles::ParticlesSystem::Reset()
{
	m_particles.Reset();
}

std::vector<math::Real> rendering::particles::ParticlesSystem::GetInstanceData(const Camera* camera, std::vector<math::Real>& particleInstanceVboData) const
{
	const auto cameraViewMatrix = camera->GetViewMatrix();
	const auto cameraViewProjectionMatrix = camera->GetViewProjection();
	particleInstanceVboData.clear();
	for (size_t i = 0; i < GetAliveParticlesCount(); ++i)
	{
		math::Matrix4D modelMatrix(m_particles.GetPosition(i));
		// To make the particle always face the camera we can either use the geometry shader (as in the Bilboard shader) or
		// set the 3x3 top-left submatrix of the model matrix to be a transposed version of the 3x3 top-left submatrix of the camera's view matrix.
		for (int row = 0; row < math::Matrix4D::SIZE - 1; ++row)
		{
			for (int col = 0; col < math::Matrix4D::SIZE - 1; ++col)
			{
				modelMatrix.SetElement(row, col, cameraViewMatrix.GetElement(col, row));
			}
		}

		if (IsAttributeEnabled(particles::attributes::ROTATION))
		{
			const auto particleRotation(math::Quaternion(math::Vector3D(0.0f, 0.0f, 1.0f), m_particles.GetRotation(i)).ToRotationMatrix());
			if (IsAttributeEnabled(particles::attributes::SCALE) && !math::AlmostEqual(m_particles.GetScale(i), REAL_ONE))
			{
				modelMatrix = modelMatrix * particleRotation * math::Matrix4D(m_particles.GetScale(i));
			}
			else
			{
				modelMatrix = modelMatrix * particleRotation;
			}
		}
		else if (IsAttributeEnabled(particles::attributes::SCALE) && !math::AlmostEqual(m_particles.GetScale(i), REAL_ONE))
		{
			modelMatrix = modelMatrix * math::Matrix4D(m_particles.GetScale(i));
		}

		const auto mvpMatrix = cameraViewProjectionMatrix * modelMatrix;
		for (int row = 0; row < math::Matrix4D::SIZE; ++row)
		{
			for (int col = 0; col < math::Matrix4D::SIZE; ++col)
			{
				particleInstanceVboData.push_back(mvpMatrix.GetElement(row, col));
			}
		}

//#ifdef TEXTURE_ATLAS_OFFSET_CALCULATION		
//		math::Vector2D textureOffset0;
//		math::Vector2D textureOffset1;
//		math::Real textureAtlasBlendFactor;
//		particles[i].CalculateTextureAtlasInfo(particleTexture->GetRowsCount(), textureOffset0, textureOffset1, textureAtlasBlendFactor);
//		m_particleInstanceVboData.push_back(textureOffset0.GetX());
//		m_particleInstanceVboData.push_back(textureOffset0.GetY());
//		m_particleInstanceVboData.push_back(textureOffset1.GetX());
//		m_particleInstanceVboData.push_back(textureOffset1.GetY());
//		m_particleInstanceVboData.push_back(textureAtlasBlendFactor);
//#else
//		particleInstanceVboData.push_back(CalculateLifeStageFactor(i));
//#endif

		if (IsAttributeEnabled(attributes::COLOR))
		{
			const auto& color = m_particles.GetColor(i);
			particleInstanceVboData.push_back(color.GetRed());
			particleInstanceVboData.push_back(color.GetGreen());
			particleInstanceVboData.push_back(color.GetBlue());
			particleInstanceVboData.push_back(color.GetAlpha());
		}
	}
	return particleInstanceVboData;
}