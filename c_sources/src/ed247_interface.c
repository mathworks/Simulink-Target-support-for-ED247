/*
 * Copyright 2020 The MathWorks, Inc.
 */

#include "ed247_interface.h"

/*
 * ED247 INTERFACE
 */
configuration_status_t read_ed247_configuration(const char* filename, IO_t *io, const char* logfilename){

	int i,streamidx;
	char folder[STRING_MAX_LENGTH];

    libed247_configuration_t    config = LIBED247_CONFIGURATION_DEFAULT;
	ed247_status_t				status;
	ed247_stream_list_t			streams;
	ed247_stream_t				stream;
	const ed247_stream_info_t*	stream_info;

	io->inputs->nsignals 	= 0;
	io->inputs->nstreams 	= 0;
	io->outputs->nsignals 	= 0;
	io->outputs->nstreams 	= 0;

	if (fileexists(filename) != 0){
		return INVALID_FILE;
	}
	fileparts(filename,folder);

    #ifndef DISABLE_LOG // Dependending on ED247 version, the log structure contains these additional fields or not 
    if (logfilename != NULL){
        config.log_filepath = logfilename;
        config.log_level = ED247_LOG_LEVEL_DEBUG;
    }
    #endif
    
	status = ed247_load(filename,&config,&(io->_context));
	if (checkStatus(status,"ed247_load",1)){return LOAD_FAILURE;}

	status = ed247_find_streams(io->_context,".*",&(streams));
	if (checkStatus(status,"ed247_find_streams",2)){return FIND_STREAMS_FAILURE;}
	
	while(ed247_stream_list_next(streams,&stream) == ED247_STATUS_SUCCESS && stream != NULL){

		status = ed247_stream_get_info(stream,&stream_info);
		if (checkStatus(status,"ed247_stream_get_info",3)){return STREAM_GET_INFO_FAILURE;}

		// Stream assistant is not available for all stream types
		if (stream_info->type == ED247_STREAM_TYPE_ANALOG || 
			stream_info->type == ED247_STREAM_TYPE_DISCRETE ||
			stream_info->type == ED247_STREAM_TYPE_NAD ||
			stream_info->type == ED247_STREAM_TYPE_VNAD)
		{
			local_signals_from_ecic(io, stream, stream_info);
		}

		// ICD is defined only for A664 and A429
		else if (stream_info->type == ED247_STREAM_TYPE_A429 || 
			stream_info->type == ED247_STREAM_TYPE_A664 || 
			stream_info->type == ED247_STREAM_TYPE_A825)
		{
			local_signals_from_icd(io, stream, stream_info, folder);
		}

	}

	status = ed247_stream_list_free(streams);
	if (checkStatus(status,"ed247_stream_list_free",1)){return STREAM_LIST_FREE_FAILURE;}

	return CONFIGURATION_SUCCESS;

}

send_status_t send_simulink_to_ed247(IO_t *io){

	int i, j;
	int status;

	if (io->inputs->nsignals == 0){
		myprintf("\t\t- %s : No inputs\n", "SimulinkToED247");
		return NO_DATA_TO_SEND;
	}

	for (i=0;i < io->inputs->nstreams; i++){

		// ANA, DIS, NAD and VNAD use assistant
		// A429 and A664 use push sample
		switch (io->inputs->streams[i].stream_type){

			case ED247_STREAM_TYPE_ANALOG:
			case ED247_STREAM_TYPE_DISCRETE:
			case ED247_STREAM_TYPE_NAD:
			case ED247_STREAM_TYPE_VNAD:

				for (j=0;j < io->inputs->streams[i].nsignals; j++){
					status = ed247_stream_assistant_write_signal(io->inputs->streams[i].assistant,io->inputs->streams[i].signals[j]->signal,io->inputs->streams[i].signals[j]->valuePtr,io->inputs->streams[i].signals[j]->sample_size);
					if (checkStatus(status,"ed247_stream_assistant_write_signal",3)){return STREAM_ASSISTANT_WRITE_SIGNAL_FAILURE;}
				}

				status = ed247_stream_assistant_push_sample(io->inputs->streams[i].assistant, NULL, NULL);
				if (checkStatus(status,"ed247_stream_assistant_push_sample",2)){return STREAM_ASSISTANT_PUSH_SAMPLE_FAILURE;}

				break;

			case ED247_STREAM_TYPE_A429:
			case ED247_STREAM_TYPE_A664:
			case ED247_STREAM_TYPE_A825:

				for (j=0;j < io->inputs->streams[i].nsignals; j++){
					status = ed247_stream_push_sample(io->inputs->streams[i].stream, io->inputs->streams[i].signals[j]->valuePtr,io->inputs->streams[i].signals[j]->sample_size, NULL, NULL);

					if (io->inputs->streams[i].stream_type == ED247_STREAM_TYPE_A825 && checkStatus(status,"ed247_stream_push_sample",3)){
						// Ignore failure on A825 (messages can be declared as input with no data to transmit)
						return SEND_A825_SKIP;
					} else if (checkStatus(status,"ed247_stream_push_sample",3)){return STREAM_PUSH_SAMPLE_FAILURE;}
				}

				break;

			default:
				return UNKNOWN_STREAM_TYPE;

		}
		
	}

	status = ed247_send_pushed_samples(io->_context);
	if (checkStatus(status,"ed247_send_pushed_samples",1)){return SEND_PUSHED_SAMPLES_FAILURE;}

	return SEND_OK;

}

