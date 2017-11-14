#include "stdafx.h"
#include "IAudioEngine.h"


audio::IAudioEngine::IAudioEngine(const std::string& audioDirectory) :
	m_audioDirectory(audioDirectory)
{
}


audio::IAudioEngine::~IAudioEngine()
{
}
