
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
			// Cleanup Scenes
			std::vector<Scene*>::iterator iter_scenes;
			for ( iter_scenes = m_renderPasses.begin(); iter_scenes != m_renderPasses.end(); iter_scenes++ ) {
				delete (*iter_scenes);
			}
			m_renderPasses.clear();

			// Cleanup Meshes
			std::unordered_map< std::string, Mesh* >::iterator iter_meshes;
			for ( iter_meshes = m_meshes.begin(); iter_meshes != m_meshes.end(); iter_meshes++ ) {
				delete (*iter_meshes).second;
			}
			m_meshes.clear();

			// Cleanup Textures
			std::unordered_map< std::string, Texture* >::iterator iter_textures;
			for ( iter_textures = m_textures.begin(); iter_textures != m_textures.end(); iter_textures++ ) {
				delete (*iter_textures).second;
			}
			m_textures.clear();

			// Cleanup Shaders
			std::unordered_map< std::string, Shader* >::iterator iter_shaders;
			for ( iter_shaders = m_shaders.begin(); iter_shaders != m_shaders.end(); iter_shaders++ ) {
				delete (*iter_shaders).second;
			}
			m_shaders.clear();
		}

		// Add a render pass to the end of the render list.
		// Render passes must be added in the correct order, as dependecies are not checked.
		void Universe::addRenderPass( Scene * renderPass ) {
			m_renderPasses.push_back( renderPass );
		}

		// Load and compile a shader
		Shader * Universe::addShader( const char * shaderName, Shader * shader ) {
			m_shaders[ shaderName ] = shader;
			return shader;
		}
		// Get an existing shader program
		Shader * Universe::getShader( const char * shaderName ) {
			std::unordered_map< std::string, Shader* >::iterator iter = m_shaders.find( shaderName );
			if (iter == m_shaders.end()) {
				return NULL;
			} else {
				return (*iter).second;
			}
		}

		// Load a mesh into memory. If a mesh with the same name already exists, return a pointer to that mesh.
		Mesh * Universe::loadMesh( const char * meshName, const char * file, Shader * shader ) {
			std::unordered_map< std::string, Mesh* >::iterator iter = m_meshes.find( meshName );
			if (iter == m_meshes.end()) {
				Mesh * mesh = new Mesh( file, shader );
				m_meshes[ meshName ] = mesh;
				return mesh;
			} else {
				return (*iter).second;
			}
		}
		// Add a mesh if it doesn't already exist
		void Universe::addMesh( const char * meshName, Mesh * mesh ) {
			std::unordered_map< std::string, Mesh* >::iterator iter = m_meshes.find( meshName );
			if (iter == m_meshes.end()) {
				m_meshes[ meshName ] = mesh;
			} else {
				// todo: Warning/Error: Mesh already added to universe!
			}
		}
		// Retrieve an already loaded mesh
		Mesh * Universe::getMesh( const char * meshName ) {
			std::unordered_map< std::string, Mesh* >::iterator iter = m_meshes.find( meshName );
			if (iter == m_meshes.end()) {
				return NULL;
			} else {
				return (*iter).second;
			}
		}

		// Load a texture into memory.  If the texture already exists in memory, return a pointer to that texture.
		Texture * Universe::loadTexture( const char * textureName, const char * file, bool mipmap, bool invertY ) {
			std::unordered_map< std::string, Texture* >::iterator iter = m_textures.find( textureName );
			if (iter == m_textures.end()) {
				Texture * texture = new Texture( file, mipmap, invertY );
				m_textures[ textureName ] = texture;
				return texture;
			} else {
				return (*iter).second;
			}
		}
		// Retrieve an already loaded texture
		Texture * Universe::getTexture( const char * textureName ) {
			std::unordered_map< std::string, Texture* >::iterator iter = m_textures.find( textureName );
			if (iter == m_textures.end()) {
				return NULL;
			} else {
				return (*iter).second;
			}
		}

		void Universe::display( float elapsedMilliseconds ) {
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			bool clearScreen = true;
			std::vector<Scene*>::iterator iter;

#ifdef ENABLE_DEBUG
			m_cache_renderedPolygons = 0;
			m_cache_renderedObjects = 0;
#endif

			for (iter = m_renderPasses.begin(); iter != m_renderPasses.end(); iter++) {
				(*iter)->draw( elapsedMilliseconds, clearScreen );
				clearScreen = false;

#ifdef ENABLE_DEBUG
				m_cache_renderedPolygons += (*iter)->m_cache_renderedPolygons;
				m_cache_renderedObjects += (*iter)->m_cache_renderedObjects;
#endif
			}
		}

	}
}