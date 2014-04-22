
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

		class Input {
		public:
			Input( GLFWwindow * window );

			void setAsActiveInput();

			void showMouse();
			void hideMouse();

			void lockMouse( int x, int y );
			void unlockMouse();

			Input_ButtonState getKey( int key );
			bool getKeySimple( int key );

			void addKeyboardBinding( int key, Input_Keyboard * binding );
			void removeKeyboardBinding( int key, Input_Keyboard * binding );

			Core::vec2i getMousePosition();
			Core::vec2i getMouseMove();
			Input_ButtonState getMouseButton( int button );
			bool getMouseButtonSimple( int button );
			int getMouseScroll();

			void addMouseBinding( int button, Input_Mouse * binding );
			void removeMouseBinding( int button, Input_Mouse * binding );

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