receive_status_t receive_ed247_to_simulink(IO_t *io, int *n){

	int i,j;
	int status;
	ed247_stream_list_t	streams;
	bool empty;
	size_t sample_size;
	const void* sample_data;

	if (io->outputs->nsignals == 0){
		myprintf("\t- %s : No outputs\n", "ED247ToSimulink");
		return NO_DATA_TO_RECEIVE;
	}

	if (n != NULL){*n = 0;}/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */

	status = ed247_wait_frame(io->_context, &(streams), WAIT_FRAME_TIMEOUT_US);	
	if (ED247_STATUS_SUCCESS == status){

		myprintf("\t> %s OK\n", "ed247_wait_frame");

		for (i=0;i < io->outputs->nstreams; i++){

			// ANA, DIS, NAD and VNAD use assistant
			// A429, A664 and A825 use pop sample
			switch (io->outputs->streams[i].stream_type){

				case ED247_STREAM_TYPE_ANALOG:
				case ED247_STREAM_TYPE_DISCRETE:
				case ED247_STREAM_TYPE_NAD:
				case ED247_STREAM_TYPE_VNAD:

					status = ed247_stream_assistant_pop_sample(io->outputs->streams[i].assistant, NULL, NULL, NULL, &empty);
					if (status == ED247_STATUS_SUCCESS){

						for (j=0;j < io->outputs->streams[i].nsignals; j++){

							status = ed247_stream_assistant_read_signal(io->outputs->streams[i].assistant,io->outputs->streams[i].signals[j]->signal,&sample_data,&sample_size);
							if (checkStatus(status,"ed247_stream_assistant_read_signal",2)){return STREAM_ASSISTANT_READ_SIGNAL_FAILURE;}
							if (io->outputs->streams[i].signals[j]->valuePtr != NULL && sample_data != NULL){
								memcpy(io->outputs->streams[i].signals[j]->valuePtr,(void*)sample_data,io->outputs->streams[i].signals[j]->sample_size);
								if (n != NULL){(*n)++;}/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */

							}

						}

					}

					break;

				case ED247_STREAM_TYPE_A429:
				case ED247_STREAM_TYPE_A664:
				case ED247_STREAM_TYPE_A825:

					for (j=0;j < io->outputs->streams[i].nsignals; j++){

						status = ed247_stream_pop_sample(io->outputs->streams[i].stream,&sample_data,&sample_size, NULL, NULL, NULL, &empty);
						if (status == ED247_STATUS_SUCCESS && io->outputs->streams[i].signals[j]->valuePtr != NULL && sample_data != NULL){
							memcpy(io->outputs->streams[i].signals[j]->valuePtr,(void*)sample_data,io->outputs->streams[i].signals[j]->sample_size);
							if (n != NULL){(*n)++;}/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */

						}

					}

					break;

				default:
					return UNKNOWN_STREAM_TYPE;

			}
		}
		

	}
	else if (io->outputs->streams[0].stream_type == ED247_STREAM_TYPE_A825) { 
	// Do not generate an error if no A825 message is received 
	// All A825 are registered as input AND output messages but depending on the configuration it could be only In or only Out)
		return RECEIVE_A825_SKIP;
	}
	else if (status == ED247_STATUS_TIMEOUT) {
		
		myprintf("\t\t - No data received before timeout\n");
		return NO_DATA_BEFORE_TIMEOUT;
	}
	else {
		myprintf("\t\t!! %s returns invalid status\n", "ed247_wait_frame");
		return WAIT_FRAME_FAILURE;
	}

	return RECEIVE_OK;

}

