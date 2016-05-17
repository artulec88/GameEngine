#ifndef __AUDIO_AUDIO_OBJECT_H__
#define __AUDIO_AUDIO_OBJECT_H__

#include "Audio.h"
#include "IAudioData.h"
#include "SampleInfo.h"

namespace Audio
{
	class AudioObject
	{
	/* ==================== Static variables begin ==================== */
	/* ==================== Static variables end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		AUDIO_API AudioObject(const SampleInfo& sampleInfo, IAudioData* audioData);
		AUDIO_API virtual ~AudioObject(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		/// <returns>
		/// <code>true</code> if there is still more audio to generate and
		/// <code>false</code> if we get to the end of the audio file.
		/// </returns>
		bool GenerateSamples(float* stream, size_t streamLength);

		/// <param name="pos"> The value of <code>0</code> represents the beginning of the audio file and
		/// <code>1</code> representing the end of the audio file. </param>
		void SetPos(double pos);
	private:
		/// <summary>
		/// Converts the normalized position in range [0; 1] and returns the absolute position in the data.
		/// </summary>
		size_t ConvertPosToAbsolutePos(double pos);
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		size_t m_audioPos;
		size_t m_audioLength;
		SampleInfo m_sampleInfo;
		IAudioData* m_audioData;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class AudioObject */

} /* end namespace Audio */

#endif /* __AUDIO_AUDIO_OBJECT_H__ */
