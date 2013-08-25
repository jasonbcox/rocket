
#ifndef Rocket_Core_String_H
#define Rocket_Core_String_H

#include <string>
#include <stdlib.h>

using namespace std;

typedef string CORE_STRING_TYPE;

namespace Rocket {
	namespace Core {

		//Don't inherit std::string (for various reasons)
		class rstring {
		public:
			rstring();
			rstring( const char * input );
			rstring( CORE_STRING_TYPE input );

			bool operator == ( const rstring & rhs ) const;
			bool operator != ( const rstring & rhs ) const;

			size_t operator() ( const rstring & that ) const;

			rstring & operator = ( const rstring & rhs );

			const rstring operator + ( const rstring & other ) const;
			friend rstring operator + ( const char * str, const rstring & R );

			rstring & operator += ( const rstring & rhs );
			rstring & operator += ( const char & rhs );

			rstring & operator << ( const rstring & s );
			rstring & operator << ( const int & i );
			rstring & operator << ( const unsigned int & i );

			unsigned int length();

			//string toUpper()
			//string toLower()
			//string replace()
			//string trim(white,quotes,etc)

			const char * c_str();
			string std_str();

			int toint();
			static rstring tostring( int i_int );
			static rstring tostring( unsigned int i_int );

		private:
			CORE_STRING_TYPE m_value;
		};

	}
}

#endif
