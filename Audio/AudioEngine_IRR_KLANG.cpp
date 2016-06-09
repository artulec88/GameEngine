#include "stdafx.h"
#include "AudioEngine_IRR_KLANG.h"
#include "Utility\IConfig.h"
#include "Utility\ILogger.h"


Audio::AudioEngine_IRR_KLANG::AudioEngine_IRR_KLANG(int maxChannelsCount) :
	IAudioEngine(),
	M_SONG_FADE_IN_TIME(GET_CONFIG_VALUE_AUDIO("songFadeInTime", 2.0f)), // in seconds
	m_engine(NULL),
	m_currentSong(NULL),
	m_currentSongPath(""),
	m_nextSongPath(""),
	m_fade(FadeStates::FADE_NONE)
{
	m_engine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT,
		irrklang::ESEO_MULTI_THREADED | irrklang::ESEO_LOAD_PLUGINS | irrklang::ESEO_USE_3D_BUFFERS,
		NULL);
}


Audio::AudioEngine_IRR_KLANG::~AudioEngine_IRR_KLANG()
{
	m_engine->drop();
}

void Audio::AudioEngine_IRR_KLANG::Update(Math::Real deltaTime)
{
	if ((m_currentSong != NULL) && (m_fade == FadeStates::FADE_IN))
	{
		float volume = m_currentSong->getVolume();
		float nextVolume = volume + deltaTime / M_SONG_FADE_IN_TIME;
		if (nextVolume >= 1.0f)
		{
			nextVolume = 1.0f;
			m_fade = FadeStates::FADE_NONE;
		}
		DEBUG_LOG_AUDIO("Increasing volume by %.6f. Current volume = %.2f", nextVolume - volume, nextVolume);
		m_currentSong->setVolume(nextVolume);
	}
	else if ((m_currentSong != NULL) && (m_fade == FadeStates::FADE_OUT))
	{
		float volume = m_currentSong->getVolume();
		float nextVolume = volume - deltaTime / M_SONG_FADE_IN_TIME;
		if (nextVolume <= 0.0f)
		{
			m_currentSong->stop();
			m_currentSong = NULL;
			m_currentSongPath.clear();
			m_fade = FadeStates::FADE_NONE;
		}
		else
		{
			m_currentSong->setVolume(nextVolume);
		}
	}
	else if ((m_currentSong == NULL) && (!m_nextSongPath.empty()))
	{
		PlaySong(m_nextSongPath);
		m_nextSongPath.clear();
	}

	m_engine->update(); // TODO: This is not necessary if audio engine is running in the multithreaded mode.
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
	sound = m_engine->addSoundSourceFromFile(path.c_str(), irrklang::ESM_AUTO_DETECT, false);
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
		sound->setPlaybackSpeed(pitch);

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
	// Ignoring if this song is already playing
	if (m_currentSongPath == path)
	{
		DEBUG_LOG("The song \"%s\" is already playing", m_currentSongPath.c_str());
		return;
	}

	// If a song is playing stop them and set this as the next song
	if (m_currentSong != NULL)
	{
		StopSong();
		m_nextSongPath = path;
		return;
	}

	// Find the snog in the corresponding sound map
	Filenames2SoundSources::iterator soundSourcesItr = m_soundSources[Categories::SONG].find(path);
	if (soundSourcesItr == m_soundSources[Categories::SONG].end())
	{
		WARNING_LOG_AUDIO("Cannot play the requested song \"%s\". It has not been loaded yet", path);
		return;
	}

	// Start playing song with volume set to 0 and fade in
	m_currentSongPath = path;
	m_currentSong = m_engine->play2D(soundSourcesItr->second, true, true, true);
	m_currentSong->setVolume(0.0f);
	m_currentSong->setIsPaused(false);
	m_fade = FadeStates::FADE_IN;
}

void Audio::AudioEngine_IRR_KLANG::StopSoundEffects()
{
	WARNING_LOG_AUDIO("Function not yet implemented");
}

void Audio::AudioEngine_IRR_KLANG::StopSong()
{
	if (m_currentSong != NULL)
	{
		m_fade = FadeStates::FADE_OUT;
		m_currentSong->drop();
		m_currentSong = NULL;
	}
	m_nextSongPath.clear();
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
