
#ifndef Rocket_ZTest_UnitTest_H
#define Rocket_ZTest_UnitTest_H

#include <iostream>
#include <string>
#include <vector>

namespace Rocket {
	namespace Test {

		enum TestResults {
			Test_Pass,
			Test_Fail
		};

		class UnitTest {
		public:
			UnitTest( std::string name );
			virtual ~UnitTest();
			virtual void run() = 0;

			std::string name() { return m_name; }
			Rocket::Test::TestResults result() { return m_result; }

		protected:
			std::string m_name;
			Rocket::Test::TestResults m_result;
		};

		void UnitTests_registerUnitTest( Rocket::Test::UnitTest * test );
		void UnitTests_runAll();
		void UnitTests_cleanup();

	}
}

// Unit Test Macro : Begin
#define Rocket_UnitTest( test_name ) \
class test_name##UnitTest : public Rocket::Test::UnitTest \
{ \
	public: \
		test_name##UnitTest() : Rocket::Test::UnitTest(#test_name) { } \
		virtual ~test_name##UnitTest() { } \
		void run(); \
}; \
test_name##UnitTest * test_name##_unitTest = new test_name##UnitTest; \
void test_name##UnitTest::run()
// Unit Test Macro : End (brackets '{' and '}' following the call of this macro will be defined as that class's run()


// Unit Test Tests:

// Check if a boolean expression is true
#define Rocket_UnitTest_Check_Expression( expression ) \
do { \
	if (!(expression)) { \
		std::cerr << __FILE__ << "(" << __LINE__ << ") \n\t[Check_Expression] : " << #expression << " was false.\n"; \
		m_result = Rocket::Test::Test_Fail; return; \
	} \
} while (0)

// Compare two boolean expressions
#define Rocket_UnitTest_Check_Equal( expression1, expression2 ) \
do { \
	if ((expression1) != (expression2)) { \
		std::cerr << __FILE__ << "(" << __LINE__ << ") \n\t[Check_Equal] : " << #expression1 << " != " << #expression2 << " (" <<  expression1 << " != " << expression2 << ")\n"; \
		m_result = Rocket::Test::Test_Fail; return; \
	} \
} while (0)

// Compare Float Equality
#define Rocket_UnitTest_Check_FloatEqual( expression1, expression2, precision ) \
do { \
	if ( abs((expression1) - (expression2)) > precision ) { \
		std::cerr << __FILE__ << "(" << __LINE__ << ") \n\t[Check_FloatEqual] : " << #expression1 << " != " << #expression2 << " (" <<  expression1 << " != " << expression2 << ")\n"; \
		m_result = Rocket::Test::Test_Fail; return; \
	} \
} while (0)

// Compare Char String Equality
#define Rocket_UnitTest_Check_CharStringEqual( str1, str2 ) \
do { \
	if ( std::string(str1).compare(std::string(str2)) != 0 ) { \
		std::cerr << __FILE__ << "(" << __LINE__ << ") \n\t[Check_CharStringEqual] :\n\t " << #str1 << " != " << #str2 << " \n\t('" <<  str1 << "' != '" << str2 << "')\n"; \
		m_result = Rocket::Test::Test_Fail; return; \
	} \
} while (0)

#endif
