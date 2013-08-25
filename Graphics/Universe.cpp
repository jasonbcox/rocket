
#include <vector>
#include <unordered_map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Universe.h"
#include "Scene.h"
#include "Texture.h"
#include "Shader.h"

namespace Rocket {
	namespace Graphics {

		// Creates an empty Universe
		Universe::Universe() {
#ifdef ENABLE_DEBUG
			m_cache_renderedPolygons = 0;
			m_cache_renderedObjects = 0;
#endif
		}
		Universe::~Universe() {
		}

		// Add a render pass to the end of the render list.
		// Render passes must be added in the correct order, as dependecies are not checked.
		void Universe::addRenderPass( Scene * renderPass ) {
			m_renderPasses.push_back( static_pointer_cast< Scene >( renderPass->shared_from_this() ) );
		}

		// Load and compile a shader
		void Universe::addShader( const char * shaderName, Shader * shader ) {
			m_shaders[ shaderName ] = shader->shared_from_this();
		}
		// Get an existing shader program
		shared_ptr< Shader > Universe::getShader( const char * shaderName ) {
			auto iter = m_shaders.find( shaderName );
			if (iter == m_shaders.end()) {
				return nullptr;
			} else {
				return (*iter).second;
			}
		}

		// Load a mesh into memory. If a mesh with the same name already exists, return a pointer to that mesh.
		shared_ptr< Mesh > Universe::loadMesh( const char * meshName, const char * file, Shader * shader ) {
			auto iter = m_meshes.find( meshName );
			if (iter == m_meshes.end()) {
				auto mesh = make_shared< Mesh >( file, shader );
				m_meshes[ meshName ] = mesh;
				return mesh;
			} else {
				return (*iter).second;
			}
		}
		// Add a mesh if it doesn't already exist
		void Universe::addMesh( const char * meshName, Mesh * mesh ) {
			auto iter = m_meshes.find( meshName );
			if (iter == m_meshes.end()) {
				m_meshes[ meshName ] = mesh->shared_from_this();
			} else {
				// todo: Warning/Error: Mesh already added to universe!
			}
		}
		// Retrieve an already loaded mesh
		shared_ptr< Mesh > Universe::getMesh( const char * meshName ) {
			auto iter = m_meshes.find( meshName );
			if (iter == m_meshes.end()) {
				return nullptr;
			} else {
				return (*iter).second;
			}
		}

		// Load a texture into memory.  If the texture already exists in memory, return a pointer to that texture.
		shared_ptr< Texture > Universe::loadTexture( const char * textureName, const char * file, bool mipmap, bool invertY ) {
			auto iter = m_textures.find( textureName );
			if (iter == m_textures.end()) {
				auto texture = make_shared< Texture >( file, mipmap, invertY );
				m_textures[ textureName ] = texture;
				return texture;
			} else {
				return (*iter).second;
			}
		}
		// Retrieve an already loaded texture
		shared_ptr< Texture > Universe::getTexture( const char * textureName ) {
			auto iter = m_textures.find( textureName );
			if (iter == m_textures.end()) {
				return shared_ptr< Texture >();
			} else {
				return (*iter).second;
			}
		}

		void Universe::display( float elapsedMilliseconds ) {
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			bool clearScreen = true;

#ifdef ENABLE_DEBUG
			m_cache_renderedPolygons = 0;
			m_cache_renderedObjects = 0;
#endif

			for ( auto pass : m_renderPasses ) {
				pass->draw( elapsedMilliseconds, clearScreen );
				clearScreen = false;

#ifdef ENABLE_DEBUG
				m_cache_renderedPolygons += pass->m_cache_renderedPolygons;
				m_cache_renderedObjects += pass->m_cache_renderedObjects;
#endif
			}
		}

	}
}