/*
 * MEMORY MANAGEMENT
 */
io_allocation_status_t io_allocate_memory(IO_t ** io){

	if (((*io)=(IO_t*)malloc(sizeof(IO_t))) == NULL) {
		return IO_MALLOC_FAILURE;
	}
	(*io)->_context = NULL;

	if (((*io)->inputs=(data_characteristics_t*)malloc(sizeof(data_characteristics_t))) == NULL) {
		return INPUTS_MALLOC_FAILURE;
	}

	if (((*io)->outputs=(data_characteristics_t*)malloc(sizeof(data_characteristics_t))) == NULL) {
		return OUTPUTS_MALLOC_FAILURE;
	}

	return IO_ALLOCATION_OK;

}

io_free_status_t io_free_memory(IO_t *io){

	int i;
	int status;

	if (io->_context != NULL){/* polyspace RTE:UNR [Justified:Low] Robustness */
		status = ed247_unload(io->_context);
		if (checkStatus(status,"ed247_unload",1)){return UNLOAD_FAILURE;}
	}

	if (io->inputs != NULL) {/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */
		free(io->inputs);
	}

	if (io->outputs != NULL) {/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */
		free(io->outputs);
	}

	if (io != NULL) {/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */
		free(io);
	}

	return IO_FREE_OK;
}

