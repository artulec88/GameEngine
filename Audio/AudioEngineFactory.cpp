#include "stdafx.h"
#include "AudioEngineFactory.h"
#include "AudioEngineFmod.h"
#include "AudioEngineIrrKlang.h"
#include "Utility/IConfig.h"
#include "Utility/ILogger.h"

audio::AudioEngineFactory::AudioEngineFactory(const std::string& audioDirectory) :
	m_audioDirectory(audioDirectory)
{
}


audio::AudioEngineFactory::~AudioEngineFactory()
{
}

std::unique_ptr<audio::IAudioEngine> audio::AudioEngineFactory::CreateAudioEngine(audio_engine_types::AudioEngineType audioEngineType /* = audio_engine_types::IRR_KLANG */) const
{
	switch (audioEngineType)
	{
	case audio_engine_types::FMOD:
		return std::make_unique<AudioEngineFmod>(m_audioDirectory, GET_CONFIG_VALUE_AUDIO("audioMaxChannels", 32));
	case audio_engine_types::IRR_KLANG:
		return std::make_unique<AudioEngineIrrKlang>(m_audioDirectory, GET_CONFIG_VALUE_AUDIO("audioMaxChannels", 32));
	default:
		ERROR_LOG_AUDIO("The specified audio engine type ", audioEngineType, " is not supported. Default FMOD audio engine will be used.");
		return std::make_unique<AudioEngineFmod>(m_audioDirectory, GET_CONFIG_VALUE_AUDIO("audioMaxChannels", 32));
	}
}