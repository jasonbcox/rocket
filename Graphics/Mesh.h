
#ifndef Rocket_Graphics_Mesh_H
#define Rocket_Graphics_Mesh_H

#include <set>

#include "GL/glew.h"
#include "GL/glfw.h"

#include "vector.h"
#include "Shader.h"
#include "debug.h"

namespace Rocket {
	namespace Graphics {

		static const int MESH_VBO_NUM = 1;

		class Object;
		class Scene;
		class Mesh {
		public:
			Mesh( Shader * shader, int numVertices, Core::vec4 * vertices = NULL, Core::vec3 * normals = NULL, Core::vec2 * uvCoords = NULL );
			Mesh( const char * OBJ_Wavefront_File, Shader * shader );
			~Mesh();

			void addMeshUser( Object * object );					// todo: add key-value map with Scene* keys and Object* values so that meshes can handle multiple scenes
			void removeMeshUser( Object * object );

			void drawOpaqueMeshUsers();
			void drawTransparentMeshUsers();
			int getOpaqueMeshUserCount();
			int getTransparentMeshUserCount();

			GLuint getVertexArrayObject();
			Shader * getShader();
			int getVertexCount();

			void editMesh( int startVertex, int endVertex, Core::vec4 * vertices, Core::vec3 * normals = NULL, Core::vec2 * uvCoords = NULL );

			static void generateSphericalNormals( Mesh * mesh );

#ifdef ENABLE_DEBUG
			int m_frame_renderedPolygons;
#endif

		private:
			GLuint m_vao;				// vertex array object
			GLuint m_vbo[MESH_VBO_NUM];	// vertex buffer object

			std::set<Object*> m_opaqueMeshUsers;
			std::set<Object*> m_transparentMeshUsers;

			void Mesh::generateBufferObjects();

			// A shader is the property of the mesh instead of the object because the mesh needs to pass information to a specific shader program
			Shader * m_shader;

			int m_vertexCount;
			Core::vec4 * m_vertices;
			Core::vec3 * m_normals;
			Core::vec2 * m_uv;

			// Load a .obj file
			void load_OBJ( const char * file );

			// Transfer this mesh's model information to the GPU
			// Only do this after loading the mesh, or after the mesh is modified by the CPU
			void passMeshToGPU();
		};

		Mesh * generatePrimitive_Quad( Shader * shader );

		Mesh * generatePrimitive_Cube( Shader * shader );

		Mesh * generatePrimitive_Sphere( int divisions, Shader * shader );

	}
}

#endif