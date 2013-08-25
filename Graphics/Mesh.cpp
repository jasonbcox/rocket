
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "rocket/Core/vector.h"
#include "rocket/Core/debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Universe.h"
#include "Scene.h"

namespace Rocket {
	namespace Graphics {

		void Mesh::generateBufferObjects() {
			// Generate the VertexArrayObject and VertexBufferObjects for this mesh
			glGenVertexArrays( 1, &m_vao );
			glBindVertexArray( m_vao );
			glGenBuffers( MESH_VBO_NUM, m_vbo );
		}

		Mesh::Mesh( Shader * shader, int numVertices, Core::vec4 * vertices, Core::vec3 * normals, Core::vec2 * uvCoords ) {
			m_shader = shader->shared_from_this();

			m_vertexCount = numVertices;
			m_vertices = vertices;
			m_normals = normals;
			m_uv = uvCoords;

			if ( m_uv == nullptr ) {
				m_uv = new Core::vec2[m_vertexCount];
			}

			generateBufferObjects();

			passMeshToGPU();
		}
		Mesh::Mesh( const char * OBJ_Wavefront_File, Shader * shader ) {
			m_shader = shader->shared_from_this();

			load_OBJ( OBJ_Wavefront_File );

			generateBufferObjects();

			passMeshToGPU();
			
#ifdef ENABLE_DEBUG
			m_cache_renderedPolygons = 0;
			m_cache_renderedObjects = 0;
#endif
		}
		Mesh::~Mesh() {
			glDeleteVertexArrays( 1, &m_vao );
			glDeleteBuffers( MESH_VBO_NUM, m_vbo );

			Core::Debug_Scramble( m_vertices, m_vertexCount );
			Core::Debug_Scramble( m_normals, m_vertexCount );
			Core::Debug_Scramble( m_uv, m_vertexCount );
			delete [] m_vertices;
			delete [] m_normals;
			delete [] m_uv;
		}

		void Mesh::startPassesForScene( Scene * scene ) {
			auto mapIter = m_objectUsers.find( scene );
			if ( mapIter != m_objectUsers.end() ) {
				m_currentPassIterator = (*mapIter).second.begin();
			} else {
				// Report a warning/error: Mesh not attached to this scene
				// todo: Set a flag to skip this Mesh's draw passes until a new Scene is selected
			}

#ifdef ENABLE_DEBUG
			m_cache_renderedPolygons = 0;
			m_cache_renderedObjects = 0;
#endif
		}

		void Mesh::drawCurrentPass( const Core::mat4 * cameraProjection, const Core::mat4 * cameraOrientation ) {
			m_shader->useShaderProgram();
			glBindVertexArray( getVertexArrayObject() );

			// Orientation of the camera
			*m_shader->getCameraOrientation() = cameraOrientation;

			// Camera projection
			*m_shader->getCameraPerspective() = cameraProjection;

			for ( auto objIter = m_currentPassIterator->begin(); objIter != m_currentPassIterator->end(); objIter++) {
				// Draw Object
				Object * obj = (*objIter).get();
				if ( obj->isVisible() == true ) {
					// Orientation of the object
					*m_shader->getObjecTransform() = &(obj->getFinalOrientation());

					m_shader->passUniformDataToGPU( obj->getShaderUniforms() );

					int vertexCount = getVertexCount();
					glDrawArrays( GL_TRIANGLES, 0, vertexCount );
				}
			}

#ifdef ENABLE_DEBUG
			unsigned int vertexCount = getVertexCount();
			unsigned int objectSetCount = m_currentPassIterator->size();
			m_cache_renderedPolygons += vertexCount/3 * objectSetCount;
			m_cache_renderedObjects += objectSetCount;
#endif

			// Prepare next pass
			m_currentPassIterator++;
		}


