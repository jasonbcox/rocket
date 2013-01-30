
#include "GL/glew.h"
#include "GL/glfw.h"

#include <vector>

#include "Object.h"
#include "Mesh.h"
#include "Transform.h"
#include "Scene.h"
#include "debug.h"

namespace Rocket {
	namespace Graphics {

		Object::Object( Mesh * mesh ) : Transform() {
			m_mesh = mesh;
			m_shaderUniforms = NULL;

			disableTransparency();
		}

		Object::~Object() {
			if ( m_mesh != NULL ) m_mesh->removeMeshUser( this );
		}


		void Object::clonePropertiesOnto( Object * cloneOnto ) {
			cloneOnto->m_transparencyEnabled = m_transparencyEnabled;
			if ( m_mesh != NULL ) m_mesh->addMeshUser( cloneOnto );

			//cloneOnto->m_collisionRadius = m_collisionRadius;

			cloneOnto->m_shaderUniforms = m_shaderUniforms->clone();
		}

		// Make a copy of this object, but instance the same mesh
		Object * Object::clone() {
			Object * r = new Object( m_mesh );
			clonePropertiesOnto( r );
			return r;
		}

		Object * Object::cloneInScene( Scene * scene, Transform * parent, Core::vec3 scale, Core::vec4 rotation, Core::vec3 position ) {
			Object * r = clone();

			r->scale( scale );
			r->rotate( rotation.w, Core::vec3( rotation.x, rotation.y, rotation.z ) );
			r->position( position );

			scene->addObject( r, parent );

			return r;
		}

		void Object::draw() {
			if ( m_hidden == false ) {
				if ( m_mesh != NULL ) {
					Shader * shader = m_mesh->getShader();

					// Orientation of the object
					*shader->getObjecTransform() = &getFinalOrientation();

					shader->passUniformDataToGPU( m_shaderUniforms );

					int vertexCount = m_mesh->getVertexCount();
					glDrawArrays( GL_TRIANGLES, 0, vertexCount );

#ifdef ENABLE_DEBUG
					m_mesh->m_frame_renderedPolygons += vertexCount/3;
#endif
				}
			}
		}

		void Object::calculateTransforms( float elapsedMilliseconds, const Core::mat4 & parent_orientation, bool parentCacheIsClean, bool applyUpdates ) {
			Transform::calculateTransforms( elapsedMilliseconds, parent_orientation, parentCacheIsClean, applyUpdates );
		}

		void Object::setMesh( Mesh * mesh ) {
			m_mesh = mesh;
		}

		Mesh * Object::getMesh() {
			return m_mesh;
		}

		void Object::setShaderUniforms( ShaderUniforms * shaderUniforms ) {
			m_shaderUniforms = shaderUniforms;
		}
		ShaderUniforms * Object::getShaderUniforms() {
			return m_shaderUniforms;
		}

		void Object::enableTransparency() {
			m_transparencyEnabled = true;
			if ( m_mesh != NULL ) m_mesh->addMeshUser( this );
		}
		void Object::disableTransparency() {
			m_transparencyEnabled = false;
			if ( m_mesh != NULL ) m_mesh->addMeshUser( this );
		}
		bool Object::isTransparent() {
			return m_transparencyEnabled;
		}

	}
}