#ifndef __AUDIO_FMOD_AUDIO_CONTEXT_H__
#define __AUDIO_FMOD_AUDIO_CONTEXT_H__

#include "Audio.h"
#include "IAudioContext.h"
#include "fmod.hpp"

#include <vector>

namespace audio
{
	namespace fmod
	{
		class AudioContextFmod : public IAudioContext
		{
			/* ==================== Static variables begin ==================== */
			/* ==================== Static variables end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			AUDIO_API AudioContextFmod();
			AUDIO_API virtual ~AudioContextFmod();
			
			/// <summary> FMOD audio context copy constructor. </summary>
			/// <param name="fmodAudioContext"> FMOD audio context to copy construct from. </param>
			AudioContextFmod(const AudioContextFmod& fmodAudioContext) = delete;

			/// <summary> FMOD audio context move constructor. </summary>
			/// <param name="fmodAudioContext"> FMOD audio context to move construct from. </param>
			AudioContextFmod(AudioContextFmod&& fmodAudioContext) = delete;

			/// <summary> FMOD audio context copy assignment operator. </summary>
			/// <param name="fmodAudioContext"> FMOD audio context to copy assign from. </param>
			/// <returns> The reference to the newly copy-assigned FMOD audio context. </returns>
			AudioContextFmod& operator=(const AudioContextFmod& fmodAudioContext) = delete;

			/// <summary> FMOD audio context move assignment operator. </summary>
			/// <param name="fmodAudioContext"> FMOD audio context to move assign from. </param>
			/// <returns> The reference to the newly move-assigned FMOD audio context. </returns>
			AudioContextFmod& operator=(AudioContextFmod&& fmodAudioContext) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
			AUDIO_API void PlayAudio(AudioObject& audioObject) override;
			AUDIO_API void PauseAudio(AudioObject& audioObject) override;
			AUDIO_API void StopAudio(AudioObject& audioObject) override;
			AUDIO_API void GenerateSamples(int* stream, int streamLength);
		private:
			/// <summary>
			/// Removes the audio object from the vector of audio objects currently being played.
			/// </summary>
			bool RemoveAudio(AudioObject& audioObject);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
			FMOD::System* m_system;
			std::vector<float> m_stream;
			/// <summary>
			/// A vector of audio objects currently being played.
			/// </summary>
			std::vector<AudioObject*> m_playingAudioObjects;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class FmodAudioContext */
	
	} /* end namespace fmod */

} /* end namespace audio */

#endif /* __AUDIO_FMOD_AUDIO_CONTEXT_H__ */
