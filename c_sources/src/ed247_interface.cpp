/*
 * Copyright 2020 The MathWorks, Inc.
 */

#include "ed247_interface.h"

namespace ed247simulink {

/*
 * CONSTRUCTORS
 */
ED247Connector::ED247Connector(){
	//_tools = Tools();
	_filename = NULL;
	_logfilename = NULL;
}
ED247Connector::ED247Connector(const char* filename){
	//_tools = Tools();
	_filename = filename;
	_logfilename = NULL;
}
ED247Connector::ED247Connector(const char* filename,const char* logfilename){
	//_tools = Tools();
	_filename = filename;
	_logfilename = logfilename;
}
ED247Connector::ED247Connector(const char* filename,Tools *tools){
	_tools = tools;
	_filename = filename;
	_logfilename = NULL;
}
ED247Connector::ED247Connector(const char* filename,const char* logfilename,Tools *tools){
	_tools = tools;
	_filename = filename;
	_logfilename = logfilename;
}

/*
 * ACCESSORS
 */
data_characteristics_t* ED247Connector::getInputs(){
	return _io->inputs;
}

data_characteristics_t* ED247Connector::getOutputs(){
	return _io->outputs;
}

/*
 * ED247 INTERFACE
 */
configuration_status_t ED247Connector::readED247Configuration(){

	int i,streamidx;
	char folder[STRING_MAX_LENGTH];

	libed247_configuration_t	config = LIBED247_CONFIGURATION_DEFAULT;
	configuration_status_t		read_status;
	ed247_status_t				status;
	ed247_stream_list_t			streams;
	ed247_stream_t				stream;
	const ed247_stream_info_t*	stream_info;

	_io->inputs->nsignals 	= 0;
	_io->inputs->nstreams 	= 0;
	_io->outputs->nsignals 	= 0;
	_io->outputs->nstreams 	= 0;

	if (_tools->fileexists(_filename) != 0){
		return INVALID_FILE;
	}
	_tools->fileparts(_filename,folder);

    #ifndef DISABLE_LOG // Dependending on ED247 version, the log structure contains these additional fields or not 
    if (_logfilename != NULL){
        config.log_filepath = _logfilename;
        config.log_level = ED247_LOG_LEVEL_DEBUG;
    }
    #endif
    
	status = ed247_load(_filename,&config,&(_io->_context));
	if (checkStatus(status,"ed247_load",1)){return LOAD_FAILURE;}

	status = ed247_find_streams(_io->_context,".*",&(streams));
	if (checkStatus(status,"ed247_find_streams",2)){return FIND_STREAMS_FAILURE;}

	status = ed247_stream_list_next(streams,&stream);
	checkStatus(status,"ed247_stream_list_next",2);
	while(status == ED247_STATUS_SUCCESS && stream != NULL){

		status = ed247_stream_get_info(stream,&stream_info);
		if (checkStatus(status,"ed247_stream_get_info",3)){return STREAM_GET_INFO_FAILURE;}

		// Stream assistant is not available for all stream types
		if (stream_info->type == ED247_STREAM_TYPE_ANALOG || 
			stream_info->type == ED247_STREAM_TYPE_DISCRETE ||
			stream_info->type == ED247_STREAM_TYPE_NAD ||
			stream_info->type == ED247_STREAM_TYPE_VNAD)
		{
			read_status = localSignalsFromECIC(stream, stream_info);
			if (read_status == STREAM_REACH_ARRAY_LIMIT){_tools->myprintf("WARNING: Stream array limit reached (%d), some data may be skipped\n", MAX_STREAMS);}
		}

		// ICD is defined only for A429, A664 and A825
		else if (stream_info->type == ED247_STREAM_TYPE_A429 || 
			stream_info->type == ED247_STREAM_TYPE_A664 || 
			stream_info->type == ED247_STREAM_TYPE_A825)
		{
			read_status = localSignalsFromICD(stream, stream_info, folder);
			if (read_status == STREAM_REACH_ARRAY_LIMIT){
				_tools->myprintf("WARNING: Stream array limit reached (%d), some data may be skipped\n", MAX_STREAMS);
			}
		}
		status = ed247_stream_list_next(streams,&stream);
		checkStatus(status,"ed247_stream_list_next",2);
		if (stream == NULL){_tools->myprintf("\t\t> No more stream\n");}

	}

	status = ed247_stream_list_free(streams);
	if (checkStatus(status,"ed247_stream_list_free",1)){return STREAM_LIST_FREE_FAILURE;}

	return CONFIGURATION_SUCCESS;

}

send_status_t ED247Connector::sendSimulinkToED247(){

	int i, j;
	ed247_status_t status;

	if (_io->inputs->nsignals == 0){
		_tools->myprintf("\t\t- %s : No inputs\n", "SimulinkToED247");
		return NO_DATA_TO_SEND;
	}

	for (i=0;i < _io->inputs->nstreams && i < MAX_STREAMS; i++){

		// ANA, DIS, NAD and VNAD use assistant
		// A429, A664 and A825 use push sample
		switch (_io->inputs->streams[i].stream_type){

			case ED247_STREAM_TYPE_ANALOG:
			case ED247_STREAM_TYPE_DISCRETE:
			case ED247_STREAM_TYPE_NAD:
			case ED247_STREAM_TYPE_VNAD:

				for (j=0;j < _io->inputs->streams[i].nsignals && j < MAX_SIGNALS; j++){
                    
                    if (_io->inputs->streams[i].signals[j]->do_refresh == 1){
                        status = ed247_stream_assistant_write_signal(_io->inputs->streams[i].assistant,_io->inputs->streams[i].signals[j]->signal,_io->inputs->streams[i].signals[j]->valuePtr,_io->inputs->streams[i].signals[j]->sample_size);
                        if (checkStatus(status,"ed247_stream_assistant_write_signal",3)){return STREAM_ASSISTANT_WRITE_SIGNAL_FAILURE;}
                    }
                    
				}

				status = ed247_stream_assistant_push_sample(_io->inputs->streams[i].assistant, NULL, NULL);
				if (checkStatus(status,"ed247_stream_assistant_push_sample",2)){return STREAM_ASSISTANT_PUSH_SAMPLE_FAILURE;}

				break;

			case ED247_STREAM_TYPE_A429:
			case ED247_STREAM_TYPE_A664:
			case ED247_STREAM_TYPE_A825:

				for (j=0;j < _io->inputs->streams[i].nsignals && j < MAX_SIGNALS; j++){
                    
                    _tools->myprintf("Refresh value for signal #%d = %d\n", j, _io->inputs->streams[i].signals[j]->do_refresh);
                    if (_io->inputs->streams[i].signals[j]->do_refresh == 1){
                        _tools->myprintf("Push sample for signal #%d\n", j);
                        status = ed247_stream_push_sample(_io->inputs->streams[i].stream, _io->inputs->streams[i].signals[j]->valuePtr,_io->inputs->streams[i].signals[j]->sample_size, NULL, NULL);
                    } else {
                        status = ED247_STATUS_SUCCESS;
                    }

					if (_io->inputs->streams[i].stream_type == ED247_STREAM_TYPE_A825 && checkStatus(status,"ed247_stream_push_sample",3)){
						// Ignore failure on A825 (messages can be declared as input with no data to transmit)
						return SEND_A825_SKIP;
					} else if (checkStatus(status,"ed247_stream_push_sample",3)){return STREAM_PUSH_SAMPLE_FAILURE;}
				}

				break;

			default:
				return UNKNOWN_STREAM_TYPE;

		}
		
	}

	status = ed247_send_pushed_samples(_io->_context);
	if (checkStatus(status,"ed247_send_pushed_samples",1)){return SEND_PUSHED_SAMPLES_FAILURE;}

	return SEND_OK;

}

receive_status_t ED247Connector::receiveED247ToSimulink(int *n){

	int i,j;
	ed247_status_t status;
	ed247_stream_list_t	streams;
	bool empty;
	size_t sample_size;
	const void* sample_data;

	if (_io->outputs->nsignals == 0){
		_tools->myprintf("\t- %s : No outputs\n", "ED247ToSimulink");
		return NO_DATA_TO_RECEIVE;
	}

	if (n != NULL){*n = 0;}/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */

	status = ed247_wait_frame(_io->_context, &(streams), WAIT_FRAME_TIMEOUT_US);	
	if (ED247_STATUS_SUCCESS == status){

		_tools->myprintf("\t> %s OK\n", "ed247_wait_frame");

		for (i=0;i < _io->outputs->nstreams && i < MAX_STREAMS; i++){

			// ANA, DIS, NAD and VNAD use assistant
			// A429, A664 and A825 use pop sample
			switch (_io->outputs->streams[i].stream_type){

				case ED247_STREAM_TYPE_ANALOG:
				case ED247_STREAM_TYPE_DISCRETE:
				case ED247_STREAM_TYPE_NAD:
				case ED247_STREAM_TYPE_VNAD:

					status = ed247_stream_assistant_pop_sample(_io->outputs->streams[i].assistant, NULL, NULL, NULL, &empty);
					if (status == ED247_STATUS_SUCCESS){

						_tools->myprintf("\t> %s OK\n", "ed247_stream_assistant_pop_sample");

						for (j=0;j < _io->outputs->streams[i].nsignals && j < MAX_SIGNALS; j++){

							status = ed247_stream_assistant_read_signal(_io->outputs->streams[i].assistant,_io->outputs->streams[i].signals[j]->signal,&sample_data,&sample_size);
							if (checkStatus(status,"ed247_stream_assistant_read_signal",2)){return STREAM_ASSISTANT_READ_SIGNAL_FAILURE;}
							if (_io->outputs->streams[i].signals[j]->valuePtr != NULL && sample_data != NULL){

								memcpy(_io->outputs->streams[i].signals[j]->valuePtr,(void*)sample_data,_io->outputs->streams[i].signals[j]->sample_size);
								if (n != NULL){(*n)++;}/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */

								_io->outputs->streams[i].signals[j]->do_refresh = 1;

							} else {
								_io->outputs->streams[i].signals[j]->do_refresh = 0;
							}

						}

					}

					break;

				case ED247_STREAM_TYPE_A429:
				case ED247_STREAM_TYPE_A664:
				case ED247_STREAM_TYPE_A825:

					for (j=0;j < _io->outputs->streams[i].nsignals && j < MAX_SIGNALS; j++){

						_tools->myprintf("\t> signal #%d/#%d", j+1, _io->outputs->streams[i].nsignals);

						status = ed247_stream_pop_sample(_io->outputs->streams[i].stream,&sample_data,&sample_size, NULL, NULL, NULL, &empty);
						if (status == ED247_STATUS_SUCCESS && _io->outputs->streams[i].signals[j]->valuePtr != NULL && sample_data != NULL){

							_tools->myprintf("\t> %s OK\n", "ed247_stream_pop_sample");

							memcpy(_io->outputs->streams[i].signals[j]->valuePtr,(void*)sample_data,_io->outputs->streams[i].signals[j]->sample_size);
							if (n != NULL){(*n)++;}/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */

							_io->outputs->streams[i].signals[j]->do_refresh = 1;

						} else {
							_tools->myprintf("\t> %s NOK\n", "ed247_stream_pop_sample");
							_io->outputs->streams[i].signals[j]->do_refresh = 0;
						}

					}

					break;

				default:
					return UNKNOWN_RECEIVE_STREAM_TYPE;

			}
		}
		

	}
	else if (_io->outputs->streams[0].stream_type == ED247_STREAM_TYPE_A825) { 
	// Do not generate an error if no A825 message is received 
	// All A825 are registered as input AND output messages but depending on the configuration it could be only In or only Out)

		for (i=0;i < _io->outputs->nstreams && i < MAX_STREAMS; i++){
			for (j=0;j < _io->outputs->streams[i].nsignals && j < MAX_SIGNALS; j++){
				_io->outputs->streams[i].signals[j]->do_refresh = 0;
			}
		}

		return RECEIVE_A825_SKIP;
	}
	else if (status == ED247_STATUS_TIMEOUT) {

		for (i=0;i < _io->outputs->nstreams && i < MAX_STREAMS; i++){
			for (j=0;j < _io->outputs->streams[i].nsignals && j < MAX_SIGNALS; j++){
				_io->outputs->streams[i].signals[j]->do_refresh = 0;
			}
		}

		_tools->myprintf("\t\t - No data received before timeout\n");
		return NO_DATA_BEFORE_TIMEOUT;
	}
	else {

		for (i=0;i < _io->outputs->nstreams && i < MAX_STREAMS; i++){
			for (j=0;j < _io->outputs->streams[i].nsignals && j < MAX_SIGNALS; j++){
				_io->outputs->streams[i].signals[j]->do_refresh = 0;
			}
		}

		_tools->myprintf("\t\t!! %s returns invalid status\n", "ed247_wait_frame");
		return WAIT_FRAME_FAILURE;
	}

	return RECEIVE_OK;

}

/*
 * MEMORY MANAGEMENT
 */
io_allocation_status_t ED247Connector::allocateMemory(){

	if ((_io=(IO_t*)malloc(sizeof(IO_t))) == NULL) {
		return IO_MALLOC_FAILURE;
	}
	_io->_context = NULL;

	if ((_io->inputs=(data_characteristics_t*)malloc(sizeof(data_characteristics_t))) == NULL) {
		return INPUTS_MALLOC_FAILURE;
	}

	if ((_io->outputs=(data_characteristics_t*)malloc(sizeof(data_characteristics_t))) == NULL) {
		return OUTPUTS_MALLOC_FAILURE;
	}

	return IO_ALLOCATION_OK;

}

io_free_status_t ED247Connector::freeMemory(){

	int i;
	ed247_status_t status;

	if (_io->_context != NULL){/* polyspace RTE:UNR [Justified:Low] Robustness */
		status = ed247_unload(_io->_context);
		if (checkStatus(status,"ed247_unload",1)){return UNLOAD_FAILURE;}
	}

	if (_io->inputs != NULL) {/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */
		free(_io->inputs);
	}

	if (_io->outputs != NULL) {/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */
		free(_io->outputs);
	}

	if (_io != NULL) {/* polyspace DEFECT:USELESS_IF RTE:UNR [Justified:Low] Robustness */
		free(_io);
	}

	return IO_FREE_OK;
}

/*
 * DISPLAY
 */
void ED247Connector::displayConfiguration(){

	_tools->myprintf("=== ED247 connector ==================================\n\n");
	_tools->myprintf("\tConfiguration '%s'\n", _filename);
	_tools->myprintf("\n\n");

	if (_io != NULL) {

		_tools->myprintf("\n--- INPUTS ----------------------------------------\n");
		if (_io->inputs != NULL){
			_tools->myprintf("\t- Streams = %d\n", _io->inputs->nstreams);
			_tools->myprintf("\t- Signals = %d\n", _io->inputs->nsignals);
		} else {
			_tools->myprintf("WARNING : NULL pointer\n");
		}

		_tools->myprintf("\n--- OUTPUTS ----------------------------------------\n");
		if (_io->outputs != NULL){
			_tools->myprintf("\t- Streams = %d\n", _io->outputs->nstreams);
			_tools->myprintf("\t- Signals = %d\n", _io->outputs->nsignals);
		} else {
			_tools->myprintf("WARNING : NULL pointer\n");
		}

	} else {
		_tools->myprintf("WARNING : NULL pointer for IOs\n");
	}

	_tools->myprintf("\n\n===============================================\n\n");

}

/*
 * LOCAL FUNCTIONS
 */
  configuration_status_t ED247Connector::localSignalsFromECIC(ed247_stream_t stream, const ed247_stream_info_t*	stream_info){

	int i, streamidx;

	ed247_status_t				status;

	ed247_signal_list_t			signals;
	ed247_signal_t 				signal;
	const ed247_signal_info_t*	signal_info;

	data_characteristics_t* 	current_io;
	signal_characteristics_t	signal_data;

	if (stream_info->direction == ED247_DIRECTION_IN){
		current_io = _io->outputs;
	} else if (stream_info->direction == ED247_DIRECTION_OUT) {
		current_io = _io->inputs;
	} else {
		_tools->myprintf("!!! Stream direction unknown %s, skip\n", ed247_direction_string(stream_info->direction));
		return INVALID_DIRECTION;
	}

	streamidx = current_io->nstreams;
	current_io->nstreams++;
	if (streamidx >= MAX_STREAMS){return STREAM_REACH_ARRAY_LIMIT;}

	status = ed247_stream_get_assistant(stream, &(current_io->streams[streamidx].assistant));
	if (checkStatus(status,"ed247_stream_get_assistant",4)){return STREAM_GET_ASSISTANT_FAILURE;}

	strcpy(current_io->streams[streamidx].name,stream_info->name);
	current_io->streams[streamidx].stream_type 	= stream_info->type;
	current_io->streams[streamidx].stream 		= stream;
	current_io->streams[streamidx].nsignals 	= 0;
	current_io->streams[streamidx].direction = stream_info->direction;

	status = ed247_find_stream_signals(current_io->streams[streamidx].stream,".*",&signals);
	if (checkStatus(status,"ed247_find_stream_signals",4)){return FIND_STREAM_SIGNALS_FAILURE;}

	while(current_io->nsignals < MAX_SIGNALS && ed247_signal_list_next(signals,&(signal)) == ED247_STATUS_SUCCESS && signal != NULL){

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
				signal_data.is_refresh  = 0;
				signal_data.signal_type = signal_info->type;
				signal_data.stream_type = stream_info->type;
				signal_data.dimensions	= 1;
				signal_data.width		= 1;
				signal_data.size[0]		= 1;
				signal_data.sample_time = (FLOAT_t) (stream_info->info.dis.sampling_period_us * 1e-6);

				break;

			case ED247_STREAM_TYPE_ANALOG:

				strcpy(signal_data.name,signal_info->name);
				signal_data.direction   = stream_info->direction;
				signal_data.type 		= SS_SINGLE;
				signal_data.is_refresh  = 0;
				signal_data.signal_type = signal_info->type;
				signal_data.stream_type = stream_info->type;
				signal_data.dimensions	= 1;
				signal_data.width		= 1;
				signal_data.size[0]		= 1;
				signal_data.sample_time = (FLOAT_t) (stream_info->info.ana.sampling_period_us * 1e-6);

				break;

			case ED247_STREAM_TYPE_NAD:

				strcpy(signal_data.name,signal_info->name);
				signal_data.direction	= stream_info->direction;
				signal_data.type 		= NAD2SimulinkDataType(signal_info->info.nad.nad_type);
				signal_data.is_refresh  = 0;
				signal_data.signal_type	= signal_info->type;
				signal_data.stream_type = stream_info->type;
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
				signal_data.is_refresh  = 0;
				signal_data.signal_type	= signal_info->type;
				signal_data.stream_type = stream_info->type;
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
	if (current_io->nsignals == MAX_SIGNALS){_tools->myprintf("WARNING: Limit of maximum signal number reached (%d), some signals may be missing\n", MAX_SIGNALS);}

	status = ed247_signal_list_free(signals);
	if (checkStatus(status,"ed247_signal_list_free",4)){return SIGNAL_LIST_FREE_FAILURE;}

	return CONFIGURATION_SUCCESS;

 }
 
 configuration_status_t ED247Connector::localSignalsFromICD(ed247_stream_t stream, const ed247_stream_info_t*	stream_info, char* folder){

	int i,j,streamidx;
	char tmpstr[STRING_MAX_LENGTH];
	char *ptr;

	char icdname[STRING_MAX_LENGTH];
	char busname[STRING_MAX_LENGTH];
	char icdfullpath[STRING_MAX_LENGTH];

	cmd_read_status_t 			status;
	static cmd_data_t			data;

	Cmd cmd = Cmd(_tools);

	data_characteristics_t* 	current_io;
	stream_characteristics_t*	current_stream_in;
	stream_characteristics_t*	current_stream_out;
	stream_characteristics_t*	current_stream;
	stream_characteristics_t*	input_stream;
	stream_characteristics_t*	output_stream;
	signal_characteristics_t*	current_signal;

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
	status = cmd.readData(icdfullpath, &data);
	if (status != CMD_READ_OK){return LOAD_FAILURE;}

	switch (stream_info->type){

		case ED247_STREAM_TYPE_A429:

			for (i = 0; i<data.counter.a429; i++){

				if (strcmp(data.a429[i].name,busname) == 0){

					current_stream_in 	= NULL;
					current_stream_out 	= NULL;

					for (j = 0; j<data.a429[i].n_messages; j++){

						switch (data.a429[i].messages[j].direction){

							case ED247_DIRECTION_IN:

								current_io = _io->outputs;
								if (current_io->nstreams >= MAX_STREAMS){return STREAM_REACH_ARRAY_LIMIT;}
								if (current_stream_out == NULL){
									current_stream_out = &(current_io->streams[current_io->nstreams]);
									current_stream_out->nsignals = 0;
									current_io->nstreams++;
								}
								current_stream = current_stream_out;
								break;

							case ED247_DIRECTION_OUT:

								current_io = _io->inputs;
								if (current_io->nstreams >= MAX_STREAMS){return STREAM_REACH_ARRAY_LIMIT;}
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
						strcpy(current_stream->filename,icdfullpath);
						strcpy(current_stream->icd,icdname);
						strcpy(current_stream->bus,busname);
						current_stream->stream_type = stream_info->type;
						current_stream->stream 		= stream;
						current_stream->direction 	= stream_info->direction;

						if (current_io->nsignals < MAX_SIGNALS && current_stream->nsignals < MAX_SIGNALS){

							_tools->myprintf("[A429] Store signal information #%d/#%d\n", current_io->nsignals+1, data.a429[i].n_messages);
							current_signal = &(current_io->signals[current_io->nsignals]);

							strcpy(current_signal->name,data.a429[i].messages[j].name);
							current_signal->direction			= data.a429[i].messages[j].direction;
							current_signal->is_refresh			= 1;
							current_signal->signal_type 		= (ed247_signal_type_t) data.a429[i].messages[j].type;
							current_signal->stream_type			= stream_info->type;
							current_signal->type 				= A429_DATA_TYPE;
							current_signal->dimensions			= 1;
							current_signal->width				= A429_DATA_SIZE;
							current_signal->size[0]				= A429_DATA_SIZE;
							current_signal->sample_size			= A429_DATA_SIZE;
							current_signal->sample_time			= (float) (data.a429[i].messages[j].period_us * 1e-6);
							current_signal->validity_duration	= (float) (data.a429[i].messages[j].validity_duration_us * 1e-6);

							current_stream->signals[current_stream->nsignals] = current_signal;
							current_stream->nsignals++;
							current_io->nsignals++;

						} else {
							_tools->myprintf("[A429] Cannot store signal #%d/#%d (array max size = %d)\n", current_io->nsignals+1, data.a429[i].n_messages, MAX_SIGNALS);
						}

					}

				}

			}

			break;

		case ED247_STREAM_TYPE_A664:

			switch (stream_info->direction){

				case ED247_DIRECTION_IN:
					current_io = _io->outputs;
					break;

				case ED247_DIRECTION_OUT:
					current_io = _io->inputs;
					break;

				default:
					return INVALID_DIRECTION;

			}

			if (current_io->nstreams >= MAX_STREAMS){return STREAM_REACH_ARRAY_LIMIT;}
			current_stream = &(current_io->streams[current_io->nstreams]);
			current_stream->nsignals = 0;
			current_io->nstreams++;

			strcpy(current_stream->name,stream_info->name);
			strcpy(current_stream->filename,icdfullpath);
			strcpy(current_stream->icd,icdname);
			strcpy(current_stream->bus,busname);
			current_stream->stream_type = stream_info->type;
			current_stream->stream 		= stream;
			current_stream->direction 	= stream_info->direction;

			for (i = 0; i<data.counter.a664; i++){

				if (strcmp(data.a664[i].name,busname) == 0){

					if (current_io->nsignals < MAX_SIGNALS && current_stream->nsignals < MAX_SIGNALS){

						_tools->myprintf("[A664] Store signal information #%d/#%d\n", current_io->nsignals+1, data.counter.a664);
						current_signal = &(current_io->signals[current_io->nsignals]);

						strcpy(current_signal->name,data.a664[i].name);
						current_signal->direction			= data.a664[i].direction;
						current_signal->is_refresh			= 1;
						current_signal->stream_type			= stream_info->type;
						current_signal->type 				= A664_DATA_TYPE;
						current_signal->dimensions			= 1;
						current_signal->width				= data.a664[i].sample_max_size_bytes;
						current_signal->size[0]				= data.a664[i].sample_max_size_bytes;
						current_signal->sample_size			= data.a664[i].sample_max_size_bytes * sizeof(char);
						current_signal->sample_time			= (float) (data.a664[i].period_us * 1e-6);
						current_signal->validity_duration	= (float) (data.a664[i].validity_duration_us * 1e-6);

						current_stream->signals[current_stream->nsignals] = current_signal;
						current_stream->nsignals++;
						current_io->nsignals++;
					} else {
						_tools->myprintf("[A664] Cannot store signal #%d/#%d (array max size = %d)\n", current_io->nsignals+1, data.counter.a664, MAX_SIGNALS);
					}

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
			if (_io->inputs->nstreams >= MAX_STREAMS){return STREAM_REACH_ARRAY_LIMIT;}
			input_stream = &(_io->inputs->streams[_io->inputs->nstreams]);
			input_stream->nsignals = 0;
			_io->inputs->nstreams++;

			strcpy(input_stream->name,stream_info->name);
			strcpy(input_stream->filename,icdfullpath);
			strcpy(input_stream->icd,icdname);
			strcpy(input_stream->bus,busname);
			input_stream->stream_type 	= stream_info->type;
			input_stream->stream 		= stream;
			input_stream->direction 	= ED247_DIRECTION_IN;

			// Outputs
			if (_io->outputs->nstreams >= MAX_STREAMS){return STREAM_REACH_ARRAY_LIMIT;}
			output_stream = &(_io->outputs->streams[_io->outputs->nstreams]);
			output_stream->nsignals = 0;
			_io->outputs->nstreams++;

			strcpy(output_stream->name,stream_info->name);
			strcpy(output_stream->filename,icdfullpath);
			strcpy(output_stream->icd,icdname);
			strcpy(output_stream->bus,busname);
			output_stream->stream_type 	= stream_info->type;
			output_stream->stream 		= stream;
			output_stream->direction 	= ED247_DIRECTION_OUT;

			for (i = 0; i<data.counter.a825; i++){

				if (strcmp(data.a825[i].name,busname) == 0){

					if (_io->inputs->nsignals < MAX_SIGNALS && input_stream->nsignals < MAX_SIGNALS){

						_tools->myprintf("[A825] Store signal information #%d/#%d\n", _io->inputs->nsignals+1, data.counter.a825);
						current_signal = &(_io->inputs->signals[_io->inputs->nsignals]);

						strcpy(current_signal->name,data.a825[i].name);
						current_signal->direction			= ED247_DIRECTION_IN;
						current_signal->type 				= A825_DATA_TYPE;
						current_signal->is_refresh			= 1;
						current_signal->stream_type			= stream_info->type;
						current_signal->dimensions			= 1;
						current_signal->width				= data.a825[i].sample_max_number;
						current_signal->size[0]				= data.a825[i].sample_max_number;
						current_signal->sample_size			= data.a825[i].sample_max_number * sizeof(char);
						current_signal->sample_time			= (float) (data.a825[i].period_us * 1e-6);
						current_signal->validity_duration	= (float) (data.a825[i].validity_duration_us * 1e-6);

						input_stream->signals[input_stream->nsignals] = current_signal;
						input_stream->nsignals++;
						_io->inputs->nsignals++;

					} else {
						_tools->myprintf("[A825] Cannot store signal #%d/#%d (array max size = %d)\n", _io->inputs->nsignals+1, data.counter.a825, MAX_SIGNALS);
					}

					if (_io->outputs->nsignals < MAX_SIGNALS && output_stream->nsignals < MAX_SIGNALS){

						_tools->myprintf("[A825] Store signal information #%d/#%d\n", _io->outputs->nsignals+1, data.counter.a825);
						current_signal = &(_io->outputs->signals[_io->outputs->nsignals]);

						strcpy(current_signal->name,data.a825[i].name);
						current_signal->direction			= ED247_DIRECTION_OUT;
						current_signal->type 				= A825_DATA_TYPE;
						current_signal->is_refresh			= 1;
						current_signal->stream_type			= stream_info->type;
						current_signal->dimensions			= 1;
						current_signal->width				= data.a825[i].sample_max_number;
						current_signal->size[0]				= data.a825[i].sample_max_number;
						current_signal->sample_size			= data.a825[i].sample_max_number * sizeof(char);
						current_signal->sample_time			= (float) (data.a825[i].period_us * 1e-6);
						current_signal->validity_duration	= (float) (data.a825[i].validity_duration_us * 1e-6);

						output_stream->signals[output_stream->nsignals] = current_signal;
						output_stream->nsignals++;
						_io->outputs->nsignals++;

					} else {
						_tools->myprintf("[A825] Cannot store signal #%d/#%d (array max size = %d)\n", _io->outputs->nsignals+1, data.counter.a825, MAX_SIGNALS);
					}

				}

			}

			break;

		default:/* polyspace RTE:UNR [Justified:Low] Defensive code */
			return INVALID_TYPE;

	}

	return CONFIGURATION_SUCCESS;

 }

/*
 * DEBUG / HELPERS
 */
BuiltInDTypeId ED247Connector::NAD2SimulinkDataType(ed247_nad_type_t type){

	switch (type){
		case ED247_NAD_TYPE_INT8:
			return SS_INT8;
		case ED247_NAD_TYPE_INT16:
			return SS_INT16;
		case ED247_NAD_TYPE_INT32:
			return SS_INT32;
		case ED247_NAD_TYPE_INT64:
			return (BuiltInDTypeId) SS_INT64;
		case ED247_NAD_TYPE_UINT8:
			return SS_UINT8;
		case ED247_NAD_TYPE_UINT16:
			return SS_UINT16;
		case ED247_NAD_TYPE_UINT32:
			return SS_UINT32;
		case ED247_NAD_TYPE_UINT64:
			return (BuiltInDTypeId) SS_UINT64;
		case ED247_NAD_TYPE_FLOAT32:
			return SS_SINGLE;
		case ED247_NAD_TYPE_FLOAT64:
			return SS_DOUBLE;
		default:
			return (BuiltInDTypeId) SS_INVALID;
	}

}

int ED247Connector::sizeofNADDataType(ed247_nad_type_t type){

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

char ED247Connector::checkStatus(ed247_status_t status, const char* fcnname, int level){

	int i;
	char format[1024] = "";
	for (i=0; i< level; i++){
		strcat(format,"\t");
	}

	if (ED247_STATUS_SUCCESS != status){
		strcat(format,"!! %s returns invalid status (%d / %d)\n");
		_tools->myprintf(format, fcnname, status, ED247_STATUS_SUCCESS);
		return 1;
	} else {
		strcat(format,"> %s OK\n");
		_tools->myprintf(format, fcnname);
		return 0;
	}

}

}
