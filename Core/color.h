
#ifndef Rocket_Core_Color_H
#define Rocket_Core_Color_H

/* Color Classes:
 * mono - monochrome (0 - 255)
 */

namespace Rocket {
	namespace Core {
		namespace Color {
		
			class mono {
			public:
				mono();
				mono(short value);

				void add(short value);

			private:
				short m_value;
			};

			class rgb {
			public:
				rgb();
				rgb(short red, short green, short blue);

				void add(short red, short green, short blue);
				void addr(short red);
				void addg(short green);
				void addb(short blue);
				
			private:
				mono m_red;
				mono m_green;
				mono m_blue;
			};

			class rgba : public rgb {
			public:
				rgba();
				rgba(short red, short green, short blue, short alpha);

				void add(short red, short green, short blue, short alpha);
				void adda(short alpha);

			private:
				mono m_alpha;
			};
		}
	}
}

#endif