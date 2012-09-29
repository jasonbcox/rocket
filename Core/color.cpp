
#include "color.h"

namespace Rocket {
	namespace Core {
		namespace Color {
			
			mono::mono() {
				m_value = 0;
			}
			mono::mono(short value) {
				m_value = value % 256;
			}

			void mono::add(short value) {
				m_value = (m_value + value) % 256;
			}


			rgb::rgb() {
				m_red = mono();
				m_green = mono();
				m_blue = mono();
			}
			rgb::rgb(short red, short green, short blue) {
				m_red = mono(red);
				m_green = mono(green);
				m_blue = mono(blue);
			}

			void rgb::add(short red, short green, short blue) {
				addr(red);
				addg(green);
				addb(blue);
			}
			void rgb::addr(short red) {
				m_red.add(red);
			}
			void rgb::addg(short green) {
				m_green.add(green);
			}
			void rgb::addb(short blue) {
				m_blue.add(blue);
			}


			rgba::rgba() : rgb() {
				m_alpha = mono();
			}
			rgba::rgba(short red, short green, short blue, short alpha) : rgb(red, green, blue) {
				m_alpha = mono(alpha);
			}

			void rgba::add(short red, short green, short blue, short alpha) {
				addr(red);
				addg(green);
				addb(blue);
				adda(alpha);
			}
			void rgba::adda(short alpha) {
				m_alpha.add(alpha);
			}

		}
	}
}