/*
 * Copyright 2020 The MathWorks, Inc.
 */

#define S_FUNCTION_NAME ed247_sfun /* Defines and Includes */
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "ed247_interface.h"

#define MAX_COUNTER 10000000
/*
typedef struct {
	void*	io;
} signal_info_t;
*/

typedef enum { 
	CONFIGURATION=0, 
	RECEIVE=1, 
	SEND=2
} BLOCK_TYPE_T ;

static IO_t *io;

IO_t* configureInitialize(SimStruct *S);
void configureStart(SimStruct *S, IO_t *io);
void configureOutputs(SimStruct *S, IO_t* io);
void configureUpdate(SimStruct *S, IO_t* io);
void configureTerminate(SimStruct *S, IO_t* io);
void configureRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[100], real_T refreshIndex[100]);

void receiveInitialize(SimStruct *S, IO_t *io);
void receiveStart(SimStruct *S, IO_t *io);
void receiveOutputs(SimStruct *S, IO_t* io);
void receiveUpdate(SimStruct *S, IO_t* io);
void receiveTerminate(SimStruct *S, IO_t* io);
void receiveRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[100], real_T refreshIndex[100]);

void sendInitialize(SimStruct *S, IO_t *io);
void sendStart(SimStruct *S, IO_t *io);
void sendOutputs(SimStruct *S, IO_t* io);
void sendUpdate(SimStruct *S, IO_t* io);
void sendTerminate(SimStruct *S, IO_t* io);
void sendRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[100], real_T refreshIndex[100]);

static void mdlInitializeSizes(SimStruct *S)
{

	int i;
	int isig,iport,idim,nports;
	int32_T* d;

	ssAllowSignalsWithMoreThan2D(S);
	DECL_AND_INIT_DIMSINFO(di);

	/*
	 * PARAMETERS
	 */
	ssSetNumSFcnParams(S, 5);
	if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
		return; /* Parameter mismatch reported by the Simulink engine*/
	}
	ssSetSFcnParamTunable(S, 0, SS_PRM_NOT_TUNABLE); // Block type (configuration, send, receive)
	ssSetSFcnParamTunable(S, 1, SS_PRM_NOT_TUNABLE); // Configuration file (used by Configuration block)
	ssSetSFcnParamTunable(S, 2, SS_PRM_NOT_TUNABLE); // Log file (used by Configuration block)
	ssSetSFcnParamTunable(S, 3, SS_PRM_NOT_TUNABLE); // Refresh factor (used by Receive block)
	ssSetSFcnParamTunable(S, 4, SS_PRM_NOT_TUNABLE); // Sample time (used by Receive block)

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == SEND){

		myprintf("SEND block (%d)\n", (int) *blockType);
		if (io == NULL) {
			myprintf("No IO defined\n");
			return;
		}
		sendInitialize(S, io);

	} else if (*blockType == RECEIVE){

		myprintf("RECEIVE block (%d)\n", (int) *blockType);
		if (io == NULL) {
			myprintf("No IO defined\n");
			return;
		}
		receiveInitialize(S, io);

	} else {
		myprintf("CONFIGURATION block (%d)\n", (int_T) *blockType);
		io = configureInitialize(S);
	}

	/*
	* CONFIGURATION
	*/
	ssSetNumSampleTimes(S, 1);

	/* Take care when specifying exception free code - see sfuntmpl.doc */
	ssSetOptions(S,
			SS_OPTION_WORKS_WITH_CODE_REUSE |
			SS_OPTION_EXCEPTION_FREE_CODE |
			SS_OPTION_CALL_TERMINATE_ON_EXIT |
			SS_OPTION_USE_TLC_WITH_ACCELERATOR);

}
static void mdlInitializeSampleTimes(SimStruct *S)
{
	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	real_T * sampleTime = (real_T *)( mxGetData(ssGetSFcnParam(S,4)) );

	if (*blockType == RECEIVE && *sampleTime > 0){
		ssSetSampleTime(S, 0, *sampleTime);
		ssSetOffsetTime(S, 0, 0.0);
	} else {
		ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
		ssSetOffsetTime(S, 0, 0.0);
	}

}

#define MDL_START
#ifdef MDL_START
static void mdlStart(SimStruct *S)
{
	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );

	if (*blockType == SEND){
		sendStart(S,io);
	} else if (*blockType == RECEIVE){
		receiveStart(S,io);
	} else {
		configureStart(S,io);
	}

}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{

	if (io == NULL) {
		myprintf("No IO defined\n");
		return;
	}

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );

	if (*blockType == RECEIVE){
		receiveOutputs(S,io);
	} else if (*blockType == SEND){
		sendOutputs(S,io);
	} else {
		configureOutputs(S,io);
	}

}

