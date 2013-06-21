
#ifndef Rocket_Graphics_Object_Newton_H
#define Rocket_Graphics_Object_Newton_H

#include "rocket/Core/matrix.h"
#include "Mesh.h"
#include "Transform.h"
#include "Object.h"
#include "Shader.h"

namespace Rocket {
	namespace Graphics {

		class Scene;
		class Object_Newton : public Object {
		public:
			Object_Newton( Mesh * mesh = NULL );
			Object_Newton( const Object & base );
			virtual ~Object_Newton();

			void setMass( float m );
			float getMass();
			void setInertia( float I );
			float getInertia();

			void setVelocity( const Core::vec3 & v );					// velocity relative to parent-space
			void setAngularVelocity( const Core::vec3 & rv );			// local angular velocity relative

			// Acceleration is applied for elapsedMilliseconds of time when update() is called
			void applyAcceleration( const Core::vec3 & a );				// acceleration relative to parent-space
			void applyAngularAcceleration( const Core::vec3 & ra );		// local angular accerelation relative
			void applyForce( const Core::vec3 & F );
			void applyTorque( const Core::vec3 & T );
			void applyTorque( const Core::vec3 & r, const Core::vec3 & F );

			void update( bool recursive, float elapsedMilliseconds ) override;

			Object_Newton * cloneNewton();
			Object_Newton * cloneNewtonInScene( Scene * scene, Transform * parent, const Core::vec3 & scale, const Core::vec4 & rotation, const Core::vec3 & position );

		private:
			float m_mass;
			float m_inertia;

			Core::vec3 m_v;		// velocity
			Core::vec3 m_omega;	// angular velocity

			// m_frame_* members are reinitialized every frame
			Core::vec3 m_frame_a;		// acceleration
			Core::vec3 m_frame_alpha;	// angular acceleration

		};

	}
}

#endif