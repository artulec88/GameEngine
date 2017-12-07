#include "stdafx.h"
#include "AudioEngineFmod.h"

#include "Math/Vector.h"
#include "Math/RandomGeneratorFactory.h"

#include "Utility/IConfig.h"
#include "Utility/ILogger.h"

#include "fmod_errors.h" // for error-checking

audio::AudioEngineFmod::AudioEngineFmod(const std::string& audioDirectory, const int maxChannelsCount) :
	IAudioEngine(audioDirectory),
	m_maxChannelsCount(maxChannelsCount),
	m_system(nullptr),
	m_master(nullptr),
	m_currentSong(nullptr),
	m_currentSongPath(""),
	m_nextSongPath(""),
	m_fade(fade_states::FADE_NONE)
{
	// Based on tutorial: https://cuboidzone.wordpress.com/category/tutorials/
	FMOD_RESULT fmodResult = System_Create(&m_system); // Create the main system object
	CHECK_CONDITION_EXIT_ALWAYS_AUDIO(fmodResult == FMOD_OK, utility::logging::CRITICAL, "Failed to create an audio system with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));

	auto driversCount = 0;
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
	for (auto i = 0; i < categories::COUNT; ++i)
	{
		m_system->createChannelGroup(nullptr /* name of the channel group */, &m_groups[i]);
		m_master->addGroup(m_groups[i]);
	}

	// Set up modes for each category
	m_modes[categories::SOUND_EFFECT] = FMOD_DEFAULT;
	m_modes[categories::SOUND_EFFECT_3D] = FMOD_3D;
	m_modes[categories::SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;

	NOTICE_LOG_AUDIO("Audio engine created.");
}


audio::AudioEngineFmod::~AudioEngineFmod()
{
	FMOD_RESULT fmodResult = FMOD_OK;
	// Releasing sounds in each category
	for (auto i = 0; i < categories::COUNT; ++i)
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

void audio::AudioEngineFmod::Update(math::Real deltaTime)
{
	if (m_currentSong != nullptr)
	{
		if (fade_states::FADE_IN == m_fade)
		{
			float volume;
			m_currentSong->getVolume(&volume);
			auto nextVolume = volume + deltaTime / M_SONG_FADE_IN_TIME;
			if (nextVolume >= 1.0f)
			{
				nextVolume = 1.0f;
				m_fade = fade_states::FADE_NONE;
			}
			DEBUG_LOG_AUDIO("Increasing volume by ", nextVolume - volume, ". Current volume = ", nextVolume);
			m_currentSong->setVolume(nextVolume);
		}
		else if (fade_states::FADE_OUT == m_fade)
		{
			float volume;
			m_currentSong->getVolume(&volume);
			const auto nextVolume = volume - deltaTime / M_SONG_FADE_IN_TIME;
			if (nextVolume <= 0.0f)
			{
				m_currentSong->stop();
				m_currentSong = nullptr;
				m_currentSongPath.clear();
				m_fade = fade_states::FADE_NONE;
			}
			else
			{
				m_currentSong->setVolume(nextVolume);
			}
		}
	}
	else if (!m_nextSongPath.empty())
	{
		PlaySong(m_nextSongPath);
		m_nextSongPath.clear();
	}

	FMOD_RESULT fmodResult = m_system->update();
	CHECK_CONDITION_ALWAYS_AUDIO(fmodResult == FMOD_OK, utility::logging::ERR, "Updating audio system has ended with error code ", fmodResult, ". ", FMOD_ErrorString(fmodResult));
}

void audio::AudioEngineFmod::LoadSoundEffect(const std::string& path)
{
	Load(categories::SOUND_EFFECT, path);
}

void audio::AudioEngineFmod::LoadSoundEffect3D(const std::string& path)
{
	Load(categories::SOUND_EFFECT_3D, path);
}

void audio::AudioEngineFmod::LoadSong(const std::string& path)
{
	Load(categories::SONG, path);
}

void audio::AudioEngineFmod::Load(categories::Category type, const std::string& path)
{
	if (m_sounds[type].find(path) != m_sounds[type].end())
	{
		DEBUG_LOG_AUDIO("The sound \"", path, "\" has been loaded already");
		return;
	}
	FMOD::Sound* sound;
	m_system->createSound(path.c_str(), m_modes[type], nullptr, &sound);
	m_sounds[type].insert(make_pair(path, sound));
}

math::Real audio::AudioEngineFmod::ChangeOctave(math::Real frequency, math::Real variation) const
{
	return frequency * pow(s_octaveRatio, variation);
}

math::Real audio::AudioEngineFmod::ChangeSemitone(math::Real frequency, math::Real variation) const
{
	return frequency * pow(s_semitoneRatio, variation);
}

void audio::AudioEngineFmod::PlaySoundEffect(const std::string& path)
{
	float volume, pitch;
	m_groups[categories::SOUND_EFFECT]->getVolume(&volume);
	m_groups[categories::SOUND_EFFECT]->getPitch(&pitch);
	PlaySoundEffect(path, volume, pitch);
}

void audio::AudioEngineFmod::PlaySoundEffect(const std::string& path, math::Real volume, math::Real pitch)
{
	// Trying to find sound effect and return if not found
	const auto soundItr = m_sounds[categories::SOUND_EFFECT].find(path);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(soundItr != m_sounds[categories::SOUND_EFFECT].end(), utility::logging::WARNING,
		"The requested sound effect \"", path, "\" has not been found");

	// Calculating random volume and pitch in selected range
	//const math::random::RandomGenerator& randomGenerator = math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::Generators::SIMPLE);
	//math::Real volume = randomGenerator.NextFloat(minVolume, maxVolume);
	//math::Real pitch = randomGenerator.NextFloat(minPitch, maxPitch);

	// Playing the sound effect with these initial values
	FMOD::Channel* channel;
	m_system->playSound(soundItr->second, m_groups[categories::SOUND_EFFECT], true, &channel);
	channel->setVolume(volume);
	math::Real frequency;
	channel->getFrequency(&frequency);
	channel->setFrequency(ChangeSemitone(frequency, pitch));
	channel->setPaused(false);
}

void audio::AudioEngineFmod::PlaySoundEffect3D(const std::string& path, math::Real volume, math::Real pitch, const math::Vector3D& position, const math::Vector3D& velocity)
{
	// Trying to find sound effect and return if not found
	auto soundItr = m_sounds[categories::SOUND_EFFECT_3D].find(path);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(soundItr != m_sounds[categories::SOUND_EFFECT].end(), utility::logging::WARNING,
		"The requested 3D sound effect \"", path, "\" has not been found");

	// Playing the sound effect with these initial values
	FMOD::Channel* channel;
	m_system->playSound(soundItr->second, m_groups[categories::SOUND_EFFECT_3D], true, &channel);
	channel->setVolume(volume);
	math::Real frequency;
	channel->getFrequency(&frequency);
	channel->setFrequency(ChangeSemitone(frequency, pitch));
	const FMOD_VECTOR fmodPosition = { position.x, position.y, position.z };
	const FMOD_VECTOR fmodVelocity = { velocity.x, velocity.y, velocity.z };
	channel->set3DAttributes(&fmodPosition, &fmodVelocity);
	channel->setPaused(false);
}

void audio::AudioEngineFmod::PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */)
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
	const auto soundItr = m_sounds[categories::SONG].find(path);
	CHECK_CONDITION_RETURN_VOID_ALWAYS_AUDIO(soundItr != m_sounds[categories::SONG].end(), utility::logging::WARNING,
		"Cannot play the requested song \"", path, "\". It has not been loaded yet.");

	// Start playing song with volume set to 0 and fade in
	m_currentSongPath = path;
	m_system->playSound(soundItr->second, m_groups[categories::SONG], true, &m_currentSong);
	//m_currentSong->setChannelGroup
	m_currentSong->setVolume(0.0f);
	m_currentSong->setPaused(false);
	m_fade = fade_states::FADE_IN;
}

void audio::AudioEngineFmod::SetMasterVolume(math::Real volume)
{
	m_master->setVolume(volume);
}

void audio::AudioEngineFmod::SetSoundEffectsVolume(math::Real volume)
{
	m_groups[categories::SOUND_EFFECT]->setVolume(volume);
}

void audio::AudioEngineFmod::SetSongsVolume(math::Real volume)
{
	m_groups[categories::SONG]->setVolume(volume);
}