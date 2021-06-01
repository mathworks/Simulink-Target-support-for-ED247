#include "ed247_sfun_send.h"

#ifdef ED247_SEND_INITIALIZE
void sendInitialize(SimStruct *S, IO_t *io){

	int isig,iport,idim,nports;
	int_T isRefreshEnabled;
	int32_T* d;

	ssAllowSignalsWithMoreThan2D(S);
	DECL_AND_INIT_DIMSINFO(di);

	ssSetNumDWork(S, 0);

	myprintf("\n\n=== SEND INITIALIZATION START ===\n");

	isRefreshEnabled = *((int_T *)( mxGetData(ssGetSFcnParam(S,3)) ));
	myprintf("Is refresh enabled = %d\n",isRefreshEnabled);

	/*
	 * INPUTS
	 */
	nports = io->inputs->nsignals;
	for (iport = 0; iport < io->inputs->nsignals && iport < MAX_SIGNALS; iport++){
		if (io->inputs->signals[iport].is_refresh == 1 && isRefreshEnabled == 1){
			nports++;
		}
	}
	myprintf("%d streams\n",io->inputs->nstreams);
	myprintf("%d input messages\n", io->inputs->nsignals);
	myprintf("%d input ports\n",  nports);

	if (!ssSetNumInputPorts(S, nports)) return;

	iport = 0;
	for (isig = 0; isig < io->inputs->nsignals && isig < MAX_SIGNALS; isig++){

		//
		// Data port
		//
		myprintf("Port %d : Signal\n", iport);
		myprintf("\t-Width = %d\n", io->inputs->signals[isig].width);
		myprintf("\t-Dimensions = %d\n", io->inputs->signals[isig].dimensions);

		di.width	= io->inputs->signals[isig].width;
		di.numDims	= io->inputs->signals[isig].dimensions;

		d = (int32_T*) malloc(di.numDims*sizeof(int32_T));
		for (idim = 0; idim < di.numDims && idim < MAX_DIMENSIONS; idim++){
			myprintf("\t\t-Dimension #%d = %d\n", idim, io->inputs->signals[isig].size[idim]);
			d[idim] = (int32_T)(io->inputs->signals[isig].size[idim]);
		}
		di.dims = &(d[0]);
		if(!ssSetInputPortDimensionInfo(S, iport, &di)) return;

		ssSetInputPortWidth(S, iport, io->inputs->signals[isig].width);
		ssSetInputPortDirectFeedThrough(S, iport, 1);
		ssSetInputPortDataType(S, iport, io->inputs->signals[isig].type);
		ssSetInputPortRequiredContiguous(S, iport, 1);

		free(d);

		io->inputs->signals[isig].port_index = iport;
		iport++;

		//
		// Refresh ports
		//
		if (io->inputs->signals[isig].is_refresh == 1 && isRefreshEnabled == 1){

			myprintf("Port %d : Refresh\n", iport);

			ssSetInputPortVectorDimension(S, iport, 1);
			ssSetInputPortDirectFeedThrough(S, iport, 1);
			ssSetInputPortDataType(S, iport, SS_BOOLEAN);
			ssSetInputPortRequiredContiguous(S, iport, 1);

			io->inputs->signals[isig].refresh_index = iport;
			iport++;

		} else {
			io->inputs->signals[isig].refresh_index = -1;
		}

	}

	myprintf("\n=== SEND INITIALIZATION END ===\n\n");

}
#endif

#ifdef ED247_SEND_START
void sendStart(SimStruct *S, IO_t *io){}
#endif

#ifdef ED247_SEND_OUTPUTS
void sendOutputs(SimStruct *S, IO_t* io){

	int isig, iport,status;
	int8_T* refresh;

	for (isig = 0; isig < io->inputs->nsignals && isig < MAX_SIGNALS; isig++){

		iport = io->inputs->signals[isig].port_index;
		io->inputs->signals[isig].valuePtr = (void*)ssGetInputPortSignal(S,iport);

		if (io->inputs->signals[isig].is_refresh == 1 && io->inputs->signals[isig].refresh_index >= 0){
			refresh = (int8_T*)ssGetInputPortSignal(S,io->inputs->signals[isig].refresh_index); 
			myprintf("Refresh port #%d = %d\n", io->inputs->signals[isig].refresh_index, *refresh);
			io->inputs->signals[isig].do_refresh = *refresh;
		} else {
			io->inputs->signals[isig].do_refresh = 1;
		}

		if (io->inputs->signals[isig].do_refresh == 1){
			myprintf("Send data from port %d to signal %d\n", iport, isig);
		}

	}

	status = (int)send_simulink_to_ed247(io);
	myprintf("Send status = %d\n", status);

}
#endif

#ifdef ED247_SEND_UPDATE
void sendUpdate(SimStruct *S, IO_t* io){}
#endif

#ifdef ED247_SEND_TERMINATE
void sendTerminate(SimStruct *S, IO_t* io){}
#endif

#ifdef ED247_SEND_RTW
void sendRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[MAX_SIGNALS], real_T refreshIndex[MAX_SIGNALS]){

	int i;

	*blockTypeID = 2.0;

	if (io->inputs->nsignals <= MAX_SIGNALS){
		*nSignals = io->inputs->nsignals;
	} else {
		*nSignals = MAX_SIGNALS;
	}

	for (i = 0; i < *nSignals; i++){

		portIndex[i] = (real_T) io->inputs->signals[i].port_index;
		if (io->inputs->signals[i].is_refresh == 1 && io->inputs->signals[i].refresh_index >= 0){ 
			refreshIndex[i] = (real_T) io->inputs->signals[i].refresh_index;
		} else {
			refreshIndex[i] = -1.0;
		}

	}

}
#endif