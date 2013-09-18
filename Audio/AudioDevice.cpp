
#include "AudioDevice.h"
#include "rocket/Core/debug.h"

namespace Rocket {
	namespace Audio {

		AudioDevice::AudioDevice( unsigned int numChannels, unsigned int sampleRate ) {
			m_dac = new RtAudio();
			if ( m_dac->getDeviceCount() < 1 ) {
				Debug_ThrowError( "Error: No audio devices detected.", 0 );
			}
			m_streamStarted = false;
			m_numChannels = numChannels;
			m_sampleRate = sampleRate;
			m_bufferFrames = 256;

			m_outputParameters.deviceId = m_dac->getDefaultOutputDevice();
			m_outputParameters.nChannels = numChannels;
			m_outputParameters.firstChannel = 0;
		}
		AudioDevice::~AudioDevice() {
			if ( m_streamStarted == true ) {
				try {
					stopStream();
				} catch ( RtError & e ) {
					Debug_ThrowError( "Error: Couldn't stop audio stream.", e.getMessage() );
				}
			}
			if ( m_dac->isStreamOpen() ) m_dac->closeStream();
			delete m_dac;
		}

		void AudioDevice::startStream() {
			try {
				m_dac->openStream( &m_outputParameters, NULL, RTAUDIO_FLOAT64,
								m_sampleRate, &m_bufferFrames, &m_callback, this );
				m_dac->startStream();
			} catch ( RtError & e ) {
				Debug_ThrowError( "Error: Failed to start audio stream.", e.getMessage() );
			}
			m_streamStarted = true;
		}
		void AudioDevice::stopStream() {
			m_streamStarted = false;
		}

		void AudioDevice::addSound( Sound * sound ) {
			m_sounds.push_back( sound->shared_from_this() );
		}

		// m_callback() - Callback that fills the sound buffer as needed to produce
		// a continuous stream.
		int AudioDevice::m_callback( void * outputBuffer,
										void * inputBuffer,
										unsigned int numBufferFrames,
										double streamTime,
										RtAudioStreamStatus status,
										void * data ) {
			double * buffer = (double*)outputBuffer;
			AudioDevice * device = (AudioDevice*)data;

			memset( buffer, 0, numBufferFrames * device->m_numChannels * sizeof( double ) );

			for ( auto sound : device->m_sounds ) {
				sound->addToBuffer( buffer, numBufferFrames );
			}

			// Take care of clipping
			for ( unsigned int f = 0; f < numBufferFrames; f++ ) {
				unsigned int fw = f * device->m_numChannels;
				for ( unsigned int c = 0; c < device->m_numChannels; c++ ) {
					unsigned int pos = fw + c;
					if ( buffer[ pos ] > 1.0 ) buffer[ pos ] = 1.0;
					if ( buffer[ pos ] < -1.0 ) buffer[ pos ] = -1.0;
				}
			}

			return 0;
		}

	}
}
