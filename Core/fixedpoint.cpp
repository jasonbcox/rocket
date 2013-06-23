
#include <sstream>
#include <math.h>

#include "debug.h"
#include "fixedpoint.h"
#include "mathconstants.h"

namespace Rocket {
	namespace Core {

		fixedpoint::fixedpoint( FixedPoint_OutputType value ) {
			m_scalingFactor = FixedPoint_DefaultScalingFactor;
			m_precision = FixedPoint_DefaultPrecision;

			FixedPoint_OutputType p = pow( 10.0f, m_precision );
			FixedPoint_OutputType v = value * p;
			m_baseFactor = ((long)v) * m_scalingFactor;
		}
		fixedpoint::fixedpoint( long baseFactor, int scalingFactor, int precision ) {
			m_baseFactor = baseFactor;
			m_scalingFactor = scalingFactor;
			m_precision = precision;
		}

		fixedpoint & fixedpoint::operator = ( const fixedpoint & rhs ) {
			if ( this != & rhs ) {
				m_baseFactor = rhs.m_baseFactor;
				m_scalingFactor = rhs.m_scalingFactor;
			}
			return *this;
		}


		const fixedpoint fixedpoint::operator - () const {
			return fixedpoint( -m_baseFactor, m_scalingFactor, m_precision );
		}


		const fixedpoint fixedpoint::operator + ( const fixedpoint & other ) const {
			long leftBaseFactor = m_baseFactor;
			long rightBaseFactor = other.m_baseFactor;
			int finalScalingFactor = m_scalingFactor;

			// Get common scaling factor before adding
			if ( other.m_scalingFactor > m_scalingFactor ) {
				leftBaseFactor *= (int)( other.m_scalingFactor*1.0/(m_scalingFactor*1.0) );
				finalScalingFactor = other.m_scalingFactor;
			} else if ( other.m_scalingFactor < m_scalingFactor ) {
				rightBaseFactor *= (int)( m_scalingFactor*1.0/(other.m_scalingFactor*1.0) );
			}
			return fixedpoint( leftBaseFactor + rightBaseFactor, finalScalingFactor, m_precision );
		}
		const fixedpoint fixedpoint::operator - ( const fixedpoint & other ) const {
			return ( *this + (-other) );
		}
		const fixedpoint fixedpoint::operator * ( const fixedpoint & other ) const {
			long long num = (long long)m_baseFactor * (long long)other.m_baseFactor;
			num /= (long long)pow( 10.0, m_precision );
			return fixedpoint( (int)num , m_scalingFactor * other.m_scalingFactor, m_precision );
		}
		const fixedpoint fixedpoint::operator / ( const fixedpoint & other ) const {
			// Extend the left base by a factor of 10^precision in order to eliminate rounding errors
			long long leftExtendedBase = (long long)( m_baseFactor * pow( 10.0, m_precision ) );

			if ( other.m_scalingFactor > m_scalingFactor ) {
				leftExtendedBase *= (long long)( other.m_scalingFactor*1.0/(m_scalingFactor*1.0) );
			}

			return fixedpoint( (int)( leftExtendedBase / (long long)other.m_baseFactor ), m_scalingFactor / other.m_scalingFactor, m_precision );
		}

		fixedpoint & fixedpoint::operator += ( const fixedpoint & rhs ) {
			fixedpoint left = fixedpoint( m_baseFactor, m_scalingFactor, m_precision );
			left = left + rhs;
			m_baseFactor = left.m_baseFactor;
			m_scalingFactor = left.m_scalingFactor;
			return *this;
		}
		fixedpoint & fixedpoint::operator -= ( const fixedpoint & rhs ) {
			fixedpoint left = fixedpoint( m_baseFactor, m_scalingFactor, m_precision );
			left = left - rhs;
			m_baseFactor = left.m_baseFactor;
			m_scalingFactor = left.m_scalingFactor;
			return *this;
		}
		fixedpoint & fixedpoint::operator *= ( const fixedpoint & rhs ) {
			fixedpoint left = fixedpoint( m_baseFactor, m_scalingFactor, m_precision );
			left = left * rhs;
			m_baseFactor = left.m_baseFactor;
			m_scalingFactor = left.m_scalingFactor;
			return *this;
		}
		fixedpoint & fixedpoint::operator /= ( const fixedpoint & rhs ) {
			fixedpoint left = fixedpoint( m_baseFactor, m_scalingFactor, m_precision );
			left = left / rhs;
			m_baseFactor = left.m_baseFactor;
			m_scalingFactor = left.m_scalingFactor;
			return *this;
		}

