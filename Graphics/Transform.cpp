
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Transform.h"
#include "Scene.h"
#include "rocket/Core/matrix.h"
#include "rocket/Core/debug.h"
#include "rocket/Core/string_.h"
#include <sstream>

namespace Rocket {
	namespace Graphics {

		Transform::Transform() {
			m_parent = NULL;

			m_zIndexer = NULL;

			scale( Core::vec3(1,1,1) );
			position( Core::vec3(0,0,0) );
			m_rotation = Core::Quaternion( 0.0f, Core::vec3(0,1,0) );

			m_hidden = false;
			m_updated = false;

			setOrientationCacheAsDirty();
		}
		Transform::~Transform() {
			// Delete all child transforms
			std::vector< Transform* >::iterator child;
			for ( child = m_children.begin(); child != m_children.end(); child++ ) {
				(*child)->m_parent = NULL;	// The destructor on this Transform (the parent of the child) is already being destroyed, so do NOT attempt to remove
											//		the child's self from this Transform (the parent)
				delete (*child);
			}
			m_children.clear();

			// Remove self from parent
			if ( m_parent != NULL ) {
				for (child = m_parent->m_children.begin(); child != m_parent->m_children.end(); child++) {
					if ( (*child) == this ) {
						m_parent->m_children.erase( child );
						break;
					}
				}
			}

			m_owners.clear();
		}

		void Transform::addChild( Transform * child, bool coupleChildToParent ) {
			m_children.push_back( child );
			for ( unsigned int scene = 0; scene < m_owners.size(); scene++ ) {
				child->addOwner( m_owners[ scene ] );
			}

			// todo: since coupleChildToParent is almost always only false with Scene parents, maybe just
			//			add a member to Transform with a default value for coupleChildToParent (false for Scenes, true for everthing else)
			if ( coupleChildToParent == true ) child->m_parent = this;
		}

		void Transform::scale( const Core::vec3 & scale ) {
			m_scale = scale;
			setOrientationCacheAsDirty();
		}

		void Transform::position( const Core::vec3 & offset ) {
			m_position = offset;
			setOrientationCacheAsDirty();
		}
		const Core::vec3 & Transform::position() {
			return m_position;
		}

		bool Transform::cleanParentOrientationCache() {
			if ( m_parent == NULL ) {
				if ( m_cache_descendantOrientationIsClean == false ) {
					calculateTransforms( 0.0f, Core::mat4(), false, false );
					return false;
				}
				return true;
			} else {
				bool clean = m_parent->cleanParentOrientationCache();
				if ( ( clean == true ) && ( m_cache_descendantOrientationIsClean == false ) ) {
					calculateTransforms( 0.0f, m_cache_parentOrientation, false, false );
					return false;
				}
				// if the parent's cache was not clean, the updates have already been made
				return clean;
			}
		}
		const Core::mat4 & Transform::getParentOrientation() {
			cleanParentOrientationCache();
			return m_cache_parentOrientation;
		}
		/*void Transform::positionWorld( Core::vec3 pos ) {
			// todo: position in world space, relative to parent
			//m_position = pos;
		}*/
		Core::vec3 Transform::positionWorld() {
			Core::mat4 orient = getParentOrientation() * orientation();
			return Core::vec3( orient[0][3], orient[1][3], orient[2][3] );
		}

		void Transform::rotate( const Core::vec4 & quaternion ) {
			m_rotation = Core::MatrixToQuaternion( QuaternionRotate( quaternion ) * Core::QuaternionRotate( m_rotation ) );
			setOrientationCacheAsDirty();
		}
		void Transform::rotate( float angle, Core::vec3 axisOfRotation ) {
			if ( (axisOfRotation.x == 0.0f) && (axisOfRotation.y == 0.0f) && (axisOfRotation.z == 0.0f) ) {
				axisOfRotation.y = 1.0f;
			}
			m_rotation = Core::MatrixToQuaternion( Rotate( angle, axisOfRotation ) * Core::QuaternionRotate( m_rotation ) );
			setOrientationCacheAsDirty();
		}
		void Transform::rotatePitch( float pitch ) {
			rotate( pitch, Core::vec3(1,0,0) );
		}
		void Transform::rotateYaw( float yaw ) {
			rotate( yaw, Core::vec3(0,1,0) );
		}
		void Transform::rotateRoll( float roll ) {
			rotate( roll, Core::vec3(0,0,1) );
		}
		Core::vec3 Transform::rotation_euler() {
			return QuaternionToEuler( m_rotation );
		}
		const Core::vec4 & Transform::rotation_quaternion() {
			return m_rotation;
		}

		const Core::mat4 & Transform::orientation() {
			if ( m_cache_orientationIsClean == false ) {
				m_cache_orientation = Translate( m_position ) * QuaternionRotate( m_rotation ) * Scale( m_scale );
				m_cache_orientationIsClean = true;
			}
			return m_cache_orientation;
		}

		void Transform::move( float distance ) {
			Core::vec4 pos = QuaternionRotate( m_rotation ) * Core::vec4( 0.0f, 0.0f, distance, 0.0f );
			position( pos.xyz() + position() );
		}

		void Transform::move( const Core::vec3 & v ) {
			// todo: to move in world-coordinate space, you need to take into account the parent's matrices
			position( v + position() );
		}

		void Transform::hide() {
			m_hidden = true;
		}
		void Transform::show() {
			m_hidden = false;
		}
		bool Transform::isVisible() {
			return !m_hidden;
		}

