
#include "Clickable.h"

namespace Rocket {
	namespace Graphics {

		Clickable::Clickable( int left, int top, int right, int bottom ) : Input_Mouse( Input_ButtonState::NotPressed ) {
			m_clickableArea = Core::vec4i( left, top, right, bottom );
		}
		Clickable::~Clickable() {
		}

		//! Updates this Clickable's state when an event occurs
		void Clickable::update( Input_ButtonState state, int x, int y ) {
			if ( ( x >= m_clickableArea.x() ) && ( y >= m_clickableArea.y() ) && ( x <= m_clickableArea.z() ) && ( y <= m_clickableArea.z() ) ) {
				m_state = state;
			} else {
				m_state = Input_ButtonState::Released;
			}
		}

		
	}
}
