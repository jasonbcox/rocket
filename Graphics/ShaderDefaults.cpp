
#include "ShaderDefaults.h"
#include "debug.h"

namespace Rocket {
	namespace Graphics {

		namespace ShaderDefaults {

			ShaderUniforms_Texture::ShaderUniforms_Texture() {
			}
			ShaderUniforms_Texture::~ShaderUniforms_Texture() {
			}

			ShaderUniforms_Texture * ShaderUniforms_Texture::clone() {
				ShaderUniforms_Texture * r = new ShaderUniforms_Texture();

				r->m_texture = m_texture;
				r->m_textureScale = m_textureScale;
				r->m_textureOffset = m_textureOffset;

				r->m_color = m_color;
				r->m_alphaTest = m_alphaTest;
				r->m_alphaTransparency = m_alphaTransparency;

				return r;
			}

			Shader_Texture::Shader_Texture( const char * file_vertexShader, const char * file_fragmentShader ) : Shader( file_vertexShader, file_fragmentShader ) {
				m_cache_texture.uniformName = "textureMap";
				m_cache_textureScale.uniformName = "textureScale";
				m_cache_textureOffset.uniformName = "textureOffset";

				m_cache_color.uniformName = "uniform_color";
				m_cache_alphaTest.uniformName = "uniform_alphaTest";
				m_cache_alphaTransparency.uniformName = "uniform_alphaTransparency";
			}
			Shader_Texture::~Shader_Texture() {
			}

			void Shader_Texture::refreshMyUniformLocationCache() {
				refreshMyDefaultUniformLocationCache();
				m_cache_texture.refreshCache( m_shaderNumber );
				m_cache_textureScale.refreshCache( m_shaderNumber );
				m_cache_textureOffset.refreshCache( m_shaderNumber );

				m_cache_color.refreshCache( m_shaderNumber );
				m_cache_alphaTest.refreshCache( m_shaderNumber );
				m_cache_alphaTransparency.refreshCache( m_shaderNumber );
			}

			void Shader_Texture::passMyUniformDataToGPU( ShaderUniforms * i_uniforms ) {
				ShaderUniforms_Texture * uniforms = (ShaderUniforms_Texture*)i_uniforms;
				passMyDefaultUniformDataToGPU( uniforms );

				m_texture = &uniforms->m_texture;
				m_textureScale = &uniforms->m_textureScale;
				m_textureOffset = &uniforms->m_textureOffset;

				m_color = &uniforms->m_color;
				m_alphaTest = &uniforms->m_alphaTest;
				m_alphaTransparency = &uniforms->m_alphaTransparency;

				if ( m_cache_texture.uniformNeedsRefreshing( m_texture, sizeof(Shader_UniformTexture) ) == true ) {
					// Texture number (0, 1, etc...)
					glUniform1i( m_cache_texture.cachedUniformLocation, m_texture->m_textureSlot );
					// Active texture (GL_TEXTURE0, etc...)
					glActiveTexture( GL_TEXTURE0 + m_texture->m_textureSlot );
					glBindTexture( GL_TEXTURE_2D, m_texture->m_texture->getID() );
					// Texture properties
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_texture->m_wrapS );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_texture->m_wrapT );
				}
				if ( m_cache_textureScale.uniformNeedsRefreshing( m_textureScale, sizeof(Core::vec2) ) == true ) {
					glUniform2fv( m_cache_textureScale.cachedUniformLocation, 1, *m_textureScale );
				}
				if ( m_cache_textureOffset.uniformNeedsRefreshing( m_textureOffset, sizeof(Core::vec2) ) == true ) {
					glUniform2fv( m_cache_textureOffset.cachedUniformLocation, 1, *m_textureOffset );
				}

				if ( m_cache_color.uniformNeedsRefreshing( m_color, sizeof(Core::vec3) ) == true ) {
					glUniform3fv( m_cache_color.cachedUniformLocation, 1, *m_color );
				}
				if ( m_cache_alphaTest.uniformNeedsRefreshing( m_alphaTest, sizeof(float) ) == true ) {
					glUniform1f( m_cache_alphaTest.cachedUniformLocation, *m_alphaTest );
				}
				if ( m_cache_alphaTransparency.uniformNeedsRefreshing( m_alphaTransparency, sizeof(float) ) == true ) {
					glUniform1f( m_cache_alphaTransparency.cachedUniformLocation, *m_alphaTransparency );
				}
			}

			void setObjectUniforms_texture( Rocket::Graphics::Object * object,
												Rocket::Graphics::Shader_UniformTexture textureData,
												Rocket::Core::vec2 textureScale,
												Rocket::Core::vec2 textureOffset,
												Rocket::Core::vec3 objectRGBColor,
												bool transparencyEnabled, float alphaTest, float alphaTransparency )
			{
				ShaderUniforms_Texture * uniforms = new ShaderUniforms_Texture();
				uniforms->m_texture = textureData;
				uniforms->m_textureScale = textureScale;
				uniforms->m_textureOffset = textureOffset;

				uniforms->m_color = objectRGBColor;
				uniforms->m_alphaTest = alphaTest;
				uniforms->m_alphaTransparency = alphaTransparency;
				if ( transparencyEnabled == true ) {
					object->enableTransparency();
				} else {
					object->disableTransparency();
				}

				object->setShaderUniforms( uniforms );
			}

		}

	}
}