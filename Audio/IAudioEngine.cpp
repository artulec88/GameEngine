#include "stdafx.h"
#include "IAudioEngine.h"

#include "Utility/IConfig.h"

audio::IAudioEngine::IAudioEngine(const std::string& audioDirectory) :
	m_audioDirectory(audioDirectory),
	M_SONG_FADE_IN_TIME(GET_CONFIG_VALUE_AUDIO("songFadeInTime", 2.0f)) // in seconds
{
}


audio::IAudioEngine::~IAudioEngine()
{
}
