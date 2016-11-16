#include "stdafx.h"
#include "FMOD_AudioContext.h"


Audio::Fmod::FmodAudioContext::FmodAudioContext()
{
}


Audio::Fmod::FmodAudioContext::~FmodAudioContext()
{
}

void Audio::Fmod::FmodAudioContext::PlayAudio(AudioObject& audioObject)
{
	RemoveAudio(audioObject);
	m_playingAudioObjects.push_back(&audioObject);
}

void Audio::Fmod::FmodAudioContext::PauseAudio(AudioObject& audioObject)
{
	RemoveAudio(audioObject);
}

void Audio::Fmod::FmodAudioContext::StopAudio(AudioObject& audioObject)
{
	if (RemoveAudio(audioObject))
	{
		audioObject.SetPos(0.0);
	}
}

void Audio::Fmod::FmodAudioContext::GenerateSamples(int* stream, int streamLength)
{
}

bool Audio::Fmod::FmodAudioContext::RemoveAudio(AudioObject& audioObject)
{
	return true;
}