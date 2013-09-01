
#include "Button.h"

namespace Rocket {
	namespace Graphics {

		Button::Button( Texture * texture, int width, int height ) :  Sprite( texture, width, height ) {
			m_inputButton = Input_Button();
			m_buttonState = NotPressed;
		}
		Button::~Button() { }

		Button_ButtonState getState() {
			return m_state;
		}

		void update( bool recursive, float elapsedMilliseconds ) {
			// play correct Sprite animations
			update( recursive, elapsedMilliseconds );
		}

	}
}
