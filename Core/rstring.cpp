
#include <string>

#include "rstring.h"

namespace Rocket {
	namespace Core {

		rstring::rstring() {
			m_value = "";
		}
		rstring::rstring( const char * input ) {
			m_value = input;
		}
		rstring::rstring( CORE_STRING_TYPE input ) {
			m_value = input;
		}

		bool rstring::operator == ( const rstring & rhs ) const {
			return m_value == rhs.m_value;
		}

		bool rstring::operator != ( const rstring & rhs ) const {
			return !(*this == rhs);
		}

		rstring & rstring::operator = ( const rstring & rhs ) {
			if (this != & rhs) {
				m_value = rhs.m_value;
			}
			return *this;
		}

		const rstring rstring::operator + ( const rstring & other ) const {
			return m_value + other.m_value;
		}
		rstring operator + ( const char * str, const rstring & R ) {
			return rstring( str ) + R;
		}

		rstring & rstring::operator += ( const rstring & rhs ) {
			m_value = m_value + rhs.m_value;
			return *this;
		}
		rstring & rstring::operator += ( const char & rhs ) {
			m_value = m_value + rhs;
			return *this;
		}

		rstring & rstring::operator << ( const rstring & s ) {
			m_value = rstring( m_value + s.m_value ).m_value;
			return *this;
		}
		rstring & rstring::operator << ( const int & i ) {
			m_value = rstring( m_value + tostring( i ).m_value ).m_value;
			return *this;
		}
		rstring & rstring::operator << ( const unsigned int & i ) {
			m_value = rstring( m_value + tostring( i ).m_value ).m_value;
			return *this;
		}


		unsigned int rstring::length() {
			return m_value.length();
		}

		const char * rstring::c_str() {
			return m_value.c_str();
		}

		string rstring::std_str() {
			return string( m_value );
		}

		int rstring::toint() {
			return atoi(m_value.c_str());
		}
		rstring rstring::tostring( int i_int ) {
			if ( i_int < 0 ) {
				rstring r = "-";
				return rstring( "-" ) + tostring( (unsigned int)(-i_int) );
			} else {
				return tostring( (unsigned int)i_int );
			}
		}
		void tostringhelper( unsigned int i, rstring & s ) {
			unsigned int n = i % 10;
			unsigned int rest = ( i - n ) / 10;
			if ( rest > 0 ) tostringhelper( rest, s );
			s += ( '0' + n );
		}
		rstring rstring::tostring( unsigned int i_uint ) {
			rstring r( "" );
			tostringhelper( i_uint, r );
			return r;
		}

	}
}