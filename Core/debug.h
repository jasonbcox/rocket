
#ifndef Rocket_Core_Debug_H
#define Rocket_Core_Debug_H

// Comment this out to disable debugging
#define ENABLE_DEBUG

// Whether or not to print to the console
#define DEBUG_PRINT_TO_CONSOLE 0

#ifdef ENABLE_DEBUG
namespace Rocket {
	namespace Core {

		static const unsigned long Debug_LogViewRefreshInterval = 1000;

		void Debug_AddToLog( const char * entry );

		char * Debug_PrintLogView();

		void Debug_StartTimer( const char * timerName );
		void Debug_StopTimer( const char * timerName );

		void Debug_PauseTimer( const char * timerName );
		void Debug_UnPauseTimer( const char * timerName );
		void Debug_ResetTimer( const char * timerName );

		// add direct support for class-origination and class members
		// add support for grabbing lists of local variables

	}
}
#else
// These functions get optimized out by the compiler
namespace Rocket {
	namespace Core {

#define Debug_AddToLog( const char * entry )

#define Debug_PrintLogView()

#define Debug_StartTimer( const char * timerName )
#define Debug_StopTimer( const char * timerName )

#define Debug_PauseTimer( const char * timerName )
#define Debug_UnPauseTimer( const char * timerName )
#define Debug_ResetTimer( const char * timerName )

	}
}
#endif

#endif