
#include "timer.h"

namespace Rocket {
	namespace Core {

#ifdef OS_WINDOWS
		unsigned long timer() { return GetTickCount(); }
		void sleep( int ms ) { Sleep(ms); }
#else
		unsigned long timer() {
			struct timeval t;
			gettimeofday( &t, NULL );
			return (unsigned long)( t.tv_sec * 1000.0 + t.tv_usec/1000.0 + 0.5 );
		}
		void sleep( int ms ) { usleep( ms * 1000 ); }
#endif

		unsigned long elapsedtime( unsigned long T ) { return (timer() - T); }

		clock::clock() {
			m_time = 0;
		}
		clock::clock(CORE_TIME_TYPE time) {
			m_time = time;
		}
		clock::clock(int days, int hours, int minutes, int seconds, int milliseconds) {
			m_time = 0;
			add_days(days);
			add_hours(hours);
			add_min(minutes);
			add_s(seconds);
			add_ms(milliseconds);
		}

		void clock::add_ms(int milliseconds) {
			m_time += milliseconds;
		}

		void clock::add_s(int seconds) {
			m_time += 1000*seconds;
		}

		void clock::add_min(int minutes) {
			m_time += 1000*60*minutes;
		}

		void clock::add_hours(int hours) {
			m_time += 1000*60*60*hours;
		}

		void clock::add_days(int days) {
			m_time += 1000*60*60*24*days;
		}

		void clock::add_month(CORE_MONTHS month) {
			switch(month) {
			case January:
			case March:
			case May:
			case July:
			case August:
			case October:
			case December:
				add_days(31);
				break;
			case February:
				add_days(28);
				break;
			case April:
			case June:
			case September:
			case November:
				add_days(30);
				break;
			}
		}

		void clock::add_months(CORE_MONTHS start_month, CORE_MONTHS end_month) {
			CORE_MONTHS this_month = start_month;
			while (this_month != end_month) {
				add_month(start_month);
				this_month = (CORE_MONTHS)(this_month + 1);
				if (this_month == December) this_month = January;
			}
		}

		void clock::add_years(int years) {
			m_time += 1000ULL*60ULL*60ULL*24ULL*365ULL*((CORE_TIME_TYPE)years);
		}

	}
}