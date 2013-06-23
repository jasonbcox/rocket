
#ifndef Rocket_Graphics_Scene_H
#define Rocket_Graphics_Scene_H

#include <vector>
#include <list>

#include "rocket/Core/matrix.h"
#include "Transform.h"
#include "Object.h"
#include "Mesh.h"
#include "Input.h"

#include "rocket/Core/debug.h"

namespace Rocket {
	namespace Graphics {

		static const float OrthographicDrawDistance = 10000.0f;
		static const int NoZIndexTag = 10001;		// Always greater OrthographicDrawDistance

		namespace CameraControls {
		enum CameraControls {
			MoveForward = 0,
			MoveBackward,
			StrafeLeft,
			StrafeRight,
			ElevateUp,
			ElevateDown,
			TurnLeft,
			TurnRight,
			PitchUp,
			PitchDown,
			RollLeft,
			RollRight,
			END_OF_CAMERA_CONTROLS
		};
		}

		class Scene : public Transform {
		public:
			Scene( float orthoWidth, float orthoHeight );								// Create an orthographic scene
			Scene( float FOVy, float aspectRatio, float nearClip, float farClip );		// Create a perspective scene
			~Scene();

			void addMesh( Mesh * mesh );
			// todo: removeMesh() and/or deleteAllMeshes() so that mesh cleanup is automated

			// Note: addObject() decouples the object from the Scene if parent is NULL
			// This means that, from the perspective of any object, "world space" is with respect to its containing Scene node
			// Any complex scene graphs with Scenes as descendents of other Scenes will not work as expected for retrieving world positions
			// In order to fix this, consider removing the ability to decouple children from parents, and override Transform::positionWorld() such that,
			// for Scenes, the orientation is not taken into account in the same manner as Object nodes (as camera positions are inverted, etc.)
			void addObject( Object * object, Transform * parent );

			//void draw( bool drawTransparentObjects );
			void draw( float elapsedMilliseconds, bool clearScreen );
			// elapsedMilliseconds since the last call to draw(). This value is used to correctly interpolate movement of objects within the scene.

			//void queueTransparentObject( Object * object );

			void renderToTexture( int width, int height );

			// Camera Functions
			const Core::mat4 & getCameraProjection();
			const Core::vec3 & getCameraPosition();
			Core::vec3 getCameraDirection();		// unit vector
			Core::vec3 getCameraRotation();			// Euler angles
			const Core::mat4 & getCameraOrientation();
			const Core::mat4 & getCameraOrientationInverse();

			void Camera_Rotate( float angle, Core::vec3 axis );
			void Camera_Move( float distance );
			void Camera_Strafe( float distance );
			void Camera_Elevate( float distance );

			void Camera_Move( Core::vec3 velocity );

			void Camera_Orient( Core::mat4 orientation );
			void Camera_Position( Core::vec3 position );
			void Camera_SetRotation( Core::vec4 quaternion );

			void setCameraInput( Input * input );
			void setCameraMouseSensitivity( float sensitivity );
			void setCameraControl_Move( int forward, int backward );
			void setCameraControl_Strafe( int left, int right );
			void setCameraControl_Elevate( int up, int down );
			void setCameraControl_Turn( int left, int right );
			void setCameraControl_Pitch( int up, int down );
			void setCameraControl_Roll( int left, int right );

			void setCameraMoveSpeed( const Core::vec3 & speed );
			void setCameraTurnSpeed( const Core::vec3 & speed );
			void ControlCamera( float elapsedTime );

			// pickScreen() returns the world position of the picked location
			// if optionalDepth is not specified (equal to 0.0f), the depth is picked from the scene's depth buffer
			Rocket::Core::vec4 pickScreen( int x, int y, float optionalDepth = 0.0f );

			// GL properties
			void enableDepthTest();
			void disableDepthTest();

			std::list<std::pair< Transform*, int >> * zIndexer();

#ifdef ENABLE_DEBUG
			int m_cache_renderedPolygons;
			int m_cache_renderedObjects;
#endif

		private:
			void Init();

			void drawPass();

			// set to non-zero to render to texture:
			unsigned int m_frameTexture;
			unsigned int m_frameBufferObject;		// 0 to use default (screen buffer)
			unsigned int m_frameDepthBuffer;

			// GL properties
			bool m_glDepthTest;

			// Persistant matrices
			Core::mat4 m_camera_projection;			// Camera View Matrix (Camera to Projection)

			// Camera Data
			Core::vec3 m_camera_position;
			void calculateNewPosition( Core::mat4 orientation );	// sets m_camera_position and updates m_frame_camera_orientation
			Core::vec4 m_camera_rotation;							// Quaterion
			void calculateNewRotation( Core::mat4 orientation );	// sets m_camera_rotation and updates m_frame_camera_orientation

			bool m_cache_camera_orientationIsClean;
			Core::mat4 m_cache_camera_orientation;					// Camera Position and Rotation Matrix (World to Camera) - reinitialized every frame to erase floating point error

			bool m_cache_camera_orientationInverseIsClean;
			Core::mat4 m_cache_camera_orientationInverse;

			// always call setCameraOrientation() instead of setting it to a value manually, as the cache variables will not be updated properly
			void setCameraOrientation( Core::mat4 orientation );

			Input * m_camera_input;
			float m_camera_mouseSensitivity;
			int m_camera_controls[CameraControls::END_OF_CAMERA_CONTROLS];

			Core::vec3 m_camera_moveSpeed;	// ( x - strafe, y - elevate, z - move )
			Core::vec3 m_camera_turnSpeed;	// ( x - pitch, y - turn, z - roll )


			// A list of all meshes used to render objects in this scene
			std::vector<Mesh*> m_meshes;

			// Scene children that are rendered directly on top of this Scene
			std::vector<Scene*> m_composites;

			// List of Transforms to keep track of z ordered items
			std::list<std::pair< Transform*, int >> m_zIndexer;

		};
	}
}


#endif