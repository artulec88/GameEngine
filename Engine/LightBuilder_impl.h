#include "stdafx.h"
#include "LightBuilder.h"

template <class T>
Engine::LightBuilder<T>::LightBuilder(const ShaderFactory& shaderFactory) :
	Utility::Builder<T>(),
	m_lightIndex(0),
	m_lightIndexStr("0"),
	m_shaderFactory(shaderFactory)
{
}

template <class T>
Engine::LightBuilder<T>::~LightBuilder(void)
{
}

template <class T>
void Engine::LightBuilder<T>::BuildPart1()
{
	SetupLightShaders();
}

template <class T>
void Engine::LightBuilder<T>::BuildPart2()
{
	SetupLightParams();
}

template <class T>
void Engine::LightBuilder<T>::SetLightIndex(int lightIndex)
{
	m_lightIndex = lightIndex;
	m_lightIndexStr = std::to_string(lightIndex);
}
