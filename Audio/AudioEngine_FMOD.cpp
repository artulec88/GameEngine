#include "stdafx.h"
#include "AudioEngine_FMOD.h"
#include "Utility/IConfig.h"
#include "Utility/ILogger.h"

#include "Math\RandomGeneratorFactory.h"

#include "fmod_errors.h" // for error-checking

audio::AudioEngine_FMOD::AudioEngine_FMOD(const std::string& audioDirectory, const int maxChannelsCount) :
	IAudioEngine(audioDirectory),
	M_SONG_FADE_IN_TIME(GET_CONFIG_VALUE_AUDIO("songFadeInTime", 2.0f)), // in seconds
	m_maxChannelsCount(maxChannelsCount),
	m_system(nullptr),
	m_master(nullptr),
	m_currentSong(nullptr),
	m_currentSongPath(""),
	m_nextSongPath(""),
	m_fade(FadeStates::FADE_NONE)
{
	// Based on tutorial: https://cuboidzone.wordpress.com/category/tutorials/
	FMOD_RESULT fmodResult = FMOD::System_Create(&m_system); // Create the main system object
	CHECK_CONDITION_EXIT_ALWAYS_AUDIO(fmodResult == FMOD_OK, utility::logging::CRITICAL, "Failed to create an audio system with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));

	int driversCount = 0;
	fmodResult = m_system->getNumDrivers(&driversCount);
	CHECK_CONDITION_EXIT_ALWAYS_AUDIO(fmodResult == FMOD_OK && driversCount != 0, utility::logging::CRITICAL, "Failed to create an audio system. Drivers count = ", driversCount);

	unsigned int version;
	fmodResult = m_system->getVersion(&version);
	CHECK_CONDITION_EXIT_ALWAYS_AUDIO(fmodResult == FMOD_OK && version >= FMOD_VERSION, utility::logging::ERR,
		"Failed to create an audio system. FMOD lib version ", version, " doesn't match header version ", FMOD_VERSION);

	fmodResult = m_system->init(m_maxChannelsCount, FMOD_INIT_NORMAL, nullptr); // initialize FMOD
	CHECK_CONDITION_EXIT_ALWAYS_AUDIO(fmodResult == FMOD_OK, utility::logging::CRITICAL, "Initializing audio system has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));

	// Creating channels groups for each category
	m_system->getMasterChannelGroup(&m_master);
	for (int i = 0; i < Categories::COUNT; ++i)
	{
		m_system->createChannelGroup(nullptr /* name of the channel group */, &m_groups[i]);
		m_master->addGroup(m_groups[i]);
	}

	// Set up modes for each category
	m_modes[Categories::SOUND_EFFECT] = FMOD_DEFAULT;
	m_modes[Categories::SOUND_EFFECT_3D] = FMOD_3D;
	m_modes[Categories::SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;

	NOTICE_LOG_AUDIO("Audio engine created.");
}


audio::AudioEngine_FMOD::~AudioEngine_FMOD()
{
	FMOD_RESULT fmodResult = FMOD_OK;
	// Releasing sounds in each category
	for (int i = 0; i < Categories::COUNT; ++i)
	{
		for (auto soundItr = m_sounds[i].begin(); soundItr != m_sounds[i].end(); ++soundItr)
		{
			fmodResult = soundItr->second->release();
			CHECK_CONDITION_ALWAYS_AUDIO(fmodResult == FMOD_OK, utility::logging::ERR, "Releasing sound has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));
		}
		m_sounds[i].clear();
	}

	// Releasing system
	fmodResult = m_system->close();
	CHECK_CONDITION_ALWAYS_AUDIO(fmodResult == FMOD_OK, utility::logging::ERR, "Closing audio system has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));
	fmodResult = m_system->release();
	CHECK_CONDITION_ALWAYS_AUDIO(fmodResult == FMOD_OK, utility::logging::ERR, "Releasing audio system has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));
	NOTICE_LOG_AUDIO("Audio engine destroyed.");
}

void audio::AudioEngine_FMOD::Update(math::Real deltaTime)
{
	if ((m_currentSong != nullptr) && (m_fade == FadeStates::FADE_IN))
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
	else if ((m_currentSong != nullptr) && (m_fade == FadeStates::FADE_OUT))
	{
		float volume;
		m_currentSong->getVolume(&volume);
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

	FMOD_RESULT fmodResult = m_system->update();
	CHECK_CONDITION_ALWAYS_AUDIO(fmodResult == FMOD_OK, utility::logging::ERR, "Updating audio system has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));
}

void audio::AudioEngine_FMOD::LoadSoundEffect(const std::string& path)
{
	Load(Categories::SOUND_EFFECT, path);
}

void audio::AudioEngine_FMOD::LoadSoundEffect3D(const std::string& path)
{
	Load(Categories::SOUND_EFFECT_3D, path);
}

void audio::AudioEngine_FMOD::LoadSong(const std::string& path)
{
	Load(Categories::SONG, path);
}

void audio::AudioEngine_FMOD::Load(Categories::Category type, const std::string& path)
{
	if (m_sounds[type].find(path) != m_sounds[type].end())
	{
		DEBUG_LOG_AUDIO("The sound \"", path, "\" has been loaded already");
		return;
	}
	FMOD::Sound* sound;
	m_system->createSound(path.c_str(), m_modes[type], nullptr, &sound);
	m_sounds[type].insert(std::make_pair(path, sound));
}

math::Real audio::AudioEngine_FMOD::ChangeOctave(math::Real frequency, math::Real variation) const
{
	return frequency * pow(OCTAVE_RATIO, variation);
}

math::Real audio::AudioEngine_FMOD::ChangeSemitone(math::Real frequency, math::Real variation) const
{
	return frequency * pow(SEMITONE_RATIO, variation);
}

void audio::AudioEngine_FMOD::PlaySoundEffect(const std::string& path)
{
	float volume, pitch;
	m_groups[Categories::SOUND_EFFECT]->getVolume(&volume);
	m_groups[Categories::SOUND_EFFECT]->getPitch(&pitch);
	PlaySoundEffect(path, volume, pitch);
}

void audio::AudioEngine_FMOD::PlaySoundEffect(const std::string& path, math::Real volume, math::Real pitch)
{
	// Trying to find sound effect and return if not found
	const auto soundItr = m_sounds[Categories::SOUND_EFFECT].find(path);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(soundItr != m_sounds[Categories::SOUND_EFFECT].end(), utility::logging::WARNING,
		"The requested sound effect \"", path, "\" has not been found");

	// Calculating random volume and pitch in selected range
	//const math::random::RandomGenerator& randomGenerator = math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::Generators::SIMPLE);
	//math::Real volume = randomGenerator.NextFloat(minVolume, maxVolume);
	//math::Real pitch = randomGenerator.NextFloat(minPitch, maxPitch);

	// Playing the sound effect with these initial values
	FMOD::Channel* channel;
	m_system->playSound(soundItr->second, m_groups[Categories::SOUND_EFFECT], true, &channel);
	channel->setVolume(volume);
	math::Real frequency;
	channel->getFrequency(&frequency);
	channel->setFrequency(ChangeSemitone(frequency, pitch));
	channel->setPaused(false);
}

void audio::AudioEngine_FMOD::PlaySoundEffect3D(const std::string& path, math::Real volume, math::Real pitch, const math::Vector3D& position, const math::Vector3D& velocity)
{
	// Trying to find sound effect and return if not found
	auto soundItr = m_sounds[Categories::SOUND_EFFECT_3D].find(path);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(soundItr != m_sounds[Categories::SOUND_EFFECT].end(), utility::logging::WARNING,
		"The requested 3D sound effect \"", path, "\" has not been found");

	// Playing the sound effect with these initial values
	FMOD::Channel* channel;
	m_system->playSound(soundItr->second, m_groups[Categories::SOUND_EFFECT_3D], true, &channel);
	channel->setVolume(volume);
	math::Real frequency;
	channel->getFrequency(&frequency);
	channel->setFrequency(ChangeSemitone(frequency, pitch));
	const FMOD_VECTOR fmodPosition = { position.x, position.y, position.z };
	const FMOD_VECTOR fmodVelocity = { velocity.x, velocity.y, velocity.z };
	channel->set3DAttributes(&fmodPosition, &fmodVelocity);
	channel->setPaused(false);
}

void audio::AudioEngine_FMOD::PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */)
{
	// Ignoring if this song is already playing
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(m_currentSongPath != path, utility::logging::DEBUG, "The song \"", m_currentSongPath, "\" is already playing");

	// If a song is playing stop then and set this as the next song
	if (m_currentSong != nullptr)
	{
		StopSong();
		m_nextSongPath = path;
		return;
	}

	// Find the song in the corresponding sound map
	const auto soundItr = m_sounds[Categories::SONG].find(path);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(soundItr != m_sounds[Categories::SONG].end(), utility::logging::WARNING,
		"Cannot play the requested song \"", path, "\". It has not been loaded yet.");

	// Start playing song with volume set to 0 and fade in
	m_currentSongPath = path;
	m_system->playSound(soundItr->second, m_groups[Categories::SONG], true, &m_currentSong);
	//m_currentSong->setChannelGroup
	m_currentSong->setVolume(0.0f);
	m_currentSong->setPaused(false);
	m_fade = FadeStates::FADE_IN;
}

void audio::AudioEngine_FMOD::SetMasterVolume(math::Real volume)
{
	m_master->setVolume(volume);
}

void audio::AudioEngine_FMOD::SetSoundEffectsVolume(math::Real volume)
{
	m_groups[Categories::SOUND_EFFECT]->setVolume(volume);
}

void audio::AudioEngine_FMOD::SetSongsVolume(math::Real volume)
{
	m_groups[Categories::SONG]->setVolume(volume);
}