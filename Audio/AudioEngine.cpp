#include "stdafx.h"
#include "AudioEngine.h"
#include "Utility\ILogger.h"

#define NUM_BUFFERS 3

Audio::AudioEngine::AudioEngine()
{
	ALCdevice* device = alcOpenDevice(NULL); // select the "preferred device"
	CHECK_CONDITION_EXIT_ALWAYS(device != NULL, Utility::Critical, "Failed to create an audio device");
	
	ALCcontext* context = alcCreateContext(device, NULL);
	CHECK_CONDITION_EXIT_ALWAYS(context != NULL, Utility::Critical, "Failed to create an audio context");

	alcMakeContextCurrent(context);
}


Audio::AudioEngine::~AudioEngine()
{
}

void Audio::AudioEngine::Play() const
{
	//ALuint source, buffers[NUM_BUFFERS];
	//ALuint frequency;
	//ALenum format;
	//unsigned char* buffer;

	//int pid, files[2];
	//FILE* f;

	//alGenBuffers(NUM_BUFFERS, buffers);
	//alGenSources(1, &source);
	//CHECK_CONDITION(alGetError() != AL_NO_ERROR, Utility::Emergency, "Failed to generate buffers and sources");
}