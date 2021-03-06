
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Object.h"
#include "Mesh.h"
#include "Transform.h"
#include "Scene.h"
#include "rocket/Core/debug.h"

namespace Rocket {
	namespace Graphics {

		Object::Object( Mesh * mesh ) : Transform() {
			if ( mesh == nullptr ) {
				m_mesh = shared_ptr< Mesh >();
			} else {
				m_mesh = mesh->shared_from_this();
			}
			m_shaderUniforms = make_shared< ShaderUniforms >();

			disableTransparency();
		}
		Object::~Object() {
			if ( m_mesh.get() != nullptr ) {
				for ( auto owner : m_owners ) {
					m_mesh->removeMeshUserFromScene( this, owner.get() );
				}
			}
		}

		void Object::clonePropertiesOnto( Object * cloneOnto ) {
			cloneOnto->m_transparencyEnabled = m_transparencyEnabled;
			if ( m_mesh.get() != nullptr ) m_mesh->addMeshUser( cloneOnto );

			//cloneOnto->m_collisionRadius = m_collisionRadius;

			cloneOnto->m_shaderUniforms = m_shaderUniforms->clone()->shared_from_this();
		}

		//! Creates a copy of this Object, sharing the Mesh (not copied), and copying all Object properties (Transforms not included)
		shared_ptr< Object > Object::clone() {
			auto r = make_shared< Object >( m_mesh.get() );
			clonePropertiesOnto( r.get() );
			return r;
		}

		//! Make a clone of this Object and add it to the given Scene with an optional parent Tranform. Transform properties are copied from this Object.
		shared_ptr< Object > Object::cloneInScene( Scene * scene, Transform * parent, Core::vec3 scale, Core::vec4 rotation, Core::vec3 position ) {
			shared_ptr< Object > r = clone();

			r->scale( scale );
			r->rotate( rotation.w(), rotation.xyz() );
			r->position( position );

			scene->addObject( r.get(), parent );

			return r;
		}

		//! Calculate the Object's world matrices before drawing
		void Object::calculateTransforms( float elapsedMilliseconds, const Core::mat4 & parent_orientation, bool parentCacheIsClean, bool applyUpdates ) {
			Transform::calculateTransforms( elapsedMilliseconds, parent_orientation, parentCacheIsClean, applyUpdates );
		}

		//! Returns the Mesh that renders this Object
		Mesh * Object::getMesh() {
			return m_mesh.get();
		}

		//! Remove the given Scene from the list of Scene owners that render this Object
		void Object::removeOwner( Scene * scene ) {
			Transform::removeOwner( scene );
			if ( m_mesh.get() != nullptr ) m_mesh->removeMeshUserFromScene( this, scene );
		}

		//! Set this Object's Shader properties (uniforms)
		void Object::setShaderUniforms( ShaderUniforms * shaderUniforms ) {
			m_shaderUniforms = shaderUniforms->shared_from_this();
		}
		//! Returns this Object's current Shader properties
		ShaderUniforms * Object::getShaderUniforms() {
			return m_shaderUniforms.get();
		}

		//! Enables rendering of this Object with transparency
		void Object::enableTransparency() {
			m_transparencyEnabled = true;
			if ( m_mesh.get() != nullptr ) m_mesh->addMeshUser( this );
		}
		//! Disables rendering of this Object with transparency
		void Object::disableTransparency() {
			m_transparencyEnabled = false;
			if ( m_mesh.get() != nullptr ) m_mesh->addMeshUser( this );
		}
		//! Returns true if this Object is rendered with transparency enabled
		bool Object::isTransparent() {
			return m_transparencyEnabled;
		}

	}
}

