
#ifndef Rocket_Graphics_Transform_H
#define Rocket_Graphics_Transform_H

#include <vector>
#include <list>
#include <utility>
#include <memory>

using namespace std;

#include "rocket/Core/matrix.h"

namespace Rocket {
	namespace Graphics {
		
		class Transform;
		
		typedef list< pair< weak_ptr< Transform >, int > > zIndexerType;

		class Scene;
		class Transform : public enable_shared_from_this< Transform > {
		public:
			Transform();
			virtual ~Transform();

			void addChild( Transform * child, bool coupleChildToParent );

			void scale( const Core::vec3 & scale );

			void position( const Core::vec3 & offset );
			const Core::vec3 & position();

			const Core::mat4 & getParentOrientation();
			const Core::mat4 & getFinalOrientation();
			//void positionWorld( Core::vec3 pos );	// todo: position in world space, relative to parent
			Core::vec3 positionWorld();

			// todo: rotate() around world space axes
			void rotate( const Core::vec4 & quaternion );
			void rotate( float angle, Core::vec3 axisOfRotation );
			void rotatePitch( float pitch );
			void rotateYaw( float yaw );
			void rotateRoll( float roll );
			const Core::vec4 & rotation_quaternion();
			Core::vec3 rotation_euler();		// todo: doesn't work (see vector.h)

			const Core::mat4 & orientation();	// scale + position + rotation

			void move( float distance );
			void move( const Core::vec3 & v );

			void hide();
			void show();
			bool isVisible();

			virtual void update( bool recursive, float elapsedMilliseconds );

			virtual void calculateTransforms( float elapsedMilliseconds, const Core::mat4 & parent_orientation, bool parentCacheIsClean, bool applyUpdates );

			// Optional z-indexing functions
			void zIndexer_Add( zIndexerType * zIndexer, int zIndexTag );
			zIndexerType::iterator zIndexer_AddBehind( Transform * relativeTo, int zIndexTag );
			zIndexerType::iterator zIndexer_AddInFront( Transform * relativeTo, int zIndexTag );

			virtual void addOwner( Scene * scene );
			virtual void removeOwner( Scene * scene );
			virtual vector< shared_ptr< Scene > > getOwners();

		protected:
			shared_ptr< Transform > m_parent;
			vector< shared_ptr< Scene > > m_owners;

			bool m_hidden;
			bool m_updated;

			vector< shared_ptr< Transform > > m_children;

			bool m_cache_orientationIsClean;		// true if this Transform's own orientation cache is up-to-date
			Core::mat4 m_cache_orientation;

			bool m_cache_descendantOrientationIsClean;		// true if this Tranform's children have up-to-date parent orientation
			Core::mat4 m_cache_parentOrientation;		// cache for the concatenation of orientation matrices of all parent nodes
			bool cleanParentOrientationCache();			// recalculates m_cache_descendantOrientation if needed. returns false if an update was required

			bool m_cache_finalOrientationIsClean;
			Core::mat4 m_cache_finalOrientation;		// m_cache_descendantOrientation * orientation()

			void setOrientationCacheAsDirty();
			//const Core::mat4 & getFinalOrientation();

			Core::vec3 m_scale;
			Core::vec3 m_position;
			Core::vec4 m_rotation;		// quaternion

			// Optional z-indexing members
			zIndexerType * m_zIndexer;						// The zIndexer that this Transform belongs to (optional; nullptr if it does not belong to any)
			zIndexerType::iterator m_zIndexer_myIterator;		// The iterator that points to this Transform in m_zIndexer

		private:
			virtual void updateBase( bool recursive, float elapsedMilliseconds ) final;		// recursively update all children (do NOT overload)
		};

	}
}


#endif
