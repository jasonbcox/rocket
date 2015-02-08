
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Transform.h"
#include "Scene.h"
#include "rocket/Core/matrix.h"
#include "rocket/Core/debug.h"
#include <sstream>

namespace Rocket {
	namespace Graphics {

		Transform::Transform() {
			m_parent = nullptr;

			m_zIndexer = nullptr;

			scale( Core::vec3(1,1,1) );
			position( Core::vec3(0,0,0) );
			m_rotation = Core::Quaternion( 0.0f, Core::vec3(0,1,0) );

			m_hidden = false;
			m_updated = false;

			setOrientationCacheAsDirty();
		}
		Transform::~Transform() {
			// Remove self from parent
			if ( m_parent != nullptr ) {
				for ( auto child_iter = m_parent->m_children.begin(); child_iter != m_parent->m_children.end(); child_iter++ ) {
					if ( child_iter->get() == this ) {
						m_parent->m_children.erase( child_iter );
						break;
					}
				}
			}
		}

		/*! Adds a Transform to this Tranform's list of children
			If coupleChildToParent is true, child's m_parent is set to this Transform, otherwise it is not set.
			For adding children to a Scene, always set coupleChildToParent as false.
		*/
		void Transform::addChild( Transform * child, bool coupleChildToParent ) {
			m_children.push_back( child->shared_from_this() );
			for ( auto scene : getOwners() ) {
				child->addOwner( scene.get() );
			}

			// todo: since coupleChildToParent is almost always only false with Scene parents, maybe just
			//			add a member to Transform with a default value for coupleChildToParent (false for Scenes, true for everthing else)
			if ( coupleChildToParent == true ) child->m_parent = this->shared_from_this();
		}

		//! Set this Transform's scale
		void Transform::scale( const Core::vec3 & scale ) {
			m_scale = scale;
			setOrientationCacheAsDirty();
		}

		//! Set this Transform's local position
		void Transform::position( const Core::vec3 & offset ) {
			m_position = offset;
			setOrientationCacheAsDirty();
		}
		//! Return this Transform's local position
		const Core::vec3 & Transform::position() {
			return m_position;
		}

