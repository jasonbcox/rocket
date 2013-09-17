cmake_minimum_required( VERSION 2.8 )
project( GSM610 )

add_library( GSM610
	gsm.h
	gsm_encode.c
	gsm_decode.c
	gsm_create.c
	gsm_destroy.c
	code.c
	decode.c
	add.c
	gsm610_priv.h
	config.h
	gsm_option.c
	long_term.c
	lpc.c
	preprocess.c
	rpe.c
	short_term.c
	table.c
)

