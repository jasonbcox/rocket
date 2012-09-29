
#include <unordered_map>

#include "glew.h"
#include "glfw.h"

#include "Input.h"

namespace Rocket {
	namespace Graphics {

		Input_Interactable_Button::Input_Interactable_Button( Input_ButtonState::Input_ButtonState defaultState ) {
			m_state = defaultState;
		}

		// Return the state
		// If the state is Hit, change it to Pressed and return Hit (this way, the hit was known to have been processed)
		// If the state is Released, change it to NotPressed and return Released
		// Otherwise, just return the state
		Input_ButtonState::Input_ButtonState Input_Interactable_Button::getState() {
			if ( m_state == Input_ButtonState::Hit ) {
				m_state = Input_ButtonState::Pressed;
				return Input_ButtonState::Hit;
			} else if ( m_state == Input_ButtonState::Released ) {
				m_state = Input_ButtonState::NotPressed;
				return Input_ButtonState::Released;
			}
			return m_state;
		}
		// Return true if the key is Hit or Pressed
		bool Input_Interactable_Button::getStateSimple() {
			if ( ( m_state == Input_ButtonState::Hit ) || ( m_state == Input_ButtonState::Pressed ) ) {
				return true;
			} else {
				return false;
			}
		}

		Input_Interactable_Keyboard::Input_Interactable_Keyboard( Input_ButtonState::Input_ButtonState defaultState ) : Input_Interactable_Button( defaultState ) {
		}

		Input_Interactable_Mouse::Input_Interactable_Mouse( Input_ButtonState::Input_ButtonState defaultState ) : Input_Interactable_Button( defaultState ) {
		}

		// Definition for static member
		Input * Input::Global_Input = NULL;

		Input::Input() {
			m_mouseEnabled = true;
			m_mousePosition = Core::vec2i( 0, 0 );
			m_mouseMove = Core::vec2i( 0, 0 );
			m_mouseLocked = false;
			m_mouseLockTo = Core::vec2i( 0, 0 );
			m_mouseScroll = 0;

			// must be set before callbacks are declared or callbacks may occur between
			// it being set and the callback declaration
			setAsActiveInput();

			glfwSetKeyCallback( callback_keyboard );

			glfwSetMousePosCallback( callback_mouseMove );
			glfwSetMouseButtonCallback( callback_mouseButton );
			glfwSetMouseWheelCallback( callback_mouseScroll );
		}

		void Input::setAsActiveInput() {
			Input::Global_Input = this;
		}

		void Input::showMouse() {
			// Shows the mouse cursor, completely unlocked
			m_mouseEnabled = true;
			glfwEnable( GLFW_MOUSE_CURSOR );
		}
		void Input::hideMouse() {
			// Hides the mouse cursor, as well as locking it to the middle of the screen
			m_mouseEnabled = false;
			glfwDisable( GLFW_MOUSE_CURSOR );
		}

		void Input::lockMouse( int x, int y ) {
			m_mouseLocked = true;
			m_mouseLockTo = Core::vec2i( x, y );
			m_mousePosition = m_mouseLockTo;
		}
		void Input::unlockMouse() {
			m_mouseLocked = false;
		}

		// Return the state of a key
		// If the state is Hit, change it to Pressed and return Hit (this way, the hit was known to have been processed)
		// If the state is Released, change it to NotPressed and return Released
		// Otherwise, just return the state
		Input_ButtonState::Input_ButtonState Input::getKey( int key ) {
			std::unordered_map< int, Input_ButtonState::Input_ButtonState >::iterator iter = m_keyboard.find( key );
			if (iter == m_keyboard.end()) {
				return Input_ButtonState::NotPressed;
			} else {
				if ((*iter).second == Input_ButtonState::Hit) {
					m_keyboard[ key ] = Input_ButtonState::Pressed;
					return Input_ButtonState::Hit;
				} else if ((*iter).second == Input_ButtonState::Released) {
					m_keyboard[ key ] = Input_ButtonState::NotPressed;
					return Input_ButtonState::Released;
				}
				return (*iter).second;
			}
		}
		// Return true if the key is Hit or Pressed
		bool Input::getKeySimple( int key ) {
			std::unordered_map< int, Input_ButtonState::Input_ButtonState >::iterator iter = m_keyboard.find( key );
			if (iter == m_keyboard.end()) {
				return false;
			} else {
				if ( ((*iter).second == Input_ButtonState::Hit) || ((*iter).second == Input_ButtonState::Pressed) ) {
					return true;
				} else {
					return false;
				}
			}
		}

		void Input::addKeyboardBinding( int key, Input_Interactable_Keyboard * binding ) {
			m_keyboardBindings[ key ].push_back( binding );
		}


