
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SOIL2/SOIL2.h"

#include "Texture.h"

namespace Rocket {
	namespace Graphics {

		Texture::Texture( const char * file, bool mipmap, bool invertY ) {
			unsigned int flags = 0;
			if (mipmap == true) flags = flags | SOIL_FLAG_MIPMAPS;
			if (invertY == true) flags = flags | SOIL_FLAG_INVERT_Y;

			unsigned int width = 0, height = 0;
			m_textureID = SOIL_load_OGL_texture( file, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags, &width, &height );
			m_size = Core::vec2i( width, height );
		}

		Texture::~Texture() {
			glDeleteTextures( 1, &m_textureID );
		}

		//! Returns the texture ID of this Texture
		unsigned int Texture::getID() {
			return m_textureID;
		}

		//! Returns the size of this Texture in pixels
		Core::vec2i Texture::getSize() {
			return m_size;
		}

	}
}

