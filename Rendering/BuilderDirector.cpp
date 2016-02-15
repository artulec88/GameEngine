#include "StdAfx.h"
#include "BuilderDirector.h"

using namespace Rendering;

BuilderDirector::BuilderDirector(Builder& builder) :
	m_builder(builder)
{
}


BuilderDirector::~BuilderDirector(void)
{
}


void BuilderDirector::Construct() const
{
	m_builder.BuildPart1();
	m_builder.BuildPart2();
#ifdef BUILD_MESH_RENDERER
	m_builder.BuildMeshRenderer();
#endif
}