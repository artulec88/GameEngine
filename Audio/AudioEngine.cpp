#include "stdafx.h"
#include "AudioEngine.h"
#include "Utility\ILogger.h"

#include "Math\RandomGeneratorFactory.h"

#include "fmod_errors.h" // for error-checking

/* static */ const Math::Real Audio::AudioEngine::OCTAVE_RATIO = 2.0f;
/* static */ const Math::Real Audio::AudioEngine::SEMITONE_RATIO = pow(2.0f, 1.0f / 12.0f);

Audio::AudioEngine::AudioEngine(int maxChannelsCount) :
	m_maxChannelsCount(maxChannelsCount),
	m_system(NULL),
	m_master(NULL),
	m_currentSong(NULL),
	m_fade(FadeStates::FADE_NONE)
{
	// Based on tutorial: https://cuboidzone.wordpress.com/category/tutorials/
	FMOD_RESULT fmodResult = FMOD::System_Create(&m_system); // Create the main system object
	CHECK_CONDITION_EXIT_ALWAYS(fmodResult == FMOD_OK, Utility::Critical, "Failed to create an audio system with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));

	int driversCount = 0;
	m_system->getNumDrivers(&driversCount);
	CHECK_CONDITION_EXIT_ALWAYS(driversCount != 0, Utility::Critical, "Failed to create an audio system. Drivers count = %d", driversCount);

	unsigned int version;
	fmodResult = m_system->getVersion(&version);
	CHECK_CONDITION_EXIT_ALWAYS(version >= FMOD_VERSION, Utility::Error,
		"Failed to create an audio system. FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);

	fmodResult = m_system->init(m_maxChannelsCount, FMOD_INIT_NORMAL, NULL); // initialize FMOD
	CHECK_CONDITION_EXIT_ALWAYS(fmodResult == FMOD_OK, Utility::Critical, "Initializing audio system has ended with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));

	// Creating channels groups for each category
	m_system->getMasterChannelGroup(&m_master);
	for (int i = 0; i < Categories::COUNT; ++i)
	{
		m_system->createChannelGroup(NULL /* name of the channel group */, &m_groups[i]);
		m_master->addGroup(m_groups[i]);
	}

	// Set up modes for each category
	m_modes[Categories::SOUND_EFFECT] = FMOD_DEFAULT;
	m_modes[Categories::SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;

	NOTICE_LOG("Audio engine created.");
}


Audio::AudioEngine::~AudioEngine()
{
	FMOD_RESULT fmodResult = FMOD_OK;
	// Releasing sounds in each category
	Filenames2Sounds::iterator soundItr;
	for (int i = 0; i < Categories::COUNT; ++i)
	{
		for (soundItr = m_sounds[i].begin(); soundItr != m_sounds[i].end(); ++soundItr)
		{
			fmodResult = soundItr->second->release();
			CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Releasing sound has ended with error code %d. %s",
				fmodResult, FMOD_ErrorString(fmodResult));
		}
		m_sounds[i].clear();
	}

	// Releasing system
	fmodResult = m_system->close();
	CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Closing audio system has ended with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));
	fmodResult = m_system->release();
	CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Releasing audio system has ended with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));
	NOTICE_LOG("Audio engine destroyed.");
}

void Audio::AudioEngine::Update(Math::Real deltaTime)
{
	const float fadeTime = 1.0f; // in seconds
	if ((m_currentSong != NULL) && (m_fade == FadeStates::FADE_IN))
	{
		float volume;
		m_currentSong->getVolume(&volume);
		float nextVolume = volume + deltaTime / fadeTime;
		if (nextVolume >= 1.0f)
		{
			nextVolume = 1.0f;
			m_fade = FadeStates::FADE_NONE;
		}
		DEBUG_LOG("Increasing volume by %.6f. Current volume = %.2f", nextVolume - volume, nextVolume);
		m_currentSong->setVolume(nextVolume);
	}
	else if ((m_currentSong != NULL) && (m_fade == FadeStates::FADE_OUT))
	{
		float volume;
		m_currentSong->getVolume(&volume);
		float nextVolume = volume - deltaTime / fadeTime;
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
	CHECK_CONDITION_ALWAYS(fmodResult == FMOD_OK, Utility::Error, "Updating audio system has ended with error code %d. %s",
		fmodResult, FMOD_ErrorString(fmodResult));
}

void Audio::AudioEngine::LoadSoundEffect(const std::string& path)
{
	Load(Categories::SOUND_EFFECT, path);
}

void Audio::AudioEngine::LoadSong(const std::string& path)
{
	Load(Categories::SONG, path);
}

void Audio::AudioEngine::Load(Categories::Category type, const std::string& path)
{
	if (m_sounds[type].find(path) != m_sounds[type].end())
	{
		DEBUG_LOG("The sound \"%s\" has been loaded already", path.c_str());
		return;
	}
	FMOD::Sound* sound;
	m_system->createSound(path.c_str(), m_modes[type], NULL, &sound);
	m_sounds[type].insert(std::make_pair(path, sound));
}

Math::Real Audio::AudioEngine::ChangeOctave(Math::Real frequency, Math::Real variation) const
{
	return frequency * pow(OCTAVE_RATIO, variation);
}

Math::Real Audio::AudioEngine::ChangeSemitone(Math::Real frequency, Math::Real variation) const
{
	return frequency * pow(SEMITONE_RATIO, variation);
}

void Audio::AudioEngine::PlaySoundEffect(const std::string& path, Math::Real minVolume, Math::Real maxVolume, Math::Real minPitch, Math::Real maxPitch)
{
	// Trying to find sound effect and return if not found
	Filenames2Sounds::iterator soundItr = m_sounds[Categories::SOUND_EFFECT].find(path);
	if (soundItr == m_sounds[Categories::SOUND_EFFECT].end())
	{
		WARNING_LOG("The requested sound effect \"%s\" has not been found", path.c_str());
		return;
	}

	// Calculating random volume and pitch in selected range
	const Math::Random::RandomGenerator& randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);
	Math::Real volume = randomGenerator.NextFloat(minVolume, maxVolume);
	Math::Real pitch = randomGenerator.NextFloat(minPitch, maxPitch);

	// Playing the sound effect with these initial values
	FMOD::Channel* channel;
	m_system->playSound(soundItr->second, m_groups[Categories::SOUND_EFFECT], true, &channel);
	channel->setVolume(volume);
	Math::Real frequency;
	channel->getFrequency(&frequency);
	channel->setFrequency(ChangeSemitone(frequency, pitch));
	channel->setPaused(false);
}

void Audio::AudioEngine::PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */)
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
	Filenames2Sounds::iterator soundItr = m_sounds[Categories::SONG].find(path);
	if (soundItr == m_sounds[Categories::SONG].end())
	{
		WARNING_LOG("Cannot play the requested song \"%s\". It has not been loaded yet", path);
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

void Audio::AudioEngine::SetMasterVolume(Math::Real volume)
{
	m_master->setVolume(volume);
}

void Audio::AudioEngine::SetSoundEffectsVolume(Math::Real volume)
{
	m_groups[Categories::SOUND_EFFECT]->setVolume(volume);
}

void Audio::AudioEngine::SetSongsVolume(Math::Real volume)
{
	m_groups[Categories::SONG]->setVolume(volume);
}