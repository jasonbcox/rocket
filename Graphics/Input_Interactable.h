
#ifndef Rocket_Graphics_Input_Interactable_H
#define Rocket_Graphics_Input_Interactable_H

namespace Rocket {
	namespace Graphics {

		//! Simple button state types
		enum class Input_ButtonState : int {
			Hit = 1,	//!< The button was pressed recently (the press event has not been handled yet)
			Pressed,	//!< The button is currently being pressed
			Released,	//!< The button was released recently (the release event has not been handled yet)
			NotPressed	//!< The button is currently not being pressed
		};

		class Input_Interactable {
		public:
			Input_Interactable( Input_ButtonState defaultState = Input_ButtonState::NotPressed );

			bool isHit();

		protected:
			Input_ButtonState m_state;
		};


		class Input_Button : public Input_Interactable {
		public:
			Input_Button( Input_ButtonState defaultState = Input_ButtonState::NotPressed );

			Input_ButtonState getState();

		protected:
			Input_ButtonState m_state;
		};

		class Input_Keyboard : public Input_Button {
		public:
			Input_Keyboard( Input_ButtonState defaultState = Input_ButtonState::NotPressed );

			virtual void update( Input_ButtonState state ) = 0;
		};

		class Input_Mouse : public Input_Button {
		public:
			Input_Mouse( Input_ButtonState defaultState = Input_ButtonState::NotPressed );

			virtual void update( Input_ButtonState state, int x, int y ) = 0;
		};

	}
}


#endif
