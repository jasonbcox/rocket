cmake_minimum_required( VERSION 2.8 )
project( libsndfile )

set( libsndfile_sources
	sndfile.h
	sndfile.c
	common.h
	common.c
	chanmap.h
	chanmap.c
	command.c
	sfconfig.h
	config.h
	sfendian.h
	file_io.c
	strings.c
	float32.c
	double64.c
	mat4.c
	mat5.c
	chunk.c
	broadcast.c
	audio_detect.c
	dither.c
	aiff.c
	alaw.c
	au.c
	avr.c
	caf.c
	dwd.c
	dwvw.c
	flac.c
	g72x.c
	gsm610.c
	htk.c
	id3.c
	ima_adpcm.c
	ima_oki_adpcm.h
	ima_oki_adpcm.c
	ircam.c
	mpc2k.c
	ms_adpcm.c
	nist.c
	ogg.h
	ogg.c
	ogg_pcm.c
	ogg_speex.c
	ogg_vorbis.c
	paf.c
	pcm.c
	pvf.c
	raw.c
	rf64.c
	rx2.c
	sd2.c
	sds.c
	svx.c
	txw.c
	ulaw.c
	voc.c
	vox_adpcm.c
	w64.c
	wav.c
	wav_w64.h
	wav_w64.c
	wve.c
	xi.c
)

set( CMAKE_POSITION_INDEPENDENT_CODE ON )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )

add_subdirectory( GSM610 )
add_subdirectory( G72x )

add_library( libsndfile SHARED
	${libsndfile_sources}
)

target_link_libraries( libsndfile
	GSM610
	G72x
	FLAC
	ogg
	vorbis
	vorbisenc
)

