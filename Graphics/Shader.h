
#ifndef Rocket_Graphics_Shader_H
#define Rocket_Graphics_Shader_H

#include <map>
#include <vector>
#include <string>
#include <utility>

#include "GL/glew.h"
#include "GL/glfw.h"

#include "matrix.h"
#include "Texture.h"

namespace Rocket {
	namespace Graphics {

		class Shader_UniformTexture {
		public:
			Shader_UniformTexture( unsigned int textureSlot = 0, Texture * texture = NULL, unsigned int wrapS = GL_CLAMP_TO_EDGE, unsigned int wrapT = GL_CLAMP_TO_EDGE );
			~Shader_UniformTexture();

			unsigned int m_textureSlot;		// 0, etc...
			Texture * m_texture;
			unsigned int m_wrapS;
			unsigned int m_wrapT;
		};

		static const long InvalidShaderUniformLocation = -1;

		class ShaderUniformCache {
		public:
			ShaderUniformCache();
			~ShaderUniformCache();
			
			void refreshCache( unsigned int shaderNumber );
			bool uniformNeedsRefreshing( void * newValue, size_t size );

			void * cachedValue;
			size_t cacheSize;
			bool cacheIsClean;
			long cachedUniformLocation;
			std::string uniformName;
		};

		class ShaderUniforms {
		public:
			ShaderUniforms();
			~ShaderUniforms();

			virtual ShaderUniforms * clone();
		};

		class Shader {
		public:
			Shader( const char * file_vertexShader, const char * file_fragmentShader );
			~Shader();

			unsigned int getShaderNumber();

			void refreshUniformLocationCache();
			void passUniformDataToGPU( ShaderUniforms * uniforms );

			void useShaderProgram();

			const Core::mat4 ** getCameraOrientation();
			const Core::mat4 ** getCameraPerspective();
			const Core::mat4 ** getObjecTransform();

		protected:
			static unsigned int Global_InUseShaderProgram;

			unsigned int m_shaderNumber;

			virtual void refreshMyUniformLocationCache() = 0;
			virtual void passMyUniformDataToGPU( ShaderUniforms * uniforms ) = 0;

			void refreshMyDefaultUniformLocationCache();
			void passMyDefaultUniformDataToGPU( ShaderUniforms * uniforms );

			Core::mat4 * m_cameraOrientation;
			Core::mat4 * m_cameraPerspective;
			Core::mat4 * m_objectTransform;
			ShaderUniformCache m_cache_cameraOrientation;
			ShaderUniformCache m_cache_cameraPerspective;
			ShaderUniformCache m_cache_objectTransform;
		};

	}
}


#endif