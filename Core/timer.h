
#ifndef Rocket_Core_Timer_H
#define Rocket_Core_Timer_H

#include "system.h"

#ifdef OS_WINDOWS
#include <Windows.h>

#else
#include <sys/time.h>
#endif

typedef unsigned long long CORE_TIME_TYPE;

namespace Rocket {
	namespace Core {

		unsigned long timer();							// timer in ms resolution
		unsigned long elapsedtime( unsigned long T );	// elapsed time in ms
		void sleep( int ms );

		enum CORE_MONTHS : unsigned int {
			January = 1,
			February,
			March,
			April,
			May,
			June,
			July,
			August,
			September,
			October,
			November,
			December
		};

		class clock {
		public:
			clock();
			clock(CORE_TIME_TYPE time);
			clock(int days, int hours, int minutes, int seconds, int milliseconds);

			void add_ms(int milliseconds);
			void add_s(int seconds);
			void add_min(int minutes);
			void add_hours(int hours);
			void add_days(int days);
			void add_month(CORE_MONTHS month);
			void add_months(CORE_MONTHS start_month, CORE_MONTHS end_month);
			void add_years(int years);

		private:
			CORE_TIME_TYPE m_time;
		};

	}
}

#endif