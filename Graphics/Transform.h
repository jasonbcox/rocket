
#ifndef Rocket_Graphics_Transform_H
#define Rocket_Graphics_Transform_H

#include <vector>
#include <list>
#include <utility>

#include "matrix.h"

namespace Rocket {
	namespace Graphics {

		class Transform;
		class Scene;
		class Transform {
		public:
			Transform();
			~Transform();

			// addChild() adds child to this Transform's list of children
			// if coupleChildToParent is true, child's m_parent is set to this Transform, otherwise, it is not set.
			// For adding children to a Scene, always set coupleChildToParent as false.
			void addChild( Transform * child, bool coupleChildToParent );

			void scale( const Core::vec3 & scale );			// set the scale

			void position( const Core::vec3 & offset );		// set the local position
			const Core::vec3 & position();					// get the local position

			// getParentOrientation() returns the concatenation of all ancestor matrices
			const Core::mat4 & getParentOrientation();
			//void positionWorld( Core::vec3 pos );	// todo: position in world space, relative to parent
			Core::vec3 positionWorld();		// get position in world space (with respect to root parent node)

			// todo: rotate() around world space axes
			void rotate( const Core::vec4 & quaternion );					// transform the current local rotation by a quaternion
			void rotate( float angle, Core::vec3 axisOfRotation );			// transform the current local rotation by angle on axisOfRotation
			void rotatePitch( float pitch );								// pitch the current local rotation (X-axis rotation)
			void rotateYaw( float yaw );									// yaw the current local rotation (Y-axis rotation)
			void rotateRoll( float roll );									// roll the current local rotation (Z-axis rotation)
			const Core::vec4 & rotation_quaternion();
			Core::vec3 rotation_euler();		// todo: doesn't work (see vector.h)

			const Core::mat4 & orientation();	// scale + position + rotation

			void move( float distance );
			void move( const Core::vec3 & v );

			void hide();
			void show();
			bool isVisible();

			void updateBase( bool recursive, float elapsedMilliseconds );				// recursively update all children (do NOT overload)
			virtual void update( bool recursive, float elapsedMilliseconds );			// update function for this object type (overloading OK)

			// the transparencyCollector must be a pointer to the scene's transparency list for a normal draw pass
			// for a transparency draw pass, transparencyCollector must be NULL
			//virtual void draw( const Core::mat4 camera_projection, const Core::mat4 camera_orientation, Core::mat4 parent_orientation, std::vector<Object*> * transparencyCollector );
			//virtual void draw( Scene * drawer, Core::mat4 parent_orientation, bool opaquePass );
			virtual void calculateTransforms( float elapsedMilliseconds, const Core::mat4 & parent_orientation, bool parentCacheIsClean, bool applyUpdates );

			// Optional z-indexing functions
			void zIndexer_Add( std::list<std::pair< Transform*, int >> * zIndexer, int zIndexTag );
			std::list<std::pair< Transform*, int >>::iterator zIndexer_AddBehind( Transform * relativeTo, int zIndexTag );
			std::list<std::pair< Transform*, int >>::iterator zIndexer_AddInFront( Transform * relativeTo, int zIndexTag );

		protected:
			Transform * m_parent;

			bool m_hidden;
			bool m_updated;

			std::vector<Transform*> m_children;

			// m_frame_* members are reinitialized every frame
			bool m_frame_clean_orientation_cache;		// true if this Transform's own orientation cache is up-to-date
			Core::mat4 m_frame_orientation;

			bool m_frame_clean_cache;					// true if this Tranform's children have up-to-date m_frame_parent_orientation
			Core::mat4 m_frame_parent_orientation;		// cache for the concatenation of orientation matrices of all parent nodes
			bool cleanParentOrientationCache();			// recalculates m_frame_parent_orientation if needed. returns false if an update was required

			bool m_frame_final_orientation_cache;
			Core::mat4 m_frame_final_orientation;		// m_frame_parent_orientation * orientation()

			void setOrientationCacheAsDirty();
			const Core::mat4 & getFinalOrientation();

			Core::vec3 m_scale;
			Core::vec3 m_position;
			Core::vec4 m_rotation;		// quaternion

			// Optional z-indexing members
			std::list<std::pair< Transform*, int >> * m_zIndexer;						// The zIndexer that this Transform belongs to (optional; NULL if it does not belong to any)
			std::list<std::pair< Transform*, int >>::iterator m_zIndexer_myIterator;	// The iterator that points to this Transform in m_zIndexer
		};

	}
}


#endif