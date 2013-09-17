cmake_minimum_required( VERSION 2.8 )
project( Rocket )

set( RUN_TESTS TRUE CACHE BOOL "Run unit tests before finishing build." )

macro( add_test target )
	add_executable( ${target} ${ARGN} )
	if ( RUN_TESTS )
		run_test( ${target} ${${target}_BINARY_DIR} )
	endif()
endmacro()
 
macro (run_test target)
	get_property( targetBinaryLocation TARGET ${target} PROPERTY LOCATION )
	add_custom_command( TARGET ${target} POST_BUILD COMMAND ${BUILD_PROFILE_RUN} ${targetBinaryLocation} )
endmacro()

set( Rocket_headers
	${Rocket_SOURCE_DIR}/UnitTest.h
)

set( Rocket_sources
	${Rocket_SOURCE_DIR}/UnitTest.cpp
	${Rocket_SOURCE_DIR}/main.cpp
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/.. )


set( CMAKE_CXX_FLAGS "-Wall -std=c++11" )

add_executable( Rocket ${Rocket_sources} ${Rocket_headers} )

add_subdirectory( Core )
add_subdirectory( Graphics )
add_subdirectory( Network )
add_subdirectory( Audio )

install( TARGETS Rocket RUNTIME DESTINATION bin )
