#include "stdafx.h"
#include "IAudioEngine.h"


Audio::IAudioEngine::IAudioEngine(const std::string& audioDirectory) :
	m_audioDirectory(audioDirectory)
{
}


Audio::IAudioEngine::~IAudioEngine()
{
}
