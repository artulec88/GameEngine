#include "stdafx.h"
#include "AudioContextFmod.h"
#include "AudioObject.h"

audio::fmod::AudioContextFmod::AudioContextFmod() :
	m_system(nullptr)
{
}


audio::fmod::AudioContextFmod::~AudioContextFmod()
{
}

void audio::fmod::AudioContextFmod::PlayAudio(AudioObject& audioObject)
{
	RemoveAudio(audioObject);
	m_playingAudioObjects.push_back(&audioObject);
}

void audio::fmod::AudioContextFmod::PauseAudio(AudioObject& audioObject)
{
	RemoveAudio(audioObject);
}

void audio::fmod::AudioContextFmod::StopAudio(AudioObject& audioObject)
{
	if (RemoveAudio(audioObject))
	{
		audioObject.SetPos(0.0);
	}
}

void audio::fmod::AudioContextFmod::GenerateSamples(int* stream, int streamLength)
{
}

bool audio::fmod::AudioContextFmod::RemoveAudio(AudioObject& audioObject)
{
	return true;
}