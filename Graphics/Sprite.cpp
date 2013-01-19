
#include "GL/glew.h"
#include "GL/glfw.h"

#include "debug.h"
#include "Sprite.h"
#include "ShaderDefaults.h"

namespace Rocket {
	namespace Graphics {

		// Definition for static members
		Mesh * Sprite::Global_SpriteMesh = NULL;
		int Sprite::Global_SpriteCount = 0;

		Sprite::Sprite( Texture * texture, int width, int height ) : Raster( NULL ) {
			m_texture = texture;

			if ( ( width <= 0 ) || ( height <= 0 ) ) {
				Core::Debug_AddToLog( "Error: Sprites must have positive width and height." );
				exit( 0 );
			}

			if ( Global_SpriteMesh == NULL ) {
				Core::Debug_AddToLog( "Error: Sprites must be enabled in a scene [ Sprite::enableSpritesInScene( scene ) ] before initializing sprite objects." );
				exit( 0 );
			}
			m_quad = new Object( Global_SpriteMesh );
			m_rasterTransform = m_quad;

			ShaderDefaults::setObjectUniforms_texture( m_quad,
				Rocket::Graphics::Shader_UniformTexture( 0, m_texture, GL_REPEAT, GL_REPEAT ),
				Rocket::Core::vec2( 1.0f, 1.0f ), Rocket::Core::vec2( 0.0f, 0.0f ), Rocket::Core::vec3( 1.0f, 1.0f, 1.0f ), false, 0.0f, 1.0f );

			Core::vec2i size = texture->getSize();
			m_uvCoords = Core::vec4i( 0, 0, size.x, size.y );
			setSize( Core::vec2i( width, height ) );
			setPosition( Core::vec2i( 0, 0 ) );
			m_angle = 0.0f;	// do not use setAngle() to init because it relies on m_angle

			Global_SpriteCount++;
		}
		Sprite::~Sprite() {
			//delete m_texture; // Do NOT delete m_texture because it is an external entity that might be shared by other objects
			delete m_quad;

			Global_SpriteCount--;
		}

		void Sprite::enableSpritesInScene( Scene * scene, Shader * meshShader ) {
			if ( Global_SpriteMesh == NULL ) Global_SpriteMesh = generatePrimitive_Quad( meshShader );
			scene->addMesh( Global_SpriteMesh );
		}

		void Sprite::addToScene( Scene * scene ) {
			scene->addObject( m_quad, NULL );
		}

		void Sprite::addAsChild( Transform * parent ) {
			parent->addChild( m_quad, true );
		}

		void Sprite::enableTransparency( float alphaTest, float alphaTransparency ) {
			m_quad->enableTransparency();
			//*(m_quad->getUniformFloat( "uniform_alphaTest" )) = alphaTest;
			//*(m_quad->getUniformFloat( "uniform_alphaTransparency" )) = alphaTransparency;
			((ShaderDefaults::ShaderUniforms_Texture*)(m_quad->getShaderUniforms()))->m_alphaTest = alphaTest;
			((ShaderDefaults::ShaderUniforms_Texture*)(m_quad->getShaderUniforms()))->m_alphaTransparency = alphaTransparency;
		}
		
		void Sprite::disableTransparency() {
			m_quad->disableTransparency();
			//*(m_quad->getUniformFloat( "uniform_alphaTest" )) = 0.0f;
			//*(m_quad->getUniformFloat( "uniform_alphaTransparency" )) = 1.0f;
			((ShaderDefaults::ShaderUniforms_Texture*)(m_quad->getShaderUniforms()))->m_alphaTest = 0.0f;
			((ShaderDefaults::ShaderUniforms_Texture*)(m_quad->getShaderUniforms()))->m_alphaTransparency = 1.0f;
		}

		void Sprite::setUV( int left, int top, int right, int bottom ) {
			/*
			m_uvCoords = Core::vec4i( left, top, right, bottom );

			// Set texture scale and offset to simulate a change in uv coords
			Core::vec2i size = m_texture->getSize();
			float x_scale, y_scale;
			float x_offset, y_offset;

			if ( left <= right ) {
				x_scale = (right - left) / (size.x*1.0f);
			} else {
				x_scale = ((size.x - left) + right) / (size.x*1.0f);
			}

			if ( top <= bottom ) {
				y_scale = (bottom - top) / (size.y*1.0f);
			} else {
				y_scale = ((size.y - top) + bottom) / (size.y*1.0f);
			}

			x_offset = left / (size.x*1.0f);
			y_offset = -1.0f*top / (size.y*1.0f) + (1.0f - y_scale);

			*((Core::vec2*)m_quad->getUniformProperty( "textureScale" )) = Core::vec2( x_scale, y_scale );
			*((Core::vec2*)m_quad->getUniformProperty( "textureOffset" )) = Core::vec2( x_offset, y_offset );
			*/
			Core::vec2i size = m_texture->getSize();
			setUV( left/(size.x*1.0f), (top+1)/(size.y*1.0f), right/(size.x*1.0f), (bottom+1)/(size.y*1.0f) );
		}

		void Sprite::setUV( float left, float top, float right, float bottom ) {
			Core::vec2i size = m_texture->getSize();
			Core::vec4i newCoords( (int)(left*size.x), (int)(top*size.y), (int)(right*size.x), (int)(bottom*size.y) );

			if ( m_uvCoords != newCoords ) {
				m_uvCoords = newCoords;

				float x_scale, y_scale;
				float x_offset, y_offset;

				if ( left <= right ) {
					x_scale = right - left;
				} else {
					x_scale = (1.0f - left) + right;
				}

				if ( top <= bottom ) {
					y_scale = bottom - top;
				} else {
					y_scale = (1.0f - top) + bottom;
				}

				x_offset = left;
				y_offset = -top + (1.0f - y_scale);

				// Set texture scale and offset to simulate a change in uv coords
				//*(m_quad->getUniformVec2( "textureScale" )) = Core::vec2( x_scale, y_scale );
				//*(m_quad->getUniformVec2( "textureOffset" )) = Core::vec2( x_offset, y_offset );
				ShaderDefaults::ShaderUniforms_Texture * uniforms = (ShaderDefaults::ShaderUniforms_Texture*)m_quad->getShaderUniforms();
				uniforms->m_textureScale = Core::vec2( x_scale, y_scale );
				uniforms->m_textureOffset = Core::vec2( x_offset, y_offset );
			}
		}

		Core::vec4i Sprite::getUV() {
			return m_uvCoords;
		}

	}
}