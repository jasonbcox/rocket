
#include "Button.h"

namespace Rocket {
	namespace Graphics {

		Button::Button( Texture * texture, int width, int height, Input * input, int button ) :  Sprite( texture, width, height ), Input_Mouse() {
			if ( input != nullptr ) bind( input, button );
			m_buttonState = Button_ButtonState::NotPressed;
			m_animationChange = true;
		}
		Button::~Button() { }

		void Button::bind( Input * input, int button ) {
			input->addMouseBinding( button, this );
		}
		void Button::unbind( Input * input, int button ) {
			input->removeMouseBinding( button, this );
		}

		void Button::setStateAnimation( Button_ButtonState state, string animationName ) {
			m_stateAnimations[ (int)state ] = animationName;
		}
		void Button::unsetStateAnimation( Button_ButtonState state ) {
			m_stateAnimations.erase( (int)state );
		}

		Button_ButtonState Button::getState() {
			return m_buttonState;
		}

		void Button::update( Input_ButtonState state, int x, int y ) {
			bool inBounds = false;
			auto position = getPosition();
			if ( ( x >= position.x ) && ( y >= position.y ) && ( x <= position.x + m_size.x ) && ( y <= position.y + m_size.y ) ) {
				inBounds = true;
			}

			auto oldState = m_buttonState;
			if ( ( state == Input_ButtonState::Hit ) || ( state == Input_ButtonState::Pressed ) ) {
				if ( inBounds == true ) {
					m_buttonState = Button_ButtonState::Pressed;
				} else {
					m_buttonState = Button_ButtonState::NotPressed;
				}
			} else {
				if ( inBounds == true ) {
					m_buttonState = Button_ButtonState::Hover;
				} else {
					m_buttonState = Button_ButtonState::NotPressed;
				}
			}
			if ( oldState != m_buttonState ) m_animationChange = true;
		}

		void Button::update( bool recursive, float elapsedMilliseconds ) {
			if ( m_animationChange == true ) {
				auto animationToPlay = m_stateAnimations.find( (int)m_buttonState );
				if ( animationToPlay != m_stateAnimations.end() ) {
					playAnimation( animationToPlay->second, 1000.0f, false );
					m_animationChange = false;
				}
			}
			Sprite::update( recursive, elapsedMilliseconds );
		}

	}
}

