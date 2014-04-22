
#ifndef Rocket_Audio_AudioDevice_H
#define Rocket_Audio_AudioDevice_H

#include <vector>
#include <memory>

#include "rtaudio/RtAudio.h"

#include "Sound.h"

using namespace std;

namespace Rocket {
	namespace Audio {

		class AudioDevice {
		public:
			AudioDevice( unsigned int numChannels, unsigned int sampleRate );
			~AudioDevice();

			void startStream();
			void stopStream();

			void addSound( Sound * sound );

		private:
			RtAudio * m_dac;
			RtAudio::StreamParameters m_outputParameters;
			bool m_streamStarted;
			unsigned int m_numChannels;
			unsigned int m_sampleRate;
			unsigned int m_bufferFrames;

			static int m_callback( void * outputBuffer,
									void * inputBuffer,
									unsigned int numBufferFrames,
									double streamTime,
									RtAudioStreamStatus status,
									void * data );

			vector< shared_ptr< Sound > > m_sounds;
		};

	}
}

#endif
