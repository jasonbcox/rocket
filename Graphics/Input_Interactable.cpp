
#include "Input_Interactable.h"

namespace Rocket {
	namespace Graphics {

		Input_Interactable::Input_Interactable( Input_ButtonState defaultState ) {
			m_state = defaultState;
		}

		//! Returns true if the key is Hit or Pressed
		bool Input_Interactable::isHit() {
			if ( ( m_state == Input_ButtonState::Hit ) || ( m_state == Input_ButtonState::Pressed ) ) {
				return true;
			} else {
				return false;
			}
		}

		Input_Button::Input_Button( Input_ButtonState defaultState ) : Input_Interactable( defaultState ) {
		}

		/*! Returns the state
			If the state is Hit, change it to Pressed and return Hit (this way, the hit was known to have been processed)
			If the state is Released, change it to NotPressed and return Released
			Otherwise, just return the current state
		*/
		Input_ButtonState Input_Button::getState() {
			if ( m_state == Input_ButtonState::Hit ) {
				m_state = Input_ButtonState::Pressed;
				return Input_ButtonState::Hit;
			} else if ( m_state == Input_ButtonState::Released ) {
				m_state = Input_ButtonState::NotPressed;
				return Input_ButtonState::Released;
			}
			return m_state;
		}

		Input_Keyboard::Input_Keyboard( Input_ButtonState defaultState ) : Input_Button( defaultState ) {
		}

		Input_Mouse::Input_Mouse( Input_ButtonState defaultState ) : Input_Button( defaultState ) {
		}

	}
}
