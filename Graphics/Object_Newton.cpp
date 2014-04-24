
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Object_Newton.h"
#include "Scene.h"
#include "rocket/Core/matrix.h"

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

		//! Sets this Object_Newton's velocity relative to its parent's space
		void Object_Newton::setVelocity( const Core::vec3 & v ) {
			m_v = v;
		}
		//! Sets this Object_Newton's local angular velocity
		void Object_Newton::setAngularVelocity( const Core::vec3 & rv ) {
			m_omega = rv;
		}

		//! Applies acceleration relative to this Object_Newton's parent's space
		void Object_Newton::applyAcceleration( const Core::vec3 & a ) {
			m_frame_a += a;
		}
		//! Applies local angular acceleration
		void Object_Newton::applyAngularAcceleration( const Core::vec3 & ra ) {
			m_frame_alpha += ra;
		}
		//! Applies the given force relative to this Object_Newton's parent's space
		void Object_Newton::applyForce( const Core::vec3 & F ) {
			applyAcceleration( F * (1.0f/m_mass) );
		}
		//! Applies local torque given a torque vector
		void Object_Newton::applyTorque( const Core::vec3 & T ) {
			applyAngularAcceleration( T * (1.0f/m_inertia) );
		}
		//! Applies local torque given a rotation vector and a force vector
		void Object_Newton::applyTorque( const Core::vec3 & r, const Core::vec3 & F ) {
			applyTorque( Core::cross( r, F ) );
		}

		//! Updates this Object_Newton's rendering state
		void Object_Newton::update( bool recursive, float elapsedMilliseconds ) {
			// todo: update velocity with acceleration (based on elapsed time)
			m_v += m_frame_a * elapsedMilliseconds;
			m_omega += m_frame_alpha * elapsedMilliseconds;

			move( m_v );
			rotate( Core::MatrixToQuaternion( Core::Rotate( m_omega.z(), Core::vec3(0,0,1) ) * Core::Rotate( m_omega.y(), Core::vec3(0,1,0) ) * Core::Rotate( m_omega.x(), Core::vec3(1,0,0) ) ) );

			Object::update( recursive, elapsedMilliseconds );

			// reset acceleration data
			m_frame_a = Core::vec3( 0, 0, 0 );
			m_frame_alpha = Core::vec3( 0, 0, 0 );
		}

		shared_ptr< Object_Newton > Object_Newton::cloneNewton() {
			shared_ptr< Object_Newton > r = make_shared< Object_Newton >( m_mesh.get() );

			clonePropertiesOnto( r.get() );

			//r->setVelocity( m_v );
			//r->setAngularVelocity( m_omega );
	
			return r;
		}

		//! Make a clone of this Object_Newton and add it to the given Scene with an optional parent Tranform. Transform properties are copied from this Object.
		shared_ptr< Object_Newton > Object_Newton::cloneNewtonInScene( Scene * scene, Transform * parent, const Core::vec3 & scale, const Core::vec4 & rotation, const Core::vec3 & position ) {
			shared_ptr< Object_Newton > r = cloneNewton();

			r->scale( scale );
			r->rotate( rotation.w(), rotation.xyz() );
			r->position( position );

			scene->addObject( r.get(), parent );

			return r;
		}

	}
}

