
#ifndef Rocket_Graphics_Input_H
#define Rocket_Graphics_Input_H

#include <unordered_map>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "rocket/Core/vector.h"
#include "Input_Interactable.h"

namespace Rocket {
	namespace Graphics {

		// Input - monitors and records all input events
		class Input {
		public:
			Input( GLFWwindow * window );

			// Set this Input object as the active input object.
			// All input callbacks will update this object ONLY.
			void setAsActiveInput();

			void showMouse();
			void hideMouse();

			void lockMouse( int x, int y );
			void unlockMouse();

			// Returns the state of the given key
			// If the state is Hit, it transitions to Pressed upon calling getKey()
			// If the state is Released, it transitions to NotPressed upon calling getKey()
			Input_ButtonState getKey( int key );
			// Returns true if the key is Hit or Pressed
			// Hit and Released do not transistion upon calling getKeySimple()
			bool getKeySimple( int key );

			// Add an input binding (button, device, etc.) to react to keyboard events
			void addKeyboardBinding( int key, Input_Keyboard * binding );

			// Returns the screen coord of the mouse
			Core::vec2i getMousePosition();
			// Returns the total movement of the mouse relative to its last position since getMouseMove() was last called
			Core::vec2i getMouseMove();
			// Returns the state of the given mouse button
			// If the state is Hit, it transitions to Pressed upon calling getMouseButton()
			// If the state is Released, it transitions to NotPressed upon calling getMouseButton()
			Input_ButtonState getMouseButton( int button );
			// Returns true if the mouse button is Hit or Pressed
			// Hit and Released do not transistion upon calling getMouseButtonSimple()
			bool getMouseButtonSimple( int button );
			// Returns the position of the mouse scroll wheel (relative to 0, the position it started at when the window context was created)
			int getMouseScroll();

			// Add an input binding (button, device, etc.) to react to mouse events
			void addMouseBinding( int button, Input_Mouse * binding );

			static void callback_keyboard( GLFWwindow * window, int key, int scancode, int state, int modifierKeys );

			static void callback_mouseMove( GLFWwindow * window, double x, double y );
			static void callback_mouseButton( GLFWwindow * window, int button, int state, int modifierKeys );
			static void callback_mouseScroll( GLFWwindow * window, double xoffset, double yoffset );

		private:
			static Input * Global_Input;
			GLFWwindow * m_window;
			
			std::unordered_map< int, Input_ButtonState > m_keyboard;
			std::unordered_map< int, std::vector< Input_Keyboard* > > m_keyboardBindings;

			std::unordered_map< int, Input_ButtonState > m_mouse;
			std::unordered_map< int, std::vector< Input_Mouse* > > m_mouseBindings;

			bool m_mouseEnabled;
			Core::vec2i m_mousePosition;
			Core::vec2i m_mouseMove;
			bool m_mouseLocked;
			Core::vec2i m_mouseLockTo;
			int m_mouseScroll;
		};

	}
}


#endif