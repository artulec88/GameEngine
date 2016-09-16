#include "stdafx.h"
#include "MeshFactory.h"
#include "MeshIDs.h"

#include <Utility\IConfig.h>
#include "Utility\FileManager.h"

Rendering::MeshFactory::MeshFactory(const std::string& modelsDirectory) :
	m_modelsDirectory(modelsDirectory)
	// TODO: If we initialize the map this way then Mesh's copy ctrs will be used (instead of move ctrs). Google that!
	//m_meshType2MeshMap({ { MeshTypes::DEFAULT, std::move(Texture(GET_CONFIG_VALUE_STR_ENGINE("defaultTexture", "defaultTexture"))) } })
{
	// This way we make sure that Mesh's move ctrs are used instead of copy ctrs.
	m_meshType2MeshMap.insert(std::make_pair(MeshIDs::SIMPLE_QUAD, Mesh(GET_CONFIG_VALUE_STR_RENDERING("simpleQuadMeshFileName", "plane4.obj"))));
}


Rendering::MeshFactory::~MeshFactory()
{
}

const Rendering::Mesh* Rendering::MeshFactory::CreateMesh(int meshID, const std::string& meshFileName)
{
	std::pair<std::map<int, Mesh>::iterator, bool> meshPair =
		m_meshType2MeshMap.insert(std::make_pair(meshID, Mesh(meshFileName)));
	CHECK_CONDITION_RENDERING(meshPair.second, Utility::Logging::WARNING, "Mesh \"", meshFileName, "\" has already been created.");
	return &meshPair.first->second;
}