/*
 * LOCAL FUNCTIONS
 */
  configuration_status_t local_signals_from_ecic(IO_t* io, ed247_stream_t stream, const ed247_stream_info_t*	stream_info){

	int i, streamidx;

	ed247_status_t				status;

	ed247_signal_list_t			signals;
	ed247_signal_t 				signal;
	const ed247_signal_info_t*	signal_info;

	data_characteristics_t* 	current_io;
	signal_characteristics_t	signal_data;

	if (stream_info->direction == ED247_DIRECTION_IN){
		current_io = io->outputs;
	} else if (stream_info->direction == ED247_DIRECTION_OUT) {
		current_io = io->inputs;
	} else {
		myprintf("!!! Stream direction unknown %s, skip\n", ed247_direction_string(stream_info->direction));
		return -1;
	}

	streamidx = current_io->nstreams;
	current_io->nstreams++;

	status = ed247_stream_get_assistant(stream, &(current_io->streams[streamidx].assistant));
	if (checkStatus(status,"ed247_stream_get_assistant",4)){return STREAM_GET_ASSISTANT_FAILURE;}

	strcpy(current_io->streams[streamidx].name,stream_info->name);
	current_io->streams[streamidx].stream_type 	= stream_info->type;
	current_io->streams[streamidx].stream 		= stream;
	current_io->streams[streamidx].nsignals 	= 0;
	current_io->streams[streamidx].direction = stream_info->direction;

	status = ed247_find_stream_signals(current_io->streams[streamidx].stream,".*",&signals);
	if (checkStatus(status,"ed247_find_stream_signals",4)){return FIND_STREAM_SIGNALS_FAILURE;}

	while(current_io->nsignals < MAX_ELEMENTS && ed247_signal_list_next(signals,&(signal)) == ED247_STATUS_SUCCESS && signal != NULL){

		signal_data.signal = signal;

		/* Signal */
		status = ed247_signal_allocate_sample(signal,&(signal_data.sample),&(signal_data.sample_size));
		if (checkStatus(status,"ed247_signal_allocate_sample",5)){return SIGNAL_ALLOCATE_SAMPLE_FAILURE;}

		status = ed247_signal_get_info(signal,&signal_info);
		if (checkStatus(status,"ed247_signal_get_info",5)){return SIGNAL_GET_INFO_FAILURE;}

		switch(stream_info->type){

			case ED247_STREAM_TYPE_DISCRETE:

				strcpy(signal_data.name,signal_info->name);
				signal_data.direction   = stream_info->direction;
				signal_data.type 		= SS_UINT8;
				signal_data.signal_type = signal_info->type;
				signal_data.dimensions	= 1;
				signal_data.width		= 1;
				signal_data.size[0]		= 1;
				signal_data.sample_time = (FLOAT_t) (stream_info->info.dis.sampling_period_us * 1e-6);

				break;

			case ED247_STREAM_TYPE_ANALOG:

				strcpy(signal_data.name,signal_info->name);
				signal_data.direction   = stream_info->direction;
				signal_data.type 		= SS_SINGLE;
				signal_data.signal_type = signal_info->type;
				signal_data.dimensions	= 1;
				signal_data.width		= 1;
				signal_data.size[0]		= 1;
				signal_data.sample_time = (FLOAT_t) (stream_info->info.ana.sampling_period_us * 1e-6);

				break;

			case ED247_STREAM_TYPE_NAD:

				strcpy(signal_data.name,signal_info->name);
				signal_data.direction	= stream_info->direction;
				signal_data.type 		= NAD2SimulinkDataType(signal_info->info.nad.nad_type);
				signal_data.signal_type	= signal_info->type;
				signal_data.dimensions	= signal_info->info.nad.dimensions_count;
				signal_data.width		= 1;
				for (i = 0; i < signal_info->info.nad.dimensions_count; i++){
					if (i >= MAX_DIMENSIONS){ return SIGNAL_GET_INFO_FAILURE; };
					signal_data.size[i]	=  signal_info->info.nad.dimensions[i];
					signal_data.width 	*= signal_info->info.nad.dimensions[i];
				}
				signal_data.sample_time	= (FLOAT_t) (stream_info->info.nad.sampling_period_us * 1e-6);

				break;

			case ED247_STREAM_TYPE_VNAD:

				strcpy(signal_data.name,signal_info->name);
				signal_data.direction	= stream_info->direction;
				signal_data.type 		= NAD2SimulinkDataType(signal_info->info.vnad.nad_type);
				signal_data.signal_type	= signal_info->type;
				signal_data.dimensions	= 1;
				signal_data.width		= signal_info->info.vnad.max_length;
				signal_data.size[0]		= signal_info->info.vnad.max_length;
				signal_data.sample_time	= (FLOAT_t) (stream_info->info.vnad.sampling_period_us * 1e-6);

				break;

			default:
				break;

		}

		/* Assign signal data in IO structure */
		current_io->signals[current_io->nsignals] = signal_data;
		current_io->streams[streamidx].signals[current_io->streams[streamidx].nsignals] = &(current_io->signals[current_io->nsignals]);
		current_io->streams[streamidx].nsignals++;
		current_io->nsignals++;

	}

	status = ed247_signal_list_free(signals);
	if (checkStatus(status,"ed247_signal_list_free",4)){return SIGNAL_LIST_FREE_FAILURE;}

	return CONFIGURATION_SUCCESS;

 }
 
 configuration_status_t local_signals_from_icd(IO_t* io, ed247_stream_t stream, const ed247_stream_info_t*	stream_info, char* folder){

	int i,j,streamidx;
	char tmpstr[STRING_MAX_LENGTH];
	char *ptr;

	char icdname[STRING_MAX_LENGTH];
	char busname[STRING_MAX_LENGTH];
	char icdfullpath[STRING_MAX_LENGTH];

	cmd_read_status_t 			status;
	cmd_data_t*					data;

	data_characteristics_t* 	current_io;
	stream_characteristics_t*	current_stream_in;
	stream_characteristics_t*	current_stream_out;
	stream_characteristics_t*	current_stream;
	stream_characteristics_t*	input_stream;
	stream_characteristics_t*	output_stream;
	signal_characteristics_t*	current_signal;

	if (cmd_allocate_memory(&data) != CMD_ALLOCATION_OK){return LOAD_FAILURE;}

	// Get ICD file name and current stream bus name
	strcpy(tmpstr,stream_info->icd);
	ptr = strtok(tmpstr, ":");
	if (ptr != NULL){
		strcpy(icdname,ptr);
		ptr = strtok(NULL, ":");
		if (ptr != NULL){
			strcpy(busname,ptr);
		}
	}
	sprintf(icdfullpath, "%s%s%s", folder, FILESEP, icdname);

	// Read CMD file
	status = cmd_read_data(icdfullpath, data);
	if (status != CMD_READ_OK){return LOAD_FAILURE;}

	switch (stream_info->type){

		case ED247_STREAM_TYPE_A429:

			for (i = 0; i<data->counter.a429; i++){

				if (strcmp(data->a429[i].name,busname) == 0){

					current_stream_in 	= NULL;
					current_stream_out 	= NULL;

					for (j = 0; j<data->a429[i].n_messages; j++){

						switch (data->a429[i].messages[j].direction){

							case ED247_DIRECTION_IN:

								current_io = io->outputs;
								if (current_stream_out == NULL){
									current_stream_out = &(current_io->streams[current_io->nstreams]);
									current_stream_out->nsignals = 0;
									current_io->nstreams++;
								}
								current_stream = current_stream_out;
								break;

							case ED247_DIRECTION_OUT:

								current_io = io->inputs;
								if (current_stream_in == NULL){
									current_stream_in = &(current_io->streams[current_io->nstreams]);
									current_stream_in->nsignals = 0;
									current_io->nstreams++;
								}
								current_stream = current_stream_in;
								break;

							default:
								continue;

						}

						strcpy(current_stream->name,stream_info->name);
						strcpy(current_stream->icd,icdname);
						strcpy(current_stream->bus,busname);
						current_stream->stream_type = stream_info->type;
						current_stream->stream 		= stream;
						current_stream->direction 	= stream_info->direction;

						current_signal = &(current_io->signals[current_io->nsignals]);

						strcpy(current_signal->name,data->a429[i].messages[j].name);
						current_signal->direction		= data->a429[i].messages[j].direction;
						current_signal->signal_type 	= data->a429[i].messages[j].type;
						current_signal->type 			= A429_DATA_TYPE;
						current_signal->dimensions		= 1;
						current_signal->width			= A429_DATA_SIZE;
						current_signal->size[0]			= A429_DATA_SIZE;
						current_signal->sample_size 	= A429_DATA_SIZE;
						current_signal->sample_time 	= (float) (data->a429[i].messages[j].period_us * 1e-6);

						current_stream->signals[current_stream->nsignals] = current_signal;
						current_stream->nsignals++;
						current_io->nsignals++;

					}

				}

			}

			break;

		case ED247_STREAM_TYPE_A664:

			switch (stream_info->direction){

				case ED247_DIRECTION_IN:
					current_io = io->outputs;
					break;

				case ED247_DIRECTION_OUT:
					current_io = io->inputs;
					break;

				default:
					return -1;

			}

			current_stream = &(current_io->streams[current_io->nstreams]);
			current_stream->nsignals = 0;
			current_io->nstreams++;

			strcpy(current_stream->name,stream_info->name);
			strcpy(current_stream->icd,icdname);
			strcpy(current_stream->bus,busname);
			current_stream->stream_type = stream_info->type;
			current_stream->stream 		= stream;
			current_stream->direction 	= stream_info->direction;

			for (i = 0; i<data->counter.a664; i++){

				if (strcmp(data->a664[i].name,busname) == 0){
					
					current_signal = &(current_io->signals[current_io->nsignals]);

					strcpy(current_signal->name,data->a664[i].name);
					current_signal->direction		= data->a664[i].direction;
					current_signal->type 			= A664_DATA_TYPE;
					current_signal->dimensions		= 1;
					current_signal->width			= data->a664[i].sample_max_size_bytes;
					current_signal->size[0]			= data->a664[i].sample_max_size_bytes;
					current_signal->sample_size 	= data->a664[i].sample_max_size_bytes * sizeof(char);
					current_signal->sample_time 	= (float) (data->a664[i].period_us * 1e-6);

					current_stream->signals[current_stream->nsignals] = current_signal;
					current_stream->nsignals++;
					current_io->nsignals++;

				}

			}

			break;

		case ED247_STREAM_TYPE_A825:
		/*
		 * A825 messages are IN/OUT
		 * Direction is controlled by the socket parametrized in XML (out of scope for this code)
		 * Consider each A825 message as 1 input AND 1 output
		 */

			// Inputs
			input_stream = &(io->inputs->streams[io->inputs->nstreams]);
			input_stream->nsignals = 0;
			io->inputs->nstreams++;

			strcpy(input_stream->name,stream_info->name);
			strcpy(input_stream->icd,icdname);
			strcpy(input_stream->bus,busname);
			input_stream->stream_type 	= stream_info->type;
			input_stream->stream 		= stream;
			input_stream->direction 	= ED247_DIRECTION_IN;

			// Outputs
			output_stream = &(io->outputs->streams[io->outputs->nstreams]);
			output_stream->nsignals = 0;
			io->outputs->nstreams++;

			strcpy(output_stream->name,stream_info->name);
			strcpy(output_stream->icd,icdname);
			strcpy(output_stream->bus,busname);
			output_stream->stream_type 	= stream_info->type;
			output_stream->stream 		= stream;
			output_stream->direction 	= ED247_DIRECTION_OUT;

			for (i = 0; i<data->counter.a825; i++){

				if (strcmp(data->a825[i].name,busname) == 0){

					current_signal = &(io->inputs->signals[io->inputs->nsignals]);

					strcpy(current_signal->name,data->a825[i].name);
					current_signal->direction		= ED247_DIRECTION_IN;
					current_signal->type 			= A825_DATA_TYPE;
					current_signal->dimensions		= 1;
					current_signal->width			= data->a825[i].sample_max_number;
					current_signal->size[0]			= data->a825[i].sample_max_number;
					current_signal->sample_size 	= data->a825[i].sample_max_number * sizeof(char);
					current_signal->sample_time 	= (float) (data->a825[i].period_us * 1e-6);

					input_stream->signals[input_stream->nsignals] = current_signal;
					input_stream->nsignals++;
					io->inputs->nsignals++;

					current_signal = &(io->outputs->signals[io->outputs->nsignals]);

					strcpy(current_signal->name,data->a825[i].name);
					current_signal->direction		= ED247_DIRECTION_OUT;
					current_signal->type 			= A825_DATA_TYPE;
					current_signal->dimensions		= 1;
					current_signal->width			= data->a825[i].sample_max_number;
					current_signal->size[0]			= data->a825[i].sample_max_number;
					current_signal->sample_size 	= data->a825[i].sample_max_number * sizeof(char);
					current_signal->sample_time 	= (float) (data->a825[i].period_us * 1e-6);

					output_stream->signals[output_stream->nsignals] = current_signal;
					output_stream->nsignals++;
					io->outputs->nsignals++;

				}

			}

			break;

		default:/* polyspace RTE:UNR [Justified:Low] Defensive code */
			return -1;

	}

	cmd_free_memory(data);
	return CONFIGURATION_SUCCESS;

 }

