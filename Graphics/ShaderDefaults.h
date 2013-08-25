
#ifndef Rocket_Graphics_ShaderDefaults_H
#define Rocket_Graphics_ShaderDefaults_H

#include "Shader.h"
#include "Universe.h"

using namespace Rocket::Core;

namespace Rocket {
	namespace Graphics {

		namespace ShaderDefaults {

			class ShaderUniforms_Texture : public ShaderUniforms {
			public:
				ShaderUniforms_Texture();
				virtual ~ShaderUniforms_Texture();

				shared_ptr< ShaderUniforms > clone();

				Shader_UniformTexture m_texture;
				Core::vec2 m_textureScale;
				Core::vec2 m_textureOffset;

				Core::vec3 m_color;
				float m_alphaTest;
				float m_alphaTransparency;
			};

			class Shader_Texture : public Shader {
			public:
				Shader_Texture( const char * file_vertexShader, const char * file_fragmentShader );
				virtual ~Shader_Texture();

				Shader_UniformTexture ** getTexture();
				Core::vec2 ** getTextureScale();
				Core::vec2 ** getTextureOffset();

				Core::vec3 ** getColor();
				float ** getAlphaTest();
				float ** getAlphaTransparency();

			protected:
				void refreshMyUniformLocationCache();
				void passMyUniformDataToGPU( ShaderUniforms * uniforms );

			private:
				Shader_UniformTexture * m_texture;
				Core::vec2 * m_textureScale;
				Core::vec2 * m_textureOffset;
				ShaderUniformCache m_cache_texture;
				ShaderUniformCache m_cache_textureScale;
				ShaderUniformCache m_cache_textureOffset;

				Core::vec3 * m_color;
				float * m_alphaTest;
				float * m_alphaTransparency;
				ShaderUniformCache m_cache_color;
				ShaderUniformCache m_cache_alphaTest;
				ShaderUniformCache m_cache_alphaTransparency;
			};

			void setObjectUniforms_texture( Object * object,
											Shader_UniformTexture textureData,
											Core::vec2 textureScale,
											Core::vec2 textureOffset,
											Core::vec3 objectColor,
											bool transparencyEnabled, float alphaTest, float alphaTransparency );

		}

	}
}

#endif