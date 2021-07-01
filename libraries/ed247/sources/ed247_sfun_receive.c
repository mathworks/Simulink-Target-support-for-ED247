#include "ed247_sfun_receive.h"

#ifdef ED247_RECEIVE_INITIALIZE
void receiveInitialize(SimStruct *S, IO_t *io) {

	int i,isig,iport,idim,nports;
	int_T nrefresh;
	int_T *refreshFactor;
	int32_T* d;

	ssAllowSignalsWithMoreThan2D(S);
	DECL_AND_INIT_DIMSINFO(di);

	myprintf("\n\n=== RECEIVE INITIALIZATION START ===\n");

	refreshFactor = (int_T *)( mxGetData(ssGetSFcnParam(S,3)) );
	myprintf("Refresh factor = %d\n",*refreshFactor);

	/*
	 * OUTPUTS
	 */

	nrefresh = 0;
	for (i = 0; i < io->outputs->nsignals && i < MAX_SIGNALS; i++){
		nrefresh += io->outputs->signals[i].is_refresh;
	}

	nports = io->outputs->nsignals;
	if (nports > MAX_SIGNALS){nports = MAX_SIGNALS;}
	if (*refreshFactor > 0){nports = nports + nrefresh;}
	myprintf("%d streams\n",io->outputs->nstreams);
	myprintf("%d output messages\n", io->outputs->nsignals);
	myprintf("%d output ports\n", nports);

	if (!ssSetNumOutputPorts(S, nports)) return;

	ssSetNumDWork(S, io->outputs->nsignals);

	iport = 0;
	for (isig = 0; isig < io->outputs->nsignals && isig < MAX_SIGNALS; isig++){

		//
		// Data port
		//
		myprintf("Port %d : Signal", iport);
		myprintf(", Width = %d", io->outputs->signals[isig].width);
		myprintf(", Dimensions = %d\n", io->outputs->signals[isig].dimensions);

		di.width	= io->outputs->signals[isig].width;
		di.numDims	= io->outputs->signals[isig].dimensions;
		d = (int32_T*) malloc(di.numDims*sizeof(int32_T));
		for (idim = 0; idim < di.numDims && idim < MAX_DIMENSIONS; idim++){
			d[idim] = (int32_T)(io->outputs->signals[isig].size[idim]);
		}
		di.dims = &(d[0]);
		if(!ssSetOutputPortDimensionInfo(S, iport, &di)) return;

		//ssSetOutputPortWidth(S, iport, io->outputs->signals[iport].width);
		ssSetOutputPortDataType(S, iport, io->outputs->signals[isig].type);

		free(d);

		io->outputs->signals[isig].port_index = iport;
		iport++;

		//
		// Refresh ports
		//
		if (*refreshFactor > 0 && io->outputs->signals[isig].is_refresh == 1){

			myprintf("Port %d : Refresh\n", iport);

			ssSetOutputPortVectorDimension(S, iport, 1);
			ssSetOutputPortDataType(S, iport, SS_BOOLEAN);

			io->outputs->signals[isig].refresh_index = iport;
			iport++;

		} else {
			io->outputs->signals[isig].refresh_index = -1;
		}

		//
		// COUNTER
		//
		ssSetDWorkWidth(S, isig, 1);
		ssSetDWorkDataType(S, isig, SS_UINT32);

	}

	myprintf("\n=== RECEIVE INITIALIZATION END ===\n\n");

}
#endif

#ifdef ED247_RECEIVE_START
void receiveStart(SimStruct *S, IO_t *io){

	uint32_T *last_update;
	int_T iCounter,nCounter;

	myprintf("Counter initialization: ");

	nCounter = ssGetNumDWork(S);
	for (iCounter = 0; iCounter < nCounter; iCounter++){
		last_update = (uint32_T*) ssGetDWork(S,iCounter);
		*last_update = MAX_COUNTER;
		myprintf("[%d] = %d ", iCounter, *last_update);
	}
	myprintf("\n");

}
#endif

