#include "stdafx.h"
#include "MeshFactory.h"
#include "MeshIDs.h"
#include "stb_image.h"

#include "Utility\IConfig.h"
#include "Utility\FileManager.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

//#define MEASURE_MESH_TIME_ENABLED

Rendering::MeshFactory::MeshFactory(const std::string& modelsDirectory, const std::string& texturesDirectory) :
	m_modelsDirectory(modelsDirectory),
	m_texturesDirectory(texturesDirectory)
	// If (: we initialize the map this way then Mesh's copy ctrs will be used (instead of move ctrs). Google that!
	//m_meshType2MeshMap({ { MeshTypes::DEFAULT, std::move(Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultTexture", "defaultTexture"))) } })
{
	CreateMesh(MeshIDs::SIMPLE_PLANE, GET_CONFIG_VALUE_STR_RENDERING("simpleQuadMeshFileName", "plane4.obj"));
#ifdef TEXTURE_ATLAS_OFFSET_CALCULATION
	m_meshType2MeshMap.insert(std::make_pair(MeshIDs::PARTICLE,
		std::make_unique<InstanceMesh>(std::vector<math::Vector2D>{ math::Vector2D(-0.5f, -0.5f), math::Vector2D(-0.5f, 0.5f), math::Vector2D(0.5f, -0.5f), math::Vector2D(0.5f, 0.5f) }.data(),
			4, GET_CONFIG_VALUE_RENDERING("maxParticlesCount", 10000), 21)));
#else
	m_meshType2MeshMap.insert(std::make_pair(MeshIDs::PARTICLE,
		std::make_unique<InstanceMesh>(std::vector<math::Vector2D>{ math::Vector2D(-0.5f, -0.5f), math::Vector2D(-0.5f, 0.5f), math::Vector2D(0.5f, -0.5f), math::Vector2D(0.5f, 0.5f) }.data(),
			4, GET_CONFIG_VALUE_RENDERING("maxParticlesCount", 10000), 17))); // TODO: The "maxParticlesCount" variable is also retrieved in the Renderer class.
#endif

#ifdef DEBUG_RENDERING_ENABLED
	m_meshType2MeshMap.insert(std::make_pair(MeshIDs::DEBUG,
		std::make_unique<Mesh>(std::vector<math::Vector2D>{ math::Vector2D(-REAL_ONE, REAL_ONE), math::Vector2D(REAL_ONE, REAL_ONE), math::Vector2D(-REAL_ONE, -REAL_ONE), math::Vector2D(REAL_ONE, -REAL_ONE) }.data(),
			nullptr, 4, GL_TRIANGLE_STRIP)));
#endif
}


Rendering::MeshFactory::~MeshFactory()
{
}

const Rendering::Mesh* Rendering::MeshFactory::CreateMesh(int meshID, const std::string& meshFileName)
{
	Rendering::CheckErrorCode(__FUNCTION__, "Started reading the mesh model.");
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
		const aiVector3D* pPos = &(model->mVertices[i]);
		const aiVector3D* pNormal = &(model->mNormals[i]);
		const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;
		const aiVector3D* pTangent = model->HasTangentsAndBitangents() ? &(model->mTangents[i]) : &aiZeroVector;
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
	Rendering::CheckErrorCode(__FUNCTION__, "Finished model loading");

	std::pair<std::map<int, std::unique_ptr<Mesh>>::iterator, bool> meshPair =
		m_meshType2MeshMap.insert(std::make_pair(meshID, std::make_unique<Mesh>(indices.data(), indices.size(),
			positions.size(), positions.data(), textureCoordinates.data(), normals.data(), tangents.data(), bitangents.data())));
	CHECK_CONDITION_RENDERING(meshPair.second, Utility::Logging::WARNING, "Mesh \"", meshFileName, "\" has already been created.");
	return meshPair.first->second.get();
}

