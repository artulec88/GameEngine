#include "stdafx.h"
#include "AudioEngine_FMOD.h"
#include "Utility\IConfig.h"
#include "Utility\ILogger.h"

#include "Math\RandomGeneratorFactory.h"

#include "fmod_errors.h" // for error-checking

/* static */ const Math::Real Audio::AudioEngine_FMOD::OCTAVE_RATIO = 2.0f;
/* static */ const Math::Real Audio::AudioEngine_FMOD::SEMITONE_RATIO = pow(2.0f, 1.0f / 12.0f);

Audio::AudioEngine_FMOD::AudioEngine_FMOD(const std::string& audioDirectory, int maxChannelsCount) :
	IAudioEngine(audioDirectory),
	M_SONG_FADE_IN_TIME(GET_CONFIG_VALUE_AUDIO("songFadeInTime", 2.0f)), // in seconds
	m_maxChannelsCount(maxChannelsCount),
	m_system(NULL),
	m_master(NULL),
	m_currentSong(NULL),
	m_currentSongPath(""),
	m_nextSongPath(""),
	m_fade(FadeStates::FADE_NONE)
{
	// Based on tutorial: https://cuboidzone.wordpress.com/category/tutorials/
	FMOD_RESULT fmodResult = FMOD::System_Create(&m_system); // Create the main system object
	CHECK_CONDITION_EXIT_ALWAYS_AUDIO(fmodResult == FMOD_OK, Utility::Logging::CRITICAL, "Failed to create an audio system with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));

	int driversCount = 0;
	fmodResult = m_system->getNumDrivers(&driversCount);
	CHECK_CONDITION_EXIT_ALWAYS_AUDIO(fmodResult == FMOD_OK && driversCount != 0, Utility::Logging::CRITICAL, "Failed to create an audio system. Drivers count = ", driversCount);

	unsigned int version;
	fmodResult = m_system->getVersion(&version);
	CHECK_CONDITION_EXIT_ALWAYS_AUDIO(fmodResult == FMOD_OK && version >= FMOD_VERSION, Utility::Logging::ERR,
		"Failed to create an audio system. FMOD lib version ", version, " doesn't match header version ", FMOD_VERSION);

	fmodResult = m_system->init(m_maxChannelsCount, FMOD_INIT_NORMAL, NULL); // initialize FMOD
	CHECK_CONDITION_EXIT_ALWAYS_AUDIO(fmodResult == FMOD_OK, Utility::Logging::CRITICAL, "Initializing audio system has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));

	// Creating channels groups for each category
	m_system->getMasterChannelGroup(&m_master);
	for (int i = 0; i < Categories::COUNT; ++i)
	{
		m_system->createChannelGroup(NULL /* name of the channel group */, &m_groups[i]);
		m_master->addGroup(m_groups[i]);
	}

	// Set up modes for each category
	m_modes[Categories::SOUND_EFFECT] = FMOD_DEFAULT;
	m_modes[Categories::SOUND_EFFECT_3D] = FMOD_3D;
	m_modes[Categories::SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;

	NOTICE_LOG_AUDIO("Audio engine created.");
}


Audio::AudioEngine_FMOD::~AudioEngine_FMOD()
{
	FMOD_RESULT fmodResult = FMOD_OK;
	// Releasing sounds in each category
	Filenames2Sounds::iterator soundItr;
	for (int i = 0; i < Categories::COUNT; ++i)
	{
		for (soundItr = m_sounds[i].begin(); soundItr != m_sounds[i].end(); ++soundItr)
		{
			fmodResult = soundItr->second->release();
			CHECK_CONDITION_ALWAYS_AUDIO(fmodResult == FMOD_OK, Utility::Logging::ERR, "Releasing sound has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));
		}
		m_sounds[i].clear();
	}

	// Releasing system
	fmodResult = m_system->close();
	CHECK_CONDITION_ALWAYS_AUDIO(fmodResult == FMOD_OK, Utility::Logging::ERR, "Closing audio system has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));
	fmodResult = m_system->release();
	CHECK_CONDITION_ALWAYS_AUDIO(fmodResult == FMOD_OK, Utility::Logging::ERR, "Releasing audio system has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));
	NOTICE_LOG_AUDIO("Audio engine destroyed.");
}

void Audio::AudioEngine_FMOD::Update(Math::Real deltaTime)
{
	if ((m_currentSong != NULL) && (m_fade == FadeStates::FADE_IN))
	{
		float volume;
		m_currentSong->getVolume(&volume);
		float nextVolume = volume + deltaTime / M_SONG_FADE_IN_TIME;
		if (nextVolume >= 1.0f)
		{
			nextVolume = 1.0f;
			m_fade = FadeStates::FADE_NONE;
		}
		DEBUG_LOG_AUDIO("Increasing volume by ", nextVolume - volume, ". Current volume = ", nextVolume);
		m_currentSong->setVolume(nextVolume);
	}
	else if ((m_currentSong != NULL) && (m_fade == FadeStates::FADE_OUT))
	{
		float volume;
		m_currentSong->getVolume(&volume);
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

	FMOD_RESULT fmodResult = m_system->update();
	CHECK_CONDITION_ALWAYS_AUDIO(fmodResult == FMOD_OK, Utility::Logging::ERR, "Updating audio system has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));
}

void Audio::AudioEngine_FMOD::LoadSoundEffect(const std::string& path)
{
	Load(Categories::SOUND_EFFECT, path);
}

void Audio::AudioEngine_FMOD::LoadSoundEffect3D(const std::string& path)
{
	Load(Categories::SOUND_EFFECT_3D, path);
}

void Audio::AudioEngine_FMOD::LoadSong(const std::string& path)
{
	Load(Categories::SONG, path);
}

void Audio::AudioEngine_FMOD::Load(Categories::Category type, const std::string& path)
{
	if (m_sounds[type].find(path) != m_sounds[type].end())
	{
		DEBUG_LOG_AUDIO("The sound \"", path, "\" has been loaded already");
		return;
	}
	FMOD::Sound* sound;
	m_system->createSound(path.c_str(), m_modes[type], NULL, &sound);
	m_sounds[type].insert(std::make_pair(path, sound));
}

Math::Real Audio::AudioEngine_FMOD::ChangeOctave(Math::Real frequency, Math::Real variation) const
{
	return frequency * pow(OCTAVE_RATIO, variation);
}

Math::Real Audio::AudioEngine_FMOD::ChangeSemitone(Math::Real frequency, Math::Real variation) const
{
	return frequency * pow(SEMITONE_RATIO, variation);
}

void Audio::AudioEngine_FMOD::PlaySoundEffect(const std::string& path)
{
	float volume, pitch;
	m_groups[Categories::SOUND_EFFECT]->getVolume(&volume);
	m_groups[Categories::SOUND_EFFECT]->getPitch(&pitch);
	PlaySoundEffect(path, volume, pitch);
}

void Audio::AudioEngine_FMOD::PlaySoundEffect(const std::string& path, Math::Real volume, Math::Real pitch)
{
	// Trying to find sound effect and return if not found
	Filenames2Sounds::iterator soundItr = m_sounds[Categories::SOUND_EFFECT].find(path);
	if (soundItr == m_sounds[Categories::SOUND_EFFECT].end())
	{
		WARNING_LOG_AUDIO("The requested sound effect \"", path, "\" has not been found");
		return;
	}

	// Calculating random volume and pitch in selected range
	//const Math::Random::RandomGenerator& randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);
	//Math::Real volume = randomGenerator.NextFloat(minVolume, maxVolume);
	//Math::Real pitch = randomGenerator.NextFloat(minPitch, maxPitch);

	// Playing the sound effect with these initial values
	FMOD::Channel* channel;
	m_system->playSound(soundItr->second, m_groups[Categories::SOUND_EFFECT], true, &channel);
	channel->setVolume(volume);
	Math::Real frequency;
	channel->getFrequency(&frequency);
	channel->setFrequency(ChangeSemitone(frequency, pitch));
	channel->setPaused(false);
}

void Audio::AudioEngine_FMOD::PlaySoundEffect3D(const std::string& path, Math::Real volume, Math::Real pitch, const Math::Vector3D& position, const Math::Vector3D& velocity)
{
	// Trying to find sound effect and return if not found
	Filenames2Sounds::iterator soundItr = m_sounds[Categories::SOUND_EFFECT_3D].find(path);
	if (soundItr == m_sounds[Categories::SOUND_EFFECT_3D].end())
	{
		WARNING_LOG_AUDIO("The requested 3D sound effect \"", path, "\" has not been found");
		return;
	}

	// Playing the sound effect with these initial values
	FMOD::Channel* channel;
	m_system->playSound(soundItr->second, m_groups[Categories::SOUND_EFFECT_3D], true, &channel);
	channel->setVolume(volume);
	Math::Real frequency;
	channel->getFrequency(&frequency);
	channel->setFrequency(ChangeSemitone(frequency, pitch));
	const FMOD_VECTOR fmodPosition = { position.GetX(), position.GetY(), position.GetZ() };
	const FMOD_VECTOR fmodVelocity = { velocity.GetX(), velocity.GetY(), velocity.GetZ() };
	channel->set3DAttributes(&fmodPosition, &fmodVelocity);
	channel->setPaused(false);
}

void Audio::AudioEngine_FMOD::PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */)
{
	// Ignoring if this song is already playing
	if (m_currentSongPath == path)
	{
		DEBUG_LOG_AUDIO("The song \"", m_currentSongPath, "\" is already playing");
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
	Filenames2Sounds::iterator soundItr = m_sounds[Categories::SONG].find(path);
	if (soundItr == m_sounds[Categories::SONG].end())
	{
		WARNING_LOG_AUDIO("Cannot play the requested song \"", path, "\". It has not been loaded yet");
		return;
	}

	// Start playing song with volume set to 0 and fade in
	m_currentSongPath = path;
	m_system->playSound(soundItr->second, m_groups[Categories::SONG], true, &m_currentSong);
	//m_currentSong->setChannelGroup
	m_currentSong->setVolume(0.0f);
	m_currentSong->setPaused(false);
	m_fade = FadeStates::FADE_IN;
}

void Audio::AudioEngine_FMOD::SetMasterVolume(Math::Real volume)
{
	m_master->setVolume(volume);
}

void Audio::AudioEngine_FMOD::SetSoundEffectsVolume(Math::Real volume)
{
	m_groups[Categories::SOUND_EFFECT]->setVolume(volume);
}

void Audio::AudioEngine_FMOD::SetSongsVolume(Math::Real volume)
{
	m_groups[Categories::SONG]->setVolume(volume);
}