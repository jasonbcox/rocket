
#ifndef Rocket_Graphics_Texture_H
#define Rocket_Graphics_Texture_H

#include "vector.h"

namespace Rocket {
	namespace Graphics {

		class Texture {
		public:
			Texture( const char * file, bool mipmap, bool invertY );
			~Texture();

			unsigned int getID();

			Core::vec2i getSize();

		protected:
			unsigned int m_textureID;

			Core::vec2i m_size;
		};

	}
}


#endif