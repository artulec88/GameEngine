#ifndef __AUDIO_IAUDIO_ENGINE_H__
#define __AUDIO_IAUDIO_ENGINE_H__

#include "Audio.h"

//#include "Math/Vector.h"

#include <string>

namespace math
{
	using Real = float;
	struct Vector3D;
} /* end namespace math */

namespace audio
{
	namespace categories
	{
		enum Category
		{
			SOUND_EFFECT = 0,
			SOUND_EFFECT_3D,
			SONG,
			COUNT // The number of categories
		}; /* end enum Category */
	} /* end namespace categories */

	namespace fade_states
	{
		enum FadeState
		{
			FADE_NONE = 0,
			FADE_IN,
			FADE_OUT
		}; /* end enum FadeState */
	} /* end namespace fade_states */


	// TODO: Instead of identifying the sounds using their "path" strings it would be more appropriate to identify them using an enum or integer value.
	// The Audio design patterns are described in Game Programming Gems 2 in chapter 6.1. Briefly, what they describe is:
	// BRIDGE: "Decouple an abstraction from its implementation so that the two can vary independently.":
	// StartSound(int soundId);
	// StopSound(int soundId);
	// LoadSound(int soundId);
	// UnloadSound(int soundId);
	// LoadSoundsGroup(int soundId);
	// UnloadSoundsGroup(int soundId);
	// 
	// Define the "Error" sound which may be played when something goes wrong.
	//
	// FACADE: "Provide a unified interface to a set of interfaces in a subsystem. Facade defines a higher-level interface that makes the subsystem earier to use"
	// SetMasterVolume(float volume);
	// GetMasterVolume();
	// StopAllSounds();
	//
	// COMPOSITE: "Compose objects into tree structures to represent part-whole hierarchies. Composite lets client treat individual objects and compositions of objects uniformly."
	// Something like a car engine may actually be composed of many different sounds. The parameters of these sounds may be mapped to various game parameters such as engine type, throttle level, speed, etc.
	// In this case, we may want to hide this complexity and provide functions specific to the type of engine we are controlling.
	// StartEngine(Car* carInstance);
	// UpdateEngine(Car* carInstance);
	// StopEngine(Car* carInstance);
	// Now the internal audio code can interpret the states and parameters available in the given carInstance object and translate them into the control of one or more sounds. Our client is able to treat
	// engine the same way, whether we are controlling an individual sound object or compositions of sound objects.
	// Control of ambient sounds can also be a situation that exhibits composite behavior. If we want to simulate an environment such as a jungle, we may be randomly playing many animal sounds. In this case,
	// we may not have a class in the game called Jungle but we may know our distance from the jungle area and how excited the animals are:
	// StartJungle(float distance, float activity);
	// UpdateJungle(float distance, float activity);
	// StopJungle();
	//
	// PROXY: "Provide a surrogate or placeholder for another object to control access to it."
	// When we start a particular instance of a sound, we may want to continue to control that instance over time. Our control parameters may be anything from 3D position to direct control over volume, pitch etc.
	// SoundHandle StartHandledSound(int soundId, ControlParameters controlParameters);
	// UpdateHandledSound(SoundHandle soundHandle, ControlParameters controlParameters);
	// StopHandledSound(SoundHandle soundHandle);
	//
	// DECORATOR: "Attach additional responsibilities to an object dynamically. Decorators provide a flexible alternative to subclassing for extending functionality."
	// * User data: One way to allow dynamic responsibilities and associations to be given to an object is to provide user data access.
	// * Callbacks: We can also provide a callback field that clients can set per instance of a sound. This callback could be defined to be triggered when a sound loops or has played for a certain amount of time.
	//
	// COMMAND: "Encapsulate a request as an object, thereby letting you parameterize clients with different requests, queue or log requests, and support undoable operations."


	/// <summary>
	/// The interface for the audio engine.
	/// </summary>
	class IAudioEngine
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Audio engine constructor.
		/// </summary>
		/// <param name="audioDirectory"> The system directory where all sounds are located. </param>
		AUDIO_API explicit IAudioEngine(const std::string& audioDirectory);

		/// <summary>
		/// Audio engine destructor.
		/// </summary>
		AUDIO_API virtual ~IAudioEngine();

		/// <summary> Audio engine copy constructor. </summary>
		/// <param name="audioEngine"> The audio engine to copy construct from. </param>
		IAudioEngine(const IAudioEngine& audioEngine) = delete;

		/// <summary> Audio engine move constructor. </summary>
		/// <param name="audioEngine"> The audio engine to move construct from. </param>
		IAudioEngine(IAudioEngine&& audioEngine) = delete;

		/// <summary> Audio engine copy assignment operator. </summary>
		/// <param name="audioEngine"> The audio engine to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned audio engine. </returns>
		IAudioEngine& operator=(const IAudioEngine& audioEngine) = delete;

		/// <summary> Audio engine move assignment operator. </summary>
		/// <param name="audioEngine"> The audio engine to move assign from. </param>
		/// <returns> The reference to the newly move-assigned audio engine. </returns>
		IAudioEngine& operator=(IAudioEngine&& audioEngine) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Updates the state of the audio engine. For irrKlang audio engine running in multithreaded mode this function may be an empty operation.
		/// </summary>
		AUDIO_API virtual void Update(math::Real deltaTime) = 0;
		AUDIO_API virtual void LoadSoundEffect(const std::string& path) = 0;
		AUDIO_API virtual void LoadSoundEffect3D(const std::string& path) = 0;
		AUDIO_API virtual void LoadSong(const std::string& path) = 0;

		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>. If the sound under the specified key has not been loaded yet, the function simply returns.
		/// The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */) = 0;
		/// <summary>
		/// Plays the sound effect specified in the <paramref name="path/>. If the sound under the specified key has not been loaded yet, the function simply returns.
		/// The method searches for the sound in the correct map and plays it back setting the volume and pitch beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */, math::Real volume, math::Real pitch) = 0;
		/// <summary>
		/// Plays the 3D sound effect specified in the <paramref name="path/>.
		/// The method searches for the sound in the correct map and plays it back setting the volume, pitch, position and velocity of the source beforehand.
		/// </summary>
		AUDIO_API virtual void PlaySoundEffect3D(const std::string& path /* TODO: Better parameter to identify which sound effect to play? */,
			math::Real volume, math::Real pitch, const math::Vector3D& position, const math::Vector3D& velocity) = 0;
		AUDIO_API virtual void PlaySong(const std::string& path /* TODO: Better parameter to identify which song to play? */) = 0;
		AUDIO_API virtual void StopSoundEffects() = 0;
		AUDIO_API virtual void StopSong() = 0;
		AUDIO_API virtual void SetMasterVolume(math::Real volume) = 0;
		AUDIO_API virtual void SetSoundEffectsVolume(math::Real volume) = 0;
		AUDIO_API virtual void SetSongsVolume(math::Real volume) = 0;
	protected:
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
		virtual math::Real ChangeOctave(math::Real frequency, math::Real variation) const = 0;

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
		virtual math::Real ChangeSemitone(math::Real frequency, math::Real variation) const = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		const std::string& m_audioDirectory;
		const float M_SONG_FADE_IN_TIME;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioEngine */

} /* end namespace audio */

#endif /* __AUDIO_IAUDIO_ENGINE_H__ */
