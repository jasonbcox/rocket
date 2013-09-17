cmake_minimum_required( VERSION 2.8 )
project( RtAudio )

set( RtAudio_main
	RtAudio.h
	RtError.h
	RtAudio.cpp
)
if( UNIX )
	set( RtAudio_defines "-DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__" )
	set( RtAudio_crossplatform )
elseif( WIN32 )
	set( RtAudio_defines )
	set( RtAudio_crossplatform
		asiodrvr.h
		asio.h
		asiolist.h
		asiosys.h
		dsound.h
		ginclude.h
		iasiodrv.h
		iasiothiscallresolver.h
		iasiothiscallresolver.cpp
		asio.cpp
		asiodrivers.cpp
		asiolist.cpp
	)
else()
	set( RtAudio_defines "" )
	set( RtAudio_crossplatform )
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RtAudio_defines}" )

add_library( RtAudio
	${RtAudio_main}
	${RtAudio_crossplatform}
)

if( UNIX )
	target_link_libraries( RtAudio
		pthread
		asound
	)
elseif( WIN32 )
endif()
