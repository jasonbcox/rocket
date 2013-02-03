
#ifndef Rocket_Graphics_Clickable_H
#define Rocket_Graphics_Clickable_H

#include "Input.h"
#include "vector.h"

namespace Rocket {
	namespace Graphics {

		// Input - monitors and records all input events
		class Clickable : public Input_Mouse {
		public:
			Clickable( int left, int top, int right, int bottom );
			virtual ~Clickable();

			virtual void update( Input_ButtonState::Input_ButtonState state, int x, int y );

		protected:
			Core::vec4i m_clickableArea;
		};

	}
}


#endif