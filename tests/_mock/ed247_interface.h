/*
 * Copyright 2020 The MathWorks, Inc.
 */

#ifndef __ED247INTERFACE__
#define __ED247INTERFACE__

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_ELEMENTS 50
#define MAX_DIMENSIONS 10
#define STRING_MAX_LENGTH 1024

#define WAIT_FRAME_TIMEOUT_US 1000

#ifndef MATLAB_MEX_FILE

	typedef enum {
		SS_DOUBLE	= 0,
		SS_SINGLE	= 1,
		SS_INT8		= 2,
		SS_UINT8	= 3,
		SS_INT16	= 4,
		SS_UINT16	= 5,
		SS_INT32	= 6,
		SS_UINT32	= 7,
		SS_BOOLEAN	= 8,
		SS_INT64	= 90, /* NOTE : Type does not exist in r2016b but appears in 2019a (see https://www.mathworks.com/help/simulink/release-notes.html?rntext=int64) */
		SS_UINT64	= 91, /* NOTE : Type does not exist in r2016b but appears in 2019a (see https://www.mathworks.com/help/simulink/release-notes.html?rntext=uint64) */
		SS_INVALID	= 99,
	} BuiltInDTypeId ;

	#include <stdio.h>

#else

	#include "simstruc.h"
	#include "sl_types_def.h"

	/* NOTE : Type does not exist in r2016b but appears in 2019a (see https://www.mathworks.com/help/simulink/release-notes.html?rntext=int64) */
	#define SS_INT64 90
	#define SS_UINT64 91
	#define SS_INVALID	99

#endif

typedef enum {
	CONFIGURATION_SUCCESS = 0,
	INVALID_FILE,
	LOAD_FAILURE,
	FIND_STREAMS_FAILURE,
	STREAM_GET_INFO_FAILURE,
	STREAM_GET_ASSISTANT_FAILURE,
	FIND_STREAM_SIGNALS_FAILURE,
	SIGNAL_ALLOCATE_SAMPLE_FAILURE,
	SIGNAL_GET_INFO_FAILURE,
	SIGNAL_LIST_FREE_FAILURE,
	STREAM_LIST_FREE_FAILURE
} configuration_status_t;

typedef enum {
	SEND_OK = 0,
	NO_DATA_TO_SEND,
	STREAM_ASSISTANT_WRITE_SIGNAL_FAILURE,
	STREAM_ASSISTANT_PUSH_SAMPLE_FAILURE,
	STREAM_PUSH_SAMPLE_FAILURE,
	UNKNOWN_STREAM_TYPE,
	SEND_PUSHED_SAMPLES_FAILURE
} send_status_t;

typedef enum {
	RECEIVE_OK = 0,
	NO_DATA_TO_RECEIVE,
	STREAM_ASSISTANT_READ_SIGNAL_FAILURE,
	STREAM_POP_SAMPLE_FAILURE,
	NO_DATA_BEFORE_TIMEOUT,
	WAIT_FRAME_FAILURE
} receive_status_t;

typedef enum {
	IO_ALLOCATION_OK = 0,
	IO_MALLOC_FAILURE,
	INPUTS_MALLOC_FAILURE,
	OUTPUTS_MALLOC_FAILURE
} io_allocation_status_t;

typedef enum {
	IO_FREE_OK = 0,
	UNLOAD_FAILURE
} io_free_status_t;

typedef struct {
	char 						name[STRING_MAX_LENGTH];
	/*ed247_direction_t 			direction;*/
	BuiltInDTypeId 				type;
	int 						dimensions;
	int 						width;
	size_t 						size[MAX_DIMENSIONS];
	float 						sample_time;
	void 						*valuePtr;
	/*ed247_signal_type_t			signal_type;
	ed247_signal_t 				signal;
	void 						*sample;
	size_t 						sample_size;*/
} signal_characteristics_t;

typedef struct {
	char 						name[STRING_MAX_LENGTH];
	/*ed247_direction_t 		direction;
	int 						nsignals;
	ed247_stream_type_t 		stream_type;
	ed247_stream_t 				stream;
	ed247_stream_assistant_t 	assistant; 				// Only for ANA, DIS, NAD, VNAD
	char 						icd[STRING_MAX_LENGTH]; // Only for A664, A429
	char 						bus[STRING_MAX_LENGTH]; // Only for A664, A429
	signal_characteristics_t*	signals[MAX_ELEMENTS];*/
} stream_characteristics_t;

typedef struct {
	int nstreams;
	int nsignals;
	stream_characteristics_t streams[MAX_ELEMENTS];
	signal_characteristics_t signals[MAX_ELEMENTS];
} data_characteristics_t;

typedef struct {
	//ed247_context_t _context;
	data_characteristics_t *inputs;
	data_characteristics_t *outputs;
} IO_t;

/*
 * ED247 INTERFACE
 */
configuration_status_t read_ed247_configuration(const char* filename, IO_t *io, const char* logfilename);
send_status_t send_simulink_to_ed247(IO_t *io);
receive_status_t receive_ed247_to_simulink(IO_t *io, int *n);

/*
 * MEMORY MANAGEMENT
 */
io_allocation_status_t io_allocate_memory(IO_t ** io);
io_free_status_t io_free_memory(IO_t * io);

/*
 * DEBUG / HELPERS
 */
BuiltInDTypeId getDataType(char* type);
void myprintf(const char *fmt, ...);

#endif