#ifdef ED247_RECEIVE_OUTPUTS
void receiveOutputs(SimStruct *S, IO_t* io){

	int isig, iport, ndata, status;
	int_T *refreshFactor = (int_T *)( mxGetData(ssGetSFcnParam(S,3)) );
	time_T sampleTime = ssGetSampleTime(S, 0);

	myprintf("\n\n=== RECEIVE OUTPUTS START ===\n");

	//
	// Prepare output (assign pointer to block output)
	//
	for (isig = 0; isig < io->outputs->nsignals && isig < MAX_SIGNALS; isig++){
		iport = io->outputs->signals[isig].port_index;
		myprintf("Attach output signal #%d to port #%d\n", isig, iport);
		io->outputs->signals[isig].valuePtr = (void*)ssGetOutputPortSignal(S,iport);
	}

	//
	// Receive data
	//
	myprintf("Receive data");
	status = (int)receive_ed247_to_simulink(io,&ndata);
	myprintf(" with status = %d", status);

	if (*refreshFactor > 0){

		for (isig = 0; isig < io->outputs->nsignals && isig < MAX_SIGNALS; isig++){

			real_T timeFromLastUpdate;
			uint32_T *last_update;
			int8_T* refresh;
			int_T irefresh = io->outputs->signals[isig].refresh_index;

			if (irefresh >= 0){

				refresh = (int8_T*)ssGetOutputPortSignal(S,irefresh);

				last_update = (uint32_T*) ssGetDWork(S,isig);
				timeFromLastUpdate = ((real_T) *last_update) * ((real_T) sampleTime);

				if (io->outputs->signals[isig].do_refresh == 1 || 
				timeFromLastUpdate < io->outputs->signals[isig].validity_duration){
					*refresh = 1;
				} else {
					*refresh = 0;
				}
				myprintf("\t#%d Refresh = %d (Validity duration = %f sec, Time from last update = %f sec)\n", isig, *refresh, io->outputs->signals[isig].validity_duration, timeFromLastUpdate);

			}

		}

	} else {
		myprintf("\tNo refresh");
	}

	myprintf("\n\n=== RECEIVE OUTPUTS END ===\n");

}
#endif

#ifdef ED247_RECEIVE_UPDATE
void receiveUpdate(SimStruct *S, IO_t* io){

	int isig;
	uint32_T *last_update;

	myprintf("\n\n=== RECEIVE UPDATE START ===\n");

	myprintf("Update receive block:\n");
	for (isig = 0; isig < io->outputs->nsignals && isig < MAX_SIGNALS; isig++){

		myprintf("\tSignal #%d", isig);

		last_update = (uint32_T*) ssGetDWork(S,isig);
		myprintf(" : last update = %d | Action = ", *last_update);

		if (io->outputs->signals[isig].do_refresh == 1){
			myprintf("Reset counter");
			last_update[0] = 0;
		} else if (*last_update < MAX_COUNTER){
			myprintf("Increment counter");
			last_update[0] = last_update[0]+1;
		} else {
			myprintf("Counter saturation");
		}
		myprintf("\n");

	}

	myprintf("\n\n=== RECEIVE OUTPUTS END ===\n");

}
#endif

#ifdef ED247_RECEIVE_TERMINATE
void receiveTerminate(SimStruct *S, IO_t* io){}
#endif

#ifdef ED247_RECEIVE_RTW
void receiveRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[MAX_SIGNALS], real_T refreshIndex[MAX_SIGNALS]){

	int i;

	*blockTypeID = 1.0;

	*nSignals = io->outputs->nsignals;
	if (*nSignals > MAX_SIGNALS){*nSignals = MAX_SIGNALS;}
	for (i = 0; i < *nSignals; i++){
		portIndex[i] = (real_T) io->outputs->signals[i].port_index;
		refreshIndex[i] = (real_T) io->outputs->signals[i].refresh_index;
	}

}
#endif