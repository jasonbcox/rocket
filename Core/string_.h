
#ifndef Rocket_Core_String_H
#define Rocket_Core_String_H

#include <string>

typedef std::string CORE_STRING_TYPE;

namespace Rocket {
	namespace Core {

		//Don't inherit std::string (for various reasons)
		class string {
		public:
			string();
			string( const char * input );
			string( CORE_STRING_TYPE input );

			bool operator == ( const string & rhs ) const;
			bool operator != ( const string & rhs ) const;

			size_t operator() ( const string & that ) const;

			string & operator = ( const string & rhs );

			const string operator + ( const string & other ) const;
			friend string operator + ( const char * str, const string & R );

			string & operator += ( const string & rhs );
			string & operator += ( const char & rhs );

			string & operator << ( const string & s );
			string & operator << ( const int & i );
			string & operator << ( const unsigned int & i );

			unsigned int length();

			//string toUpper()
			//string toLower()
			//string replace()
			//string trim(white,quotes,etc)

			const char * c_str();
			std::string std_str();

			int toint();
			static string tostring( int i_int );
			static string tostring( unsigned int i_int );

		private:
			CORE_STRING_TYPE m_value;
		};

	}
}

#endif