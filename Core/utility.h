
#ifndef Rocket_Core_Utility_H
#define Rocket_Core_Utility_H

#include <stdlib.h>

namespace Rocket {
	namespace Core {

		template< typename T >
		struct array_deleter {
			void operator () ( T const * p ) {
				delete [] p;
			}
		};

	}
}

#endif
