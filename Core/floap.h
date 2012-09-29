
#ifndef Rocket_Core_Floap_H
#define Rocket_Core_Floap_H

#include "string_.h"

typedef float CORE_FLOAP_TYPE;
static int CORE_FLOAP_SIZE = 4;

namespace Rocket {
	namespace Core {

		class floap {
		public:
			floap( CORE_FLOAP_TYPE value = 0.0f );

			floap & operator = ( const floap & rhs );

			const floap operator + ( const floap & other ) const;
			const floap operator - ( const floap & other ) const;
			const floap operator * ( const floap & other ) const;
			const floap operator / ( const floap & other ) const;

			floap & operator += ( const floap & rhs);
			floap & operator -= ( const floap & rhs);
			floap & operator *= ( const floap & rhs);
			floap & operator /= ( const floap & rhs);

			bool operator == ( const floap & R ) const;
			bool operator != ( const floap & R ) const;

			floap floor();
			floap ceiling();
			floap round();
			//roundToNearest();

			CORE_FLOAP_TYPE toValue();
			string toString();

			static CORE_FLOAP_TYPE sqrt( const floap input );

		private:
			CORE_FLOAP_TYPE m_value;
		};

	}
}

#endif