		void Transform::updateBase( bool recursive, float elapsedMilliseconds ) {
			if ( recursive == true ) {
				std::vector< Transform* >::iterator child;
				for(child = m_children.begin(); child != m_children.end(); child++) {
					(*child)->update( true, elapsedMilliseconds );
				}
			}
			m_updated = true;
		}
		void Transform::update( bool recursive, float elapsedMilliseconds ) {
			// update() must always call updateBase() in order to process recursion properly
			updateBase( recursive, elapsedMilliseconds );
		}

		void Transform::calculateTransforms( float elapsedMilliseconds, const Core::mat4 & parent_orientation, bool parentCacheIsClean, bool applyUpdates ) {
			// don't save parent matrix if the cache is clean!
			bool nextParentCacheIsClean = true;
			if ( parentCacheIsClean == false ) {
				m_cache_parentOrientation = parent_orientation;
			}
			if ( m_cache_descendantOrientationIsClean == false ) {
				m_cache_descendantOrientationIsClean = true;
				nextParentCacheIsClean = false;
			}

			if ( applyUpdates == true ) {
				if ( m_updated == false ) update( false, elapsedMilliseconds );
			}

			std::vector< Transform* >::iterator child;
			for ( child = m_children.begin(); child != m_children.end(); child++ ) {
				(*child)->calculateTransforms( elapsedMilliseconds, getFinalOrientation(), nextParentCacheIsClean, applyUpdates );
			}

			if ( applyUpdates == true ) {
				m_updated = false;
			}
		}

		void Transform::setOrientationCacheAsDirty() {
			m_cache_orientationIsClean = false;
			m_cache_descendantOrientationIsClean = false;
			m_frame_final_orientation_cache = false;
		}

		const Core::mat4 & Transform::getFinalOrientation() {
			if ( m_frame_final_orientation_cache == false ) {
				m_frame_final_orientation = m_cache_parentOrientation * orientation();
				m_frame_final_orientation_cache = true;
			}
			return m_frame_final_orientation;
		}

		// Z-Indexing Functions
		void Transform::zIndexer_Add( std::list< std::pair< Transform*, int > > * zIndexer, int zIndexTag ) {
			m_zIndexer = zIndexer;

			// Find the nearest elements to zIndexTag
			std::list<std::pair< Transform*, int >>::iterator highTag, lowTag, iter;
			highTag = zIndexer->end();
			lowTag = zIndexer->end();
			for ( iter = zIndexer->begin(); iter != zIndexer->end(); iter++ ) {
				int iterTag = iter->second;
				if ( iterTag != NoZIndexTag ) {
					if ( iterTag > zIndexTag ) {
						highTag = iter;
					} else {
						lowTag = iter;
						break;
					}
				}
			}

			if ( lowTag != zIndexer->end() ) {
				m_zIndexer_myIterator = zIndexer_AddInFront( lowTag->first, zIndexTag );
			} else if ( highTag != zIndexer->end() ) {
				m_zIndexer_myIterator = zIndexer_AddBehind( highTag->first, zIndexTag );
			} else {
				// No tags exist, so add this to the very front
				zIndexer->push_front( std::pair< Transform*, int >( this, zIndexTag ) );
				m_zIndexer_myIterator = zIndexer->begin();

				iter = m_zIndexer_myIterator;
				iter++;
				Core::vec3 pos = position();
				pos.z = iter->first->position().z + 1.0f;

				// Make sure it fits within the viewspace
				// If this code is ever hit, you're doing something wrong (try z-indexing your transforms better!)
				if ( pos.z > OrthographicDrawDistance ) pos.z = OrthographicDrawDistance;

				position( pos );
			}
		}
		std::list< std::pair< Transform*, int > >::iterator Transform::zIndexer_AddBehind( Transform * relativeTo, int zIndexTag ) {
			m_zIndexer = relativeTo->m_zIndexer;
			std::list< std::pair< Transform*, int > >::iterator iter = relativeTo->m_zIndexer_myIterator;
			float highZ = iter->first->position().z;
			iter++;
			float lowZ = iter->first->position().z;
			Core::vec3 pos = position();
			pos.z = ( highZ - lowZ ) / 2.0f;
			position( pos );
			return m_zIndexer->insert( iter, std::pair< Transform*, int >( this, zIndexTag ) );
		}
		std::list< std::pair< Transform*, int > >::iterator Transform::zIndexer_AddInFront( Transform * relativeTo, int zIndexTag ) {
			m_zIndexer = relativeTo->m_zIndexer;
			std::list<std::pair< Transform*, int >>::iterator iter = relativeTo->m_zIndexer_myIterator;
			float lowZ = iter->first->position().z;
			iter--;
			float highZ = iter->first->position().z;
			Core::vec3 pos = position();
			pos.z = ( highZ - lowZ ) / 2.0f;
			position( pos );
			return m_zIndexer->insert( relativeTo->m_zIndexer_myIterator, std::pair< Transform*, int >( this, zIndexTag ) );
		}

		// Add a scene to the list of owners that contain this object
		void Transform::addOwner( Scene * scene ) {
			std::vector< Scene* >::iterator iter;
			for ( iter = m_owners.begin(); iter != m_owners.end(); iter++ ) {
				if ( (*iter) == scene ) return;
			}
			m_owners.push_back( scene );

			// Add owner for children too
			std::vector< Transform* >::iterator child;
			for ( child = m_children.begin(); child != m_children.end(); child++ ) {
				(*child)->addOwner( scene );
			}
		}
		// Remove a scene from the list of owners
		void Transform::removeOwner( Scene * scene ) {
			std::vector< Scene* >::iterator iter;
			for ( iter = m_owners.begin(); iter != m_owners.end(); iter++ ) {
				if ( (*iter) == scene ) {
					m_owners.erase( iter );
					return;
				}
			}
		}
		// Return a list of all scene owners of this object
		std::vector< Scene* > Transform::getOwners() {
			return m_owners;
		}

	}
}