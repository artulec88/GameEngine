#include "stdafx.h"
#include "FMOD_AudioContext.h"


audio::Fmod::FmodAudioContext::FmodAudioContext()
{
}


audio::Fmod::FmodAudioContext::~FmodAudioContext()
{
}

void audio::Fmod::FmodAudioContext::PlayAudio(AudioObject& audioObject)
{
	RemoveAudio(audioObject);
	m_playingAudioObjects.push_back(&audioObject);
}

void audio::Fmod::FmodAudioContext::PauseAudio(AudioObject& audioObject)
{
	RemoveAudio(audioObject);
}

void audio::Fmod::FmodAudioContext::StopAudio(AudioObject& audioObject)
{
	if (RemoveAudio(audioObject))
	{
		audioObject.SetPos(0.0);
	}
}

void audio::Fmod::FmodAudioContext::GenerateSamples(int* stream, int streamLength)
{
}

bool audio::Fmod::FmodAudioContext::RemoveAudio(AudioObject& audioObject)
{
	return true;
}