#define MDL_UPDATE
#ifdef MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid){

	if (io == NULL) {
		myprintf("No IO defined\n");
		return;
	}

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == RECEIVE){
		receiveUpdate(S,io);
	} else if (*blockType == SEND){
		sendUpdate(S,io);
	} else {
		configureUpdate(S,io);
	}

}
#endif

static void mdlTerminate(SimStruct *S){

	if (io == NULL) {
		myprintf("No IO defined\n");
		return;
	}

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == RECEIVE){
		receiveTerminate(S,io);
	} else if (*blockType == SEND){
		sendTerminate(S,io);
	} else {
		configureTerminate(S,io);
	}

}

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
/* Function: mdlRTW ===========================================================
 */
static void mdlRTW(SimStruct *S)
{
	char_T  *configurationFile;
	real_T  blockTypeID;
	BLOCK_TYPE_T *blockType;

	real_T portIndex[100];
	real_T refreshIndex[100];
	int_T nSignals;

	blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == SEND){
		sendRTW(S, io, &blockTypeID, &nSignals, portIndex, refreshIndex);

	} else if (*blockType == RECEIVE){
		receiveRTW(S, io, &blockTypeID, &nSignals, portIndex, refreshIndex);

	} else {
		configureRTW(S, io, &blockTypeID, &nSignals, portIndex, refreshIndex);
	}

	configurationFile = (char_T *)( mxGetData(ssGetSFcnParam(S,1)) );
	if (!ssWriteRTWParamSettings(S, 5, 
			SSWRITE_VALUE_QSTR, "Filename",     configurationFile,
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID,
			SSWRITE_VALUE_NUM,  "NumSignals",   (real_T) nSignals,
			SSWRITE_VALUE_VECT, "PortIndex",    portIndex, nSignals, 
			SSWRITE_VALUE_VECT, "RefreshIndex", refreshIndex, nSignals)) {
		return; /* An error occurred. */
	}

}
#endif /* MDL_RTW */

#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif

