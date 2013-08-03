
#include "debug.h"

#ifdef ENABLE_DEBUG

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>

#include "timer.h"
#include "string_.h"

namespace Rocket {
	namespace Core {

		unsigned long Debug_ScrambleID = 0xDEADF00D;

		void Debug_Scramble ( void * pointer, size_t size ) {
			memset( pointer, Debug_ScrambleID, size );
			Debug_ScrambleID++;
		}

		//std::vector<std::string> Debug_Log;
		std::vector<std::string> Debug_LogView;
		unsigned long Debug_LogViewTimer = 0;
		char * lastDebugOutput = NULL;

		// name, <timer, elapsed>
		std::map< std::string, std::pair<unsigned long, long> > Debug_Timers;

		void Debug_AddToLog( const char * entry ) {
			//std::cout << "  -  " << entry << "\n";
			//Debug_Log.push_back( entry );
			Debug_LogView.push_back( entry );
		}

		char * Debug_PrintLogView() {
			std::string output = "";
			unsigned long elapsed = Core::elapsedtime( Debug_LogViewTimer );
			if ( elapsed > Debug_LogViewRefreshInterval ) {
				// Add debug timers to logview before displaying
				std::map< std::string, std::pair<unsigned long, long> >::iterator timerIter;
				for ( timerIter = Debug_Timers.begin(); timerIter != Debug_Timers.end(); timerIter++ ) {
					Core::string timerState = "";
					timerState << "Timer[" << (*timerIter).first << "]: " << (unsigned int)(*timerIter).second.second;
					Debug_AddToLog( timerState.c_str() );
				}

				// clear screen
				if ( DEBUG_PRINT_TO_CONSOLE ) std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
				for ( auto entry : Debug_LogView ) {
					output += entry;
					output += "\n";
				}
				Debug_LogView.clear();

				if ( DEBUG_PRINT_TO_CONSOLE ) std::cout << output.c_str();

				Debug_LogViewTimer = Core::timer();
			} else {
				Debug_LogView.clear();
			}

			if ( lastDebugOutput == NULL ) lastDebugOutput = new char[4096];
			memset( lastDebugOutput, '\0', 4096 );
			memcpy( lastDebugOutput, output.c_str(), output.size() < 4096 ? output.size() : 4096 );

			return lastDebugOutput;
		}

		void Debug_StartTimer( const char * timerName ) {
			Debug_Timers[timerName] = std::pair<unsigned long, long>( Core::timer(), -1 );
		}

		void Debug_StopTimer( const char * timerName ) {
			std::map< std::string, std::pair<unsigned long, long> >::iterator iter = Debug_Timers.find( timerName );
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
			std::map< std::string, std::pair<unsigned long, long> >::iterator iter = Debug_Timers.find( timerName );
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