		void Mesh::addMeshUser( Object * object ) {
			auto objectOwners = object->getOwners();
			MeshDrawPasses removeFromPass = MeshDrawPasses::Transparent;
			MeshDrawPasses addToPass = MeshDrawPasses::Opaque;
			if ( object->isTransparent() == true ) {
				removeFromPass = MeshDrawPasses::Opaque;
				addToPass = MeshDrawPasses::Transparent;
			}

			for ( auto owner : objectOwners ) {
				auto mapIter = m_objectUsers.find( owner.get() );
				if ( mapIter != m_objectUsers.end() ) {
					// Remove existing user from the other pass
					for ( auto objectIter = mapIter->second[ (unsigned int)removeFromPass ].begin(); objectIter != mapIter->second[ (unsigned int)removeFromPass ].end(); objectIter++ ) {
						if ( objectIter->get() == object ) {
							mapIter->second[ (unsigned int)removeFromPass ].erase( objectIter );
							break;
						}
					}
				} else {
					// Add scene to m_objectUsers and add the correct passes
					mapIter = addSceneToUserList( owner.get() );
				}

				// If user isn't already there, add user to correct pass
				bool userAlreadyInPass = false;
				for ( auto user : mapIter->second[ (unsigned int)addToPass ] ) {
					if ( user.get() == object ) {
						userAlreadyInPass = true;
						break;
					}
				}
				if ( userAlreadyInPass == false ) {
					mapIter->second[ (unsigned int)addToPass ].push_back( static_pointer_cast< Object >( object->shared_from_this() ) );
				}
			}
		}

		std::unordered_map< Scene*, renderPassListType >::iterator Mesh::addSceneToUserList( Scene * scene ) {
			auto mapIter = m_objectUsers.insert( std::pair< Scene*, renderPassListType >( scene, renderPassListType() ) ).first;
			for ( unsigned int i = 0; i < (unsigned int)MeshDrawPasses::END_OF_DRAW_PASSES; i++ ) {
				mapIter->second.push_back( objectUsersListType() );
			}
			return mapIter;
		}

		void Mesh::removeMeshUserFromScene( const Object * meshUser, Scene * scene ) {
			auto mapIter = m_objectUsers.find( scene );
			if ( mapIter != m_objectUsers.end() ) {
				// Remove existing user from all passes
				for ( unsigned int i = 0; i < (unsigned int)MeshDrawPasses::END_OF_DRAW_PASSES; i++ ) {
					for ( auto objectIter = mapIter->second[ i ].begin(); objectIter != mapIter->second[ i ].end(); objectIter++ ) {
						if ( objectIter->get() == meshUser ) {
							mapIter->second[ i ].erase( objectIter );
							break;
						}
					}
				}
			} else {
				// todo: Warning/Error: Mesh is not attached to this scene!
			}
		}

		void Mesh::passMeshToGPU() {
			if ( m_shader.get() == nullptr ) { Rocket::Core::Debug_AddToLog( "Error: Mesh must be linked to a shader." ); system( "pause" ); exit( 1 ); }
			if ( m_vertexCount <= 0 ) { Rocket::Core::Debug_AddToLog( "Error: Mesh must contain at least one vertex." ); system( "pause" ); exit( 1 ); }
			m_shader->useShaderProgram();
			GLuint shaderNum = m_shader->getShaderNumber();
			glBindVertexArray( m_vao );

			int sizeof_vertexBuffer = m_vertexCount * 4 * sizeof( GLfloat );
			int sizeof_normalBuffer = m_vertexCount * 3 * sizeof( GLfloat );
			int sizeof_uvBuffer = m_vertexCount * 2 * sizeof( GLfloat );

			// Using a single VBO:
			GLsizeiptr sizeof_total = sizeof_vertexBuffer + sizeof_normalBuffer + sizeof_uvBuffer;

			glBindBuffer( GL_ARRAY_BUFFER, m_vbo[0] );
			glBufferData( GL_ARRAY_BUFFER, sizeof_total, NULL, GL_DYNAMIC_DRAW );
			glBufferSubData( GL_ARRAY_BUFFER, 0,											sizeof_vertexBuffer,	m_vertices );
			glBufferSubData( GL_ARRAY_BUFFER, sizeof_vertexBuffer,							sizeof_normalBuffer,	m_normals );
			glBufferSubData( GL_ARRAY_BUFFER, sizeof_vertexBuffer + sizeof_normalBuffer,	sizeof_uvBuffer,		m_uv );
	
			GLint shader_vertices = glGetAttribLocation( shaderNum, "vert_position" );
			if (shader_vertices != -1) {
				glEnableVertexAttribArray( shader_vertices );
				glVertexAttribPointer( shader_vertices, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( 0 ) );
			}

			GLint shader_normals = glGetAttribLocation( shaderNum, "vert_normal" );
			if (shader_normals != -1) {
				glEnableVertexAttribArray( shader_normals );
				glVertexAttribPointer( shader_normals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof_vertexBuffer ) );
			}

