cmake_minimum_required( VERSION 2.8 )
project( RocketNetwork )

set( RocketNetwork_headers
	Packet.h
	Network.h
)
set( RocketNetwork_sources
	Packet.cpp
	Network.cpp
	PacketAccumulator.cpp
)

add_library ( RocketNetwork
	${Rocket_sources}
	${Rocket_headers}
	${RocketNetwork_sources}
	${RocketNetwork_headers}
)
target_link_libraries( RocketNetwork RocketCore )

project( RocketNetwork_UnitTests )

set( RocketNetwork_UnitTests_sources
	UnitTest_Packet.cpp
	UnitTest_Network.cpp
)

add_test ( RocketNetwork_UnitTests ${RocketNetwork_UnitTests_sources} )
target_link_libraries( RocketNetwork_UnitTests RocketNetwork )
