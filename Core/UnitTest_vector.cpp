
#include "rocket/UnitTest.h"

#include "vector.h"

Rocket_UnitTest ( VectorMath_vec2 ) {
	Rocket::Core::vec2 v1;
	Rocket_UnitTest_Check_FloatEqual( v1.x(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.y(), 0.0f, 0.0001f );

	v1.x( 2.0f );		v1.y( 3.0f );
	v1 = normalize( v1 );
	Rocket_UnitTest_Check_FloatEqual( v1.x(), 2.0f/sqrt(13.0f), 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.y(), 3.0f/sqrt(13.0f), 0.0001f );

	v1 *= 2.0f;
	Rocket_UnitTest_Check_FloatEqual( v1.length(), 2.0f, 0.0001f );

	Rocket::Core::vec2 v2(1,0);
	Rocket_UnitTest_Check_FloatEqual( v2[0], 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v2[1], 0.0f, 0.0001f );
	Rocket::Core::vec2 v3(0,1);
	Rocket_UnitTest_Check_FloatEqual( dot(v2,v3), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_Expression( !(v2 == v3) );
}

Rocket_UnitTest ( VectorMath_vec3 ) {
	Rocket::Core::vec3 v1;
	Rocket_UnitTest_Check_FloatEqual( v1.x(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.y(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.z(), 0.0f, 0.0001f );

	v1.x( 2.0f );		v1.y( 3.0f );		v1.z( 4.0f );
	v1 = normalize( v1 );
	Rocket_UnitTest_Check_FloatEqual( v1.x(), 2.0f/sqrt(29.0f), 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.y(), 3.0f/sqrt(29.0f), 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.z(), 4.0f/sqrt(29.0f), 0.0001f );

	v1 *= 2.0f;
	Rocket_UnitTest_Check_FloatEqual( v1.length(), 2.0f, 0.0001f );

	Rocket::Core::vec3 v2(1,0,0);
	Rocket::Core::vec3 v3( Rocket::Core::vec2(0,1), 0.0f );
	Rocket_UnitTest_Check_FloatEqual( dot(v2,v3), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_Expression( !(v2 == v3) );
	Rocket::Core::vec3 v4 = cross( v2, v3 );
	Rocket_UnitTest_Check_FloatEqual( v4.x(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v4.y(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v4.z(), 1.0f, 0.0001f );
}

Rocket_UnitTest ( VectorMath_vec4 ) {
	Rocket::Core::vec4 v1;
	Rocket_UnitTest_Check_FloatEqual( v1.x(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.y(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.z(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.w(), 0.0f, 0.0001f );

	v1.x( 2.0f );		v1.y( 3.0f );
	v1 = normalize( v1 );
	Rocket_UnitTest_Check_FloatEqual( v1.x(), 2.0f/sqrt(13.0f), 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.y(), 3.0f/sqrt(13.0f), 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.z(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.w(), 0.0f, 0.0001f );

	v1 *= 2.0f;
	Rocket_UnitTest_Check_FloatEqual( v1.length(), 2.0f, 0.0001f );

	Rocket::Core::vec4 v2(1,0,0,0);
	Rocket::Core::vec4 v3(0,1,0,0);
	Rocket_UnitTest_Check_FloatEqual( dot(v2,v3), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_Expression( !(v2 == v3) );
	Rocket::Core::vec4 v4 = Rocket::Core::vec4( cross( v2.xyz(), v3.xyz() ), 0.0f );
	Rocket_UnitTest_Check_FloatEqual( v4.x(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v4.y(), 0.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v4.z(), 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v4.w(), 0.0f, 0.0001f );
}
