
#include "UnitTest.h"

#include "fixedpoint.h"

using namespace Rocket;
using namespace Rocket::Core;

Rocket_UnitTest ( fixedpoint_BasicMath ) {
	fixedpoint x = 0.0f;
	Rocket_UnitTest_Check_FloatEqual( x.toValue(), 0.0f, 0.001f );

	x -= 10.0f;
	Rocket_UnitTest_Check_FloatEqual( x.toValue(), -10.0f, 0.001f );

	x += 10.1337f;
	Rocket_UnitTest_Check_FloatEqual( x.toValue(), 0.1337f, 0.001f );

	x /= 0.1337f;
	Rocket_UnitTest_Check_FloatEqual( x.toValue(), 1.0f, 0.001f );

	// Reset x due to accumulation of floating point error
	x = 1.0f;
	x *= 100.0f;
	Rocket_UnitTest_Check_FloatEqual( x.toValue(), 100.0f, 0.001f );


	x = x + 1.1f;
	Rocket_UnitTest_Check_FloatEqual( x.toValue(), 101.1f, 0.001f );

	x = x - 100.1f;
	Rocket_UnitTest_Check_FloatEqual( x.toValue(), 1.0f, 0.001f );

	x = x * 30;
	Rocket_UnitTest_Check_FloatEqual( x.toValue(), 30.0f, 0.001f );

	x = x / 6;
	Rocket_UnitTest_Check_FloatEqual( x.toValue(), 5.0f, 0.001f );


	fixedpoint y = x;
	Rocket_UnitTest_Check_FloatEqual( y.toValue(), 5.0f, 0.001f );

	fixedpoint z; z = x;
	Rocket_UnitTest_Check_FloatEqual( z.toValue(), 5.0f, 0.001f );

	x = 0.0f;
	Rocket_UnitTest_Check_FloatEqual( y.toValue(), 5.0f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( z.toValue(), 5.0f, 0.001f );
}

Rocket_UnitTest( fixedpoint_OtherFunctions ) {
	Rocket_UnitTest_Check_FloatEqual( fixedpoint(123.456f).floor().toValue(), 123.0f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( fixedpoint(123.456f).ceiling().toValue(), 124.0f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( fixedpoint(123.456f).round().toValue(), 123.0f, 0.001f );

	Rocket_UnitTest_Check_FloatEqual( fixedpoint(123.654f).floor().toValue(), 123.0f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( fixedpoint(123.654f).ceiling().toValue(), 124.0f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( fixedpoint(123.654f).round().toValue(), 124.0f, 0.001f );

	Rocket_UnitTest_Check_FloatEqual( fixedpoint(123.456f).roundToNearest(12.0f).toValue(), 120.0f, 0.001f );
}

Rocket_UnitTest( fixedpoint_StringFunctions ) {
	Rocket_UnitTest_Check_CharStringEqual( fixedpoint(123.456f).toString().c_str(), "123.456" );
}