cmake_minimum_required( VERSION 2.8 )
project( RocketGraphics )

set( RocketGraphics_headers
)
set( RocketGraphics_sources
)

add_library( RocketGraphics
	${Rocket_sources}
	${Rocket_headers}
	${RocketGraphics_sources}
	${RocketGraphics_headers}
)
