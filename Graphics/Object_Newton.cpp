
#include "glew.h"
#include "glfw.h"

#include <vector>

#include "Object_Newton.h"
#include "Scene.h"
#include "matrix.h"

namespace Rocket {
	namespace Graphics {

		Object_Newton::Object_Newton( Mesh * mesh ) : Object( mesh ) {
			setMass( 1.0f );
			setInertia( 1.0f );
			setVelocity( Core::vec3( 0, 0, 0 ) );
			setAngularVelocity( Core::vec3( 0, 0, 0 ) );

			m_frame_a = Core::vec3( 0, 0, 0 );
			m_frame_alpha = Core::vec3( 0, 0, 0 );
		}
		Object_Newton::Object_Newton( const Object & base ) : Object( base ) {
			setMass( 1.0f );
			setInertia( 1.0f );
			setVelocity( Core::vec3( 0, 0, 0 ) );
			setAngularVelocity( Core::vec3( 0, 0, 0 ) );

			m_frame_a = Core::vec3( 0, 0, 0 );
			m_frame_alpha = Core::vec3( 0, 0, 0 );
		}

		Object_Newton::~Object_Newton() {
		}

		void Object_Newton::setMass( float m ) {
			m_mass = m;
		}
		float Object_Newton::getMass() {
			return m_mass;
		}
		void Object_Newton::setInertia( float I ) {
			m_inertia = I;
		}
		float Object_Newton::getInertia() {
			return m_inertia;
		}

		void Object_Newton::setVelocity( const Core::vec3 & v ) {
			m_v = v;
		}
		void Object_Newton::setAngularVelocity( const Core::vec3 & rv ) {
			m_omega = rv;
		}

		void Object_Newton::applyAcceleration( const Core::vec3 & a ) {
			m_frame_a += a;
		}
		void Object_Newton::applyAngularAcceleration( const Core::vec3 & ra ) {
			m_frame_alpha += ra;
		}
		void Object_Newton::applyForce( const Core::vec3 & F ) {
			applyAcceleration( F * (1.0f/m_mass) );
		}
		void Object_Newton::applyTorque( const Core::vec3 & T ) {
			applyAngularAcceleration( T * (1.0f/m_inertia) );
		}
		void Object_Newton::applyTorque( const Core::vec3 & r, const Core::vec3 & F ) {
			applyTorque( Core::cross( r, F ) );
		}

		void Object_Newton::update( bool recursive, float elapsedMilliseconds ) {
			// todo: update velocity with acceleration (based on elapsed time)
			m_v += m_frame_a * elapsedMilliseconds;
			m_omega += m_frame_alpha * elapsedMilliseconds;

			move( m_v );
			rotate( Core::MatrixToQuaternion( Core::Rotate( m_omega.z, Core::vec3(0,0,1) ) * Core::Rotate( m_omega.y, Core::vec3(0,1,0) ) * Core::Rotate( m_omega.x, Core::vec3(1,0,0) ) ) );

			updateBase( recursive, elapsedMilliseconds );

			// reset acceleration data
			m_frame_a = Core::vec3( 0, 0, 0 );
			m_frame_alpha = Core::vec3( 0, 0, 0 );
		}

		Object_Newton * Object_Newton::cloneNewton() {
			// note: this can be made more efficient by writing:
			//		- a copy constructor for Object_Newton
			//		- a Object::function() to do the deep copy procedures
			//		- call that Object::function() from Object_Newton's copy constructor
			Object * obj = clone();
			Object_Newton * r = new Object_Newton( *obj );
			free( obj );	// do not call destructor, or r's members will be destroyed
			r->m_mesh->addMeshUser( r );

			//r->setVelocity( m_v );
			//r->setAngularVelocity( m_omega );
	
			return r;
		}

		Object_Newton * Object_Newton::cloneNewtonInScene( Scene * scene, Transform * parent, const Core::vec3 & scale, const Core::vec4 & rotation, const Core::vec3 & position ) {
			Object_Newton * r = cloneNewton();

			r->scale( scale );
			r->rotate( rotation.w, Core::vec3( rotation.x, rotation.y, rotation.z ) );
			r->position( position );

			scene->addObject( r, parent );

			return r;
		}

	}
}