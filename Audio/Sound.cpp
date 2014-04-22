
#include "Sound.h"
#include "rocket/Core/utility.h"
#include "rocket/Core/debug.h"

namespace Rocket {
	namespace Audio {

		Sound::Sound( const char * file ) {
			SF_INFO sndinfo;
			memset( &sndinfo, 0, sizeof( SF_INFO ) );
			// Load a sound file using libsndfile (see libsndfile documentation for
			// a full list of supported formats)
			SNDFILE * sndfile = sf_open( file, SFM_READ, &sndinfo );
			if ( !sndfile ) {
				Debug_ThrowError( "Error: Couldn't open sound file.", file );
			}

			m_numChannels = sndinfo.channels;
			m_sampleRate = sndinfo.samplerate;
			m_numFrames = sf_seek( sndfile, -1, SEEK_END );
			m_data = shared_ptr< double >( new double[ m_numFrames * m_numChannels ], array_deleter< double >() );
			sf_seek( sndfile, 0, SEEK_SET );
			int numFramesRead = sf_readf_double( sndfile, m_data.get(), m_numFrames );
			if ( numFramesRead != m_numFrames ) {
				Debug_ThrowError( "Error: Couldn't read expected number of frames.", m_numFrames, numFramesRead );
			}
			sf_close( sndfile );

			m_framePosition = 0;
			m_playing = false;
			m_repeat = false;
		}
		Sound::Sound( unsigned int numChannels, unsigned int sampleRate, 
						unsigned int numFrames, shared_ptr< double > data ) {
			m_numChannels = numChannels;
			m_sampleRate = sampleRate;
			m_numFrames = numFrames;
			m_data = data;
			m_framePosition = 0;
			m_playing = false;
			m_repeat = false;
		}
		Sound::~Sound() {
		}

		//! Returns the number of audio channels for this Sound
		unsigned int Sound::getNumChannels() {
			return m_numChannels;
		}

		//! Returns the sample rate of this Sound
		unsigned int Sound::getSampleRate() {
			return m_sampleRate;
		}

		//! Returns the current sound frame position
		unsigned int Sound::getCurrentFrame() {
			return m_framePosition;
		}

		/*!	Play this Sound
		- replace - If the sound is already playing, replace the current frame
		- position with startFrame
		- repeat - If true, the sound will repeat until stopped.
		- startFrame - The frame to begin playing at.
		*/
		void Sound::play( bool replace, bool repeat, unsigned int startFrame ) {
			if ( ( replace == true ) || ( ( replace == false ) && ( m_playing == false ) ) ) {
				m_playing = true;
				m_repeat = repeat;
				m_framePosition = startFrame;
			}
		}

		//! Pauses this Sound.  Calling play( true, false, getCurrentFrame() ) will continue from when it was paused.
		void Sound::pause() {
			m_playing = false;
		}

		//! Stops this Sound.
		void Sound::stop() {
			pause();
			m_framePosition = 0;
		}

		//!	Adds values for numBufferFrames in buffer. The values that are added begin at m_framePosition where the sound left off at for the last addToBuffer() call.  If the end of the sound data is reached and m_repeat is false, the sound stops playing.
		void Sound::addToBuffer( double * buffer, unsigned int numBufferFrames ) {
			if ( m_playing == true ) {
				unsigned int frames = numBufferFrames;
				if ( m_framePosition + frames > m_numFrames ) {
					frames = m_numFrames - m_framePosition;
					if ( m_repeat == false ) stop();
				}
				unsigned int position = m_framePosition * m_numChannels;
				unsigned int elements = frames * m_numChannels;
				double * data = m_data.get();
				for ( unsigned int i = 0; i < elements; i++ ) {
					buffer[ i ] += data[ position + i ];
				}

				m_framePosition += frames;
				if ( m_framePosition >= m_numFrames ) m_framePosition = 0;

				unsigned int framesLeft = numBufferFrames - frames;
				if ( framesLeft > 0 ) {
					addToBuffer( &buffer[ elements ], framesLeft );
				}
			}
		}

		//! Returns a copy of this Sound, sharing the data pointer
		shared_ptr< Sound > Sound::clone() {
			return make_shared< Sound >( m_numChannels, m_sampleRate, m_numFrames, m_data );
		}

	}
}
