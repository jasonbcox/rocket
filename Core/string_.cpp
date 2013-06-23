
#include <string>

#include "string_.h"

namespace Rocket {
	namespace Core {

		string::string() {
			m_value = "";
		}
		string::string( const char * input ) {
			m_value = input;
		}
		string::string( CORE_STRING_TYPE input ) {
			m_value = input;
		}

		bool string::operator == ( const string & rhs ) const {
			return m_value == rhs.m_value;
		}

		bool string::operator != ( const string & rhs ) const {
			return !(*this == rhs);
		}

		string & string::operator = ( const string & rhs ) {
			if (this != & rhs) {
				m_value = rhs.m_value;
			}
			return *this;
		}

		const string string::operator + ( const string & other ) const {
			return m_value + other.m_value;
		}
		string operator + ( const char * str, const string & R ) {
			return string( str ) + R;
		}

		string & string::operator += ( const string & rhs ) {
			m_value = m_value + rhs.m_value;
			return *this;
		}
		string & string::operator += ( const char & rhs ) {
			m_value = m_value + rhs;
			return *this;
		}

		string & string::operator << ( const string & s ) {
			m_value = string( m_value + s.m_value ).m_value;
			return *this;
		}
		string & string::operator << ( const int & i ) {
			m_value = string( m_value + tostring( i ).m_value ).m_value;
			return *this;
		}
		string & string::operator << ( const unsigned int & i ) {
			m_value = string( m_value + tostring( i ).m_value ).m_value;
			return *this;
		}


		unsigned int string::length() {
			return m_value.length();
		}

		const char * string::c_str() {
			return m_value.c_str();
		}

		std::string string::std_str() {
			return std::string( m_value );
		}

		int string::toint() {
			return atoi(m_value.c_str());
		}
		string string::tostring( int i_int ) {
			if ( i_int < 0 ) {
				string r = "-";
				return string( "-" ) + tostring( (unsigned int)(-i_int) );
			} else {
				return tostring( (unsigned int)i_int );
			}
		}
		void tostringhelper( unsigned int i, string & s ) {
			unsigned int n = i % 10;
			unsigned int rest = ( i - n ) / 10;
			if ( rest > 0 ) tostringhelper( rest, s );
			s += ( '0' + n );
		}
		string string::tostring( unsigned int i_uint ) {
			string r( "" );
			tostringhelper( i_uint, r );
			return r;
		}

	}
}