/*
 * DEBUG / HELPERS
 */
BuiltInDTypeId NAD2SimulinkDataType(ed247_nad_type_t type){

	switch (type){
		case ED247_NAD_TYPE_INT8:
			return SS_INT8;
		case ED247_NAD_TYPE_INT16:
			return SS_INT16;
		case ED247_NAD_TYPE_INT32:
			return SS_INT32;
		case ED247_NAD_TYPE_INT64:
			return SS_INT64;
		case ED247_NAD_TYPE_UINT8:
			return SS_UINT8;
		case ED247_NAD_TYPE_UINT16:
			return SS_UINT16;
		case ED247_NAD_TYPE_UINT32:
			return SS_UINT32;
		case ED247_NAD_TYPE_UINT64:
			return SS_UINT64;
		case ED247_NAD_TYPE_FLOAT32:
			return SS_SINGLE;
		case ED247_NAD_TYPE_FLOAT64:
			return SS_DOUBLE;
		default:
			return SS_INVALID;
	}

}

int sizeofNADDataType(ed247_nad_type_t type){

	switch (type){
		case ED247_NAD_TYPE_INT8:
			return sizeof(INT8_t);
		case ED247_NAD_TYPE_INT16:
			return sizeof(INT16_t);
		case ED247_NAD_TYPE_INT32:
			return sizeof(INT32_t);
		case ED247_NAD_TYPE_INT64:
			return sizeof(INT64_t);
		case ED247_NAD_TYPE_UINT8:
			return sizeof(UINT8_t);
		case ED247_NAD_TYPE_UINT16:
			return sizeof(UINT16_t);
		case ED247_NAD_TYPE_UINT32:
			return sizeof(UINT32_t);
		case ED247_NAD_TYPE_UINT64:
			return sizeof(UINT64_t);
		case ED247_NAD_TYPE_FLOAT32:
			return sizeof(FLOAT_t);
		case ED247_NAD_TYPE_FLOAT64:
			return sizeof(DOUBLE_t);
		default:
			return 0;
	}

}

char checkStatus(ed247_status_t status, char* fcnname, int level){

	int i;
	char format[1024] = "";
	for (i=0; i< level; i++){
		strcat(format,"\t");
	}

	if (ED247_STATUS_SUCCESS != status){
		strcat(format,"!! %s returns invalid status (%d / %d)\n");
		myprintf(format, fcnname, status, ED247_STATUS_SUCCESS);
		return 1;
	} else {
		strcat(format,"> %s OK\n");
		myprintf(format, fcnname);
		return 0;
	}

}
