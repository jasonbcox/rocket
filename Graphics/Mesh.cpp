
#include "GL/glew.h"
#include "GL/glfw.h"

#include "vector.h"
#include "debug.h"
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
			m_shader = shader;

			m_vertexCount = numVertices;
			m_vertices = vertices;
			m_normals = normals;
			m_uv = uvCoords;

			if (m_uv == NULL) {
				m_uv = new Core::vec2[m_vertexCount];
			}

			generateBufferObjects();

			passMeshToGPU();
		}
		Mesh::Mesh( const char * OBJ_Wavefront_File, Shader * shader ) {
			m_shader = shader;

			load_OBJ( OBJ_Wavefront_File );

			generateBufferObjects();

			passMeshToGPU();
		}
		Mesh::~Mesh() {
			// copy and then clear m_meshUsers before deleting the objects because the Object constructor will attempt to remove that element from m_meshUsers
			/*std::set<Object*> meshOpaqueList = m_opaqueMeshUsers;
			m_opaqueMeshUsers.clear();
			std::set<Object*>::iterator obj;
			for (obj = meshOpaqueList.begin(); obj != meshOpaqueList.end(); obj++) {
				delete (*obj);
			}

			std::set<Object*> meshTransparentList = m_transparentMeshUsers;
			m_transparentMeshUsers.clear();
			for (obj = meshTransparentList.begin(); obj != meshTransparentList.end(); obj++) {
				delete (*obj);
			}*/
			while( m_opaqueMeshUsers.size() > 0 ) {
				std::set<Object*>::iterator iter = m_opaqueMeshUsers.begin();
				Object * user = (*iter);
				m_opaqueMeshUsers.erase( iter );
				user->setMesh( NULL );	// prevent the object from attempt to remove itself from this mesh's user lists
				//delete user;
			}
			while( m_transparentMeshUsers.size() > 0 ) {
				std::set<Object*>::iterator iter = m_transparentMeshUsers.begin();
				Object * user = (*iter);
				m_transparentMeshUsers.erase( iter );
				user->setMesh( NULL );	// prevent the object from attempt to remove itself from this mesh's user lists
				//delete user;
			}

			glDeleteVertexArrays( 1, &m_vao );
			glDeleteBuffers( MESH_VBO_NUM, m_vbo );

			Core::Debug_Scramble( m_vertices, m_vertexCount );
			Core::Debug_Scramble( m_normals, m_vertexCount );
			Core::Debug_Scramble( m_uv, m_vertexCount );
			delete [] m_vertices;
			delete [] m_normals;
			delete [] m_uv;
		}

		void Mesh::addMeshUser( Object * object ) {
			if ( object->isTransparent() == true ) {
				m_opaqueMeshUsers.erase( object );
				m_transparentMeshUsers.insert( object );
			} else {
				m_transparentMeshUsers.erase( object );
				m_opaqueMeshUsers.insert( object );
			}
		}

		void Mesh::removeMeshUser( Object * object ) {
			m_opaqueMeshUsers.erase( object );
			m_transparentMeshUsers.erase( object );
		}

		void Mesh::drawOpaqueMeshUsers() {
			std::set<Object*>::iterator obj;
			for (obj = m_opaqueMeshUsers.begin(); obj != m_opaqueMeshUsers.end(); obj++) {
				(*obj)->draw();
			}
		}

		void Mesh::drawTransparentMeshUsers() {
			std::set<Object*>::iterator obj;
			for (obj = m_transparentMeshUsers.begin(); obj != m_transparentMeshUsers.end(); obj++) {
				(*obj)->draw();
			}
		}

		int Mesh::getOpaqueMeshUserCount() {
			return m_opaqueMeshUsers.size();
		}
		int Mesh::getTransparentMeshUserCount() {
			return m_transparentMeshUsers.size();
		}

		void Mesh::passMeshToGPU() {
			if ( m_shader == NULL ) { Rocket::Core::Debug_AddToLog( "Error: Mesh must be linked to a shader." ); system( "pause" ); exit( 1 ); }
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
			return m_shader;
		}

		int Mesh::getVertexCount() {
			return m_vertexCount;
		}

		void Mesh::editMesh( int startVertex, int endVertex, Core::vec4 * vertices, Core::vec3 * normals, Core::vec2 * uvCoords ) {
			if ( ( startVertex < 0 ) || ( startVertex > endVertex ) || ( endVertex >= m_vertexCount ) ) return;
			for ( int x = startVertex; x <= endVertex; x++ ) {
				m_vertices[x] = vertices[ x - startVertex ];
			}
			if ( normals != NULL ) {
				for ( int x = startVertex; x <= endVertex; x++ ) {
					m_normals[x] = normals[ x - startVertex ];
				}
			}
			if ( uvCoords != NULL ) {
				for ( int x = startVertex; x <= endVertex; x++ ) {
					m_uv[x] = uvCoords[ x - startVertex ];
				}
			}

			passMeshToGPU();
		}

	}
}