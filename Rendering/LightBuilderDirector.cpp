#include "StdAfx.h"
#include "LightBuilderDirector.h"

using namespace Rendering;

LightBuilderDirector::LightBuilderDirector(LightBuilder& lightBuilder) :
	m_lightBuilder(lightBuilder)
{
}


LightBuilderDirector::~LightBuilderDirector(void)
{
}


void LightBuilderDirector::Construct()
{
	m_lightBuilder.BuildPart1();
	m_lightBuilder.BuildPart2();
	m_lightBuilder.AddToEngine();
}