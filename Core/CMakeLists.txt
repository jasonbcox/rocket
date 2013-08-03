cmake_minimum_required( VERSION 2.8 )
project( RocketCore )

set( RocketCore_headers
	system.h
	string_.h
	timer.h
	mathconstants.h
	vector.h
	matrix.h
	dsp.h
	fixedpoint.h
	debug.h
)
set( RocketCore_sources
	string_.cpp
	timer.cpp
	dsp.cpp
	fixedpoint.cpp
	debug.cpp
)

add_library ( RocketCore
	${Rocket_sources}
	${Rocket_headers}
	${RocketCore_sources}
	${RocketCore_headers}
)

project( RocketCore_UnitTests )

set( RocketCore_UnitTests_Sources
	UnitTest_fixedpoint.cpp
	UnitTest_vector.cpp
	UnitTest_matrix.cpp
	UnitTest_vector.cpp
)

add_test ( RocketCore_UnitTests ${RocketCore_UnitTests_Sources} )
target_link_libraries( RocketCore_UnitTests RocketCore )