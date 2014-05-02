
#include <vector>

#include "rocket/Core/vector.h"
#include "rocket/Core/mathconstants.h"
#include "Mesh.h"
#include "Shader.h"
#include "Universe.h"

namespace Rocket {
	namespace Graphics {

		//! Create a triangle (3 vertices and perpendicular normals for each vertex)
		void triangle( const Core::vec4 & a, const Core::vec4 & b, const Core::vec4 & c, int * vertexCount, std::vector<Core::vec4> * verticesList, std::vector<Core::vec3> * normalsList ) {
			Core::vec3 normal = Core::normalize( Core::cross( (b-a).xyz(), (c-b).xyz() ) );

			normalsList->push_back( normal );	verticesList->push_back( a );
			normalsList->push_back( normal );	verticesList->push_back( b );
			normalsList->push_back( normal );	verticesList->push_back( c );
			(*vertexCount) += 3;
		}


		//! \relates Mesh
		//! Generate a quad Mesh in the given Universe, with the given Mesh name, rendered with the given Shader
		Mesh * generatePrimitive_Quad( Universe * world, const char * meshName, Shader * shader ) {
			Core::vec4 * vertices = new Core::vec4[6];
			Core::vec3 * normals = new Core::vec3[6];
			Core::vec2 * uv = new Core::vec2[6];

			vertices[0] = Core::vec4( 0.5, 0.5, 0.0, 1.0 );
			vertices[1] = Core::vec4(-0.5, 0.5, 0.0, 1.0 );
			vertices[2] = Core::vec4( 0.5,-0.5, 0.0, 1.0 );
			vertices[3] = vertices[1];
			vertices[4] = vertices[2];
			vertices[5] = Core::vec4(-0.5,-0.5, 0.0, 1.0 );

			normals[0] = Core::vec3( 0.0, 0.0, 1.0 );
			normals[1] = normals[0];
			normals[2] = normals[0];
			normals[3] = normals[0];
			normals[4] = normals[0];
			normals[5] = normals[0];

			uv[0] = Core::vec2( 1.0, 0.0 );
			uv[1] = Core::vec2( 0.0, 0.0 );
			uv[2] = Core::vec2( 1.0, 1.0 );
			uv[3] = uv[1];
			uv[4] = uv[2];
			uv[5] = Core::vec2( 0.0, 1.0 );

			auto newMesh = make_shared< Mesh >( shader, 6, vertices, normals, uv );
			world->addMesh( meshName, newMesh.get() );
			return newMesh.get();
		}


		//! \relates Mesh
		//! Generate a cube Mesh in the given Universe, with the given Mesh name, rendered with the given Shader
		Mesh * generatePrimitive_Cube( Universe * world, const char * meshName, Shader * shader ) {
			// Base cube points
			Core::vec4 seedPoints[8] = {
				Core::vec4( 0.5f, 0.5f, 0.5f, 1.0f ),
				Core::vec4(-0.5f, 0.5f, 0.5f, 1.0f ),
				Core::vec4( 0.5f,-0.5f, 0.5f, 1.0f ),
				Core::vec4( 0.5f, 0.5f,-0.5f, 1.0f ),
				Core::vec4(-0.5f,-0.5f, 0.5f, 1.0f ),
				Core::vec4( 0.5f,-0.5f,-0.5f, 1.0f ),
				Core::vec4(-0.5f, 0.5f,-0.5f, 1.0f ),
				Core::vec4(-0.5f,-0.5f,-0.5f, 1.0f )
			};

			int seedTriangles[12][3] = {
				{0,1,2},
				{1,4,2},
				{0,3,6},
				{3,0,5},
				{5,0,2},
				{6,1,0},
				{1,6,7},
				{7,4,1},
				{4,7,2},
				{2,7,5},
				{3,5,7},
				{3,7,6}
			};

			int * numVertices = new int(0);
			std::vector<Core::vec4> * verticesList = new std::vector<Core::vec4>;
			std::vector<Core::vec3> * normalsList = new std::vector<Core::vec3>;

			for (int i = 0; i < 12; i++) {
				triangle( seedPoints[seedTriangles[i][0]], seedPoints[seedTriangles[i][1]], seedPoints[seedTriangles[i][2]], numVertices, verticesList, normalsList );
			}

			// Create vertex and normals lists
			Core::vec4 * vertices = new Core::vec4[*numVertices];
			Core::vec3 * normals = new Core::vec3[*numVertices];

			for (int i = 0; i < *numVertices; i++) {
				vertices[i] = (*verticesList)[i];
				normals[i] = (*normalsList)[i];
			}

			delete verticesList;
			delete normalsList;
			int numverts = *numVertices;
			delete numVertices;

			// todo: add UV coords
			auto newMesh = make_shared< Mesh >( shader, numverts, vertices, normals, nullptr );
			world->addMesh( meshName, newMesh.get() );
			return newMesh.get();
		}


