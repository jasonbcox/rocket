
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Input.h"

namespace Rocket {
	namespace Graphics {

		// Definition for static member
		Input * Input::Global_Input = nullptr;

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

		//! Set this Input object as the active input object.  All input callbacks will update this Input object ONLY.
		void Input::setAsActiveInput() {
			Input::Global_Input = this;
		}

		//! Shows the mouse cursor and unlocks its positioning
		void Input::showMouse() {
			m_mouseEnabled = true;
			glfwSetInputMode( m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
		}
		//! Hides the mouse cursor and locks it to the middle of the screen
		void Input::hideMouse() {
			m_mouseEnabled = false;
			glfwSetInputMode( m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
		}

		//! Lock the mouse to the given screen coordinates
		void Input::lockMouse( int x, int y ) {
			m_mouseLocked = true;
			m_mouseLockTo = Core::vec2i( x, y );
			m_mousePosition = m_mouseLockTo;
		}
		//! Unlock the mouse
		void Input::unlockMouse() {
			m_mouseLocked = false;
		}

		/*! Returns the state of the given key.
			If the state is Hit, the state transitions to Pressed before returning (but still returns Hit, so any subsequent calls to getKey() will return Pressed).
			If the state is Released, the state transitions to NotPressed before returning Released.
			Otherwise, the current state is returned normally.
		*/
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

		//! Returns true if the key is Hit or Pressed
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

		//! Add an input binding to be fired when an event fires in relation to the specified key
		void Input::addKeyboardBinding( int key, Input_Keyboard * binding ) {
			m_keyboardBindings[ key ].push_back( binding );
		}
		//! Remove an input binding from a key
		void Input::removeKeyboardBinding( int key, Input_Keyboard * binding ) {
			auto searchList = m_keyboardBindings.find( key );
			if ( searchList != m_keyboardBindings.end() ) {
				for ( auto iter = searchList->second.begin(); iter != searchList->second.end(); iter++ ) {
					if ( (*iter) == binding ) {
						searchList->second.erase( iter );
						break;
					}
				}
			}
		}

		//! Returns the screen coordinates of the mouse cursor
		Core::vec2i Input::getMousePosition() {
			return m_mousePosition;
		}
		//! Returns the total movement of the mouse relative to its last position since getMouseMove() was last called
		Core::vec2i Input::getMouseMove() {
			Core::vec2i r = m_mouseMove;
			m_mouseMove = Core::vec2i( 0, 0 );
			return r;
		}

		/*! Returns the state of the given mouse button.
			If the state is Hit, the state transitions to Pressed before returning (but still returns Hit, so any subsequent calls to getKey() will return Pressed).
			If the state is Released, the state transitions to NotPressed before returning Released.
			Otherwise, the current state is returned normally.
		*/
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
		//! Returns true if the mouse button is Hit or Pressed
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

		//! Returns the position of the mouse scroll wheel (relative to 0, the position it started at when the window context was created)
		int Input::getMouseScroll() {
			return m_mouseScroll;
		}

		//! Add an input binding to be fired when an event fires in relation to the specified mouse button
		void Input::addMouseBinding( int button, Input_Mouse * binding ) {
			m_mouseBindings[ button ].push_back( binding );
		}
		//! Remove an input binding from a mouse button
		void Input::removeMouseBinding( int button, Input_Mouse * binding ) {
			auto searchList = m_mouseBindings.find( button );
			if ( searchList != m_mouseBindings.end() ) {
				for ( auto iter = searchList->second.begin(); iter != searchList->second.end(); iter++ ) {
					if ( (*iter) == binding ) {
						searchList->second.erase( iter );
						break;
					}
				}
			}
		}


		//! The callback for keyboard events from GLFW
		void Input::callback_keyboard( GLFWwindow * window, int key, int scancode, int state, int modifierKeys ) {
			Input_ButtonState newstate = Input_ButtonState::Released;
			if ( state == GLFW_PRESS ) newstate = Input_ButtonState::Hit;
			if ( state == GLFW_REPEAT ) newstate = Input_ButtonState::Pressed;
			Global_Input->m_keyboard[ key ] = newstate;

			// Update keyboard bindings
			std::vector< Input_Keyboard* > bindingsToKey = Global_Input->m_keyboardBindings[ key ];
			for ( auto binding : bindingsToKey ) {
				binding->update( newstate );
			}
		}

		//! The callback for mouse move events from GLFW
		void Input::callback_mouseMove( GLFWwindow * window, double x, double y ) {
			Core::vec2i newPos = Core::vec2i( (int)x, (int)y );
			Global_Input->m_mouseMove += Global_Input->m_mousePosition - newPos;
			if ( Global_Input->m_mouseLocked == true ) {
				Global_Input->m_mousePosition = Global_Input->m_mouseLockTo;
				//if ( Global_Input->m_mouseEnabled == true ) {
				glfwSetCursorPos( window, Global_Input->m_mouseLockTo.x(), Global_Input->m_mouseLockTo.y() );
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

		//! The callback for mouse button events from GLFW
		void Input::callback_mouseButton( GLFWwindow * window, int button, int state, int modifierKeys ) {
			Input_ButtonState newstate = Input_ButtonState::Released;
			if (state == GLFW_PRESS) newstate = Input_ButtonState::Hit;
			Global_Input->m_mouse[ button ] = newstate;

			// Update mouse bindings on the button that was pressed
			Core::vec2i mousePos = Global_Input->getMousePosition();
			std::vector< Input_Mouse* > bindingsToButton = Global_Input->m_mouseBindings[ button ];
			for ( auto binding : bindingsToButton ) {
				binding->update( newstate, mousePos.x(), mousePos.y() );
			}
		}

		//! The callback for mouse scroll events from GLFW
		void Input::callback_mouseScroll( GLFWwindow * window, double xoffset, double yoffset ) {
			Global_Input->m_mouseScroll += (int)yoffset;
		}

	}
}

