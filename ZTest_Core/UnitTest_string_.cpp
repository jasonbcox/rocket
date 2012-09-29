
#include "UnitTest.h"

#include "string_.h"

Rocket_UnitTest ( String_Basic ) {
	Rocket::Core::string str1 = "tester";
	Rocket_UnitTest_Check_CharStringEqual( str1.c_str(), "tester" );

	Rocket::Core::string str2 = "test";
	Rocket_UnitTest_Check_CharStringEqual( str2.c_str(), "test" );


	str1 = str2 + "ing";
	Rocket_UnitTest_Check_CharStringEqual( str1.c_str(), "testing" );

	str2 += "er";
	Rocket_UnitTest_Check_CharStringEqual( str2.c_str(), "tester" );

	str1 = str2;
	Rocket_UnitTest_Check_CharStringEqual( str1.c_str(), "tester" );

	char str3_c[] = "cc* test";
	Rocket::Core::string str3(str3_c);
	Rocket_UnitTest_Check_CharStringEqual( str3.c_str(), "cc* test" );

	str3_c[2] = '#';
	Rocket_UnitTest_Check_CharStringEqual( str3.c_str(), "cc* test" );
	Rocket_UnitTest_Check_CharStringEqual( str3_c, "cc# test" );

	Rocket::Core::string str4;
	str4 << 1 << " " << 2 << " " << 3;
	Rocket_UnitTest_Check_CharStringEqual( str4.c_str(), "1 2 3" );

	Rocket::Core::string str5 = "test " + str4;
	Rocket_UnitTest_Check_CharStringEqual( str5.c_str(), "test 1 2 3" );
}