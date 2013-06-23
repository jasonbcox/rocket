
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "rocket/Core/debug.h"
#include "Sprite.h"
#include "ShaderDefaults.h"

namespace Rocket {
	namespace Graphics {

		// Definition for static members
		Mesh * Sprite::g_SpriteMesh = NULL;
		int Sprite::g_SpriteCount = 0;

		Sprite::Sprite( Texture * texture, int width, int height ) : Raster( this ), Object( g_SpriteMesh ) {
			m_texture = texture;

			if ( ( width <= 0 ) || ( height <= 0 ) ) {
				Core::Debug_AddToLog( "Error: Sprites must have positive width and height." );
				exit( 0 );
			}

			if ( g_SpriteMesh == NULL ) {
				Core::Debug_AddToLog( "Error: Sprites must be enabled in a scene [ Sprite::enableSpritesInScene( scene ) ] before initializing sprite objects." );
				exit( 0 );
			}

			ShaderDefaults::setObjectUniforms_texture( this,
				Rocket::Graphics::Shader_UniformTexture( 0, m_texture, GL_REPEAT, GL_REPEAT ),
				Rocket::Core::vec2( 1.0f, 1.0f ), Rocket::Core::vec2( 0.0f, 0.0f ), Rocket::Core::vec3( 1.0f, 1.0f, 1.0f ), false, 0.0f, 1.0f );

			Core::vec2i size = texture->getSize();
			m_uvCoords = Core::vec4i( 0, 0, size.x, size.y );
			setSize( Core::vec2i( width, height ) );
			setPosition( Core::vec2i( 0, 0 ) );
			m_angle = 0.0f;	// do not use setAngle() to init because it relies on m_angle

			g_SpriteCount++;
		}
		Sprite::~Sprite() {
			//delete m_texture; // Do NOT delete m_texture because it is an external entity that might be shared by other objects

			g_SpriteCount--;
		}

		void Sprite::enableSpritesInScene( Universe * world, Scene * scene, Shader * meshShader ) {
			if ( g_SpriteMesh == NULL ) g_SpriteMesh = generatePrimitive_Quad( world, "SPRITE_MESH", meshShader );
			scene->addMesh( g_SpriteMesh );
		}

		void Sprite::enableTransparency( float alphaTest, float alphaTransparency ) {
			Object::enableTransparency();

			ShaderDefaults::ShaderUniforms_Texture * uniforms = (ShaderDefaults::ShaderUniforms_Texture*)getShaderUniforms();
			uniforms->m_alphaTest = alphaTest;
			uniforms->m_alphaTransparency = alphaTransparency;
		}
		
		void Sprite::disableTransparency() {
			Object::disableTransparency();

			ShaderDefaults::ShaderUniforms_Texture * uniforms = (ShaderDefaults::ShaderUniforms_Texture*)getShaderUniforms();
			uniforms->m_alphaTest = 0.0f;
			uniforms->m_alphaTransparency = 1.0f;
		}

		void Sprite::setUV( int left, int top, int right, int bottom ) {
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
				ShaderDefaults::ShaderUniforms_Texture * uniforms = (ShaderDefaults::ShaderUniforms_Texture*)getShaderUniforms();
				uniforms->m_textureScale = Core::vec2( x_scale, y_scale );
				uniforms->m_textureOffset = Core::vec2( x_offset, y_offset );
			}
		}

		Core::vec4i Sprite::getUV() {
			return m_uvCoords;
		}

	}
}