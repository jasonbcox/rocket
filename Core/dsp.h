
#ifndef Rocket_Core_DSP_H
#define Rocket_Core_DSP_H


namespace Rocket {
	namespace DigitalSignalProcessing {

		typedef double complexSubType;

		struct ComplexData {
			complexSubType r;
			complexSubType i;
		};

		// Fast Fourier Transform
		ComplexData * FFT( ComplexData * data, unsigned int FourierDecimation );

		double ** generatePowerSpectrum( ComplexData * data, unsigned int N, unsigned int FourierDecimation, unsigned int PowerResolution );

	}

}


#endif