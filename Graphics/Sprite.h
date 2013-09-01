
#ifndef Rocket_Graphics_Sprite_H
#define Rocket_Graphics_Sprite_H

#include <vector>
#include <unordered_map>

using namespace std;

#include "Raster.h"
#include "Texture.h"
#include "Object.h"
#include "Mesh.h"
#include "Shader.h"
#include "Scene.h"
#include "rocket/Core/vector.h"

using namespace Rocket::Core;

namespace Rocket {
	namespace Graphics {

		// Sprite - A texture applied to a quad, usually to be drawn in an orthographic space
		// A sprite sizes and positions based on the assumption that 1.0 units = 1 pixel
		// A sprite positions based on the assumption that 0,0 is the top left of the orthographic space
		class Sprite : public Object, public Raster {
		public:
			Sprite( Texture * texture, int width, int height );
			virtual ~Sprite();

			static void enableSpritesInScene( Universe * world, Scene * scene, Shader * meshShader );

			void enableTransparency( float alphaTest, float alphaTransparency );
			void disableTransparency();

			// Set UV coords in pixel-space (0 - size)
			void setUV( int left, int top, int right, int bottom );
			// Set UV coords in texel-space (0.0 - 1.0)
			void setUV( float left, float top, float right, float bottom );
			// Get UV coords in pixel-space
			Core::vec4i getUV();

			// setFrames() sets the sections of the sprite sheet to animate through when an animation is activated
			void setFrames( string animationName, vector< vec2i > frames );
			void playAnimation( string animationName, float timeMilliseconds, bool repeat, int startFrame );
			void pauseAnimation();
			int currentFrame();

			virtual void update( bool recursive, float elapsedMilliseconds );

		protected:
			static shared_ptr< Mesh > g_SpriteMesh;
			static int g_SpriteCount;
			
			shared_ptr< Texture > m_texture;
			unordered_map< string, vector< vec2i > > m_frames;

			bool m_animationPlaying;
			vector< vec2i > m_animationFrames;
			int m_animationTotalFrames;
			int m_animationCurrentFrame;
			float m_animationTotalTime;
			float m_animationTime;
			bool m_animationRepeat;

			Core::vec4i m_uvCoords;
		};

	}
}


#endif
