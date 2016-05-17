#ifndef __AUDIO_AUDIO_ENGINE_H__
#define __AUDIO_AUDIO_ENGINE_H__

#include "Audio.h"
#include "Sound.h"
#include "fmod.hpp"

#include "Math\Math.h"

#include <string>
#include <vector>
#include <map>

namespace Audio
{
	namespace Categories
	{
		enum Category
		{
			SOUND_EFFECT = 0,
			//SOUND_EFFECT_3D,
			SONG,
			COUNT // The number of categories
		}; /* end enum Category */
	} /* end namespace Categories */

	namespace FadeStates
	{
		enum FadeState
		{
			FADE_NONE = 0,
			FADE_IN,
			FADE_OUT
		}; /* end enum FadeState */
	} /* end namespace FadeStates */

	class AudioEngine
	{
		/* TODO: Better key for more efficient map search ? */
		typedef std::map<std::string, FMOD::Sound*> Filenames2Sounds;
	/* ==================== Static variables begin ==================== */
	private:
		static const Math::Real OCTAVE_RATIO;
		static const Math::Real SEMITONE_RATIO;
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API explicit AudioEngine(int maxChannelsCount);
		AUDIO_API ~AudioEngine(void);
	private:
		AudioEngine(const AudioEngine& audioEngine);
		void operator=(const AudioEngine& audioEngine);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		AUDIO_API void Update(Math::Real deltaTime);
		AUDIO_API void LoadSoundEffect(const std::string& path);
		AUDIO_API void LoadSong(const std::string& path);
		//AUDIO_API void LoadSoundEffect3D(const std::string& path);
		
		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>.
		/// The method searches for the sound in the correct map, and plays it back like before,
		/// except that it sets the volume and pitch of the sound using random values generated within the selected ranges.
		/// </summary>
		AUDIO_API void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */,
			Math::Real minVolume, Math::Real maxVolume, Math::Real minPitch, Math::Real maxPitch);
		AUDIO_API void PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */);
		AUDIO_API void StopSoundEffects() { m_groups[Categories::SOUND_EFFECT]->stop(); }
		AUDIO_API void StopSong()
		{
			if (m_currentSong != NULL)
			{
				m_fade = FadeStates::FADE_OUT;
			}
			m_nextSongPath.clear();
		}
		AUDIO_API void SetMasterVolume(Math::Real volume);
		AUDIO_API void SetSoundEffectsVolume(Math::Real volume);
		AUDIO_API void SetSongsVolume(Math::Real volume);
	private:
		void Load(Categories::Category type, const std::string& path);

		/// <summary>
		/// A helper method for calculating the octaves. By using this method it becomes simple to modify the pitch of a sound.
		/// For example, to decrease the pitch of a sound by 3 octaves, we could do the following:
		/// <code>
		/// float frequency;
		/// channel->getFrequency(&frequency);
		/// float newFrequency = ChangeOctave(frequency, -3.0f);
		/// channel->setFrequency(newFrequency);
		/// </code>
		/// Every time we multiply / divide a frequency by two we get a new frequency that sounds one octave higher / lower.
		/// </summary>
		/// <returns>
		/// New frequency of a sound.
		/// </returns>
		Math::Real ChangeOctave(Math::Real frequency, Math::Real variation) const;

		/// <summary>
		/// A helper method for calculating the semitones. By using this method it becomes simple to modify the pitch of a sound.
		/// For example, to decrease the pitch of a sound by 3 semitones, we could do the following:
		/// <code>
		/// float frequency;
		/// channel->getFrequency(&frequency);
		/// float newFrequency = ChangeSemitone(frequency, -3.0f);
		/// channel->setFrequency(newFrequency);
		/// </code>
		/// Every time we multiply / divide a frequency by two and a half we get a new frequency that sounds one semitone higher / lower.
		/// </summary>
		/// <returns>
		/// New frequency of a sound.
		/// </returns>
		Math::Real ChangeSemitone(Math::Real frequency, Math::Real variation) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		int m_maxChannelsCount;
		FMOD::System* m_system;
		FMOD::ChannelGroup* m_master;
		FMOD::ChannelGroup* m_groups[Categories::COUNT];
		Filenames2Sounds m_sounds[Categories::COUNT];
		FMOD_MODE m_modes[Categories::COUNT];
		FMOD::Channel* m_currentSong;
		std::string m_currentSongPath;
		std::string m_nextSongPath;

		FadeStates::FadeState m_fade;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AudioEngine */

} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_ENGINE_H__ */
