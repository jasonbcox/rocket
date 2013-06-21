
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "Mesh.h"
#include "rocket/Core/vector.h"

#include "rocket/Core/system.h"
#ifdef OS_WINDOWS
#define strtok_r strtok_s
#endif

namespace Rocket {
	namespace Graphics {

		std::vector<std::string> &split( const std::string &s, char delim, std::vector<std::string> &elems ) {
			std::stringstream ss(s);
			std::string item;
			while(std::getline(ss, item, delim)) {
				elems.push_back(item);
			}
			return elems;
		}
		std::vector<std::string> split( const std::string &s, char delim ) {
			std::vector<std::string> elems;
			return split(s, delim, elems);
		}



		void Mesh::load_OBJ( const char * file ) {
			// Storage for raw data.
			std::vector<Core::vec4> vertices;
			std::vector<Core::vec3> vertex_normals;
			std::vector<Core::vec2> tex_coords;
			std::vector<std::vector<int>> faces;


			// Read lines from file.
			std::ifstream myfile( file );
			if (myfile.is_open()) {
				std::string line;
				while (myfile.good()) {
					char str[50];
					myfile.getline(str, 50);

					// Tokenize the file line.
					char * tok;
					char * next_tok;
					tok = strtok_r(str, " ", &next_tok);
	
					std::vector<std::string> curr_line;

					while (tok != NULL) {
						curr_line.push_back(std::string(tok));
						tok = strtok_r(NULL, " ", &next_tok);
					}

					if (curr_line.empty()) continue;

					// Check for data type.
					if (curr_line[0] == "v") {
						// Vertex
						vertices.push_back( Core::vec4( (float)atof(curr_line[1].c_str()), (float)atof(curr_line[2].c_str()), (float)atof(curr_line[3].c_str()), 1.0f ) );
					} else if(curr_line[0] == "vn") {
						// Vertex Normal
						vertex_normals.push_back( Core::vec3( (float)atof(curr_line[1].c_str()), (float)atof(curr_line[2].c_str()), (float)atof(curr_line[3].c_str()) ) );
					} else if(curr_line[0] == "vt") {
						// Texture Coordinate
						tex_coords.push_back( Core::vec2( (float)atof(curr_line[1].c_str()), (float)atof(curr_line[2].c_str()) ) );
					} else if(curr_line[0] == "f") {
						// Face
						for (unsigned int n = 1; n < curr_line.size(); n++) {
							std::vector<std::string> point = split( curr_line[n], '/' );
							std::vector<int> p;
							for (std::vector<std::string>::iterator it = point.begin(); it != point.end(); it++) {
								p.push_back(atoi(it->c_str()));
							}
							faces.push_back(p);
						}	
					} else {
						continue;
					}
				}

				myfile.close();
			} else {
				std::cerr << "Unable to open file.\n";
			}

	
			// Order the raw data into ordered vectors.
			std::vector<Core::vec4> vertices_ordered;
			std::vector<Core::vec3> vertex_normals_ordered;
			std::vector<Core::vec2> tex_coords_ordered;

			for (std::vector<std::vector<int>>::iterator it = faces.begin(); it != faces.end(); it++) {
				vertices_ordered.push_back(vertices[it->at(0) - 1]);
				tex_coords_ordered.push_back(tex_coords[it->at(1) - 1]);
				vertex_normals_ordered.push_back(vertex_normals[it->at(2) - 1]);
			}


			// Copy ordered data to dynamically allocated arrays.
			m_vertexCount = vertices_ordered.size();
			m_vertices = new Core::vec4[vertices_ordered.size()];
			m_normals = new Core::vec3[vertex_normals_ordered.size()];
			m_uv = new Core::vec2[tex_coords_ordered.size()];

			int vIndex = 0;
			for(std::vector<Core::vec4>::iterator it = vertices_ordered.begin(); it != vertices_ordered.end(); it++) {
				m_vertices[vIndex] = *it;
				vIndex++;
			}
			int vnIndex = 0;
			for(std::vector<Core::vec3>::iterator it = vertex_normals_ordered.begin(); it != vertex_normals_ordered.end(); it++) {
				m_normals[vnIndex] = *it;
				vnIndex++;
			}
			int uvIndex = 0;
			for(std::vector<Core::vec2>::iterator it = tex_coords_ordered.begin(); it != tex_coords_ordered.end(); it++) {
				m_uv[uvIndex] = *it;
				uvIndex++;
			}
		}

	}
}
