#include "StdAfx.h"
#include "BuilderDirector.h"

Engine::BuilderDirector::BuilderDirector(Builder& builder) :
	m_builder(builder)
{
}


Engine::BuilderDirector::~BuilderDirector(void)
{
}


void Engine::BuilderDirector::Construct() const
{
	m_builder.BuildPart1();
	m_builder.BuildPart2();
#ifdef BUILD_MESH_RENDERER
	m_builder.BuildMeshRenderer();
#endif
}