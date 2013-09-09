
#ifndef Rocket_Core_Debug_H
#define Rocket_Core_Debug_H

#include <iostream>
#include <string.h>
#include <assert.h>

#include "rstring.h"

using namespace Rocket::Core;

// Comment this out to disable debugging
#define ENABLE_DEBUG

#define FOREACH1( fn, arg ) fn( arg )
#define FOREACH2( fn, arg, ... ) do { fn( arg ); FOREACH1( fn, __VA_ARGS__ ); } while(0)
#define FOREACH3( fn, arg, ... ) do { fn( arg ); FOREACH2( fn, __VA_ARGS__ ); } while(0)
#define FOREACH4( fn, arg, ... ) do { fn( arg ); FOREACH3( fn, __VA_ARGS__ ); } while(0)
#define FOREACH5( fn, arg, ... ) do { fn( arg ); FOREACH4( fn, __VA_ARGS__ ); } while(0)
#define FOREACH6( fn, arg, ... ) do { fn( arg ); FOREACH5( fn, __VA_ARGS__ ); } while(0)
#define FOREACH7( fn, arg, ... ) do { fn( arg ); FOREACH6( fn, __VA_ARGS__ ); } while(0)
#define FOREACH8( fn, arg, ... ) do { fn( arg ); FOREACH7( fn, __VA_ARGS__ ); } while(0)
#define FOREACH9( fn, arg, ... ) do { fn( arg ); FOREACH8( fn, __VA_ARGS__ ); } while(0)
#define FOREACH10( fn, arg, ... ) do { fn( arg ); FOREACH9( fn, __VA_ARGS__ ); } while(0)
#define FOREACH11( fn, arg, ... ) do { fn( arg ); FOREACH10( fn, __VA_ARGS__ ); } while(0)
#define FOREACH12( fn, arg, ... ) do { fn( arg ); FOREACH11( fn, __VA_ARGS__ ); } while(0)
#define FOREACH13( fn, arg, ... ) do { fn( arg ); FOREACH12( fn, __VA_ARGS__ ); } while(0)
#define FOREACH14( fn, arg, ... ) do { fn( arg ); FOREACH13( fn, __VA_ARGS__ ); } while(0)
#define FOREACH15( fn, arg, ... ) do { fn( arg ); FOREACH14( fn, __VA_ARGS__ ); } while(0)
#define FOREACH16( fn, arg, ... ) do { fn( arg ); FOREACH15( fn, __VA_ARGS__ ); } while(0)

# define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16, MACRO_NAME, ... ) MACRO_NAME

// FOREACH() - for-each loop on up to 16 arguments, calling fn() on each argument
// Note: At LEAST one argument must be given other than fn()
#define FOREACH( fn, ... ) \
	GET_MACRO( __VA_ARGS__, \
		FOREACH16,FOREACH15,FOREACH14,FOREACH13, \
		FOREACH12,FOREACH11,FOREACH10,FOREACH9, \
		FOREACH8,FOREACH7,FOREACH6,FOREACH5, \
		FOREACH4,FOREACH3,FOREACH2,FOREACH1 )( fn, __VA_ARGS__ )

// Whether or not to print to the console
#define DEBUG_PRINT_TO_CONSOLE 0

#ifdef ENABLE_DEBUG
namespace Rocket {
	namespace Core {

		void Debug_Scramble ( void * pointer, size_t size );

		#define Debug_PrintVar( arg ) \
		do { \
			std::cout << #arg; \
			std::cout << ": "; \
			std::cout << arg; \
			std::cout << "\n"; \
		} while(0)

		// Display errorText and values for up to 16 variables
		// Note: At LEAST one variable must be given
		#define Debug_ThrowError( errorText, ... ) \
		do { \
			std::cout << errorText << "\n"; \
			FOREACH( Debug_PrintVar, __VA_ARGS__ ); \
			assert( false ); \
		} while(0)

		void Debug_StartTimer( const char * timerName );
		void Debug_StopTimer( const char * timerName );

		void Debug_PauseTimer( const char * timerName );
		void Debug_UnPauseTimer( const char * timerName );
		void Debug_ResetTimer( const char * timerName );

	}
}
#else
// These functions get optimized out by the compiler
namespace Rocket {
	namespace Core {

#define void Debug_Scramble ( void * pointer, size_t size )

#define Debug_StartTimer( const char * timerName )
#define Debug_StopTimer( const char * timerName )

#define Debug_PauseTimer( const char * timerName )
#define Debug_UnPauseTimer( const char * timerName )
#define Debug_ResetTimer( const char * timerName )

	}
}
#endif

#endif
