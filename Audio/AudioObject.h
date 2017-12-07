#ifndef __AUDIO_AUDIO_OBJECT_H__
#define __AUDIO_AUDIO_OBJECT_H__

#include "Audio.h"
#include "SampleInfo.h"

namespace audio
{
	class IAudioData;

	/// <summary>
	/// The abstract representation of what an audio is. It is a sort of a wrapper for an audio data.
	/// </summary>
	class AudioObject
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API AudioObject(const SampleInfo& sampleInfo, IAudioData* audioData);
		AUDIO_API virtual ~AudioObject();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary>
		/// Generates the samples for the audio object.
		/// </summary>
		/// <param name="stream">The stream of audio.</param>
		/// <param name="streamLength">The audio stream length.</param>
		/// <returns>
		/// <code>true</code> if there is still more audio to generate and
		/// <code>false</code> if we get to the end of the audio file.
		/// </returns>
		AUDIO_API bool GenerateSamples(float* stream, size_t streamLength);

		/// <summary>
		/// Changes the position in the audio stream.
		/// </summary>
		/// <param name="pos"> The value of <code>0.0</code> represents the beginning of the audio file and
		/// <code>1.0</code> representing the end of the audio file. </param>
		AUDIO_API void SetPos(double pos);
	private:
		/// <summary>
		/// Converts the normalized position in range [0; 1] and returns the absolute position in the data.
		/// </summary>
		size_t ConvertPosToAbsolutePos(const double pos) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		size_t m_audioPos;
		size_t m_audioLength;
		SampleInfo m_sampleInfo;
		IAudioData* m_audioData;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class AudioObject */

} /* end namespace audio */

#endif /* __AUDIO_AUDIO_OBJECT_H__ */
