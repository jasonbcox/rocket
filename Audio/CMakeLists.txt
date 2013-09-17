cmake_minimum_required( VERSION 2.8 )
project( RocketAudio )

set( RocketAudio_headers
	AudioDevice.h
	Sound.h
)
set( RocketAudio_sources
	AudioDevice.cpp
	Sound.cpp
)

add_library( RocketAudio
	${Rocket_sources}
	${Rocket_headers}
	${RocketAudio_sources}
	${RocketAudio_headers}
)

add_subdirectory( rtaudio )
add_subdirectory( libsndfile )

target_link_libraries( RocketAudio
	RtAudio
	libsndfile
)

project ( RocketAudio_UnitTests )

set( RocketAudio_UnitTests_sources
	UnitTest_Sound.cpp
)

add_test( RocketAudio_UnitTests ${RocketAudio_UnitTests_sources} )
target_link_libraries( RocketAudio_UnitTests RocketAudio )

