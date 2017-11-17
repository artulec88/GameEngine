#ifndef __AUDIO_AUDIO_ENGINE_FMOD_H__
#define __AUDIO_AUDIO_ENGINE_FMOD_H__

#include "Audio.h"
#include "IAudioEngine.h"
#include "fmod.hpp"
#include <map>

namespace audio
{
	class AudioEngineFmod : public IAudioEngine
	{
		/* TODO: Better key for more efficient map search ? */
		using Filenames2Sounds = std::map<std::string, FMOD::Sound*>;
	/* ==================== Static variables begin ==================== */
		static constexpr math::Real s_octaveRatio = 2.0f;
		static constexpr math::Real s_semitoneRatio = 1.0594630943592952645618252949463f; // = 2^(1/12)
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// FMOD Audio engine constructor. It intializes the main FMOD system object.
		/// </summary>
		/// <param name="audioDirectory"> The system location where audio files are located. </param>
		/// <param name="maxChannelsCount"> The maximum number of channels to be used by the audio engine. </param>
		AUDIO_API explicit AudioEngineFmod(const std::string& audioDirectory, const int maxChannelsCount);

		/// <summary>
		/// FMOD Audio engine destructor. It takes care of releasing the main FMOD system object,
		/// as well as all the sound objects that were created.
		/// </summary>
		AUDIO_API ~AudioEngineFmod();

		/// <summary> FMOD audio engine copy constructor. </summary>
		/// <param name="audioEngine"> The FMOD audio engine to copy construct from. </param>
		AudioEngineFmod(const AudioEngineFmod& audioEngine) = delete;

		/// <summary> FMOD audio engine move constructor. </summary>
		/// <param name="audioEngine"> The FMOD audio engine to move construct from. </param>
		AudioEngineFmod(AudioEngineFmod&& audioEngine) = delete;

		/// <summary> FMOD audio engine copy assignment operator. </summary>
		/// <param name="audioEngine"> The FMOD audio engine to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned FMOD audio engine. </returns>
		AudioEngineFmod& operator=(const AudioEngineFmod& audioEngine) = delete;

		/// <summary> FMOD audio engine move assignment operator. </summary>
		/// <param name="audioEngine"> The FMOD audio engine to move assign from. </param>
		/// <returns> The reference to the newly move-assigned FMOD audio engine. </returns>
		AudioEngineFmod& operator=(AudioEngineFmod&& audioEngine) = delete;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
		AUDIO_API void Update(math::Real deltaTime) override;
		AUDIO_API void LoadSoundEffect(const std::string& path) override;
		AUDIO_API void LoadSoundEffect3D(const std::string& path) override;
		AUDIO_API void LoadSong(const std::string& path) override;
	
		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>. It starts by first checking if the sound has already been loaded,
		/// and does nothing if this is the case. The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */) override;
		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>. It starts by first checking if the sound has already been loaded,
		/// and does nothing if this is the case. The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, math::Real volume, math::Real pitch) override;
		/// <summary>
		/// Plays the 3D sound effect specified in the <paramref name="path/>. It starts by first checking if the sound has already been loaded,
		/// and does nothing if this is the case. The method searches for the sound in the correct map and plays it back setting the volume, pitch, position and velocity of the source beforehand.
		/// </summary>
		AUDIO_API void PlaySoundEffect3D(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, math::Real volume, math::Real pitch, const math::Vector3D& position, const math::Vector3D& velocity) override;
		AUDIO_API void PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */) override;
		AUDIO_API void StopSoundEffects() override { m_groups[categories::SOUND_EFFECT]->stop(); }
		AUDIO_API void StopSong() override
		{
			if (m_currentSong != nullptr)
			{
				m_fade = fade_states::FADE_OUT;
			}
			m_nextSongPath.clear();
		}
		AUDIO_API void SetMasterVolume(math::Real volume) override;
		AUDIO_API void SetSoundEffectsVolume(math::Real volume) override;
		AUDIO_API void SetSongsVolume(math::Real volume) override;
	private:
		/// <summary>
		/// Loads the audio file from the specified <paramref name="path"/>.
		/// First though, the function checks whether the specified audio file has already been loaded.
		/// If that's the case, then there is no need to load the audio file again, so the method simply returns.
		/// Otherwise, the file is loaded and saved in the sounds map under the key indicated by the <paramref name="path"/> parameter.
		/// </summary>
		/// <param name="type">The type of the sound, e.g. song, sound effect, 3D sound effect.</param>
		/// <param name="path">The path of the audio file.</param>
		void Load(categories::Category type, const std::string& path);

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
		math::Real ChangeOctave(math::Real frequency, math::Real variation) const override;

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
		math::Real ChangeSemitone(math::Real frequency, math::Real variation) const override;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
		int m_maxChannelsCount;
		FMOD::System* m_system;
		FMOD::ChannelGroup* m_master;
		FMOD::ChannelGroup* m_groups[categories::COUNT];
		Filenames2Sounds m_sounds[categories::COUNT];
		FMOD_MODE m_modes[categories::COUNT];
		FMOD::Channel* m_currentSong;
		std::string m_currentSongPath;
		std::string m_nextSongPath;

		fade_states::FadeState m_fade;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AudioEngine_FMOD */

} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_ENGINE_FMOD_H__ */
