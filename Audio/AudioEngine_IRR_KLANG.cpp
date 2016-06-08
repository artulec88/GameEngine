#include "stdafx.h"
#include "AudioEngine_IRR_KLANG.h"


Audio::AudioEngine_IRR_KLANG::AudioEngine_IRR_KLANG(int maxChannelsCount) :
	IAudioEngine(),
	m_engine(NULL)
{
	m_engine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT, irrklang::ESEO_DEFAULT_OPTIONS, NULL);
}


Audio::AudioEngine_IRR_KLANG::~AudioEngine_IRR_KLANG()
{
	m_engine->drop();
}

void Audio::AudioEngine_IRR_KLANG::Update(Math::Real deltaTime)
{
	m_engine->update();
}

void Audio::AudioEngine_IRR_KLANG::LoadSoundEffect(const std::string& path)
{
	Load(Categories::SOUND_EFFECT, path);
}

void Audio::AudioEngine_IRR_KLANG::LoadSoundEffect3D(const std::string& path)
{
	Load(Categories::SOUND_EFFECT_3D, path);
}

void Audio::AudioEngine_IRR_KLANG::LoadSong(const std::string& path)
{
	Load(Categories::SONG, path);
}

void Audio::AudioEngine_IRR_KLANG::Load(Categories::Category type, const std::string& path)
{
	if (m_soundSources[type].find(path) != m_soundSources[type].end())
	{
		DEBUG_LOG("The sound \"%s\" has been loaded already", path.c_str());
		return;
	}
	irrklang::ISoundSource* sound;
	m_engine->addSoundSourceFromFile(path.c_str(), irrklang::ESM_AUTO_DETECT, false);
	m_soundSources[type].insert(std::make_pair(path, sound));
}

void Audio::AudioEngine_IRR_KLANG::PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, Math::Real volume, Math::Real pitch)
{
	// Trying to find sound effect and return if not found
	Filenames2SoundSources::iterator soundItr = m_soundSources[Categories::SOUND_EFFECT].find(path);
	if (soundItr == m_soundSources[Categories::SOUND_EFFECT].end())
	{
		WARNING_LOG_AUDIO("The requested sound effect \"%s\" has not been found", path.c_str());
		return;
	}

	irrklang::ISound* sound = m_engine->play2D(soundItr->second, false, true, true, true);
	if (sound != NULL)
	{
		sound->setVolume(volume);
		// TODO: set pitch

		sound->setIsPaused(false);

		sound->drop();
		sound = NULL;
	}
}

void Audio::AudioEngine_IRR_KLANG::PlaySoundEffect3D(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, Math::Real volume, Math::Real pitch, const Math::Vector3D& position, const Math::Vector3D& velocity)
{
}

void Audio::AudioEngine_IRR_KLANG::PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */)
{
}

void Audio::AudioEngine_IRR_KLANG::StopSoundEffects()
{
	WARNING_LOG_AUDIO("Function not yet implemented");
}

void Audio::AudioEngine_IRR_KLANG::StopSong()
{
	//if (m_currentSong != NULL)
	//{
	//	m_fade = FadeStates::FADE_OUT;
	//}
	//m_nextSongPath.clear();
}

void Audio::AudioEngine_IRR_KLANG::SetMasterVolume(Math::Real volume)
{
	// TODO: volume must lie in range [0; 1]. Check that it does.
	m_engine->setSoundVolume(volume);
}

void Audio::AudioEngine_IRR_KLANG::SetSoundEffectsVolume(Math::Real volume)
{
}

void Audio::AudioEngine_IRR_KLANG::SetSongsVolume(Math::Real volume)
{
}

Math::Real Audio::AudioEngine_IRR_KLANG::ChangeOctave(Math::Real frequency, Math::Real variation) const
{
	return 0.0f;
}

Math::Real Audio::AudioEngine_IRR_KLANG::ChangeSemitone(Math::Real frequency, Math::Real variation) const
{
	return 0.0f;
}
