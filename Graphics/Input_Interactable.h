
#ifndef Rocket_Graphics_Input_Interactable_H
#define Rocket_Graphics_Input_Interactable_H

namespace Rocket {
	namespace Graphics {

		// Hit - the button was pressed recently (the press event has not been handled yet)
		// Pressed - the button is currently being pressed
		// Released - the button was released recently (the release event has not been handled yet)
		// NotPressed - the button is currently not being pressed
		namespace Input_ButtonState {
		enum Input_ButtonState {
			Hit = 1,
			Pressed,
			Released,
			NotPressed
		};
		}

		class Input_Interactable {
		public:
			Input_Interactable( Input_ButtonState::Input_ButtonState defaultState = Input_ButtonState::NotPressed );

			// Returns true if the key is Hit or Pressed
			bool isHit();

		protected:
			Input_ButtonState::Input_ButtonState m_state;
		};


		class Input_Button : public Input_Interactable {
		public:
			Input_Button( Input_ButtonState::Input_ButtonState defaultState = Input_ButtonState::NotPressed );

			// Returns the current state
			// If the state is Hit, it transitions to Pressed upon calling getState()
			// If the state is Released, it transitions to NotPressed upon calling getState()
			Input_ButtonState::Input_ButtonState getState();

		protected:
			Input_ButtonState::Input_ButtonState m_state;
		};

		class Input_Keyboard : public Input_Button {
		public:
			Input_Keyboard( Input_ButtonState::Input_ButtonState defaultState = Input_ButtonState::NotPressed );

			virtual void update( Input_ButtonState::Input_ButtonState state ) = 0;
		};

		class Input_Mouse : public Input_Button {
		public:
			Input_Mouse( Input_ButtonState::Input_ButtonState defaultState = Input_ButtonState::NotPressed );

			virtual void update( Input_ButtonState::Input_ButtonState state, int x, int y ) = 0;
		};

	}
}


#endif