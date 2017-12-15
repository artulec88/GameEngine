#include "stdafx.h"
#include "MeshFactory.h"
#include "MeshIDs.h"
#include "stb_image.h"

#include "Utility/IConfig.h"
#include "Utility/FileManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#define MEASURE_MESH_TIME_ENABLED

rendering::MeshFactory::MeshFactory(const std::string& modelsDirectory, const std::string& texturesDirectory) :
	m_modelsDirectory(modelsDirectory),
	m_texturesDirectory(texturesDirectory)
	// If (: we initialize the map this way then Mesh's copy ctrs will be used (instead of move ctrs). Google that!
	//m_meshType2MeshMap({ { MeshTypes::DEFAULT, std::move(Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultTexture", "defaultTexture"))) } })
{
	CreateMesh(mesh_ids::SIMPLE_PLANE, GET_CONFIG_VALUE_STR_RENDERING("simpleQuadMeshFileName", "plane4.obj"));
	m_meshType2MeshMap.insert(make_pair(mesh_ids::PARTICLE,
		std::make_unique<InstanceMesh>(std::vector<math::Vector2D>{ math::Vector2D(-0.5f, -0.5f), math::Vector2D(-0.5f, 0.5f), math::Vector2D(0.5f, -0.5f), math::Vector2D(0.5f, 0.5f) }.data(),
			4, GET_CONFIG_VALUE_RENDERING("maxParticlesCount", 10000), std::vector<GLint>{4, 4, 4, 4, 1}))); // TODO: The "maxParticlesCount" variable is also retrieved in the Renderer class.
	m_meshType2MeshMap.insert(make_pair(mesh_ids::PARTICLE_COLOR,
		std::make_unique<InstanceMesh>(std::vector<math::Vector2D>{ math::Vector2D(-0.5f, -0.5f), math::Vector2D(-0.5f, 0.5f), math::Vector2D(0.5f, -0.5f), math::Vector2D(0.5f, 0.5f) }.data(),
		4, GET_CONFIG_VALUE_RENDERING("maxParticlesCount", 10000), std::vector<GLint>{4, 4, 4, 4, 4}))); // TODO: The "maxParticlesCount" variable is also retrieved in the Renderer class.))
	m_meshType2MeshMap.insert(make_pair(mesh_ids::PARTICLE_UNIFORM_COLOR,
		std::make_unique<InstanceMesh>(std::vector<math::Vector2D>{ math::Vector2D(-0.5f, -0.5f), math::Vector2D(-0.5f, 0.5f), math::Vector2D(0.5f, -0.5f), math::Vector2D(0.5f, 0.5f) }.data(),
			4, GET_CONFIG_VALUE_RENDERING("maxParticlesCount", 10000), std::vector<GLint>{4, 4, 4, 4}))); // TODO: The "maxParticlesCount" variable is also retrieved in the Renderer class.))

#ifdef DEBUG_RENDERING_ENABLED
	m_meshType2MeshMap.insert(make_pair(mesh_ids::DEBUG,
		std::make_unique<Mesh>(std::vector<math::Vector2D>{ math::Vector2D(-REAL_ONE, REAL_ONE), math::Vector2D(REAL_ONE, REAL_ONE), math::Vector2D(-REAL_ONE, -REAL_ONE), math::Vector2D(REAL_ONE, -REAL_ONE) }.data(),
			nullptr, 4, GL_TRIANGLE_STRIP)));
#endif
}


rendering::MeshFactory::~MeshFactory()
{
}

