
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "rocket/Core/debug.h"
#include "Sprite.h"
#include "ShaderDefaults.h"

using namespace Rocket::Core;
using namespace Rocket::Graphics;

namespace Rocket {
	namespace Graphics {

		// Definition for static members
		shared_ptr< Mesh > Sprite::g_SpriteMesh = shared_ptr< Mesh >();
		int Sprite::g_SpriteCount = 0;

		Sprite::Sprite( Texture * texture, int width, int height ) : Object( g_SpriteMesh.get() ), Raster( this ) {
			m_texture = texture->shared_from_this();

			if ( ( width <= 0 ) || ( height <= 0 ) ) {
				Debug_ThrowError( "Error: Sprites must have positive width and height.", width, height );
			}

			if ( g_SpriteMesh.get() == nullptr ) {
				Debug_ThrowError( "Error: Sprites must be enabled in a scene [ Sprite::enableSpritesInScene( scene ) ] before initializing sprite objects.", g_SpriteMesh.get() );
			}

			ShaderDefaults::setObjectUniforms_texture( this,
				Shader_UniformTexture( 0, texture, GL_REPEAT, GL_REPEAT ),
				vec2( 1.0f, 1.0f ), vec2( 0.0f, 0.0f ), vec3( 1.0f, 1.0f, 1.0f ), false, 0.0f, 1.0f );

			vec2i size = texture->getSize();
			m_uvCoords = vec4i( 0, 0, size.x(), size.y() );
			setSize( vec2i( width, height ) );
			setPosition( vec2i( 0, 0 ) );
			m_angle = 0.0f;	// do not use setAngle() to init because it relies on m_angle

			m_animationPlaying = false;

			g_SpriteCount++;
		}
		Sprite::~Sprite() {
			//delete m_texture; // Do NOT delete m_texture because it is an external entity that might be shared by other objects

			g_SpriteCount--;
		}

		void Sprite::enableSpritesInScene( Universe * world, Scene * scene, Shader * meshShader ) {
			if ( g_SpriteMesh.get() == nullptr ) g_SpriteMesh = generatePrimitive_Quad( world, "SPRITE_MESH", meshShader )->shared_from_this();
			scene->addMesh( g_SpriteMesh.get() );
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

		void Sprite::setUVPixels( int left, int top, int right, int bottom ) {
			vec2i size = m_texture->getSize();
			setUV( left/(size.x()*1.0f), (top+1)/(size.y()*1.0f), right/(size.x()*1.0f), (bottom+1)/(size.y()*1.0f) );
		}

		void Sprite::setUV( float left, float top, float right, float bottom ) {
			vec2i size = m_texture->getSize();
			vec4i newCoords( (int)(left*size.x()), (int)(top*size.y()), (int)(right*size.x()), (int)(bottom*size.y()) );

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
				uniforms->m_textureScale = vec2( x_scale, y_scale );
				uniforms->m_textureOffset = vec2( x_offset, y_offset );
			}
		}

		vec4i Sprite::getUV() {
			return m_uvCoords;
		}

		void Sprite::addAnimation( string animationName, vector< vec4i > frames ) {
			m_frames[ animationName ] = frames;
		}
		void Sprite::playAnimation( string animationName, float timeMilliseconds, bool repeat, int startFrame ) {
			auto frames = m_frames.find( animationName );
			if ( frames != m_frames.end() ) {
				m_animationPlaying = true;
				m_animationFrames = frames->second;
				m_animationTotalFrames = m_animationFrames.size();
				m_animationCurrentFrame = startFrame;
				m_animationTotalTime = timeMilliseconds;
				m_animationTime = startFrame * 1.0f / ( m_animationTotalFrames * 1.0f ) * timeMilliseconds;
				m_animationRepeat = repeat;
			}
		}
		void Sprite::pauseAnimation() {
			m_animationPlaying = false;
		}
		int Sprite::currentFrame() {
			return m_animationCurrentFrame;
		}

		void Sprite::update( bool recursive, float elapsedMilliseconds ) {
			if ( m_animationPlaying == true ) {
				m_animationTime += elapsedMilliseconds;
				int nextFrame = m_animationTime / m_animationTotalTime * m_animationTotalFrames;
				if ( nextFrame >= m_animationTotalFrames ) {
					if ( m_animationRepeat == true ) {
						m_animationCurrentFrame = nextFrame - m_animationTotalFrames;
						m_animationTime -= m_animationTotalTime;
					} else {
						m_animationCurrentFrame = m_animationTotalFrames - 1;
						m_animationTime = m_animationTotalTime;
					}
				} else {
					m_animationCurrentFrame = nextFrame;
				}

				vec4i frameUV = m_animationFrames[ m_animationCurrentFrame ];
				setUVPixels( frameUV.x(), frameUV.y(), frameUV.z(), frameUV.w() );
			}

			Object::update( recursive, elapsedMilliseconds );
		}

	}
}
