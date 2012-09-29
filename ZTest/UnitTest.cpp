
#include <iostream>
#include <string>

#include "UnitTest.h"

namespace Rocket {
	namespace Test {

		// Globals
		unsigned int results_passed = 0;
		unsigned int results_failed = 0;
		unsigned int results_total = 0;
		std::vector <Rocket::Test::UnitTest*> * testList = NULL;

		UnitTest::UnitTest( std::string name ) : m_name(name), m_result( Rocket::Test::Test_Pass ) {
			Rocket::Test::UnitTests_registerUnitTest( this );
		}

		// Add a unit test to the list of all unit tests
		void UnitTests_registerUnitTest( Rocket::Test::UnitTest * test ) {
			if (testList == NULL) {
				testList = new std::vector <Rocket::Test::UnitTest*>();
			}

			testList->push_back( test );
			results_total++;
		}

		// Run all registered unit tests and output the number that passed
		void UnitTests_runAll() {
			if (testList == NULL) return;	// No tests to run

			std::cerr << "Running " << results_total << " unit tests:\n";

			for (unsigned int i = 0; i < testList->size(); i++) {
				(*testList)[i]->run();

				if ((*testList)[i]->result() == Test_Pass) {
					results_passed++;
				} else {
					std::cerr << (*testList)[i]->name() << " failed.\n";
					results_failed++;
				}
			}

			std::cerr << "Results: " << results_passed << "/" << results_total << " unit tests passed.\n";

			UnitTests_cleanup();
		}

		void UnitTests_cleanup() {
			if (testList != NULL) return;

			for (unsigned int i = 0; i < testList->size(); i++) {
				delete (*testList)[i];
			}

			testList->clear();
			delete testList;
		}

	}
}