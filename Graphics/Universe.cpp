
#include <vector>
#include <unordered_map>
#include <string>

#include "glew.h"
#include "glfw.h"

#include "Universe.h"
#include "Scene.h"
#include "Texture.h"
#include "Shader.h"

namespace Rocket {
	namespace Graphics {

		// Creates an empty Universe
		Universe::Universe() {
		}
		Universe::~Universe() {
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
			m_frame_renderedPolygons = 0;
			m_frame_renderedObjects = 0;
#endif

			for (iter = m_renderPasses.begin(); iter != m_renderPasses.end(); iter++) {
				(*iter)->draw( elapsedMilliseconds, clearScreen );
				clearScreen = false;

#ifdef ENABLE_DEBUG
				m_frame_renderedPolygons += (*iter)->m_frame_renderedPolygons;
				m_frame_renderedObjects += (*iter)->m_frame_renderedObjects;
#endif
			}
		}

	}
}