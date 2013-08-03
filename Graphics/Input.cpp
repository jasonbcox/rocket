
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Input.h"

namespace Rocket {
	namespace Graphics {

		// Definition for static member
		Input * Input::Global_Input = NULL;

		Input::Input( GLFWwindow * window ) {
			m_window = window;
			m_mouseEnabled = true;
			m_mousePosition = Core::vec2i( 0, 0 );
			m_mouseMove = Core::vec2i( 0, 0 );
			m_mouseLocked = false;
			m_mouseLockTo = Core::vec2i( 0, 0 );
			m_mouseScroll = 0;

			// must be set before callbacks are declared or callbacks may occur between
			// it being set and the callback declaration
			setAsActiveInput();

			glfwSetKeyCallback( m_window, callback_keyboard );

			glfwSetCursorPosCallback( m_window, callback_mouseMove );
			glfwSetMouseButtonCallback( m_window, callback_mouseButton );
			glfwSetScrollCallback( m_window, callback_mouseScroll );
		}

		void Input::setAsActiveInput() {
			Input::Global_Input = this;
		}

		void Input::showMouse() {
			// Shows the mouse cursor, completely unlocked
			m_mouseEnabled = true;
			glfwSetInputMode( m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
		}
		void Input::hideMouse() {
			// Hides the mouse cursor, as well as locking it to the middle of the screen
			m_mouseEnabled = false;
			glfwSetInputMode( m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
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
		Input_ButtonState Input::getKey( int key ) {
			std::unordered_map< int, Input_ButtonState >::iterator iter = m_keyboard.find( key );
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
			std::unordered_map< int, Input_ButtonState >::iterator iter = m_keyboard.find( key );
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

		void Input::addKeyboardBinding( int key, Input_Keyboard * binding ) {
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
		Input_ButtonState Input::getMouseButton( int button ) {
			std::unordered_map< int, Input_ButtonState >::iterator iter = m_mouse.find( button );
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
			std::unordered_map< int, Input_ButtonState >::iterator iter = m_mouse.find( button );
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

		void Input::addMouseBinding( int button, Input_Mouse * binding ) {
			m_mouseBindings[ button ].push_back( binding );
		}


		void Input::callback_keyboard( GLFWwindow * window, int key, int scancode, int state, int modifierKeys ) {
			Input_ButtonState newstate = Input_ButtonState::Released;
			if (state == GLFW_PRESS) newstate = Input_ButtonState::Hit;
			Global_Input->m_keyboard[ key ] = newstate;

			// Update keyboard bindings
			std::vector< Input_Keyboard* > bindingsToKey = Global_Input->m_keyboardBindings[ key ];
			for ( auto binding : bindingsToKey ) {
				binding->update( newstate );
			}
		}

		void Input::callback_mouseMove( GLFWwindow * window, double x, double y ) {
			Core::vec2i newPos = Core::vec2i( (int)x, (int)y );
			Global_Input->m_mouseMove += Global_Input->m_mousePosition - newPos;
			if ( Global_Input->m_mouseLocked == true ) {
				Global_Input->m_mousePosition = Global_Input->m_mouseLockTo;
				//if ( Global_Input->m_mouseEnabled == true ) {
				glfwSetCursorPos( window, Global_Input->m_mouseLockTo.x, Global_Input->m_mouseLockTo.y );
				//}
			} else {
				Global_Input->m_mousePosition = newPos;
			}

			// Update mouse bindings for all buttons
			std::unordered_map< int, Input_ButtonState >::iterator iterButton;
			for ( iterButton = Global_Input->m_mouse.begin(); iterButton != Global_Input->m_mouse.end(); iterButton++ ) {
				std::vector< Input_Mouse* > bindingsToButton = Global_Input->m_mouseBindings[ (*iterButton).first ];
				for ( auto binding : bindingsToButton ) {
					binding->update( (*iterButton).second, x, y );
				}
			}
		}
		void Input::callback_mouseButton( GLFWwindow * window, int button, int state, int modifierKeys ) {
			Input_ButtonState newstate = Input_ButtonState::Released;
			if (state == GLFW_PRESS) newstate = Input_ButtonState::Hit;
			Global_Input->m_mouse[ button ] = newstate;

			// Update mouse bindings on the button that was pressed
			Core::vec2i mousePos = Global_Input->getMousePosition();
			std::vector< Input_Mouse* > bindingsToButton = Global_Input->m_mouseBindings[ button ];
			for ( auto binding : bindingsToButton ) {
				binding->update( newstate, mousePos.x, mousePos.y );
			}
		}
		void Input::callback_mouseScroll( GLFWwindow * window, double xoffset, double yoffset ) {
			Global_Input->m_mouseScroll += (int)yoffset;
		}

	}
}