		Core::vec2i Input::getMousePosition() {
			return m_mousePosition;
		}
		Core::vec2i Input::getMouseMove() {
			Core::vec2i r = m_mouseMove;
			m_mouseMove = Core::vec2i( 0, 0 );
			return r;
		}

		// Return the state of a mouse button
		// If the state is Hit, change it to Pressed and return Hit (this way, the hit was known to have been processed)
		// If the state is Released, change it to NotPressed and return Released
		// Otherwise, just return the state
		Input_ButtonState::Input_ButtonState Input::getMouseButton( int button ) {
			std::unordered_map< int, Input_ButtonState::Input_ButtonState >::iterator iter = m_mouse.find( button );
			if (iter == m_mouse.end()) {
				return Input_ButtonState::NotPressed;
			} else {
				if ((*iter).second == Input_ButtonState::Hit) {
					m_mouse[ button ] = Input_ButtonState::Pressed;
					return Input_ButtonState::Hit;
				} else if ((*iter).second == Input_ButtonState::Released) {
					m_mouse[ button ] = Input_ButtonState::NotPressed;
					return Input_ButtonState::Released;
				}
				return (*iter).second;
			}
		}
		// Return true if the mouse button is Hit or Pressed
		bool Input::getMouseButtonSimple( int button ) {
			std::unordered_map< int, Input_ButtonState::Input_ButtonState >::iterator iter = m_mouse.find( button );
			if (iter == m_mouse.end()) {
				return false;
			} else {
				if ( ((*iter).second == Input_ButtonState::Hit) || ((*iter).second == Input_ButtonState::Pressed) ) {
					return true;
				} else {
					return false;
				}
			}
		}

		int Input::getMouseScroll() {
			return m_mouseScroll;
		}

		void Input::addMouseBinding( int button, Input_Interactable_Mouse * binding ) {
			m_mouseBindings[ button ].push_back( binding );
		}


		void Input::callback_keyboard( int key, int state ) {
			Input_ButtonState::Input_ButtonState newstate = Input_ButtonState::Released;
			if (state == GLFW_PRESS) newstate = Input_ButtonState::Hit;
			Global_Input->m_keyboard[ key ] = newstate;

			// Update keyboard bindings
			std::vector< Input_Interactable_Keyboard* > bindingsToKey = Global_Input->m_keyboardBindings[ key ];
			std::vector< Input_Interactable_Keyboard* >::iterator iter;
			for ( iter = bindingsToKey.begin(); iter != bindingsToKey.end(); iter++ ) {
				(*iter)->update( newstate );
			}
		}

		void Input::callback_mouseMove( int x, int y ) {
			Core::vec2i newPos = Core::vec2i( x, y );
			Global_Input->m_mouseMove += Global_Input->m_mousePosition - newPos;
			if ( Global_Input->m_mouseLocked == true ) {
				Global_Input->m_mousePosition = Global_Input->m_mouseLockTo;
				//if ( Global_Input->m_mouseEnabled == true ) {
					glfwSetMousePos( Global_Input->m_mouseLockTo.x, Global_Input->m_mouseLockTo.y );
				//}
			} else {
				Global_Input->m_mousePosition = newPos;
			}

			// Update mouse bindings for all buttons
			std::unordered_map< int, Input_ButtonState::Input_ButtonState >::iterator iterButton;
			for ( iterButton = Global_Input->m_mouse.begin(); iterButton != Global_Input->m_mouse.end(); iterButton++ ) {
				std::vector< Input_Interactable_Mouse* > bindingsToButton = Global_Input->m_mouseBindings[ (*iterButton).first ];
				std::vector< Input_Interactable_Mouse* >::iterator iter;
				for ( iter = bindingsToButton.begin(); iter != bindingsToButton.end(); iter++ ) {
					(*iter)->update( (*iterButton).second, x, y );
				}
			}
		}
		void Input::callback_mouseButton( int button, int state ) {
			Input_ButtonState::Input_ButtonState newstate = Input_ButtonState::Released;
			if (state == GLFW_PRESS) newstate = Input_ButtonState::Hit;
			Global_Input->m_mouse[ button ] = newstate;

			// Update mouse bindings on the button that was pressed
			Core::vec2i mousePos = Global_Input->getMousePosition();
			std::vector< Input_Interactable_Mouse* > bindingsToButton = Global_Input->m_mouseBindings[ button ];
			std::vector< Input_Interactable_Mouse* >::iterator iter;
			for ( iter = bindingsToButton.begin(); iter != bindingsToButton.end(); iter++ ) {
				(*iter)->update( newstate, mousePos.x, mousePos.y );
			}
		}
		void Input::callback_mouseScroll( int position ) {
			Global_Input->m_mouseScroll = position;
		}

	}
}