
#include "UnitTest.h"

int main() {
	Rocket::Test::UnitTests_runAll();
#ifdef OS_WINDOWS
	system("pause");
#endif
	return 0;
}