		Core::vec4 unit( const Core::vec4 & p ) {
			float len = p.xyz().lengthSquared();
			Core::vec4 t;
			if ( len > MathConstants::ZeroTolerance ) {
				len = sqrt( len );
				t.x( p.x() / len );
				t.y( p.y() / len );
				t.z( p.z() / len );
				t.w( 1.0 );
			}
			return t;
		}

		void divide_triangle( const Core::vec4 & a, const Core::vec4 & b, const Core::vec4 & c, int divisions, int * vertexCount, std::vector<Core::vec4> * verticesList, std::vector<Core::vec3> * normalsList ) {
			if ( divisions > 0 ) {
				Core::vec4 v1 = unit( a + b );
				Core::vec4 v2 = unit( a + c );
				Core::vec4 v3 = unit( b + c );
				divide_triangle( a, v1, v2, divisions - 1, vertexCount, verticesList, normalsList );
				divide_triangle( c, v2, v3, divisions - 1, vertexCount, verticesList, normalsList );
				divide_triangle( b, v3, v1, divisions - 1, vertexCount, verticesList, normalsList );
				divide_triangle( v1, v3, v2, divisions - 1, vertexCount, verticesList, normalsList );
			} else {
				triangle( a, b, c, vertexCount, verticesList, normalsList );
			}
		}

		//! \relates Mesh
		//! Generate a sphere Mesh by starting with a tetrahedron and dividing it divisions number of times.  It will be created in the given Universe, with the given Mesh name, and rendered with the given Shader.
		Mesh * generatePrimitive_Sphere( Universe * world, const char * meshName, int divisions, Shader * shader ) {
			if (divisions < 0) divisions = 0;

			int * numVertices = new int(0);
			std::vector<Core::vec4> * verticesList = new std::vector<Core::vec4>;
			std::vector<Core::vec3> * normalsList = new std::vector<Core::vec3>;

			// Base tetrahedron points
			Core::vec4 seedPoints[4] = {
				Core::vec4( 0.0f, 0.0f, 1.0f, 1.0f ),
				Core::vec4( 0.0f, 0.942809f, -0.333333f, 1.0f ),
				Core::vec4( -0.816497f, -0.471405f, -0.333333f, 1.0f ),
				Core::vec4( 0.816497f, -0.471405f, -0.333333f, 1.0f ),
			};

			divide_triangle(seedPoints[0], seedPoints[1], seedPoints[2], divisions, numVertices, verticesList, normalsList );
			divide_triangle(seedPoints[3], seedPoints[2], seedPoints[1], divisions, numVertices, verticesList, normalsList );
			divide_triangle(seedPoints[0], seedPoints[3], seedPoints[1], divisions, numVertices, verticesList, normalsList );
			divide_triangle(seedPoints[0], seedPoints[2], seedPoints[3], divisions, numVertices, verticesList, normalsList );

			// Create vertex and normals lists
			Core::vec4 * vertices = new Core::vec4[*numVertices];
			Core::vec3 * normals = new Core::vec3[*numVertices];
			Core::vec2 * uvs = new Core::vec2[*numVertices];

			for (int i = 0; i < *numVertices; i++) {
				vertices[i] = (*verticesList)[i];
				normals[i] = (*normalsList)[i];
				uvs[i] = Core::vec2( vertices[i].x(), vertices[i].y() );
			}

			delete verticesList;
			delete normalsList;
			int numverts = *numVertices;
			delete numVertices;

			auto newMesh = make_shared< Mesh >( shader, numverts, vertices, normals, uvs );
			world->addMesh( meshName, newMesh.get() );
			return newMesh.get();
		}

		//! Generate blended normals based on the center of a sphere
		void Mesh::generateSphericalNormals( Mesh * mesh ) {
			for (int i = 0; i < mesh->m_vertexCount; i++) {
				Core::vec4 normal = normalize( mesh->m_vertices[i] );
				mesh->m_normals[i] = Core::vec3( normal.x(), normal.y(), normal.z() );
			}
		}

	}
}

