cmake_minimum_required( VERSION 2.8 )
project( G72x )

add_library( G72x
	g72x.h
	g72x_priv.h
	g72x.c
	g721.c
	g723_16.c
	g723_24.c
	g723_40.c
)