const rendering::Mesh* rendering::MeshFactory::CreateMesh(int meshId, const std::string& meshFileName)
{
	CheckErrorCode(__FUNCTION__, "Started reading the mesh model.");
	CHECK_CONDITION_RENDERING(!meshFileName.empty(), Utility::Logging::ERR, "Mesh data cannot be initialized. File name is not specified");

#ifdef MEASURE_MESH_TIME_ENABLED
	Utility::timing::Timer timer;
	timer.Start();
#endif
	INFO_LOG_RENDERING("Loading model from file \"", meshFileName, "\"");

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile((m_modelsDirectory + meshFileName).c_str(),
		aiProcess_Triangulate | /* aiProcess_FlipWindingOrder | */
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	CHECK_CONDITION_EXIT_RENDERING(scene != nullptr, Utility::Logging::CRITICAL, "Error while loading a mesh \"", meshFileName, "\"");
	CHECK_CONDITION_EXIT_RENDERING((scene->mMeshes != nullptr) && (scene->mNumMeshes > 0), Utility::Logging::CRITICAL,
		"Incorrect number of meshes loaded- ", scene->mNumMeshes, "- check the model \"", meshFileName,
		"\". One of the possible solutions is to check whether the model has any additional lines at the end.");

	const aiMesh* model = scene->mMeshes[0];
	std::vector<math::Vector3D> positions;
	positions.reserve(model->mNumVertices);
	std::vector<math::Vector2D> textureCoordinates;
	textureCoordinates.reserve(model->mNumVertices);
	std::vector<math::Vector3D> normals;
	normals.reserve(model->mNumVertices);
	std::vector<math::Vector3D> tangents;
	tangents.reserve(model->mNumVertices);
	std::vector<math::Vector3D> bitangents;
	bitangents.reserve(model->mNumVertices);

	const aiVector3D aiZeroVector(REAL_ZERO, REAL_ZERO, REAL_ZERO);
	for (unsigned int i = 0; i < model->mNumVertices; ++i)
	{
		const auto* pPos = &model->mVertices[i];
		const auto pNormal = &model->mNormals[i];
		const auto pTexCoord = model->HasTextureCoords(0) ? &model->mTextureCoords[0][i] : &aiZeroVector;
		auto pTangent = model->HasTangentsAndBitangents() ? &model->mTangents[i] : &aiZeroVector;
		if (pTangent == nullptr)
		{
			ERROR_LOG_RENDERING("Tangent calculated incorrectly for the mesh file name \"", meshFileName, "\"");
			pTangent = &aiZeroVector;
		}
		//const aiVector3D* pBitangent = model->HasTangentsAndBitangents() ? &(model->mBitangents[i]) : &aiZeroVector;
		//if (pBitangent == nullptr)
		//{
		//	ERROR_LOG_RENDERING("Bitangent calculated incorrectly");
		//	pBitangent = &aiZeroVector;
		//}

		//math::Vector3D vertexBitangent(pBitangent->x, pBitangent->y, pBitangent->z);

		positions.emplace_back(pPos->x, pPos->y, pPos->z);
		textureCoordinates.emplace_back(pTexCoord->x, pTexCoord->y);
		normals.emplace_back(pNormal->x, pNormal->y, pNormal->z);
		tangents.emplace_back(pTangent->x, pTangent->y, pTangent->z);
	}

	std::vector<int> indices;
	indices.reserve(model->mNumFaces * 3 /* each face is a triangle and has 3 vertices */);
	for (unsigned int i = 0; i < model->mNumFaces; ++i)
	{
		const aiFace& face = model->mFaces[i];
		CHECK_CONDITION_ALWAYS_RENDERING(face.mNumIndices == 3, utility::logging::WARNING, "The face has ", face.mNumIndices, " indices when only triangle faces are supported.");
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
#ifdef MEASURE_MESH_TIME_ENABLED
	timer.Stop();
	INFO_LOG_RENDERING("Loading model took ", timer.GetTimeSpan().ToString());
#endif
	CheckErrorCode(__FUNCTION__, "Finished model loading");

	const auto meshPair = m_meshType2MeshMap.insert(make_pair(meshId, std::make_unique<Mesh>(indices.data(), indices.size(),
		positions.size(), positions.data(), textureCoordinates.data(), normals.data(), tangents.data(), bitangents.data())));
	CHECK_CONDITION_RENDERING(meshPair.second, Utility::Logging::WARNING, "Mesh \"", meshFileName, "\" has already been created.");
	return meshPair.first->second.get();
}

const rendering::Mesh* rendering::MeshFactory::CreateMeshFromSurface(int meshId, const math::Surface& surface)
{
	std::vector<math::Vector3D> positions;
	positions.reserve(surface.GetHorizontalVerticesCount() * surface.GetVerticalVerticesCount());
	std::vector<math::Vector2D> textureCoordinates;
	textureCoordinates.reserve(surface.GetHorizontalVerticesCount() * surface.GetVerticalVerticesCount());
	std::vector<math::Vector3D> normals;
	normals.reserve(surface.GetHorizontalVerticesCount() * surface.GetVerticalVerticesCount());
	std::vector<math::Vector3D> tangents;
	tangents.reserve(surface.GetHorizontalVerticesCount() * surface.GetVerticalVerticesCount());
	for (auto z = 0; z < surface.GetVerticalVerticesCount(); ++z)
	{
		const auto zReal = static_cast<math::Real>(z);
		for (auto x = 0; x < surface.GetHorizontalVerticesCount(); ++x)
		{
			const auto xReal = static_cast<math::Real>(x);
			//CRITICAL_LOG_RENDERING("Height[", x, "][", z, "] = ", terrainHeight);
			positions.emplace_back(surface.GetPositionAt(x, z));
			//CRITICAL_LOG_RENDERING("counter = ", positions.size(), "; x = ", x, "; z = ", z, "; Position = ", positions.back());
			textureCoordinates.emplace_back(xReal / (surface.GetHorizontalVerticesCount() - 1), zReal / (surface.GetVerticalVerticesCount() - 1));
			normals.push_back(CalculateNormal(x, z, surface));
			tangents.emplace_back(REAL_ONE, REAL_ZERO, REAL_ZERO); // TODO: Calculate tangent
		}
	}

	std::vector<int> indices;
	indices.reserve((surface.GetHorizontalVerticesCount() - 1) * (surface.GetVerticalVerticesCount() - 1) * 6);
	for (auto gz = 0; gz < surface.GetVerticalVerticesCount() - 1; ++gz)
	{
		for (auto gx = 0; gx < surface.GetHorizontalVerticesCount() - 1; ++gx)
		{
			const auto bottomLeft = gz * surface.GetHorizontalVerticesCount() + gx;
			const auto bottomRight = bottomLeft + 1;
			const auto topLeft = (gz + 1) * surface.GetHorizontalVerticesCount() + gx;
			const auto topRight = topLeft + 1;
			indices.push_back(bottomLeft);
			indices.push_back(topLeft);
			indices.push_back(bottomRight);
			indices.push_back(bottomRight);
			indices.push_back(topLeft);
			indices.push_back(topRight);
		}
	}

	//for (size_t i = 0; i < indices.size(); ++i)
	//{
	//	ERROR_LOG_RENDERING("index[", i, "]: ", indices[i]);
	//}

	//#ifdef DELOCUST_ENABLED
	//int i = 0;
	//for (std::vector<Vertex>::const_iterator vertexItr = vertices.begin(); vertexItr != vertices.end(); ++vertexItr, ++i)
	//{
	//CRITICAL_LOG_RENDERING("vertex[", i, "]:\n\tPos:\t", vertexItr->m_pos, "\n\tTex:\t", vertexItr->m_texCoord, "\n\tNormal:\t", vertexItr->m_normal);
	//}
	//for (size_t i = 0; i < INDICES_COUNT; ++i)
	//{
	//	ERROR_LOG_RENDERING("index[", i, "]: ", indices[i]);
	//}
	//#endif

	const auto meshPair = m_meshType2MeshMap.insert(make_pair(meshId, std::make_unique<Mesh>(indices.data(), indices.size(),
		positions.size(), positions.data(), textureCoordinates.data(), normals.data(), tangents.data())));
	CHECK_CONDITION_RENDERING(meshPair.second, Utility::Logging::WARNING, "Mesh \"", meshFileName, "\" has already been created.");
	DEBUG_LOG_RENDERING("Terrain mesh has been created.");
	return meshPair.first->second.get();
}

math::Vector3D rendering::MeshFactory::CalculateNormal(int x, int z, const math::Surface& surface) const
{
	const auto heightLeft = x >= 1 ? surface.GetHeightAt(x - 1, z) : REAL_ZERO;
	const auto heightRight = x < surface.GetHorizontalVerticesCount() - 1 ? surface.GetHeightAt(x + 1, z) : REAL_ZERO;
	const auto heightDown = z >= 1 ? surface.GetHeightAt(x, z - 1) : REAL_ZERO;
	const auto heightUp = z < surface.GetVerticalVerticesCount() - 1 ? surface.GetHeightAt(x, z + 1) : REAL_ZERO;
	math::Vector3D normal(heightLeft - heightRight, 2.0f, heightDown - heightUp);
	normal.Normalize();
	return normal;
}