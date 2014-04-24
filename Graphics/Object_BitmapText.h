
#ifndef Rocket_Graphics_Object_BitmapText_H
#define Rocket_Graphics_Object_BitmapText_H

#include <string>
#include <map>
#include <memory>

using namespace std;

#include "Scene.h"
#include "Sprite.h"
#include "Texture.h"
#include "Object.h"
#include "Raster.h"
#include "Mesh.h"
#include "rocket/Core/rstring.h"

namespace Rocket {
	namespace Graphics {

		struct BitmapFontGlyph {
			int fontSize;					// font size in pixels (maximum full width/height of a glyph)
			int width, height;				// width and height of the glyph itself
			Core::vec2i origin;				// origin of the base line
			Core::vec2i bearing;			// bearing.x = origin to left side; bearying.y = baseline to top
			int advance;					// full width of the glyph

			BitmapFontGlyph() {}
			BitmapFontGlyph( int size, int w, int h, Core::vec2i o, Core::vec2i b, int a ) : fontSize(size), width(w), height(h), origin(o), bearing(b), advance(a) {}
		};

		//! A single texture applied to multiple quads (Sprites) where each quad is a glyph/character
		class Object_BitmapText : public Object, public Raster {
		public:
			Object_BitmapText( Texture * bitmap );
			virtual ~Object_BitmapText();

			void setText( const string & text );
			const string & getText();

			void enableTransparency( float alphaTest, float alphaTransparency );
			void disableTransparency();

		protected:
			shared_ptr< Texture > m_bitmap;
			vector< shared_ptr< Sprite > > m_quads;
			string m_text;

			bool m_transparency;
			float m_alphaTest;
			float m_alphaTransparency;

			std::map< char, BitmapFontGlyph > m_fontSet;
		};

	}
}


#endif
