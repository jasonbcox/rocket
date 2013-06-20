
#ifndef Rocket_Core_FixedPoint_H
#define Rocket_Core_FixedPoint_H

#include "string_.h"
#include <cmath>

namespace Rocket {
	namespace Core {

		typedef float FixedPoint_OutputType;
		static const int FixedPoint_OutputTypeScalingFactor = 1;
		typedef double FixedPoint_LongOutputType;

		static const int FixedPoint_DefaultScalingFactor = 1;
		static const int FixedPoint_DefaultPrecision = 4;

		class fixedpoint {
		public:
			fixedpoint( FixedPoint_OutputType value = 0.0f );	// value is assumed to have a scaling factor of FixedPoint_OutputTypeScalingFactor and will be converted to FixedPoint_DefaultScalingFactor
			fixedpoint( long baseFactor, int scalingFactor, int precision = FixedPoint_DefaultPrecision );

			fixedpoint & operator = ( const fixedpoint & rhs );

			const fixedpoint operator - () const;

			const fixedpoint operator + ( const fixedpoint & other ) const;
			const fixedpoint operator - ( const fixedpoint & other ) const;
			const fixedpoint operator * ( const fixedpoint & other ) const;
			const fixedpoint operator / ( const fixedpoint & other ) const;

			fixedpoint & operator += ( const fixedpoint & rhs);
			fixedpoint & operator -= ( const fixedpoint & rhs);
			fixedpoint & operator *= ( const fixedpoint & rhs);
			fixedpoint & operator /= ( const fixedpoint & rhs);

			bool operator == ( const fixedpoint & rhs ) const;
			bool operator != ( const fixedpoint & rhs ) const;

			bool operator > ( const fixedpoint & rhs ) const;
			bool operator < ( const fixedpoint & rhs ) const;
			bool operator >= ( const fixedpoint & rhs ) const;
			bool operator <= ( const fixedpoint & rhs ) const;

			fixedpoint floor();
			fixedpoint ceiling();
			fixedpoint round();
			fixedpoint roundToNearest( const fixedpoint & nearestNumber );

			FixedPoint_OutputType toValue();
			string toString();

			static FixedPoint_OutputType sqrt( const fixedpoint input );

			int getScalingFactor();
			int getPrecision();

			void setScalingFactor( int scalingFactor );
			void setPrecision( int precision );
			fixedpoint convert( int scalingFactor, int precision ) const;

		private:
			long m_baseFactor;
			int m_scalingFactor;
			int m_precision;			// Use left-hand precision for all arithmetic.  m_precision is the number of decimal places this numbe is accurate to.
		};

	}
}

#endif
