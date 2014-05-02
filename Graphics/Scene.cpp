

#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "rocket/Core/matrix.h"
#include "Transform.h"
#include "Object.h"

#include "rocket/Core/debug.h"

namespace Rocket {
	namespace Graphics {

		void Scene::Init() {
			m_camera_position = Core::vec3(0,0,0);
			m_camera_rotation = Core::Quaternion( 0.0f, Core::vec3(0,1,0) );
			m_cache_camera_orientationIsClean = false;
			m_cache_camera_orientationInverseIsClean = false;

			m_frameTexture = 0;
			m_frameBufferObject = 0;
			m_frameDepthBuffer = 0;
			
#ifdef ENABLE_DEBUG
			m_cache_renderedPolygons = 0;
			m_cache_renderedObjects = 0;
#endif

			setCameraInput( nullptr );
		}

		/*! Create a Scene that is rendered with an orthographic camera
		- orthoWidth/orthoHeight - Number of pixels to render in that dimension
		*/
		Scene::Scene( float orthoWidth, float orthoHeight ) {
			m_camera_projection = Core::Orthographic( 0.0f, orthoWidth, orthoHeight, 0.0f, -OrthographicDrawDistance, OrthographicDrawDistance );
			
			Init();

			disableDepthTest();
		}
		/*! Create a Scene that is rendered with a perspective camera
		- FOVy - Y-Axis (vertical) field of view in degrees.  Typical values for this range from 60.0 - 90.0
		- aspectRatio - Ratio of width to height
		- nearClip - Distance of the near clip plane
		- farClip - Distance of the far clip plane
		*/
		Scene::Scene( float FOVy, float aspectRatio, float nearClip, float farClip ) {
			m_camera_projection = Core::Perspective( FOVy, aspectRatio, nearClip, farClip );
			
			Init();

			enableDepthTest();
		}
		Scene::~Scene() {
		}

		//! Add a Mesh to this Scene's list of Meshes that it can render
		void Scene::addMesh( Mesh * mesh ) {
			m_meshes.push_back( mesh->shared_from_this() );
			mesh->addSceneToUserList( this );
		}

		//! Add an Object to this Scene's render tree
		void Scene::addObject( Object * object, Transform * parent ) {
			bool added = false;
			if ( parent == nullptr ) {
				addChild( object, false );
				added = true;
			} else {
				for ( auto owner : parent->getOwners() ) {
					if ( owner.get() == this ) {
						parent->addChild( object, true );
						added = true;
						break;
					}
				}
				if ( added != true ) {
					Debug_ThrowError( "Error: Parent isn't owned by this Scene.", 0 );
				}
			}
			if ( added == true ) {
				object->addOwner( this );
				Mesh * mesh = object->getMesh();
				if ( mesh != nullptr ) mesh->addMeshUser( object );
			}
		}

		//! Add a Scene to this Scene's this of composites.  A composite Scene is rendered after this Scene in draw()
		void Scene::addComposite( Scene * scene ) {
			m_composites.push_back( static_pointer_cast< Scene >( scene->shared_from_this() ) );
		}

		//! Draws all Meshes for the current pass
		void Scene::drawPass() {
			for ( auto mesh : m_meshes ) {
				mesh->drawCurrentPass( &getCameraProjection(), &getCameraOrientation() );
#ifdef ENABLE_DEBUG
				m_cache_renderedPolygons += mesh->m_cache_renderedPolygons;
				m_cache_renderedObjects += mesh->m_cache_renderedObjects;
#endif
			}
		}