		//! Returns true if this Transform's parent's cache is clean
		bool Transform::cleanParentOrientationCache() {
			if ( m_parent == nullptr ) {
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
		//! Returns the concatenation of all of this Transform's ancestors' matrices
		const Core::mat4 & Transform::getParentOrientation() {
			cleanParentOrientationCache();
			return m_cache_parentOrientation;
		}

		//! Rotate this Transform using a quaternion (local rotation)
		void Transform::rotate( const Core::vec4 & quaternion ) {
			m_rotation = Core::MatrixToQuaternion( QuaternionRotate( quaternion ) * Core::QuaternionRotate( m_rotation ) );
			setOrientationCacheAsDirty();
		}
		//! Rotate this Transform by the specified angle around the given axis (local rotation)
		void Transform::rotate( float angle, Core::vec3 axisOfRotation ) {
			if ( (axisOfRotation.x() == 0.0f) && (axisOfRotation.y() == 0.0f) && (axisOfRotation.z() == 0.0f) ) {
				axisOfRotation.y( 1.0f );
			}
			m_rotation = Core::MatrixToQuaternion( Rotate( angle, axisOfRotation ) * Core::QuaternionRotate( m_rotation ) );
			setOrientationCacheAsDirty();
		}
		//! Pitch-rotate (X-Axis) this Transform by the given angle (local rotation)
		void Transform::rotatePitch( float pitch ) {
			rotate( pitch, Core::vec3(1,0,0) );
		}
		//! Yaw-rotate (Y-Axis) this Transform by the given angle (local rotation)
		void Transform::rotateYaw( float yaw ) {
			rotate( yaw, Core::vec3(0,1,0) );
		}
		//! Roll-rotate (Z-Axis) this Transform by the given angle (local rotation)
		void Transform::rotateRoll( float roll ) {
			rotate( roll, Core::vec3(0,0,1) );
		}
		//! Returns this Transform's local Euler rotation
		Core::vec3 Transform::rotation_euler() {
			return QuaternionToEuler( m_rotation );
		}
		//! Returns this Transform's local rotation as a quaternion
		const Core::vec4 & Transform::rotation_quaternion() {
			return m_rotation;
		}

		//! Returns this Transform's orientation matrix
		const Core::mat4 & Transform::orientation() {
			if ( m_cache_orientationIsClean == false ) {
				m_cache_orientation = Translate( m_position ) * QuaternionRotate( m_rotation ) * Scale( m_scale );
				m_cache_orientationIsClean = true;
			}
			return m_cache_orientation;
		}

		//! Move this Transform forward by the given distance
		void Transform::move( float distance ) {
			Core::vec4 pos = QuaternionRotate( m_rotation ) * Core::vec4( 0.0f, 0.0f, distance, 0.0f );
			position( pos.xyz() + position() );
		}

		//! Move this Transform by the given vector
		void Transform::move( const Core::vec3 & v ) {
			// todo: to move in world-coordinate space, you need to take into account the parent's matrices
			position( v + position() );
		}

		//! Set this Transform to hidden (neither it nor its children are rendered)
		void Transform::hide() {
			m_hidden = true;
		}
		//! Set this Transform to visible
		void Transform::show() {
			m_hidden = false;
		}
		//! Returns true if this Transform is visible
		bool Transform::isVisible() {
			return !m_hidden;
		}

		//! Updates this Transform's rendering with the given elapsed time since the last rendering operation
		void Transform::updateBase( bool recursive, float elapsedMilliseconds ) {
			if ( recursive == true ) {
				for ( auto child : m_children ) {
					child->update( true, elapsedMilliseconds );
				}
			}
			m_updated = true;
		}
		//! Shell function of updateBase() so that update() can be virtual and overriden while updateBase() gets called by any version of update()
		void Transform::update( bool recursive, float elapsedMilliseconds ) {
			// update() must always call updateBase() in order to process recursion properly
			updateBase( recursive, elapsedMilliseconds );
		}

		//! Calculate and cache this Transforms matrices
		void Transform::calculateTransforms( float elapsedMilliseconds, const Core::mat4 & parent_orientation, bool parentCacheIsClean, bool applyUpdates ) {
			// don't save parent matrix if the cache is clean!
			bool nextParentCacheIsClean = true;
			if ( parentCacheIsClean == false ) {
				m_cache_parentOrientation = parent_orientation;
				m_cache_finalOrientationIsClean = false;
			}

			if ( applyUpdates == true ) {
				if ( m_updated == false ) update( false, elapsedMilliseconds );
			}

			if ( m_cache_descendantOrientationIsClean == false ) {
				m_cache_descendantOrientationIsClean = true;
				nextParentCacheIsClean = false;
			}

			for ( auto child : m_children ) {
				child->calculateTransforms( elapsedMilliseconds, getFinalOrientation(), nextParentCacheIsClean, applyUpdates );
			}

			if ( applyUpdates == true ) {
				m_updated = false;
			}
		}

		//! Sets this Transform's orientation cache as dirty
		void Transform::setOrientationCacheAsDirty() {
			m_cache_orientationIsClean = false;
			m_cache_descendantOrientationIsClean = false;
			m_cache_finalOrientationIsClean = false;
		}

		//! Return this Transform's orientation concatenated with all ancestors' matrices
		const Core::mat4 & Transform::getFinalOrientation() {
			if ( m_cache_finalOrientationIsClean == false ) {
				m_cache_finalOrientation = m_cache_parentOrientation * orientation();
				m_cache_finalOrientationIsClean = true;
			}
			return m_cache_finalOrientation;
		}

		// Z-Indexing Functions
		//! Adds this Transform to a zIndexer at the given location
		void Transform::zIndexer_Add( zIndexerType * zIndexer, int zIndexTag ) {
			m_zIndexer = zIndexer;

			// Find the nearest elements to zIndexTag
			zIndexerType::iterator highTag, lowTag, iter;
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
				m_zIndexer_myIterator = zIndexer_AddInFront( lowTag->first.lock().get(), zIndexTag );
			} else if ( highTag != zIndexer->end() ) {
				m_zIndexer_myIterator = zIndexer_AddBehind( highTag->first.lock().get(), zIndexTag );
			} else {
				// No tags exist, so add this to the very front
				zIndexer->push_front( std::pair< weak_ptr< Transform >, int >( this->shared_from_this(), zIndexTag ) );
				m_zIndexer_myIterator = zIndexer->begin();

				iter = m_zIndexer_myIterator;
				iter++;
				Core::vec3 pos = position();
				pos.z( iter->first.lock().get()->position().z() + 1.0f );

				// Make sure it fits within the viewspace
				// If this code is ever hit, you're doing something wrong (try z-indexing your transforms better!)
				if ( pos.z() > OrthographicDrawDistance ) pos.z( OrthographicDrawDistance );

				position( pos );
			}
		}
		//! Adds this Transform behind the given Transform (in the same zIndexer)
		zIndexerType::iterator Transform::zIndexer_AddBehind( Transform * relativeTo, int zIndexTag ) {
			m_zIndexer = relativeTo->m_zIndexer;
			zIndexerType::iterator iter = relativeTo->m_zIndexer_myIterator;
			float highZ = iter->first.lock().get()->position().z();
			iter++;
			float lowZ = iter->first.lock().get()->position().z();
			Core::vec3 pos = position();
			pos.z( ( highZ - lowZ ) / 2.0f );
			position( pos );
			return m_zIndexer->insert( iter, std::pair< weak_ptr< Transform >, int >( this->shared_from_this(), zIndexTag ) );
		}
		//! Adds this Transform in front of the given Transform (in the same zIndexer)
		zIndexerType::iterator Transform::zIndexer_AddInFront( Transform * relativeTo, int zIndexTag ) {
			m_zIndexer = relativeTo->m_zIndexer;
			zIndexerType::iterator iter = relativeTo->m_zIndexer_myIterator;
			float lowZ = iter->first.lock().get()->position().z();
			iter--;
			float highZ = iter->first.lock().get()->position().z();
			Core::vec3 pos = position();
			pos.z( ( highZ - lowZ ) / 2.0f );
			position( pos );
			return m_zIndexer->insert( relativeTo->m_zIndexer_myIterator, std::pair< weak_ptr< Transform >, int >( this->shared_from_this(), zIndexTag ) );
		}

		//! Add a Scene to the list of owners that contain this Transform
		void Transform::addOwner( Scene * scene ) {
			for ( auto owner : getOwners() ) {
				if ( owner.get() == scene ) return;
			}
			m_owners.push_back( static_pointer_cast< Scene >( scene->shared_from_this() ) );

			// Add owner for children too
			for ( auto child : m_children ) {
				child->addOwner( scene );
			}
		}
		//! Remove a Scene from the list of owners
		void Transform::removeOwner( Scene * scene ) {
			for ( auto owner_iter = m_owners.begin(); owner_iter != m_owners.end(); owner_iter++ ) {
				if ( owner_iter->get() == scene ) {
					m_owners.erase( owner_iter );
					return;
				}
			}
		}
		//! Returns the list of all Scene owners of this Transform
		std::vector< shared_ptr< Scene > > Transform::getOwners() {
			return m_owners;
		}

	}
}

