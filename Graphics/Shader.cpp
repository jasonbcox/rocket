
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Universe.h"
#include "Shader.h"
#include "rocket/Core/matrix.h"
#include "Texture.h"

namespace Rocket {
	namespace Graphics {

		unsigned int Shader::Global_InUseShaderProgram = 0;

		/*! Create a Shader_UniformTexture using a Texture as a uniform in the specified texture slot with the given properties
			wrapS - GL wrap value for the S-Axis
			wrapT - GL wrap value for the T-Axis
		*/
		Shader_UniformTexture::Shader_UniformTexture( unsigned int textureSlot, Texture * texture, unsigned int wrapS, unsigned int wrapT ) {
			m_textureSlot = textureSlot;
			if ( texture == nullptr ) {
				m_texture = shared_ptr< Texture >();
			} else {
				m_texture = texture->shared_from_this();
			}
			m_wrapS = wrapS;
			m_wrapT = wrapT;
		}
		Shader_UniformTexture::~Shader_UniformTexture() {
		}

		//! Create a ShaderUniformCache for caching a Shader's uniform value
		ShaderUniformCache::ShaderUniformCache() {
			cachedValue = nullptr;
			cacheSize = 0;
			cacheIsClean = false;
			cachedUniformLocation = InvalidShaderUniformLocation;
		}
		ShaderUniformCache::~ShaderUniformCache() {
			if ( cacheSize > 0 ) {
				Core::Debug_Scramble( cachedValue, cacheSize );
				free( cachedValue );
			}
		}

		//! Reset the uniform's location data with respect to the given shader
		void ShaderUniformCache::refreshCache( unsigned int shaderNumber ) {
			cachedUniformLocation = glGetUniformLocation( shaderNumber, uniformName.c_str() );
			if ( cachedUniformLocation < 0 ) {
				// error
			}
		}

		//! Returns true if the uniform needs refreshing given a newValue and the size of that data
		bool ShaderUniformCache::uniformNeedsRefreshing( void * newValue, size_t size ) {
			if ( ( cacheIsClean == false ) || ( cacheSize != size ) || ( memcmp( cachedValue, newValue, size ) != 0 ) ) {
				if ( cacheSize != size ) {
					if ( cacheSize > 0 ) {
						Core::Debug_Scramble( cachedValue, cacheSize );
						free( cachedValue );
					}
					cacheSize = size;
					cachedValue = malloc( size );
				}
				if ( size > 0 ) memcpy( cachedValue, newValue, size );
				cacheIsClean = true;
				return true;
			}
			return false;
		}

		ShaderUniforms::ShaderUniforms() {
		}
		ShaderUniforms::~ShaderUniforms() {
		}

		//! Returns a copy of this ShaderUniforms
		shared_ptr< ShaderUniforms > ShaderUniforms::clone() {
			auto r = make_shared< ShaderUniforms >();
			return r;
		}

		// Create a NULL-terminated string by reading the provided file
		static char * readFile( const char * fileName ) {
			FILE * fp = fopen( fileName, "rb" );	// "rb" is necessary to correctly read the end-of-line characters
			if ( fp == NULL ) { return NULL; }

			// Get file size
			fseek( fp, 0L, SEEK_END );
			long size = ftell( fp );

			// Read file
			fseek( fp, 0L, SEEK_SET );
			char * buf = new char[size + 1];
			fread( buf, 1, size, fp );
			buf[size] = '\0';
			fclose(fp);

			return buf;
		}

		//! Load the shader files and compile them into a shader program
		Shader::Shader( const char * file_vertexShader, const char * file_fragmentShader) {
			struct TempShader {
				const char * filename;
				GLenum type;
				GLchar * source;
			} shaders[2] = {
				{ file_vertexShader, GL_VERTEX_SHADER, NULL },
				{ file_fragmentShader, GL_FRAGMENT_SHADER, NULL }
			};

			m_shaderNumber = glCreateProgram();											GL_GET_ERROR();

			for ( int i = 0; i < 2; ++i ) {
				TempShader & s = shaders[i];
				s.source = readFile( s.filename );
				if ( shaders[i].source == NULL ) {
					std::cerr << "Failed to read " << s.filename << std::endl;

					system("pause");
					exit( EXIT_FAILURE );
				}

				GLuint shader = glCreateShader( s.type );								GL_GET_ERROR();
				glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );			GL_GET_ERROR();
				glCompileShader( shader );												GL_GET_ERROR();

				GLint compiled;
				glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );					GL_GET_ERROR();
				if ( !compiled ) {
					std::cerr << s.filename << " failed to compile:" << std::endl;
					GLint logSize;
					glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
					char * logMsg = new char[logSize];
					glGetShaderInfoLog( shader, logSize, NULL, logMsg );
					std::cerr << logMsg << std::endl;
					delete [] logMsg;

					system("pause");
					exit( EXIT_FAILURE );
				}