		bool fixedpoint::operator == ( const fixedpoint & rhs ) const {
			fixedpoint r = *this - rhs;
			return ( r.m_baseFactor == 0 );
		}
		bool fixedpoint::operator != ( const fixedpoint & rhs ) const {
			return !(*this == rhs );
		}
		bool fixedpoint::operator > ( const fixedpoint & rhs ) const {
			fixedpoint r = *this - rhs;
			return ( r.m_baseFactor > 0 );
		}
		bool fixedpoint::operator < ( const fixedpoint & rhs ) const {
			fixedpoint r = *this - rhs;
			return ( r.m_baseFactor < 0 );
		}
		bool fixedpoint::operator >= ( const fixedpoint & rhs ) const {
			fixedpoint r = *this - rhs;
			return ( r.m_baseFactor >= 0 );
		}
		bool fixedpoint::operator <= ( const fixedpoint & rhs ) const {
			fixedpoint r = *this - rhs;
			return ( r.m_baseFactor <= 0 );
		}

		fixedpoint fixedpoint::floor() {
			if ( m_precision > 1 ) {
				long long num = (long long)m_baseFactor * (long long)m_scalingFactor;
				num /= (long long)pow( 10.0, m_precision - 1 );
				long long rest = ( num / 10 ) * 10;
				rest *= (long long)pow( 10.0, m_precision - 1 );
				return fixedpoint( (int)( rest / (long long)m_scalingFactor ), m_scalingFactor, m_precision );
			}
			return fixedpoint( m_baseFactor, m_scalingFactor, m_precision );
		}
		fixedpoint fixedpoint::ceiling() {
			if ( m_precision > 1 ) {
				long long num = (long long)m_baseFactor * (long long)m_scalingFactor;
				num /= (long long)pow( 10.0, m_precision - 1 );
				long long rest = ( num / 10 ) * 10;
				num -= rest;
				if ( num > 0 ) rest += 10;
				rest *= (long long)pow( 10.0, m_precision - 1 );
				return fixedpoint( (int)( rest / (long long)m_scalingFactor ), m_scalingFactor, m_precision );
			}
			return fixedpoint( m_baseFactor, m_scalingFactor, m_precision );
		}
		fixedpoint fixedpoint::round() {
			if ( m_precision > 1 ) {
				long long num = (long long)m_baseFactor * (long long)m_scalingFactor;
				num /= (long long)pow( 10.0, m_precision - 1 );
				long long rest = ( num / 10 ) * 10;
				num -= rest;
				if ( num >= 5 ) rest += 10;
				rest *= (long long)pow( 10.0, m_precision - 1 );
				return fixedpoint( (int)( rest / (long long)m_scalingFactor ), m_scalingFactor, m_precision );
			}
			return fixedpoint( m_baseFactor, m_scalingFactor, m_precision );
			/*fixedpoint r = fixedpoint( m_baseFactor, m_scalingFactor, m_precision );
			fixedpoint half = fixedpoint( 0.5 );
			r += half;
			return r.floor();*/
		}
		fixedpoint fixedpoint::roundToNearest( const fixedpoint & nearestNumber ) {
			fixedpoint thisObject = ( (*this) / nearestNumber );
			double r = thisObject.toValue();
			r = std::floor( r + 0.5 );
			return nearestNumber * (FixedPoint_OutputType)r;
		}

		FixedPoint_OutputType fixedpoint::toValue() {
			long long num = m_baseFactor * m_scalingFactor;
			FixedPoint_LongOutputType r = num * pow( 0.1, m_precision );
			return (FixedPoint_OutputType)r;
		}
		string fixedpoint::toString() {
			std::stringstream ss;
			ss << toValue();
			return ss.str();
		}


		FixedPoint_OutputType fixedpoint::sqrt( const fixedpoint input ) {
			return sqrt( ((fixedpoint)input).toValue() );
		}


		int fixedpoint::getScalingFactor() {
			return m_scalingFactor;
		}
		int fixedpoint::getPrecision() {
			return m_precision;
		}

		void fixedpoint::setScalingFactor( int scalingFactor ) {
			long baseFactor = m_baseFactor * scalingFactor;
			m_baseFactor = (int)( baseFactor / (long)m_scalingFactor );
			m_scalingFactor = scalingFactor;
		}
		void fixedpoint::setPrecision( int precision ) {
			m_baseFactor *= (int)pow( 10.0, precision - m_precision );
			m_precision = precision;
		}
		fixedpoint fixedpoint::convert( int scalingFactor, int precision ) const {
			fixedpoint r = fixedpoint( m_baseFactor, m_scalingFactor, m_precision );
			r.setScalingFactor( scalingFactor );
			r.setPrecision( precision );
			return r;
		}

	}
}