/* ========================================== */
void sendInitialize(SimStruct *S, IO_t *io){

	int isig,iport,idim,nports;
	int32_T* d;

	ssAllowSignalsWithMoreThan2D(S);
	DECL_AND_INIT_DIMSINFO(di);

	ssSetNumDWork(S, 0);

	/*
	 * INPUTS
	 */
	nports = io->inputs->nsignals;
	for (iport = 0; iport < io->inputs->nsignals; iport++){
		if (io->inputs->signals[iport].is_refresh == 1){
			nports++;
		}
	}
	myprintf("%d streams\n",io->inputs->nstreams);
	myprintf("%d input messages\n", io->inputs->nsignals);
	myprintf("%d input ports\n",  nports);

	if (!ssSetNumInputPorts(S, nports)) return;

	iport = 0;
	for (isig = 0; isig < io->inputs->nsignals; isig++){

		//
		// Data port
		//
		myprintf("Port %d : Signal\n", iport);
		myprintf("\t-Width = %d\n", io->inputs->signals[isig].width);
		myprintf("\t-Dimensions = %d\n", io->inputs->signals[isig].dimensions);

		di.width	= io->inputs->signals[isig].width;
		di.numDims	= io->inputs->signals[isig].dimensions;

		d = (int32_T*) malloc(di.numDims*sizeof(int32_T));
		for (idim = 0; idim < di.numDims; idim++){
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
		if (io->inputs->signals[isig].is_refresh == 1){

			myprintf("Port %d : Refresh\n", iport);

			ssSetInputPortVectorDimension(S, iport, 1);
			ssSetInputPortDirectFeedThrough(S, iport, 1);
			ssSetInputPortDataType(S, iport, SS_BOOLEAN);
			ssSetInputPortRequiredContiguous(S, iport, 1);

			io->inputs->signals[isig].refresh_index = iport;
			iport++;

		}

	}

}

void receiveInitialize(SimStruct *S, IO_t *io) {

	int i,isig,iport,idim,nports;
	int_T nrefresh;
	int_T *refreshFactor;
	int32_T* d;

	ssAllowSignalsWithMoreThan2D(S);
	DECL_AND_INIT_DIMSINFO(di);

	refreshFactor = (int_T *)( mxGetData(ssGetSFcnParam(S,3)) );
	myprintf("Refresh factor = %d\n",*refreshFactor);

	/*
	 * OUTPUTS
	 */

	nrefresh = 0;
	for (i = 0; i < io->outputs->nsignals; i++){
		nrefresh += io->outputs->signals[isig].is_refresh;
	}

	nports = io->outputs->nsignals;
	if (*refreshFactor > 0){nports = nports + nrefresh;}
	myprintf("%d streams\n",io->outputs->nstreams);
	myprintf("%d output messages\n", io->outputs->nsignals);
	myprintf("%d output ports\n", nports);

	if (!ssSetNumOutputPorts(S, nports)) return;

	ssSetNumDWork(S, io->outputs->nsignals);

	iport = 0;
	for (isig = 0; isig < io->outputs->nsignals; isig++){

		//
		// Data port
		//
		myprintf("Port %d : Signal", iport);
		myprintf(", Width = %d", io->outputs->signals[isig].width);
		myprintf(", Dimensions = %d\n", io->outputs->signals[isig].dimensions);

		di.width	= io->outputs->signals[isig].width;
		di.numDims	= io->outputs->signals[isig].dimensions;
		d = (int32_T*) malloc(di.numDims*sizeof(int32_T));
		for (idim = 0; idim < di.numDims; idim++){
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

}

IO_t* configureInitialize(SimStruct *S){

	IO_t *io;

	ssSetNumDWork(S, 0);

	io_allocate_memory(&io);

	char_T *configurationFile = (char_T *)( mxGetData(ssGetSFcnParam(S,1)) );
	char_T *logFile = (char_T *)( mxGetData(ssGetSFcnParam(S,2)) );
	if (configurationFile != NULL){
		myprintf("Configuration file = '%s'\n", configurationFile);
		myprintf("Log file = '%s'\n", logFile);
		read_ed247_configuration(configurationFile, io, logFile);

		myprintf("%d inputs\n", io->inputs->nsignals);
		myprintf("%d outputs\n", io->outputs->nsignals);
	}
	
	return io;

}

void sendStart(SimStruct *S, IO_t *io){}
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
void configureStart(SimStruct *S, IO_t *io){}

void receiveOutputs(SimStruct *S, IO_t* io){

	int isig, iport, ndata, status;
	int_T *refreshFactor = (int_T *)( mxGetData(ssGetSFcnParam(S,3)) );
	time_T sampleTime = ssGetSampleTime(S, 0);

	//
	// Prepare output (assign pointer to block output)
	//
	for (isig = 0; isig < io->outputs->nsignals; isig++){
		iport = io->outputs->signals[isig].port_index;
		myprintf("Attach output signal #%d to port #%d\n", isig, iport);
		io->outputs->signals[isig].valuePtr = (void*)ssGetOutputPortSignal(S,iport);
	}

	//
	// Receive data
	//
	myprintf("Receive data\t");
	status = (int)receive_ed247_to_simulink(io,&ndata);
	myprintf("\tstatus = %d", status);
	myprintf("\t - do refresh = %d\n", io->outputs->signals[isig].do_refresh);

	if (*refreshFactor > 0){

		for (isig = 0; isig < io->outputs->nsignals; isig++){

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

}

void sendOutputs(SimStruct *S, IO_t* io){

	int isig, iport,status;
	int8_T* refresh;

	for (isig = 0; isig < io->inputs->nsignals; isig++){

		iport = io->inputs->signals[isig].port_index;
		io->inputs->signals[isig].valuePtr = (void*)ssGetInputPortSignal(S,iport);

		if (io->inputs->signals[isig].is_refresh == 1){
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

void configureOutputs(SimStruct *S, IO_t* io){}

void receiveUpdate(SimStruct *S, IO_t* io){

	int isig;
	uint32_T *last_update;

	myprintf("Update receive block:\n");
	for (isig = 0; isig < io->outputs->nsignals; isig++){

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

}
void sendUpdate(SimStruct *S, IO_t* io){}
void configureUpdate(SimStruct *S, IO_t* io){}

void receiveTerminate(SimStruct *S, IO_t* io){}
void sendTerminate(SimStruct *S, IO_t* io){}
void configureTerminate(SimStruct *S, IO_t* io){}

void receiveRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[100], real_T refreshIndex[100]){

	int i;

	*blockTypeID = 1.0;

	*nSignals = io->outputs->nsignals;
	for (i = 0; i < *nSignals; i++){
		portIndex[i] = (real_T) io->outputs->signals[i].port_index;
		refreshIndex[i] = (real_T) io->outputs->signals[i].refresh_index;
	}

}

void sendRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[100], real_T refreshIndex[100]){

	int i;

	*blockTypeID = 2.0;

	*nSignals = io->inputs->nsignals;
	for (i = 0; i < *nSignals; i++){

		portIndex[i] = (real_T) io->inputs->signals[i].port_index;
		if (io->inputs->signals[i].is_refresh == 1){ 
			refreshIndex[i] = (real_T) io->inputs->signals[i].refresh_index;
		} else {
			refreshIndex[i] = -1.0;
		}

	}

}
void configureRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[100], real_T refreshIndex[100]){
	*blockTypeID = 0.0;
}