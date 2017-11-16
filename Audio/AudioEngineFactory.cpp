#include "stdafx.h"
#include "AudioEngineFactory.h"
#include "AudioEngine_FMOD.h"
#include "AudioEngine_IRR_KLANG.h"
#include "Utility/IConfig.h"
#include "Utility/ILogger.h"

audio::AudioEngineFactory::AudioEngineFactory(const std::string& audioDirectory) :
	m_audioDirectory(audioDirectory)
{
}


audio::AudioEngineFactory::~AudioEngineFactory()
{
}

std::unique_ptr<audio::IAudioEngine> audio::AudioEngineFactory::CreateAudioEngine(AudioEngineTypes::AudioEngineType audioEngineType /* = AudioEngineTypes::IRR_KLANG */) const
{
	switch (audioEngineType)
	{
	case AudioEngineTypes::FMOD:
		return std::make_unique<AudioEngine_FMOD>(m_audioDirectory, GET_CONFIG_VALUE_AUDIO("audioMaxChannels", 32));
	case AudioEngineTypes::IRR_KLANG:
		return std::make_unique<AudioEngine_IRR_KLANG>(m_audioDirectory, GET_CONFIG_VALUE_AUDIO("audioMaxChannels", 32));
	default:
		ERROR_LOG_AUDIO("The specified audio engine type ", audioEngineType, " is not supported. Default FMOD audio engine will be used.");
		return std::make_unique<AudioEngine_FMOD>(m_audioDirectory, GET_CONFIG_VALUE_AUDIO("audioMaxChannels", 32));
	}
}