		//! Renders all Objects in this Scene, interpolating movement using elapsedMilliseconds since the last call to draw()
		void Scene::draw( float elapsedMilliseconds, bool clearScreen ) {
			glBindFramebuffer( GL_FRAMEBUFFER, m_frameBufferObject );
			if ( m_glDepthTest == true ) {
				glEnable( GL_DEPTH_TEST );
			} else {
				glDisable( GL_DEPTH_TEST );
			}
			
			if ( clearScreen == true ) glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

#ifdef ENABLE_DEBUG
			m_cache_renderedPolygons = 0;
			m_cache_renderedObjects = 0;
#endif

			// Calculate transforms and update all nodes
			for ( auto child : m_children ) {
				child->calculateTransforms( elapsedMilliseconds, Core::mat4(), false, true );
			}

			// Prepare meshes for drawing passes
			for ( auto mesh : m_meshes ) {
				mesh->startPassesForScene( this );
			}

			// Opaque pass
			drawPass();

			// Transparency pass
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			drawPass();

			glDisable( GL_BLEND );

			//glBindFramebuffer(GL_FRAMEBUFFER, 0 );
			//glBindTexture( GL_TEXTURE_2D, m_renderPasses[0]->m_frameTexture );
			//glGenerateMipmap(GL_TEXTURE_2D);
			
			// Render composites
			for ( auto composite : m_composites ) {
				composite->draw( elapsedMilliseconds, false );
				
#ifdef ENABLE_DEBUG
				m_cache_renderedPolygons += composite->m_cache_renderedPolygons;
				m_cache_renderedObjects += composite->m_cache_renderedObjects;
#endif
			}
		}

		//! Set this Scene to render to a new image buffer
		void Scene::renderToTexture( int width, int height ) {
			// Create RGB texture for the previous scene to render to
			glGenTextures( 1, &m_frameTexture );
			glBindTexture( GL_TEXTURE_2D, m_frameTexture );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			// NULL means reserve texture memory, but texels are undefined
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL );
			glGenerateMipmap( GL_TEXTURE_2D );

			// Create the frame buffer object for the previous scene to render with
			glGenFramebuffers( 1, &m_frameBufferObject );
			glBindFramebuffer( GL_FRAMEBUFFER, m_frameBufferObject );

			// Attach 2D texture to this FBO
			glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameTexture, 0 );

