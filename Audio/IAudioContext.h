#ifndef __AUDIO_I_AUDIO_CONTEXT_H__
#define __AUDIO_I_AUDIO_CONTEXT_H__

#include "Audio.h"

namespace audio
{
	class AudioObject;

	/// <summary>
	/// The interface for basic audio functionality, e.g. playing, pausing and stopping audio.
	/// </summary>
	class IAudioContext
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API IAudioContext();
		AUDIO_API virtual ~IAudioContext();

		/// <summary> Audio context copy constructor. </summary>
		/// <param name="audioContext"> Audio context to copy construct from. </param>
		IAudioContext(const IAudioContext& audioContext) = delete;

		/// <summary> Audio context move constructor. </summary>
		/// <param name="audioContext"> Audio context to move construct from. </param>
		IAudioContext(IAudioContext&& audioContext) = delete;

		/// <summary> Audio context copy assignment operator. </summary>
		/// <param name="audioContext"> Audio context to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned audio context. </returns>
		IAudioContext& operator=(const IAudioContext& audioContext) = delete;

		/// <summary> Audio context move assignment operator. </summary>
		/// <param name="audioContext"> Audio context to move assign from. </param>
		/// <returns> The reference to the newly move-assigned audio context. </returns>
		IAudioContext& operator=(IAudioContext&& audioContext) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		AUDIO_API virtual void PlayAudio(AudioObject& audioObject) = 0;
		AUDIO_API virtual void PauseAudio(AudioObject& audioObject) = 0;
		AUDIO_API virtual void StopAudio(AudioObject& audioObject) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/* ==================== Non-static member variables end ==================== */
	}; /* end class IAudioContext */

} /* end namespace audio */

#endif /* __AUDIO_I_AUDIO_CONTEXT_H__ */
