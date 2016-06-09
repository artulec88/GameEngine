#ifndef __AUDIO_AUDIO_ENGINE_FMOD_H__
#define __AUDIO_AUDIO_ENGINE_FMOD_H__

#include "Audio.h"
#include "IAudioEngine.h"
#include "Sound.h"
#include "fmod.hpp"
#include <vector>
#include <map>

namespace Audio
{
	class AudioEngine_FMOD : public IAudioEngine
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
		/// <summary>
		/// FMOD Audio engine constructor. It intializes the main FMOD system object.
		/// </summary>
		AUDIO_API explicit AudioEngine_FMOD(int maxChannelsCount);
		/// <summary>
		/// FMOD Audio engine destructor. It takes care of releasing the main FMOD system object,
		/// as well as all the sound objects that were created.
		/// </summary>
		AUDIO_API ~AudioEngine_FMOD(void);
	private:
		AudioEngine_FMOD(const AudioEngine_FMOD& audioEngine);
		void operator=(const AudioEngine_FMOD& audioEngine);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		AUDIO_API virtual void Update(Math::Real deltaTime);
		AUDIO_API virtual void LoadSoundEffect(const std::string& path);
		AUDIO_API virtual void LoadSoundEffect3D(const std::string& path);
		AUDIO_API virtual void LoadSong(const std::string& path);
	
		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>. It starts by first checking if the sound has already been loaded,
		/// and does nothing if this is the case. The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */);
		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>. It starts by first checking if the sound has already been loaded,
		/// and does nothing if this is the case. The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, Math::Real volume, Math::Real pitch);
		/// <summary>
		/// Plays the 3D sound effect specified in the <paramref name="path/>. It starts by first checking if the sound has already been loaded,
		/// and does nothing if this is the case. The method searches for the sound in the correct map and plays it back setting the volume, pitch, position and velocity of the source beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect3D(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, Math::Real volume, Math::Real pitch, const Math::Vector3D& position, const Math::Vector3D& velocity);
		AUDIO_API virtual void PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */);
		AUDIO_API virtual void StopSoundEffects() { m_groups[Categories::SOUND_EFFECT]->stop(); }
		AUDIO_API virtual void StopSong()
		{
			if (m_currentSong != NULL)
			{
				m_fade = FadeStates::FADE_OUT;
			}
			m_nextSongPath.clear();
		}
		AUDIO_API virtual void SetMasterVolume(Math::Real volume);
		AUDIO_API virtual void SetSoundEffectsVolume(Math::Real volume);
		AUDIO_API virtual void SetSongsVolume(Math::Real volume);
	private:
		/// <summary>
		/// Loads the audio file from the specified <paramref name="path"/>.
		/// First though, the function checks whether the specified audio file has already been loaded.
		/// If that's the case, then there is no need to load the audio file again, so the method simply returns.
		/// Otherwise, the file is loaded and saved in the sounds map under the key indicated by the <paramref name="path"/> parameter.
		/// </summary>
		/// <param name="type">The type of the sound, e.g. song, sound effect, 3D sound effect.</param>
		/// <param name="path">The path of the audio file.</param>
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
		virtual Math::Real ChangeOctave(Math::Real frequency, Math::Real variation) const;

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
		virtual Math::Real ChangeSemitone(Math::Real frequency, Math::Real variation) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		const float M_SONG_FADE_IN_TIME;
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
	}; /* end class AudioEngine_FMOD */

} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_ENGINE_FMOD_H__ */