			// Create the depth buffer for the previous scene to render to
			glGenRenderbuffers( 1, &m_frameDepthBuffer );
			glBindRenderbuffer( GL_RENDERBUFFER, m_frameDepthBuffer );
			glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height );

			// Attach depth buffer to FBO
			glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_frameDepthBuffer );
		}

		//! Returns the View matrix (camera to projection)
		const Core::mat4 & Scene::getCameraProjection() {
			return m_camera_projection;
		}
		//! Returns the camera's current position
		const Core::vec3 & Scene::getCameraPosition() {
			return m_camera_position;
		}
		//! Returns the camera's current unit vector direction
		Core::vec3 Scene::getCameraDirection() {
			return Core::QuaternionToUnitVector( m_camera_rotation );
		}
		//! Returns the camera's euler direction
		Core::vec3 Scene::getCameraRotation() {
			return Core::QuaternionToEuler( m_camera_rotation );
		}

		//! Calculates the camera's new position given a new orientation matrix
		void Scene::calculateNewPosition( Core::mat4 orientation ) {
			Core::vec4 pos = orientation.inverse() * Core::vec4( -orientation(0,3), -orientation(1,3), -orientation(2,3), 0.0f );
			m_camera_position = pos.xyz();
		}
		//! Calculates the camera's new rotation given a new orientation matrix
		void Scene::calculateNewRotation( Core::mat4 orientation ) {
			m_camera_rotation = Core::MatrixToQuaternion( orientation );
		}

		//! Returns the camera's current orientation matrix
		const Core::mat4 & Scene::getCameraOrientation() {
			if ( m_cache_camera_orientationIsClean == false ) {
				m_cache_camera_orientationIsClean = true;
				m_cache_camera_orientation = Core::Translate( m_camera_position ) * Core::QuaternionRotate( m_camera_rotation );
			}
			return m_cache_camera_orientation;
		}

		//! Returns the inverse of the camera's orientation matrix
		const Core::mat4 & Scene::getCameraOrientationInverse() {
			if ( m_cache_camera_orientationInverseIsClean == false ) {
				m_cache_camera_orientationInverseIsClean = true;
				m_cache_camera_orientationInverse = m_cache_camera_orientation.inverse();
			}
			return m_cache_camera_orientationInverse;
		}

		//! Sets the camera's orientation matrix cache
		void Scene::setCameraOrientation( Core::mat4 orientation ) {
			m_cache_camera_orientation = orientation;
			m_cache_camera_orientationIsClean = true;
			m_cache_camera_orientationInverseIsClean = false;
		}

		//! Rotates the camera by the given angle around the specified axis
		void Scene::Camera_Rotate( float angle, Core::vec3 axis ) {
			Core::mat4 cameraOrientation = getCameraOrientation();
			setCameraOrientation( Core::Rotate( angle, axis ) * cameraOrientation );
			calculateNewRotation( m_cache_camera_orientation );
		}
		//! Moves the camera forward by distance
		void Scene::Camera_Move( float distance ) {
			Core::mat4 cameraOrientation = getCameraOrientation();
			setCameraOrientation( Core::Translate( Core::vec3( 0.0f, 0.0f, distance ) ) * cameraOrientation );
			calculateNewPosition( m_cache_camera_orientation );
		}
		//! Strafes the camera to the right by distance
		void Scene::Camera_Strafe( float distance ) {
			Core::mat4 cameraOrientation = getCameraOrientation();
			setCameraOrientation( Core::Translate( Core::vec3( -distance, 0.0f, 0.0f ) ) * cameraOrientation );
			calculateNewPosition( m_cache_camera_orientation );
		}
		//! Elevates the camera up by distance
		void Scene::Camera_Elevate( float distance ) {
			Core::mat4 cameraOrientation = getCameraOrientation();
			setCameraOrientation( Core::Translate( Core::vec3( 0.0f, -distance, 0.0f ) ) * cameraOrientation );
			calculateNewPosition( m_cache_camera_orientation );
		}

		//! Moves the camera by the given velocity vector
		void Scene::Camera_Move( Core::vec3 velocity ) {
			Core::mat4 cameraOrientation = getCameraOrientation();
			//vec3 v = getSlideCollision( getCameraPosition(), velocity, 0.3f );
			//vec4 move = m_camera_orientation * vec4( v, 0.0f );

			Core::vec4 move = cameraOrientation * Core::vec4( velocity, 0.0f );
			setCameraOrientation( Core::Translate( (-move).xyz() ) * cameraOrientation );
			calculateNewPosition( m_cache_camera_orientation );
		}

		//! Sets the camera's orientation matrix
		void Scene::Camera_Orient( Core::mat4 orientation ) {
			setCameraOrientation( orientation );
			calculateNewPosition( m_cache_camera_orientation );
		}

		//! Sets the camera's position
		void Scene::Camera_Position( Core::vec3 position ) {
			Core::mat4 orientationAtOrigin = Core::QuaternionRotate( m_camera_rotation );
			Core::vec4 move = orientationAtOrigin * Core::vec4( position, 0.0f );

			setCameraOrientation( Core::Translate( (-move).xyz() ) * orientationAtOrigin );
			calculateNewPosition( m_cache_camera_orientation );
		}

		//! Sets the camera's rotation using a quaternion
		void Scene::Camera_SetRotation( Core::vec4 quaternion ) {
			setCameraOrientation( Translate( getCameraPosition() ) * Core::QuaternionRotate( quaternion ) );
			calculateNewRotation( m_cache_camera_orientation );
		}

		//! Sets the camera's controls to be bound to the specified Input
		void Scene::setCameraInput( Input * input ) {
			m_camera_input = input;

			setCameraMouseSensitivity( 50.0f );
			setCameraControl_Move( 0, 0 );
			setCameraControl_Strafe( 0, 0 );
			setCameraControl_Elevate( 0, 0 );
			setCameraControl_Turn( 0, 0 );
			setCameraControl_Pitch( 0, 0 );
			setCameraControl_Roll( 0, 0 );
			m_camera_moveSpeed = Core::vec3( 0.0f, 0.0f, 0.0f );
			m_camera_turnSpeed = Core::vec3( 0.0f, 0.0f, 0.0f );
		}
		//! Set the camera's mouse sensitivity
		void Scene::setCameraMouseSensitivity( float sensitivity ) {
			m_camera_mouseSensitivity = sensitivity;
		}
		//! Set the camera's move speed
		void Scene::setCameraControl_Move( int forward, int backward ) {
			m_camera_controls[ (unsigned int)CameraControls::MoveForward ] = forward;
			m_camera_controls[ (unsigned int)CameraControls::MoveBackward ] = backward;
		}
		//! Set the camera's strafe speed
		void Scene::setCameraControl_Strafe( int left, int right ) {
			m_camera_controls[ (unsigned int)CameraControls::StrafeLeft ] = left;
			m_camera_controls[ (unsigned int)CameraControls::StrafeRight ] = right;
		}
		//! Set the camera's elevate speed
		void Scene::setCameraControl_Elevate( int up, int down ) {
			m_camera_controls[ (unsigned int)CameraControls::ElevateUp ] = up;
			m_camera_controls[ (unsigned int)CameraControls::ElevateDown ] = down;
		}
		//! Set the camera's turn speed (Y-Axis)
		void Scene::setCameraControl_Turn( int left, int right ) {
			m_camera_controls[ (unsigned int)CameraControls::TurnLeft ] = left;
			m_camera_controls[ (unsigned int)CameraControls::TurnRight ] = right;
		}
		//! Set the camera's pitch speed (X-Axis)
		void Scene::setCameraControl_Pitch( int up, int down ) {
			m_camera_controls[ (unsigned int)CameraControls::PitchUp ] = up;
			m_camera_controls[ (unsigned int)CameraControls::PitchDown ] = down;
		}
		//! Set the camera's roll speed (Z-Axis)
		void Scene::setCameraControl_Roll( int left, int right ) {
			m_camera_controls[ (unsigned int)CameraControls::RollLeft ] = left;
			m_camera_controls[ (unsigned int)CameraControls::RollRight ] = right;
		}

		//! Set the camera's movement speed using a vector
		void Scene::setCameraMoveSpeed( const Core::vec3 & speed ) {
			m_camera_moveSpeed = speed;
		}
		//! Set the camera's turn speed using a vector
		void Scene::setCameraTurnSpeed( const Core::vec3 & speed ) {
			m_camera_turnSpeed = speed;
		}

		//! Gather inputs over the given elapsedTime since the last call to ControlCamera and apply them to the camera
		void Scene::ControlCamera( float elapsedTime ) {
			Core::vec3 strafeAxis = ( getCameraOrientationInverse() * Core::vec4( 1, 0, 0, 0 ) ).xyz();
			strafeAxis.y( 0 );
			if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::StrafeLeft ] ) ) Camera_Move( strafeAxis * -m_camera_moveSpeed.x() * elapsedTime );
			if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::StrafeRight ] ) ) Camera_Move( strafeAxis * m_camera_moveSpeed.x() * elapsedTime );

			Core::vec3 moveAxis = ( getCameraOrientationInverse() * Core::vec4( 0, 0, 1, 0 ) ).xyz();
			moveAxis.y( 0 );
			if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::MoveForward ] ) ) Camera_Move( moveAxis * -m_camera_moveSpeed.z() * elapsedTime );
			if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::MoveBackward ] ) ) Camera_Move( moveAxis * m_camera_moveSpeed.z() * elapsedTime );

			// Control looking with mouse if all look controls aren't set
			Core::vec3 lookSpeed = Core::vec3( 0.0f, 0.0f, 0.0f );
			if ( ( m_camera_controls[ (unsigned int)CameraControls::TurnLeft ] == 0 )
				|| ( m_camera_controls[ (unsigned int)CameraControls::TurnRight ] == 0 )
				|| ( m_camera_controls[ (unsigned int)CameraControls::PitchUp ] == 0 )
				|| ( m_camera_controls[ (unsigned int)CameraControls::PitchDown ] == 0 ) ) {

					Core::vec2i mouseMove = m_camera_input->getMouseMove();
					lookSpeed.x( -m_camera_turnSpeed.x() * mouseMove.y() * 1.0f / m_camera_mouseSensitivity );
					lookSpeed.y( -m_camera_turnSpeed.y() * mouseMove.x() * 1.0f / m_camera_mouseSensitivity );
			} else {
				if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::TurnLeft ] ) ) lookSpeed.y( lookSpeed.y() - m_camera_turnSpeed.y() );
				if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::TurnRight ] ) ) lookSpeed.y( lookSpeed.y() + m_camera_turnSpeed.y() );

				if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::PitchUp ] ) ) lookSpeed.x( lookSpeed.x() - m_camera_turnSpeed.x() );
				if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::PitchDown ] ) ) lookSpeed.x( lookSpeed.x() + m_camera_turnSpeed.x() );
			}

			// rotate around local X-axis
			Camera_Rotate( lookSpeed.x() * elapsedTime, Core::vec3( 1, 0, 0 ) );

			// rotate around global Y-axis
			Core::mat4 cameraOrientationInverse = getCameraOrientationInverse();
			Core::vec3 rotationAxis = ( cameraOrientationInverse.transposed() * Core::vec4( 0, 1, 0, 0 ) ).xyz();
			Camera_Rotate( lookSpeed.y() * elapsedTime, rotationAxis );

			if ( ( m_camera_controls[ (unsigned int)CameraControls::ElevateUp ] != 0 )
				&& ( m_camera_controls[ (unsigned int)CameraControls::ElevateDown ] != 0 ) ) {

					Core::vec3 elevateAxis = ( getCameraOrientationInverse() * Core::vec4( 0, 1, 0, 0 ) ).xyz();
					elevateAxis.x( 0 );	elevateAxis.z( 0 );
					if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::ElevateUp ] ) ) Camera_Move( elevateAxis * m_camera_moveSpeed.z() * elapsedTime );
					if ( m_camera_input->getKeySimple( m_camera_controls[ (unsigned int)CameraControls::ElevateDown ] ) ) Camera_Move( elevateAxis * -m_camera_moveSpeed.z() * elapsedTime );
			}
		}

		//! Returns the world position of the picked location.  If optionalDepth is not specified (equal to 0.0f), the depth is picked from the scene's depth buffer.
		Rocket::Core::vec4 Scene::pickScreen( int x, int y, float optionalDepth ) {
			GLint viewport[4];
			GLdouble modelview[ 16 ];
			GLdouble projection[ 16 ];
			GLfloat winX, winY, winZ;
			GLdouble posX, posY, posZ;

			glGetDoublev( GL_MODELVIEW_MATRIX, modelview );		// get modelview matrix from GPU
			glGetDoublev( GL_PROJECTION_MATRIX, projection );	// get projection matrix from GPU
			glGetIntegerv( GL_VIEWPORT, viewport );				// get viewport from GPU

			winX = (float)x;
			winY = (float)viewport[3] - (float)y;
			if ( optionalDepth == 0.0f ) {
				glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );	// get depth from depth buffer
			} else {
				winZ = optionalDepth;
			}

			gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ );

			return Rocket::Core::normalize( Rocket::Core::vec4( (float)posX, (float)posY, (float)posZ, 0.0f ) );
		}

		//! Enable depth testing
		void Scene::enableDepthTest() {
			m_glDepthTest = true;
		}
		//! Disable depth testing
		void Scene::disableDepthTest() {
			m_glDepthTest = false;
		}

		//! Return the zIndexer used in this Scene (whether utilized or not)
		zIndexerType * Scene::zIndexer() {
			return &m_zIndexer;
		}

		/*! Override Transform::getOwners() because this Scene's Transform doesn't
			know that it owns itself (and can't know because the list of owners is
			a vector of shared_ptr, and we don't want a shared_ptr pointing to its
			own container).
		*/
		vector< shared_ptr< Scene > > Scene::getOwners() {
			vector< shared_ptr< Scene > > r = Transform::getOwners();
			r.push_back( static_pointer_cast< Scene >( this->shared_from_this() ) );
			return r;
		}

	}
}

