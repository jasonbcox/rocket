
#include "Button.h"

namespace Rocket {
	namespace Graphics {

		Button::Button( Texture * texture, int width, int height, Input * input, int mouseButton ) :  Sprite( texture, width, height ), Input_Mouse() {
			if ( input != nullptr ) bind( input, mouseButton );
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

		void Button::setState( Button_ButtonState state ) {
			if ( m_buttonState != state ) m_animationChange = true;
			m_buttonState = state;
		}
		void Button::setPressed( bool isPressed ) {
			switch ( m_buttonState ) {
				case Button_ButtonState::Pressed:
					if ( isPressed == false ) setState( Button_ButtonState::NotPressed );
					break;
				case Button_ButtonState::NotPressed:
					if ( isPressed == true ) setState( Button_ButtonState::Pressed );
					break;
				case Button_ButtonState::Hover:
					if ( isPressed == true ) setState( Button_ButtonState::HoverPressed );
					break;
				case Button_ButtonState::HoverPressed:
					if ( isPressed == false ) setState( Button_ButtonState::Hover );
					break;
				case Button_ButtonState::Disabled:
					if ( isPressed == true ) setState( Button_ButtonState::DisabledPressed );
					break;
				case Button_ButtonState::DisabledPressed:
					if ( isPressed == false ) setState( Button_ButtonState::Disabled );
					break;
				case Button_ButtonState::HoverDisabled:
					if ( isPressed == true ) setState( Button_ButtonState::HoverDisabledPressed );
					break;
				case Button_ButtonState::HoverDisabledPressed:
					if ( isPressed == false ) setState( Button_ButtonState::HoverDisabled );
					break;
				default:
					if ( isPressed == true ) {
						setState( Button_ButtonState::Pressed );
					} else {
						setState( Button_ButtonState::NotPressed );
					}
			}
		}
		void Button::setHover( bool isHovering ) {
			switch ( m_buttonState ) {
				case Button_ButtonState::Pressed:
					if ( isHovering == true ) setState( Button_ButtonState::HoverPressed );
					break;
				case Button_ButtonState::NotPressed:
					if ( isHovering == true ) setState( Button_ButtonState::Hover );
					break;
				case Button_ButtonState::Hover:
					if ( isHovering == false ) setState( Button_ButtonState::NotPressed );
					break;
				case Button_ButtonState::HoverPressed:
					if ( isHovering == false ) setState( Button_ButtonState::Pressed );
					break;
				case Button_ButtonState::Disabled:
					if ( isHovering == true ) setState( Button_ButtonState::HoverDisabled );
					break;
				case Button_ButtonState::DisabledPressed:
					if ( isHovering == true ) setState( Button_ButtonState::HoverDisabledPressed );
					break;
				case Button_ButtonState::HoverDisabled:
					if ( isHovering == false ) setState( Button_ButtonState::Disabled );
					break;
				case Button_ButtonState::HoverDisabledPressed:
					if ( isHovering == false ) setState( Button_ButtonState::DisabledPressed );
					break;
				default:
					if ( isHovering == true ) {
						setState( Button_ButtonState::Hover );
					} else {
						setState( Button_ButtonState::NotPressed );
					}
			}
		}
		void Button::setDisabled( bool isDisabled ) {
		}

		Button_ButtonState Button::getState() {
			return m_buttonState;
		}
		bool Button::isPressed() {
			return ( m_buttonState == Button_ButtonState::Pressed ) ||
					( m_buttonState == Button_ButtonState::HoverPressed ) ||
					( m_buttonState == Button_ButtonState::DisabledPressed ) ||
					( m_buttonState == Button_ButtonState::HoverDisabledPressed );
		}
		bool Button::isHover() {
			return ( m_buttonState == Button_ButtonState::Hover ) ||
					( m_buttonState == Button_ButtonState::HoverPressed ) ||
					( m_buttonState == Button_ButtonState::HoverDisabled ) ||
					( m_buttonState == Button_ButtonState::HoverDisabledPressed );
		}
		bool Button::isDisabled() {
			return ( m_buttonState == Button_ButtonState::Disabled ) ||
					( m_buttonState == Button_ButtonState::DisabledPressed ) ||
					( m_buttonState == Button_ButtonState::HoverDisabled ) ||
					( m_buttonState == Button_ButtonState::HoverDisabledPressed );
		}

		void Button::update( Input_ButtonState state, int x, int y ) {
			bool inBounds = false;
			auto position = getPosition();
			if ( ( x >= position.x ) && ( y >= position.y ) && ( x <= position.x + m_size.x ) && ( y <= position.y + m_size.y ) ) {
				inBounds = true;
			}

			setHover( inBounds );
			if ( ( ( state == Input_ButtonState::Hit ) || ( state == Input_ButtonState::Pressed ) ) && ( inBounds == true ) && ( isDisabled() == false ) ) {
				setPressed( true );
			} else {
				setPressed( false );
			}
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


		ToggleButton::ToggleButton( Texture * texture, int width, int height, Input * input, int mouseButton ) : Button( texture, width, height, input, mouseButton ) {
		}
		ToggleButton::~ToggleButton() {
		}

		void ToggleButton::toggle() {
			setPressed( !isPressed() );
		}

		void ToggleButton::update( Input_ButtonState state, int x, int y ) {
			bool inBounds = false;
			auto position = getPosition();
			if ( ( x >= position.x ) && ( y >= position.y ) && ( x <= position.x + m_size.x ) && ( y <= position.y + m_size.y ) ) {
				inBounds = true;
			}

			setHover( inBounds );
			if ( ( state == Input_ButtonState::Hit ) && ( inBounds == true ) && ( isDisabled() == false ) ) {
				toggle();
			}
		}

	}
}

