
#include "dsp.h"

#include <math.h>

namespace Rocket {
	namespace DigitalSignalProcessing {

		// Fast Fourier Transform
		// FourierDecimation must be a power of 2
		ComplexData * FFT( ComplexData * data, unsigned int FourierDecimation ) {
			if ( FourierDecimation == 1 ) {
				return data;
			} else {
				unsigned int halfDecimation = FourierDecimation / 2;
				double pi2oDecimation = 6.28318 / ( FourierDecimation * 1.0f ); //+1.570795

				ComplexData * output = new ComplexData[ FourierDecimation ];
				ComplexData * spliteven = new ComplexData[ halfDecimation ];
				ComplexData * splitodd = new ComplexData[ halfDecimation ];

				for ( unsigned int i = 0; i < halfDecimation; i++ ) {
					spliteven[i].r = data[ i*2 ].r;
					spliteven[i].i = data[ i*2 ].i;
				}
				ComplexData * even = FFT( spliteven, halfDecimation );

				for ( unsigned int i = 0; i < halfDecimation; i++) {
					splitodd[i].r = data[ i*2 + 1 ].r;
					splitodd[i].i = data[ i*2 + 1 ].i;
				}
				ComplexData * odd = FFT( splitodd, halfDecimation );

				for ( unsigned int k = 0; k < halfDecimation; k++ ) {
					double A = pi2oDecimation * k;
					double cosA = cos( A );
					double sinA = sin( A );
					double oddk_r = odd[k].r * cosA + odd[k].i * sinA;		//e^A = cosA + sinA
					double oddk_i = -odd[k].r * sinA + odd[k].i * cosA;		//e^A = sinA - cosA
					output[k].r = even[k].r + oddk_r;
					output[k].i = even[k].i + oddk_i;
					output[ k + halfDecimation ].r = even[k].r - oddk_r;
					output[ k + halfDecimation ].i = even[k].i - oddk_i;
				}

				delete [] spliteven;
				delete [] splitodd;
				delete [] even;
				delete [] odd;
				return output;
			}
		}

		// generatePowerSpectrum() returns a two-dimensional array of the maximum power over that point of resolution
		// data is a list of imaginary numbers (with real and imaginary components)
		// N is the number of elements in data
		// FourierDecimation is the decimation value used for the FFT (must be a power of 2)
		// PowerResolution is the width of the power spectrum
		double ** generatePowerSpectrum( ComplexData * data, unsigned int N, unsigned int FourierDecimation, unsigned int PowerResolution ) {
			if ( PowerResolution > FourierDecimation ) PowerResolution = FourierDecimation;

			double ** power = new double*[ N ];
			for ( unsigned int x = 0; x < N; x++ ) power[x] = new double[ PowerResolution ];

			for ( unsigned int row = 0; row < N/PowerResolution; row++ ) {
				ComplexData * fftData = FFT( data, FourierDecimation );
				int block = FourierDecimation / PowerResolution;
				int shift = PowerResolution / 2;

				for ( unsigned int y = 0; y < PowerResolution; y++ ) {
					double max = 0.0;
					for ( unsigned int x = y*block; x < y*block + block; x++ ) {
						double nm = fftData[x].r * fftData[x].r  +  fftData[x].i * fftData[x].i;
						if (nm > max) max = nm;
					}
					if ( y >= PowerResolution / 2 ) shift = -(int)(PowerResolution / 2);

					power[ row ][ y+shift ] = max;
				}

				delete [] fftData;
			}

			return power;
		}

	}
}