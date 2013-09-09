
#include "debug.h"

#ifdef ENABLE_DEBUG

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>

using namespace std;

#include "timer.h"
#include "rstring.h"

namespace Rocket {
	namespace Core {

		unsigned long Debug_ScrambleID = 0xDEADF00D;

		void Debug_Scramble ( void * pointer, size_t size ) {
			memset( pointer, Debug_ScrambleID, size );
			Debug_ScrambleID++;
		}

		// name, <timer, elapsed>
		map< string, pair< unsigned long, long > > Debug_Timers;

		void Debug_StartTimer( const char * timerName ) {
			Debug_Timers[timerName] = pair< unsigned long, long >( Core::timer(), -1 );
		}

		void Debug_StopTimer( const char * timerName ) {
			map< string, pair< unsigned long, long > >::iterator iter = Debug_Timers.find( timerName );
			if ( iter != Debug_Timers.end() ) {
				if ( (*iter).second.second == -1 ) {
					(*iter).second.second = (long)Core::elapsedtime( (*iter).second.first );
				}
			}
		}

		void Debug_PauseTimer( const char * timerName ) {
			//long elapsedTime = Debug_Timers[ timerName ].second;
			//Debug_StopTimer( timerName );
			if ( Debug_Timers[ timerName ].second == -1 ) Debug_Timers[ timerName ].second = 0;
			Debug_Timers[ timerName ].second += (long)Core::elapsedtime( Debug_Timers[ timerName ].first );
		}

		void Debug_UnPauseTimer( const char * timerName ) {
			long elapsedTime = -1;
			map< string, pair< unsigned long, long > >::iterator iter = Debug_Timers.find( timerName );
			if ( iter != Debug_Timers.end() ) {
				elapsedTime = Debug_Timers[timerName].second;
			}

			Debug_StartTimer( timerName );
			Debug_Timers[timerName].second = elapsedTime;
		}

		void Debug_ResetTimer( const char * timerName ) {
			Debug_Timers.erase( timerName );
		}

	}
}

#endif
