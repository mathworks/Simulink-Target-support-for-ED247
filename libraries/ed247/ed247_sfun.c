/*
 * Copyright 2020 The MathWorks, Inc.
 */

#define S_FUNCTION_NAME ed247_sfun /* Defines and Includes */
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "ed247_interface.h"

#include "ed247_sfun_configure.h"
#include "ed247_sfun_receive.h"
#include "ed247_sfun_send.h"

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

		#if defined(ED247_SEND_INITIALIZE)
		myprintf("SEND block (%d)\n", (int) *blockType);
		if (io == NULL) {
			myprintf("[INITIALIZE] No IO defined\n");
			return;
		}
		sendInitialize(S, io);
		#else
		myprintf("SEND : No initialize function\n");
		#endif

	} else if (*blockType == RECEIVE){

		#if defined(ED247_RECEIVE_INITIALIZE)
		myprintf("RECEIVE block (%d)\n", (int) *blockType);
		if (io == NULL) {
			myprintf("[INITIALIZE] No IO defined\n");
			return;
		}
		receiveInitialize(S, io);
		#else
		myprintf("RECEIVE : No initialize function\n");
		#endif

	} else {
		#if defined(ED247_CONFIGURE_INITIALIZE)
		myprintf("CONFIGURATION block (%d)\n", (int_T) *blockType);
		io = configureInitialize(S);
		#else
		myprintf("CONFIGURATION : No initialize function\n");
		#endif
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
	if (io == NULL) {
		myprintf("[START] No IO defined\n");
		return;
	}

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );

	if (*blockType == SEND){
		#if defined(ED247_SEND_START)
		sendStart(S,io);
		#endif
	} else if (*blockType == RECEIVE){
		#if defined(ED247_RECEIVE_START)
		receiveStart(S,io);
		#endif
	} else {
		#if defined(ED247_CONFIGURE_START)
		configureStart(S,io);
		#endif
	}

}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{

	if (io == NULL) {
		myprintf("[OUTPUTS] No IO defined\n");
		return;
	}

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );

	if (*blockType == RECEIVE){
		#if defined(ED247_RECEIVE_OUTPUTS)
		receiveOutputs(S,io);
		#endif
	} else if (*blockType == SEND){
		#if defined(ED247_SEND_OUTPUTS)
		sendOutputs(S,io);
		#endif
	} else {
		#if defined(ED247_CONFIGURE_OUTPUTS)
		configureOutputs(S,io);
		#endif
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
		#if defined(ED247_RECEIVE_UPDATE)
		receiveUpdate(S,io);
		#endif
	} else if (*blockType == SEND){
		#if defined(ED247_SEND_UPDATE)
		sendUpdate(S,io);
		#endif
	} else {
		#if defined(ED247_CONFIGURE_UPDATE)
		configureUpdate(S,io);
		#endif
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
		#if defined(ED247_RECEIVE_TERMINATE)
		receiveTerminate(S,io);
		#endif
	} else if (*blockType == SEND){
		#if defined(ED247_SEND_TERMINATE)
		sendTerminate(S,io);
		#endif
	} else {
		#if defined(ED247_CONFIGURE_TERMINATE)
		configureTerminate(S,io);
		#endif
	}

}

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
/* Function: mdlRTW ===========================================================
 */
static void mdlRTW(SimStruct *S)
{
	char_T configurationFile[512];
	real_T  blockTypeID;
	BLOCK_TYPE_T *blockType;

	real_T portIndex[100];
	real_T refreshIndex[100];
	int_T nSignals;

	blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == SEND){
        
		#if defined(ED247_SEND_RTW)
		sendRTW(S, io, &blockTypeID, &nSignals, portIndex, refreshIndex);
        if (!ssWriteRTWParamSettings(S, 4, 
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID,
			SSWRITE_VALUE_NUM,  "NumSignals",   (real_T) nSignals,
			SSWRITE_VALUE_VECT, "PortIndex",    portIndex, nSignals, 
			SSWRITE_VALUE_VECT, "RefreshIndex", refreshIndex, nSignals)) {
            return; /* An error occurred. */
        }
		#endif

	} else if (*blockType == RECEIVE){
		#if defined(ED247_RECEIVE_RTW)
		receiveRTW(S, io, &blockTypeID, &nSignals, portIndex, refreshIndex);
        if (!ssWriteRTWParamSettings(S, 4, 
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID,
			SSWRITE_VALUE_NUM,  "NumSignals",   (real_T) nSignals,
			SSWRITE_VALUE_VECT, "PortIndex",    portIndex, nSignals, 
			SSWRITE_VALUE_VECT, "RefreshIndex", refreshIndex, nSignals)) {
            return; /* An error occurred. */
        }
		#endif

	} else {
		#if defined(ED247_CONFIGURE_RTW)
		configureRTW(S, io, &blockTypeID, configurationFile);
        if (!ssWriteRTWParamSettings(S, 2, 
			SSWRITE_VALUE_QSTR, "Filename",     configurationFile,
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID)) {
            return; /* An error occurred. */
        }
		#endif
	}
    
}
#endif /* MDL_RTW */

#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif
