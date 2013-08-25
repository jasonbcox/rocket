
#ifndef Rocket_Graphics_Universe_H
#define Rocket_Graphics_Universe_H

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "rocket/Core/vector.h"

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

			void addShader( const char * shaderName, Shader * shader );
			shared_ptr< Shader > getShader( const char * shaderName );

			shared_ptr< Mesh > loadMesh( const char * meshName, const char * file, Shader * shader );
			void addMesh( const char * meshName, Mesh * mesh );
			shared_ptr< Mesh > getMesh( const char * meshName );

			shared_ptr< Texture > loadTexture( const char * textureName, const char * file, bool mipmap, bool invertY );
			shared_ptr< Texture > getTexture( const char * textureName );

			void display( float elapsedMilliseconds );
			// elapsedMilliseconds since the last call to display(). This value is used to correctly sync interpolating within all render passes.

#ifdef ENABLE_DEBUG
			int m_cache_renderedPolygons;
			int m_cache_renderedObjects;
#endif

		private:
			vector< shared_ptr< Scene > > m_renderPasses;

			unordered_map< string, shared_ptr< Shader > > m_shaders;
			unordered_map< string, shared_ptr< Mesh > > m_meshes;
			unordered_map< string, shared_ptr< Texture > > m_textures;
		};

	}
}


#endif