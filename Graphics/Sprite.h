
#ifndef Rocket_Graphics_Sprite_H
#define Rocket_Graphics_Sprite_H

#include "Raster.h"
#include "Texture.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"
#include "Scene.h"
#include "vector.h"

namespace Rocket {
	namespace Graphics {

		// Sprite - A texture applied to a quad, usually to be drawn in an orthographic space
		// A sprite sizes and positions based on the assumption that 1.0 units = 1 pixel
		// A sprite positions based on the assumption that 0,0 is the top left of the orthographic space
		class Sprite : public Raster {
		public:
			Sprite( Texture * texture, int width, int height );
			~Sprite();

			static void enableSpritesInScene( Scene * scene, Shader * meshShader );
			void addToScene( Scene * scene );
			void addAsChild( Transform * parent );

			void enableTransparency( float alphaTest, float alphaTransparency );
			void disableTransparency();

			// Set UV coords in pixel-space (0 - size)
			void setUV( int left, int top, int right, int bottom );
			// Set UV coords in texel-space (0.0 - 1.0)
			void setUV( float left, float top, float right, float bottom );
			// Get UV coords in pixel-space
			Core::vec4i getUV();

		protected:
			static Mesh * Global_SpriteMesh;
			static int Global_SpriteCount;
			Texture * m_texture;
			Object * m_quad;

			Core::vec4i m_uvCoords;
		};

	}
}


#endif