const Rendering::Mesh* Rendering::MeshFactory::CreateMeshFromSurface(int meshID, const math::Surface& surface)
{
	std::vector<math::Vector3D> positions;
	positions.reserve(surface.GetHorizontalVerticesCount() * surface.GetVerticalVerticesCount());
	std::vector<math::Vector2D> textureCoordinates;
	textureCoordinates.reserve(surface.GetHorizontalVerticesCount() * surface.GetVerticalVerticesCount());
	std::vector<math::Vector3D> normals;
	normals.reserve(surface.GetHorizontalVerticesCount() * surface.GetVerticalVerticesCount());
	std::vector<math::Vector3D> tangents;
	tangents.reserve(surface.GetHorizontalVerticesCount() * surface.GetVerticalVerticesCount());
	for (int z = 0; z < surface.GetVerticalVerticesCount(); ++z)
	{
		const math::Real zReal = static_cast<math::Real>(z);
		for (int x = 0; x < surface.GetHorizontalVerticesCount(); ++x)
		{
			const math::Real xReal = static_cast<math::Real>(x);
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
	for (int gz = 0; gz < surface.GetVerticalVerticesCount() - 1; ++gz)
	{
		for (int gx = 0; gx < surface.GetHorizontalVerticesCount() - 1; ++gx)
		{
			int bottomLeft = (gz * surface.GetHorizontalVerticesCount()) + gx;
			int bottomRight = bottomLeft + 1;
			int topLeft = ((gz + 1) * surface.GetHorizontalVerticesCount()) + gx;
			int topRight = topLeft + 1;
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

	std::pair<std::map<int, std::unique_ptr<Mesh>>::iterator, bool> meshPair =
		m_meshType2MeshMap.insert(std::make_pair(meshID, std::make_unique<Mesh>(indices.data(), indices.size(),
			positions.size(), positions.data(), textureCoordinates.data(), normals.data(), tangents.data())));
	CHECK_CONDITION_RENDERING(meshPair.second, Utility::Logging::WARNING, "Mesh \"", meshFileName, "\" has already been created.");
	return meshPair.first->second.get();

	DEBUG_LOG_RENDERING("Terrain mesh has been created.");
}

math::Real Rendering::MeshFactory::CalculateHeightAt(int x, int z, const Image& heightMapImage, math::Real heightMapMaxHeight) const
{
	constexpr math::Real MAX_PIXEL_COLOR = 255.0f; // The maximum value for color of the single pixel in the height map.

	math::Real height = static_cast<math::Real>(heightMapImage.GetPixelAt(z, x)) / MAX_PIXEL_COLOR;
	//CRITICAL_LOG_RENDERING("Height[", x, "][", z, "] = ", height);
	height = (height - 0.5f) * 2.0f * heightMapMaxHeight; // rescaling the height so that it is within range [-heightMapMaxHeight; heightMapMaxHeight].
	return height;
}

math::Real Rendering::MeshFactory::CalculateHeightAt(int x, int z, int heightMapWidth, const math::HeightsGenerator& heightsGenerator) const
{
	// TODO: Range checking
	CHECK_CONDITION_RETURN_RENDERING(x >= 0 && x < m_heightMapWidth && z >= 0 && z < m_heightMapHeight, REAL_ZERO,
		Utility::Logging::ERR, "Cannot determine the height of the terrain on (", x, ", ", z, ") position. It is out of range.");
	const int heightMapIndex = GetHeightMapIndex(x, z, heightMapWidth);
	CHECK_CONDITION_RENDERING(heightMapIndex >= 0 && heightMapIndex < m_heightMapWidth * m_heightMapHeight, Utility::Logging::ERR,
		"The heightmap index calculation is incorrect. Calculated index (", heightMapIndex, ") is out of range [0; ", m_heightMapWidth * m_heightMapHeight, ")");
	//DEBUG_LOG_RENDERING("Heightmap index for [", x, ", ", z, "] = ", heightMapIndex);
	math::Real height = heightsGenerator.GenerateHeight(static_cast<math::Real>(x), static_cast<math::Real>(z));
	return height;
}

math::Vector3D Rendering::MeshFactory::CalculateNormal(int x, int z, const math::Surface& surface) const
{
	math::Real heightLeft = ((x - 1) >= 0) ? surface.GetHeightAt(x - 1, z) : REAL_ZERO;
	math::Real heightRight = ((x + 1) < surface.GetHorizontalVerticesCount()) ? surface.GetHeightAt(x + 1, z) : REAL_ZERO;
	math::Real heightDown = ((z - 1) >= 0) ? surface.GetHeightAt(x, z - 1) : REAL_ZERO;
	math::Real heightUp = ((z + 1) < surface.GetVerticalVerticesCount()) ? surface.GetHeightAt(x, z + 1) : REAL_ZERO;
	math::Vector3D normal(heightLeft - heightRight, 2.0f, heightDown - heightUp);
	normal.Normalize();
	return normal;
}