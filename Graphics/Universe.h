
#ifndef Rocket_Graphics_Universe_H
#define Rocket_Graphics_Universe_H

#include <vector>
#include <unordered_map>
#include <string>

#include "glew.h"
#include "glfw.h"

#include "Scene.h"
#include "Texture.h"
#include "Shader.h"
#include "vector.h"

namespace Rocket {
	namespace Graphics {

		// Static Globals
		static const int FRAME_TIME = 33;
		static const int WindowWidth = 1024;
		static const int WindowHeight = 768;

		#define BUFFER_OFFSET(bytes) ((GLvoid*)(bytes))

		//#define GL_GET_ERROR(flag) if(glGetError() != 0) { std::cout << std::dec << "Flag:" << flag << std::hex << "   GLError: " << glGetError() << "\n"; }

		#define GL_GET_ERROR() { int err = glGetError(); if(err != 0) { std::cout << __FILE__ << " [ " << __LINE__ << " ] :" << std::hex << "   GLError: " << err << "\n"; } }

		// Universe - Asset Management Class
		// Use this class to efficiently draw scenes and manage meshes, textures, and shaders.
		class Universe {
		public:
			Universe();
			~Universe();

			void addRenderPass( Scene * renderPass );

			Shader * addShader( const char * shaderName, Shader * shader );
			Shader * getShader( const char * shaderName );

			Texture * loadTexture( const char * textureName, const char * file, bool mipmap, bool invertY );
			Texture * getTexture( const char * textureName );

			void display( float elapsedMilliseconds );
			// elapsedMilliseconds since the last call to display(). This value is used to correctly sync interpolating within all render passes.

#ifdef ENABLE_DEBUG
			int m_frame_renderedPolygons;
			int m_frame_renderedObjects;
#endif

		private:
			std::vector<Scene*> m_renderPasses;

			std::unordered_map< std::string, Shader* > m_shaders;
			std::unordered_map< std::string, Texture* > m_textures;
		};

	}
}


#endif