#include "stdafx.h"
#include "AudioEngine_IRR_KLANG.h"
#include "Utility/IConfig.h"
#include "Utility/ILogger.h"


audio::AudioEngine_IRR_KLANG::AudioEngine_IRR_KLANG(const std::string& audioDirectory, int maxChannelsCount) :
	IAudioEngine(audioDirectory),
	M_SONG_FADE_IN_TIME(GET_CONFIG_VALUE_AUDIO("songFadeInTime", 2.0f)), // in seconds
	m_engine(nullptr),
	m_currentSong(nullptr),
	m_currentSongPath(""),
	m_nextSongPath(""),
	m_fade(FadeStates::FADE_NONE)
{
	m_engine = irrklang::createIrrKlangDevice(irrklang::ESOD_AUTO_DETECT,
		irrklang::ESEO_MULTI_THREADED | irrklang::ESEO_LOAD_PLUGINS | irrklang::ESEO_USE_3D_BUFFERS,
		nullptr);
}


audio::AudioEngine_IRR_KLANG::~AudioEngine_IRR_KLANG()
{
	m_engine->drop();
}

void audio::AudioEngine_IRR_KLANG::Update(math::Real deltaTime)
{
	if ((m_currentSong != nullptr) && (m_fade == FadeStates::FADE_IN))
	{
		float volume = m_currentSong->getVolume();
		float nextVolume = volume + deltaTime / M_SONG_FADE_IN_TIME;
		if (nextVolume >= 1.0f)
		{
			nextVolume = 1.0f;
			m_fade = FadeStates::FADE_NONE;
		}
		DEBUG_LOG_AUDIO("Increasing volume by ", nextVolume - volume, ". Current volume = ", nextVolume);
		m_currentSong->setVolume(nextVolume);
	}
	else if ((m_currentSong != nullptr) && (m_fade == FadeStates::FADE_OUT))
	{
		const auto volume = m_currentSong->getVolume();
		const auto nextVolume = volume - deltaTime / M_SONG_FADE_IN_TIME;
		if (nextVolume <= 0.0f)
		{
			m_currentSong->stop();
			m_currentSong = nullptr;
			m_currentSongPath.clear();
			m_fade = FadeStates::FADE_NONE;
		}
		else
		{
			m_currentSong->setVolume(nextVolume);
		}
	}
	else if ((m_currentSong == nullptr) && (!m_nextSongPath.empty()))
	{
		PlaySong(m_nextSongPath);
		m_nextSongPath.clear();
	}

	m_engine->update(); // TODO: This is not necessary if audio engine is running in the multithreaded mode.
}

void audio::AudioEngine_IRR_KLANG::LoadSoundEffect(const std::string& path)
{
	Load(Categories::SOUND_EFFECT, path);
}

void audio::AudioEngine_IRR_KLANG::LoadSoundEffect3D(const std::string& path)
{
	Load(Categories::SOUND_EFFECT_3D, path);
}

void audio::AudioEngine_IRR_KLANG::LoadSong(const std::string& path)
{
	Load(Categories::SONG, path);
}

void audio::AudioEngine_IRR_KLANG::Load(Categories::Category type, const std::string& path)
{
	DEBUG_LOG_AUDIO("Loading sound \"", path, "\" from directory \"", m_audioDirectory, "\".");
	if (m_soundSources[type].find(path) != m_soundSources[type].end())
	{
		DELOCUST_LOG_AUDIO("The sound \"", path, "\" has been loaded already");
		return;
	}
	irrklang::ISoundSource* sound;
	sound = m_engine->addSoundSourceFromFile((m_audioDirectory + path).c_str(), irrklang::ESM_AUTO_DETECT, false);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(sound != NULL, utility::logging::ERR, "The audio \"", path, "\" cannot be loaded.");
	m_soundSources[type].insert(std::make_pair(path, sound));
}

