
#ifndef Rocket_Graphics_Input_Interactable_H
#define Rocket_Graphics_Input_Interactable_H

#include "Input_Interactable.h"
#include "Sprite.h"

namespace Rocket {
	namespace Graphics {

		enum class Button_ButtonState : int {
			Pressed = 1,
			NotPressed,
			Hover,
			HoverPressed,
			Disabled,
			HoverDisabled
		};
		class Button : Sprite {
		public:
			Button( Texture * texture, int width, int height );
			~Button();

			virtual Button_ButtonState getState();
			virtual void update( bool recursive, float elapsedMilliseconds );

		protected:
			Input_Button m_inputButton;
			Button_ButtonState m_state;
		};

	}
}


#endif
