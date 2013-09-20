
#ifndef Rocket_Audio_Sound_H
#define Rocket_Audio_Sound_H

#include <memory>

#include "libsndfile/sndfile.h"

using namespace std;

namespace Rocket {
	namespace Audio {

		class Sound : public enable_shared_from_this< Sound > {
		public:
			Sound( const char * file );
			Sound( unsigned int numChannels, unsigned int sampleRate,
					unsigned int numFrames, shared_ptr< double > data );
			~Sound();

			unsigned int getNumChannels();
			unsigned int getSampleRate();
			unsigned int getCurrentFrame();

			// play() - Play the sound
			// replace - If the sound is already playing, replace the current frame
			// position with startFrame
			// repeat - If true, the sound will repeat until stopped.
			// startFrame - The frame to begin playing at.
			void play( bool replace = true, bool repeat = false,
						unsigned int startFrame = 0 );
			void pause();
			void stop();

			// addToBuffer() - Adds sound data to the audio stream buffer
			void addToBuffer( double * buffer, unsigned int numBufferFrames );

			// clone() - Returns a copy of this Sound, sharing the data pointer
			shared_ptr< Sound > clone();

		private:
			unsigned int m_numChannels;
			unsigned int m_sampleRate;
			unsigned int m_numFrames;
			shared_ptr< double > m_data;	// shared_ptr to an array of sound data

			bool m_playing;
			bool m_repeat;
			unsigned int m_framePosition;
		};

	}
}

#endif
