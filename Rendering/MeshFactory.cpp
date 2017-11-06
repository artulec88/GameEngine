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
		std::make_unique<InstanceMesh>(std::vector<Math::Vector2D>{ Math::Vector2D(-0.5f, -0.5f), Math::Vector2D(-0.5f, 0.5f), Math::Vector2D(0.5f, -0.5f), Math::Vector2D(0.5f, 0.5f) }.data(),
			4, GET_CONFIG_VALUE_RENDERING("maxParticlesCount", 10000), 21)));
#else
	m_meshType2MeshMap.insert(std::make_pair(MeshIDs::PARTICLE,
		std::make_unique<InstanceMesh>(std::vector<Math::Vector2D>{ Math::Vector2D(-0.5f, -0.5f), Math::Vector2D(-0.5f, 0.5f), Math::Vector2D(0.5f, -0.5f), Math::Vector2D(0.5f, 0.5f) }.data(),
			4, GET_CONFIG_VALUE_RENDERING("maxParticlesCount", 10000), 17))); // TODO: The "maxParticlesCount" variable is also retrieved in the Renderer class.
#endif

#ifdef DEBUG_RENDERING_ENABLED
	m_meshType2MeshMap.insert(std::make_pair(MeshIDs::DEBUG,
		std::make_unique<Mesh>(std::vector<Math::Vector2D>{ Math::Vector2D(-REAL_ONE, REAL_ONE), Math::Vector2D(REAL_ONE, REAL_ONE), Math::Vector2D(-REAL_ONE, -REAL_ONE), Math::Vector2D(REAL_ONE, -REAL_ONE) }.data(),
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
	Utility::Timing::Timer timer;
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
	std::vector<Math::Vector3D> positions;
	positions.reserve(model->mNumVertices);
	std::vector<Math::Vector2D> textureCoordinates;
	textureCoordinates.reserve(model->mNumVertices);
	std::vector<Math::Vector3D> normals;
	normals.reserve(model->mNumVertices);
	std::vector<Math::Vector3D> tangents;
	tangents.reserve(model->mNumVertices);
	std::vector<Math::Vector3D> bitangents;
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

		//Math::Vector3D vertexBitangent(pBitangent->x, pBitangent->y, pBitangent->z);

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
		CHECK_CONDITION_ALWAYS_RENDERING(face.mNumIndices == 3, Utility::Logging::WARNING, "The face has ", face.mNumIndices, " indices when only triangle faces are supported.");
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

const Rendering::Mesh* Rendering::MeshFactory::CreateMeshFromHeightMap(int meshID, const std::string& heightMapFileName, Math::Real heightMapMaxHeight /* = 5.0f */)
{
	/* Loading heightmap begin */
	std::string name = m_texturesDirectory + heightMapFileName;
	int bytesPerPixel, heightMapWidth, heightMapHeight;
	ERROR_LOG_RENDERING("Failure reason before = \"", stbi_failure_reason(), "\".");
	unsigned char* heightMapData = stbi_load(name.c_str(), &heightMapWidth, &heightMapHeight, &bytesPerPixel,
		1 /* we only care about one RED component for now (the heightmap is grayscale, so we could use GREEN or BLUE components as well) */);
	ERROR_LOG_RENDERING("Failure reason after = \"", stbi_failure_reason(), "\".");
	CHECK_CONDITION_EXIT_RENDERING(heightMapData != nullptr, Utility::Logging::ERR, "Unable to load terrain height map from the file \"", name, "\"");
	CHECK_CONDITION_RENDERING(m_heightMapWidth < 32768 && m_heightMapHeight < 32768, Utility::Logging::EMERGENCY, "The heightmap's size is too big to be used in the rendering engine.");
	//for (int i = 0; i < heightMapWidth; ++i)
	//{
	//	for (int j = 0; j < heightMapHeight; ++j)
	//	{
	//		CRITICAL_LOG_RENDERING("HeightMap[", i * heightMapWidth + j, "] ([", i, "][", j, "]) = \"",
	//			static_cast<int>(heightMapData[i * heightMapWidth + j]), "\".");
	//	}
	//}
	/* Loading heightmap finished */

	std::vector<Math::Real> heights;
	heights.reserve(heightMapWidth * heightMapHeight);
	std::vector<Math::Vector3D> positions;
	positions.reserve(heightMapWidth * heightMapHeight);
	std::vector<Math::Vector2D> textureCoordinates;
	textureCoordinates.reserve(heightMapWidth * heightMapHeight);
	std::vector<Math::Vector3D> normals;
	normals.reserve(heightMapWidth * heightMapHeight);
	std::vector<Math::Vector3D> tangents;
	tangents.reserve(heightMapWidth * heightMapHeight);
	for (int z = heightMapHeight - 1; z >= 0; --z)
	{
		const Math::Real zReal = static_cast<Math::Real>(z);
		for (int x = 0; x < heightMapWidth; ++x)
		{
			const Math::Real xReal = static_cast<Math::Real>(x);
			Math::Real terrainHeight = CalculateHeightAt(x, z, heightMapWidth, heightMapHeight, heightMapMaxHeight, heightMapData);
			//CRITICAL_LOG_RENDERING("Height[", x, "][", z, "] = ", terrainHeight);
			heights.push_back(terrainHeight);
			//DEBUG_LOG_RENDERING("counter = ", positions.size(), "; x = ", x, "; z = ", z, "; Position = ", position);
			positions.emplace_back(xReal / (heightMapWidth - 1), terrainHeight, zReal / (heightMapHeight - 1));
			textureCoordinates.emplace_back(xReal / (heightMapWidth - 1), zReal / (heightMapHeight - 1));
			normals.push_back(CalculateNormal(x, z, heightMapWidth, heightMapHeight, heightMapMaxHeight, heightMapData));
			tangents.emplace_back(REAL_ONE, REAL_ZERO, REAL_ZERO); // TODO: Calculate tangent
		}
	}
	stbi_image_free(heightMapData);

	std::vector<int> indices;
	indices.reserve((heightMapWidth - 1) * (heightMapHeight - 1) * 6);
	for (int gz = 0; gz < heightMapHeight - 1; ++gz)
	{
		for (int gx = 0; gx < heightMapWidth - 1; ++gx)
		{
			int topLeft = (gz * heightMapHeight) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * heightMapHeight) + gx;
			int bottomRight = bottomLeft + 1;
			indices.push_back(topLeft);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(bottomRight);
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

Math::Real Rendering::MeshFactory::CalculateHeightAt(int x, int z, int heightMapWidth, int heightMapHeight, Math::Real heightMapMaxHeight, unsigned char* heightMapData) const
{
	constexpr Math::Real MAX_PIXEL_COLOR = 255.0f; // The maximum value for color of the single pixel in the height map.

	// TODO: Range checking
	CHECK_CONDITION_RETURN_RENDERING(x >= 0 && x < heightMapWidth && z >= 0 && z < heightMapHeight, REAL_ZERO,
		Utility::Logging::ERR, "Cannot determine the height of the terrain on (", x, ", ", z, ") position. It is out of range.");
	const int heightMapIndex = GetHeightMapIndex(x, z, heightMapWidth);
	CHECK_CONDITION_RENDERING(heightMapIndex >= 0 && heightMapIndex < heightMapWidth * heightMapHeight, Utility::Logging::ERR,
		"The heightmap index calculation is incorrect. Calculated index (", heightMapIndex, ") is out of range [0; ", heightMapWidth * heightMapHeight, ")");
	//DEBUG_LOG_RENDERING("Heightmap index for [", x, ", ", z, "] = ", heightMapIndex);
	Math::Real height = static_cast<Math::Real>(heightMapData[heightMapIndex]) / MAX_PIXEL_COLOR;
	//CRITICAL_LOG_RENDERING("Height[", x, "][", z, "] = ", height);
	height = (height - 0.5f) * 2.0f * heightMapMaxHeight; // rescaling the height so that it is within range [-heightMapMaxHeight; heightMapMaxHeight].
	return height;
}

Math::Real Rendering::MeshFactory::CalculateHeightAt(int x, int z, int heightMapWidth, const Math::HeightsGenerator& heightsGenerator) const
{
	// TODO: Range checking
	CHECK_CONDITION_RETURN_RENDERING(x >= 0 && x < m_heightMapWidth && z >= 0 && z < m_heightMapHeight, REAL_ZERO,
		Utility::Logging::ERR, "Cannot determine the height of the terrain on (", x, ", ", z, ") position. It is out of range.");
	const int heightMapIndex = GetHeightMapIndex(x, z, heightMapWidth);
	CHECK_CONDITION_RENDERING(heightMapIndex >= 0 && heightMapIndex < m_heightMapWidth * m_heightMapHeight, Utility::Logging::ERR,
		"The heightmap index calculation is incorrect. Calculated index (", heightMapIndex, ") is out of range [0; ", m_heightMapWidth * m_heightMapHeight, ")");
	//DEBUG_LOG_RENDERING("Heightmap index for [", x, ", ", z, "] = ", heightMapIndex);
	Math::Real height = heightsGenerator.GenerateHeight(static_cast<Math::Real>(x), static_cast<Math::Real>(z));
	return height;
}

Math::Vector3D Rendering::MeshFactory::CalculateNormal(int x, int z, int heightMapWidth, int heightMapHeight, Math::Real heightMapMaxHeight, unsigned char* heightMapData) const
{
	Math::Real heightLeft = ((x - 1) >= 0) ? CalculateHeightAt(x - 1, z, heightMapWidth, heightMapHeight, heightMapMaxHeight, heightMapData) : REAL_ZERO;
	Math::Real heightRight = ((x + 1) < heightMapWidth) ? CalculateHeightAt(x + 1, z, heightMapWidth, heightMapHeight, heightMapMaxHeight, heightMapData) : REAL_ZERO;
	Math::Real heightDown = ((z - 1) >= 0) ? CalculateHeightAt(x, z - 1, heightMapWidth, heightMapHeight, heightMapMaxHeight, heightMapData) : REAL_ZERO;
	Math::Real heightUp = ((z + 1) < heightMapHeight) ? CalculateHeightAt(x, z + 1, heightMapWidth, heightMapHeight, heightMapMaxHeight, heightMapData) : REAL_ZERO;
	Math::Vector3D normal(heightLeft - heightRight, 2.0f, heightDown - heightUp);
	normal.Normalize();
	return normal;
}