
#ifndef Rocket_Graphics_Button_H
#define Rocket_Graphics_Button_H

#include <string>
#include <unordered_map>

#include "Input.h"
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
			DisabledPressed,
			HoverDisabled,
			HoverDisabledPressed
		};

		class Button : public Sprite, Input_Mouse {
		public:
			Button( Texture * texture, int width, int height, Input * input = nullptr, int button = 0 );
			~Button();

			void bind( Input * input, int button );
			void unbind( Input * input, int button );

			void setStateAnimation( Button_ButtonState state, string animationName );
			void unsetStateAnimation( Button_ButtonState state );

			virtual void setState( Button_ButtonState state );
			virtual void setPressed( bool isPressed );
			virtual void setHover( bool isHovering );
			virtual void setDisabled( bool isDisabled );

			virtual Button_ButtonState getState();
			virtual bool isPressed();
			virtual bool isHover();
			virtual bool isDisabled();

			virtual void update( Input_ButtonState state, int x, int y );
			virtual void update( bool recursive, float elapsedMilliseconds );

		protected:
			Button_ButtonState m_buttonState;
			bool m_animationChange;

			unordered_map< int, string > m_stateAnimations;

		};

		class ToggleButton : public Button {
		public:
			ToggleButton( Texture * texture, int width, int height, Input * input = nullptr, int button = 0 );
			~ToggleButton();

			virtual void toggle();

			virtual void update( Input_ButtonState state, int x, int y );

		};

	}
}


#endif
