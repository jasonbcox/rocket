
#include "rocket/UnitTest.h"

#include "matrix.h"

Rocket_UnitTest ( VectorMath_mat2 ) {
	Rocket::Core::mat2 m1;
	Rocket_UnitTest_Check_FloatEqual( m1[0][0], 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[1][1], 1.0f, 0.0001f );

	m1[1][1] = 2.0f;
	m1[0][1] = 1.0f;
	m1 = m1.transpose();
	Rocket_UnitTest_Check_FloatEqual( m1[1][1], 2.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[1][0], 1.0f, 0.0001f );

	Rocket::Core::vec2 v1 = m1 * Rocket::Core::vec2(1,1);
	Rocket_UnitTest_Check_FloatEqual( v1.x, 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.y, 3.0f, 0.0001f );

	Rocket::Core::mat2 m2(1,2,3,4);
	Rocket::Core::mat2 m3(5,6,7,8);
	m1 = m2 * m3;
	Rocket_UnitTest_Check_FloatEqual( m1[0][0], 19.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[0][1], 22.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[1][0], 43.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[1][1], 50.0f, 0.0001f );

	Rocket_UnitTest_Check_FloatEqual( m1.determinant(), 4.0f, 0.0001f );
}

Rocket_UnitTest ( VectorMath_mat3 ) {
	Rocket::Core::mat3 m1;
	Rocket_UnitTest_Check_FloatEqual( m1[0][0], 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[1][1], 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[2][2], 1.0f, 0.0001f );

	m1[0][2] = 2.0f;
	m1[1][2] = 1.0f;
	m1 = m1.transpose();
	Rocket_UnitTest_Check_FloatEqual( m1[2][0], 2.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[2][1], 1.0f, 0.0001f );

	Rocket::Core::vec3 v1 = m1 * Rocket::Core::vec3(1,1,1);
	Rocket_UnitTest_Check_FloatEqual( v1.x, 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.y, 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.z, 4.0f, 0.0001f );

	Rocket::Core::mat3 m2(1,2,3,4,5,6,7,8,9);
	Rocket::Core::mat3 m3(0,1,0,1,0,1,0,1,0);
	m1 = m2 * m3;
	Rocket_UnitTest_Check_FloatEqual( m1[0][0], 2.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[1][1], 10.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[2][2], 8.0f, 0.0001f );

	Rocket_UnitTest_Check_FloatEqual( m1.determinant(), 0.0f, 0.0001f );

	Rocket::Core::mat3 m4(2,1,3,5,4,3,6,5,1);
	Rocket_UnitTest_Check_FloatEqual( m4.determinant(), -6.0f, 0.0001f );

	m4 = m4.inverse();
	Rocket_UnitTest_Check_FloatEqual( m4[0][0], 1.8333f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4[1][1], 2.6666f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4[2][2], -0.5f, 0.0001f );

	float * m4array = m4;
	Rocket_UnitTest_Check_FloatEqual( m4array[0], 1.8333f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4array[4], 2.6666f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4array[8], -0.5f, 0.0001f );
}

Rocket_UnitTest ( VectorMath_mat4 ) {
	Rocket::Core::mat4 m1;
	Rocket_UnitTest_Check_FloatEqual( m1[0][0], 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[1][1], 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[2][2], 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[3][3], 1.0f, 0.0001f );

	m1[0][2] = 2.0f;
	m1[1][2] = 1.0f;
	m1 = m1.transpose();
	Rocket_UnitTest_Check_FloatEqual( m1[2][0], 2.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m1[2][1], 1.0f, 0.0001f );

	Rocket::Core::vec4 v1 = m1 * Rocket::Core::vec4(1,1,1,1);
	Rocket_UnitTest_Check_FloatEqual( v1.x, 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.y, 1.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.z, 4.0f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( v1.w, 1.0f, 0.0001f );

	Rocket::Core::mat4 m4(2,1,3,5,4,3,6,5,1,8,9,0,4,6,8,2);
	Rocket_UnitTest_Check_FloatEqual( m4.determinant(), 40.0f, 0.0001f );

	m4 = m4.inverse();
	Rocket_UnitTest_Check_FloatEqual( m4[0][0], 0.2f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4[1][1], -2.75f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4[2][2], 0.6f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4[3][3], 0.375f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4[0][3], 0.75f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4[3][1], -0.75f, 0.0001f );

	float * m4array = m4;
	Rocket_UnitTest_Check_FloatEqual( m4array[0], 0.2f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4array[5], -2.75f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4array[10], 0.6f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( m4array[15], 0.375f, 0.0001f );

	// Test quaternion conversion
	Rocket::Core::vec4 quat( 0.18257f, 0.36514f, 0.54772f, 0.73029f );
	Rocket::Core::mat4 m5 = Rocket::Core::QuaternionRotate( quat );
	quat = Rocket::Core::MatrixToQuaternion( m5 );
	Rocket_UnitTest_Check_FloatEqual( quat.x, 0.18257f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( quat.y, 0.36514f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( quat.z, 0.54772f, 0.0001f );
	Rocket_UnitTest_Check_FloatEqual( quat.w, 0.73029f, 0.0001f );
}