			GLint shader_UV = glGetAttribLocation( shaderNum, "vert_uv" );
			if (shader_UV != -1) {
				glEnableVertexAttribArray( shader_UV );
				glVertexAttribPointer( shader_UV, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( sizeof_vertexBuffer + sizeof_normalBuffer ) );
			}
	

			// Using multiple VBOs:
			/*
			GLint shader_vertices = glGetAttribLocation( m_shader, "vert_position" );
			if (shader_vertices != -1) {
				glBindBuffer( GL_ARRAY_BUFFER, m_vbo[0] );												GL_GET_ERROR();
				glBufferData( GL_ARRAY_BUFFER, sizeof_vertexBuffer, m_vertices, GL_DYNAMIC_DRAW );		GL_GET_ERROR();

				glEnableVertexAttribArray( shader_vertices );											GL_GET_ERROR();
				glVertexAttribPointer( shader_vertices, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( 0 ) );	GL_GET_ERROR();
			}

			GLint shader_normals = glGetAttribLocation( m_shader, "vert_normal" );
			if (shader_normals != -1) {
				glBindBuffer( GL_ARRAY_BUFFER, m_vbo[1] );												GL_GET_ERROR();
				glBufferData( GL_ARRAY_BUFFER, sizeof_normalBuffer, m_normals, GL_DYNAMIC_DRAW );		GL_GET_ERROR();

				glEnableVertexAttribArray( shader_normals );											GL_GET_ERROR();
				glVertexAttribPointer( shader_normals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( 0 ) );	GL_GET_ERROR();
			}

			GLint shader_UV = glGetAttribLocation( m_shader, "vert_uv" );
			if (shader_UV != -1) {
				glBindBuffer( GL_ARRAY_BUFFER, m_vbo[2] );												GL_GET_ERROR();
				glBufferData( GL_ARRAY_BUFFER, sizeof_uvBuffer, m_uv, GL_DYNAMIC_DRAW );				GL_GET_ERROR();
	
				glEnableVertexAttribArray( shader_UV );													GL_GET_ERROR();
				glVertexAttribPointer( shader_UV, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( 0 ) );		GL_GET_ERROR();
			}
			*/
		}

		GLuint Mesh::getVertexArrayObject() {
			return m_vao;
		}

		Shader * Mesh::getShader() {
			return m_shader.get();
		}

		int Mesh::getVertexCount() {
			return m_vertexCount;
		}

		void Mesh::editMesh( int startVertex, int endVertex, Core::vec4 * vertices, Core::vec3 * normals, Core::vec2 * uvCoords ) {
			if ( ( startVertex < 0 ) || ( startVertex > endVertex ) || ( endVertex >= m_vertexCount ) ) return;
			for ( int x = startVertex; x <= endVertex; x++ ) {
				m_vertices[x] = vertices[ x - startVertex ];
			}
			if ( normals != nullptr ) {
				for ( int x = startVertex; x <= endVertex; x++ ) {
					m_normals[x] = normals[ x - startVertex ];
				}
			}
			if ( uvCoords != nullptr ) {
				for ( int x = startVertex; x <= endVertex; x++ ) {
					m_uv[x] = uvCoords[ x - startVertex ];
				}
			}

			passMeshToGPU();
		}

	}
}