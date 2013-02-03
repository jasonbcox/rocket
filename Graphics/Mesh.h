
#ifndef Rocket_Graphics_Mesh_H
#define Rocket_Graphics_Mesh_H

#include <unordered_map>
#include <map>
#include <vector>

#include "GL/glew.h"
#include "GL/glfw.h"

#include "vector.h"
#include "Shader.h"
#include "debug.h"

namespace Rocket {
	namespace Graphics {

		static const int MESH_VBO_NUM = 1;

		enum MeshDrawPasses {
			Opaque = 0,
			Transparent,
			END_OF_DRAW_PASSES
		};

		class Object;
		class Scene;
		class Universe;
		class Mesh {
		public:
			Mesh( Shader * shader, int numVertices, Core::vec4 * vertices = NULL, Core::vec3 * normals = NULL, Core::vec2 * uvCoords = NULL );
			Mesh( const char * OBJ_Wavefront_File, Shader * shader );
			virtual ~Mesh();

			void startPassesForScene( Scene * scene );
			void drawCurrentPass( const Core::mat4 * cameraProjection, const Core::mat4 * cameraOrientation );

			void addMeshUser( Object * object );
			std::unordered_map<Scene*, std::vector<std::vector<Object*>>>::iterator addSceneToUserList( Scene * scene );
			void removeMeshUserFromScene( Object * object, Scene * scene );

			GLuint getVertexArrayObject();
			Shader * getShader();
			int getVertexCount();

			void editMesh( int startVertex, int endVertex, Core::vec4 * vertices, Core::vec3 * normals = NULL, Core::vec2 * uvCoords = NULL );

			static void generateSphericalNormals( Mesh * mesh );

#ifdef ENABLE_DEBUG
			unsigned int m_frame_renderedPolygons;
			unsigned int m_frame_renderedObjects;
#endif

		private:
			GLuint m_vao;				// vertex array object
			GLuint m_vbo[MESH_VBO_NUM];	// vertex buffer object

			std::unordered_map<Scene*, std::vector<std::vector<Object*>>> m_objectUsers;
			std::vector<std::vector<Object*>>::iterator m_currentPassIterator;

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

		Mesh * generatePrimitive_Quad( Universe * world, const char * meshName, Shader * shader );

		Mesh * generatePrimitive_Cube( Universe * world, const char * meshName, Shader * shader );

		Mesh * generatePrimitive_Sphere( Universe * world, const char * meshName, int divisions, Shader * shader );

	}
}

#endif