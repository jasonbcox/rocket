
#include <sstream>
#include <math.h>

#include "floap.h"
#include "mathconstants.h"

namespace Rocket {
	namespace Core {

		floap::floap( CORE_FLOAP_TYPE value ) {
			m_value = value;
		}

		floap & floap::operator = ( const floap & rhs ) {
			if (this != & rhs) {
				m_value = rhs.m_value;
			}
			return *this;
		}

		const floap floap::operator + ( const floap & other ) const {
			return m_value + other.m_value;
		}
		const floap floap::operator - ( const floap & other ) const {
			return m_value - other.m_value;
		}
		const floap floap::operator * ( const floap & other ) const {
			return m_value * other.m_value;
		}
		const floap floap::operator / ( const floap & other ) const {
			return m_value / other.m_value;
		}

		floap & floap::operator += ( const floap & rhs ) {
			m_value = m_value + rhs.m_value;
			return *this;
		}
		floap & floap::operator -= ( const floap & rhs ) {
			m_value = m_value - rhs.m_value;
			return *this;
		}
		floap & floap::operator *= ( const floap & rhs ) {
			m_value = m_value * rhs.m_value;
			return *this;
		}
		floap & floap::operator /= ( const floap & rhs ) {
			m_value = m_value / rhs.m_value;
			return *this;
		}

		bool floap::operator == ( const floap & R ) const {
			return (abs(m_value - R.m_value) <= Rocket::MathConstants::ZeroTolerance);
		}
		bool floap::operator != ( const floap & R ) const {
			return (abs(m_value - R.m_value) > Rocket::MathConstants::ZeroTolerance);
		}

		floap floap::floor() {
			return std::floor(m_value);
		}
		floap floap::ceiling() {
			return std::ceil(m_value);
		}
		floap floap::round() {
			return std::floor(m_value + 0.5f);
		}

		CORE_FLOAP_TYPE floap::toValue() {
			return m_value;
		}
		string floap::toString() {
			std::stringstream ss;
			ss << m_value;
			return ss.str();
		}


		CORE_FLOAP_TYPE floap::sqrt( const floap input ) {
			return std::sqrt(((floap)input).toValue());
		}

	}
}