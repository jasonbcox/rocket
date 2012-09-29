
#include "Object_BitmapText.h"
#include "Universe.h"

namespace Rocket {
	namespace Graphics {
		
		Object_BitmapText::Object_BitmapText( Texture * bitmap, std::string text ) : Transform(), Raster( this ) {
			m_bitmap = bitmap;

			m_transparency = true;
			m_alphaTest = 0.0f;
			m_alphaTransparency = 1.0f;
			
			// Load the font set
			int fontSize = 12;
			m_fontSet['?'] = BitmapFontGlyph( fontSize, 5, 9, Core::vec2i(3,12), Core::vec2i(2,9), 9 );		// use this in the event that a non-set character is used
			m_fontSet['a'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(17,12), Core::vec2i(0,6), 7 );
			m_fontSet['b'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(31,12), Core::vec2i(0,9), 7 );
			m_fontSet['c'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(45,12), Core::vec2i(0,6), 7 );
			m_fontSet['d'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(59,12), Core::vec2i(0,9), 7 );
			m_fontSet['e'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(73,12), Core::vec2i(0,6), 7 );
			m_fontSet['f'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(87,12), Core::vec2i(0,9), 7 );
			m_fontSet['g'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(101,12), Core::vec2i(0,6), 7 );
			m_fontSet['h'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(115,12), Core::vec2i(0,9), 7 );
			m_fontSet['i'] = BitmapFontGlyph( fontSize, 6, 9, Core::vec2i(130,12), Core::vec2i(0,9), 6 );
			m_fontSet['j'] = BitmapFontGlyph( fontSize, 6, 12, Core::vec2i(143,12), Core::vec2i(0,9), 6 );
			m_fontSet['k'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(157,12), Core::vec2i(0,9), 7 );
			m_fontSet['l'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(171,12), Core::vec2i(0,9), 7 );
			m_fontSet['m'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(185,12), Core::vec2i(0,6), 7 );
			m_fontSet['n'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(199,12), Core::vec2i(0,6), 7 );
			m_fontSet['o'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(213,12), Core::vec2i(0,6), 7 );
			m_fontSet['p'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(227,12), Core::vec2i(0,6), 7 );
			m_fontSet['q'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(241,12), Core::vec2i(0,6), 7 );
			m_fontSet['r'] = BitmapFontGlyph( fontSize, 6, 6, Core::vec2i(256,12), Core::vec2i(0,6), 6 );
			m_fontSet['s'] = BitmapFontGlyph( fontSize, 6, 6, Core::vec2i(270,12), Core::vec2i(0,6), 6 );
			m_fontSet['t'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(283,12), Core::vec2i(0,8), 7 );
			m_fontSet['u'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(297,12), Core::vec2i(0,6), 7 );
			m_fontSet['v'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(311,12), Core::vec2i(0,6), 7 );
			m_fontSet['w'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(325,12), Core::vec2i(0,6), 7 );
			m_fontSet['x'] = BitmapFontGlyph( fontSize, 7, 6, Core::vec2i(339,12), Core::vec2i(0,6), 7 );
			m_fontSet['y'] = BitmapFontGlyph( fontSize, 7, 9, Core::vec2i(353,12), Core::vec2i(0,6), 7 );
			m_fontSet['z'] = BitmapFontGlyph( fontSize, 6, 6, Core::vec2i(368,12), Core::vec2i(0,6), 6 );
			
			m_fontSet['A'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(17,28), Core::vec2i(0,8), 7 );
			m_fontSet['B'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(31,28), Core::vec2i(0,8), 7 );
			m_fontSet['C'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(45,28), Core::vec2i(0,8), 7 );
			m_fontSet['D'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(59,28), Core::vec2i(0,8), 7 );
			m_fontSet['E'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(74,28), Core::vec2i(0,8), 6 );
			m_fontSet['F'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(88,28), Core::vec2i(0,8), 6 );
			m_fontSet['G'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(101,28), Core::vec2i(0,8), 7 );
			m_fontSet['H'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(115,28), Core::vec2i(0,8), 7 );
			m_fontSet['I'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(130,28), Core::vec2i(0,8), 7 );
			m_fontSet['J'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(144,28), Core::vec2i(0,8), 6 );
			m_fontSet['K'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(157,28), Core::vec2i(0,8), 7 );
			m_fontSet['L'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(171,28), Core::vec2i(0,8), 7 );
			m_fontSet['M'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(185,28), Core::vec2i(0,8), 7 );
			m_fontSet['N'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(199,28), Core::vec2i(0,8), 7 );
			m_fontSet['O'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(213,28), Core::vec2i(0,8), 7 );
			m_fontSet['P'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(227,28), Core::vec2i(0,8), 7 );
			m_fontSet['Q'] = BitmapFontGlyph( fontSize, 8, 10, Core::vec2i(241,28), Core::vec2i(0,8), 7 );
			m_fontSet['R'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(256,28), Core::vec2i(0,8), 7 );
			m_fontSet['S'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(270,28), Core::vec2i(0,8), 7 );
			m_fontSet['T'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(283,28), Core::vec2i(0,8), 7 );
			m_fontSet['U'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(297,28), Core::vec2i(0,8), 7 );
			m_fontSet['V'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(311,28), Core::vec2i(0,8), 7 );
			m_fontSet['W'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(325,28), Core::vec2i(0,8), 7 );
			m_fontSet['X'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(339,28), Core::vec2i(0,8), 7 );
			m_fontSet['Y'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(353,28), Core::vec2i(0,8), 7 );
			m_fontSet['Z'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(368,28), Core::vec2i(0,8), 7 );
			
			m_fontSet['0'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(4,44), Core::vec2i(0,8), 6 );
			m_fontSet['1'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(18,44), Core::vec2i(0,8), 6 );
			m_fontSet['2'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(32,44), Core::vec2i(0,8), 6 );
			m_fontSet['3'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(46,44), Core::vec2i(0,8), 6 );
			m_fontSet['4'] = BitmapFontGlyph( fontSize, 7, 8, Core::vec2i(59,44), Core::vec2i(0,8), 7 );
			m_fontSet['5'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(74,44), Core::vec2i(0,8), 6 );
			m_fontSet['6'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(88,44), Core::vec2i(0,8), 6 );
			m_fontSet['7'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(102,44), Core::vec2i(0,8), 6 );
			m_fontSet['8'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(116,44), Core::vec2i(0,8), 6 );
			m_fontSet['9'] = BitmapFontGlyph( fontSize, 6, 8, Core::vec2i(130,44), Core::vec2i(0,8), 6 );

			m_fontSet[' '] = BitmapFontGlyph( fontSize, 1, 1, Core::vec2i(0,0), Core::vec2i(0,6), 5 );
			m_fontSet['.'] = BitmapFontGlyph( fontSize, 3, 2, Core::vec2i(5,28), Core::vec2i(0,2), 5 );
			m_fontSet[','] = BitmapFontGlyph( fontSize, 4, 3, Core::vec2i(4,22), Core::vec2i(0,2), 7 );
			m_fontSet['!'] = BitmapFontGlyph( fontSize, 3, 8, Core::vec2i(11,28), Core::vec2i(0,8), 5 );
			//m_fontSet['.'] = BitmapFontGlyph( fontSize, 378, 48, Core::vec2i(0,48), Core::vec2i(0,48), 378 );

			setText( text );
		}
		Object_BitmapText::~Object_BitmapText() {
			for ( unsigned int i = 0; i < m_quads.size(); i++ ) {
				delete m_quads[i];
			}
		}