void audio::AudioEngine_IRR_KLANG::PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */)
{
	const auto soundItr = m_soundSources[Categories::SOUND_EFFECT].find(path);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(soundItr != m_soundSources[Categories::SOUND_EFFECT].end(), utility::logging::WARNING,
		"The requested sound effect \"", path, "\" has not been found and thus couldn't be played.");

	m_engine->play2D(soundItr->second, false, false, false, false);
}

void audio::AudioEngine_IRR_KLANG::PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, math::Real volume, math::Real pitch)
{
	// Trying to find sound effect and return if not found
	const auto soundItr = m_soundSources[Categories::SOUND_EFFECT].find(path);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(soundItr != m_soundSources[Categories::SOUND_EFFECT].end(), utility::logging::WARNING,
		"The requested sound effect \"", path, "\" has not been found and thus couldn't be played.");

	irrklang::ISound* sound = m_engine->play2D(soundItr->second, false, true, true, true);
	if (sound != nullptr)
	{
		sound->setVolume(volume);
		sound->setPlaybackSpeed(pitch);

		sound->setIsPaused(false);

		sound->drop();
		sound = nullptr; // TODO: Assigned value is never used
	}
}

void audio::AudioEngine_IRR_KLANG::PlaySoundEffect3D(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, math::Real volume, math::Real pitch, const math::Vector3D& position, const math::Vector3D& velocity)
{
}

void audio::AudioEngine_IRR_KLANG::PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */)
{
	// Ignoring if this song is already playing
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(m_currentSongPath != path, utility::logging::DEBUG, "The song \"", m_currentSongPath, "\" is already playing");

	// If a song is playing stop them and set this as the next song
	if (m_currentSong != nullptr)
	{
		StopSong();
		m_nextSongPath = path;
		return;
	}

	// Find the song in the corresponding sound map
	const auto soundItr = m_soundSources[Categories::SONG].find(path);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(soundItr != m_soundSources[Categories::SONG].end(), utility::logging::WARNING,
		"Cannot play the requested song \"", path, "\". It has not been loaded yet.");

	// Start playing song with volume set to 0 and fade in
	m_currentSongPath = path;
	m_currentSong = m_engine->play2D(soundItr->second, true, true, true);
	m_currentSong->setVolume(0.0f);
	m_currentSong->setIsPaused(false);
	m_fade = FadeStates::FADE_IN;
}

void audio::AudioEngine_IRR_KLANG::StopSoundEffects()
{
	WARNING_LOG_AUDIO("Function not yet implemented");
}

void audio::AudioEngine_IRR_KLANG::StopSong()
{
	if (m_currentSong != nullptr)
	{
		m_fade = FadeStates::FADE_OUT;
		m_currentSong->drop();
		m_currentSong = nullptr;
	}
	m_nextSongPath.clear();
}

void audio::AudioEngine_IRR_KLANG::SetMasterVolume(math::Real volume)
{
	// TODO: volume must lie in range [0; 1]. Check that it does.
	m_engine->setSoundVolume(volume);
}

void audio::AudioEngine_IRR_KLANG::SetSoundEffectsVolume(math::Real volume)
{
	SetVolume(Categories::SOUND_EFFECT, volume);
}

void audio::AudioEngine_IRR_KLANG::SetSongsVolume(math::Real volume)
{
}

void audio::AudioEngine_IRR_KLANG::SetVolume(Categories::Category type, math::Real volume)
{
	for (Filenames2SoundSources::iterator soundSourceItr = m_soundSources[type].begin(); soundSourceItr != m_soundSources[type].end(); ++soundSourceItr)
	{
		soundSourceItr->second->setDefaultVolume(volume);
	}
}

math::Real audio::AudioEngine_IRR_KLANG::ChangeOctave(math::Real frequency, math::Real variation) const
{
	return 0.0f;
}

math::Real audio::AudioEngine_IRR_KLANG::ChangeSemitone(math::Real frequency, math::Real variation) const
{
	return 0.0f;
}