				delete [] s.source;

				glAttachShader( m_shaderNumber, shader );								GL_GET_ERROR();
				glDeleteShader( shader );												GL_GET_ERROR();
			}

			glLinkProgram( m_shaderNumber );											GL_GET_ERROR();

			GLint linked;
			glGetProgramiv( m_shaderNumber, GL_LINK_STATUS, &linked );					GL_GET_ERROR();
			if ( !linked ) {
				std::cerr << "Shader program failed to link" << std::endl;
				GLint logSize;
				glGetProgramiv( m_shaderNumber, GL_INFO_LOG_LENGTH, &logSize);
				char * logMsg = new char[logSize];
				glGetProgramInfoLog( m_shaderNumber, logSize, NULL, logMsg );
				std::cerr << logMsg << std::endl;
				delete [] logMsg;

				system("pause");
				exit( EXIT_FAILURE );
			}

			m_cache_cameraOrientation.uniformName = "orientation";
			m_cache_cameraPerspective.uniformName = "perspective";
			m_cache_objectTransform.uniformName = "transform";
		}

		Shader::~Shader() {
			glDeleteProgram( m_shaderNumber );
		}

		//! Returns the shader number of this Shader
		unsigned int Shader::getShaderNumber() {
			return m_shaderNumber;
		}

		//! Refreshes this Shader's entire uniform cache
		void Shader::refreshUniformLocationCache() {
			this->refreshMyUniformLocationCache();
		}

		//! Refreshes this Shader's camera and object matrix uniforms
		void Shader::refreshMyDefaultUniformLocationCache() {
			m_cache_cameraOrientation.refreshCache( m_shaderNumber );
			m_cache_cameraPerspective.refreshCache( m_shaderNumber );
			m_cache_objectTransform.refreshCache( m_shaderNumber );
		}

		//! Pass this Shader's uniform data to the GPU
		void Shader::passUniformDataToGPU( ShaderUniforms * uniforms ) {
			this->passMyUniformDataToGPU( uniforms );
		}

		//! Pass only this Shader's default uniforms to the GPU
		void Shader::passMyDefaultUniformDataToGPU( ShaderUniforms * uniforms ) {
			if ( m_cache_cameraOrientation.uniformNeedsRefreshing( m_cameraOrientation, sizeof(Core::mat4) ) == true ) {
				glUniformMatrix4fv( m_cache_cameraOrientation.cachedUniformLocation, 1, GL_TRUE, *m_cameraOrientation );
			}
			if ( m_cache_cameraPerspective.uniformNeedsRefreshing( m_cameraPerspective, sizeof(Core::mat4) ) == true ) {
				glUniformMatrix4fv( m_cache_cameraPerspective.cachedUniformLocation, 1, GL_TRUE, *m_cameraPerspective );
			}
			if ( m_cache_objectTransform.uniformNeedsRefreshing( m_objectTransform, sizeof(Core::mat4) ) == true ) {
				glUniformMatrix4fv( m_cache_objectTransform.cachedUniformLocation, 1, GL_TRUE, *m_objectTransform );
			}
				
			//glUniform1i( tempVarLoc, *((int*)((*localIter).m_value)) );
			//glUniform1f( tempVarLoc, *((float*)((*localIter).m_value)) );
			//glUniform2fv( tempVarLoc, 1, *((Core::vec2*)((*localIter).m_value)) );
			//glUniform3fv( tempVarLoc, 1, *((Core::vec3*)((*localIter).m_value)) );
			//glUniform4fv( tempVarLoc, 1, *((Core::vec4*)((*localIter).m_value)) );
			//glUniformMatrix3fv( tempVarLoc, 1, GL_TRUE, *((Core::mat3*)((*localIter).m_value)) );
			//glUniformMatrix4fv( tempVarLoc, 1, GL_TRUE, *((Core::mat4*)((*localIter).m_value)) );
		}

		//! Returns the camera orientation stored in this Shader's uniform
		const Core::mat4 ** Shader::getCameraOrientation() {
			return (const Core::mat4 **)&m_cameraOrientation;
		}
		//! Returns the camera perspective (view matrix) stored in this Shader's uniform
		const Core::mat4 ** Shader::getCameraPerspective() {
			return (const Core::mat4 **)&m_cameraPerspective;
		}
		//! Returns the object transform matrix stored in this Shader's uniform
		const Core::mat4 ** Shader::getObjectTransform() {
			return (const Core::mat4 **)&m_objectTransform;
		}

		//! Set the GPU to use this Shader program for rendering
		void Shader::useShaderProgram() {
			if ( Global_InUseShaderProgram != m_shaderNumber ) {
				Global_InUseShaderProgram = m_shaderNumber;
				glUseProgram( Global_InUseShaderProgram );
				refreshUniformLocationCache();
			}
		}

	}
}