		void Object_BitmapText::setText( const std::string & text ) {
			m_text = text;

			// Cleanup old quads
			for ( unsigned int i = 0; i < m_quads.size(); i++ ) {
				m_quads[i]->hide();
			}

			// Generate new quads
			Core::vec2i nextPos = Core::vec2i();
			for ( unsigned int i = 0; i < text.length(); i++ ) {
				std::map<char,BitmapFontGlyph>::iterator iter;
				iter = m_fontSet.find(text[i]);
				BitmapFontGlyph glyph;
				if ( iter == m_fontSet.end() ) {
					glyph = m_fontSet['?'];
				} else {
					glyph = m_fontSet[text[i]];
				}

				int testScale = 1;
				if ( text[i] == '\n' ) {
					nextPos.x = 0;
					nextPos.y += glyph.fontSize*testScale;
				} else {
					Sprite * quad = NULL;
					Core::vec2i spriteSize( glyph.advance*testScale, glyph.height*testScale );
					if ( i < m_quads.size() ) {
						quad = m_quads[i];
						quad->show();
						quad->setSize( spriteSize );
					} else {
						quad = new Sprite( m_bitmap, spriteSize.x, spriteSize.y );
						quad->addAsChild( this );
						if ( m_transparency == true ) {
							quad->enableTransparency( m_alphaTest, m_alphaTransparency );
						} else {
							quad->disableTransparency();
						}
						m_quads.push_back( quad );
					}

					quad->setUV( glyph.origin.x, glyph.origin.y - glyph.bearing.y, glyph.origin.x + glyph.advance, glyph.origin.y - glyph.bearing.y + glyph.height );

					if ( nextPos.x + glyph.advance*testScale > WindowWidth ) {
						nextPos.x = 0;
						nextPos.y += glyph.fontSize*testScale;
					}
					quad->setPosition( nextPos - Core::vec2i(0,glyph.bearing.y) );
					nextPos.x += glyph.advance*testScale;
				}
			}

			setPosition( Raster::m_position );
		}

		const std::string & Object_BitmapText::getText() {
			return m_text;
		}

		void Object_BitmapText::enableTransparency( float alphaTest, float alphaTransparency ) {
			m_transparency = true;
			m_alphaTest = alphaTest;
			m_alphaTransparency = alphaTransparency;

			std::vector<Sprite*>::iterator iter;
			for( iter = m_quads.begin(); iter != m_quads.end(); iter++ ) {
				(*iter)->enableTransparency( alphaTest, alphaTransparency );
			}
		}

		void Object_BitmapText::disableTransparency() {
			m_transparency = false;

			std::vector<Sprite*>::iterator iter;
			for( iter = m_quads.begin(); iter != m_quads.end(); iter++ ) {
				(*iter)->disableTransparency();
			}
		}

	}
}