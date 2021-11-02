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

typedef enum { 
	CONFIGURATION=0, 
	RECEIVE=1, 
	SEND=2
} BLOCK_TYPE_T ;

static ed247simulink::ED247Connector* connector;
static ed247simulink::Tools* tools;

static ed247sfcn::Configure* configure;
static ed247sfcn::Receive* receive;
static ed247sfcn::Send* send;

static void mdlInitializeSizes(SimStruct *S)
{

	tools->myprintf("## Start of mdlInitializeSizes\n\n");

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

	//ssSetNumPWork(S, 1);
	tools = new ed247simulink::Tools();

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == SEND){

		tools->myprintf("SEND block (%d)\n", (int) *blockType);

		if (connector == NULL) {
			tools->myprintf("[INITIALIZE] No connector defined\n");
			return;
		}
		// ed247sfcn::Send *send = new ed247sfcn::Send(S,&di,connector,tools);
		send = new ed247sfcn::Send(S,&di,connector,tools);
		send->initialize();
		// ssGetPWork(S)[0] = (void *) send;

	} else if (*blockType == RECEIVE){

		tools->myprintf("RECEIVE block (%d)\n", (int) *blockType);

		if (connector == NULL) {
			tools->myprintf("[INITIALIZE] No connector defined\n");
			return;
		}
		// ed247sfcn::Receive *receive = new ed247sfcn::Receive(S,&di,connector,tools);
		receive = new ed247sfcn::Receive(S,&di,connector,tools);
		receive->initialize();
		// ssGetPWork(S)[0] = (void *) receive;

	} else {

		tools->myprintf("CONFIGURATION block (%d)\n", (int_T) *blockType);

		char_T *configurationFile = (char_T *)( mxGetData(ssGetSFcnParam(S,1)) );
		char_T *logFile = (char_T *)( mxGetData(ssGetSFcnParam(S,2)) );
		if (configurationFile != NULL){

			tools->myprintf("Configuration file = '%s'\n", configurationFile);
			tools->myprintf("Log file = '%s'\n", logFile);

			connector = new ed247simulink::ED247Connector(configurationFile,logFile,tools);
			// ed247sfcn::Configure *configure = new ed247sfcn::Configure(S,connector,tools);
			configure = new ed247sfcn::Configure(S,connector,tools);
			configure->initialize();
			// ssGetPWork(S)[0] = (void *) configure;

		}

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

	tools->myprintf("## End of mdlInitializeSizes\n\n");

}
static void mdlInitializeSampleTimes(SimStruct *S)
{
	tools->myprintf("## Start of mdlInitializeSampleTimes\n\n");

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	real_T * sampleTime = (real_T *)( mxGetData(ssGetSFcnParam(S,4)) );

	if (*blockType == RECEIVE && *sampleTime > 0){
		ssSetSampleTime(S, 0, *sampleTime);
		ssSetOffsetTime(S, 0, 0.0);
	} else {
		ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
		ssSetOffsetTime(S, 0, 0.0);
	}

	tools->myprintf("## End of mdlInitializeSampleTimes\n\n");

}

#define MDL_START
#ifdef MDL_START
static void mdlStart(SimStruct *S)
{
	tools->myprintf("## Start of mdlStart\n\n");

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );

	if (*blockType == SEND){
		//ed247sfcn::Send *send = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		send->start();

	} else if (*blockType == RECEIVE){
		//ed247sfcn::Receive *receive = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		receive->start();

	} else {
		//ed247sfcn::Configure *configure = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		configure->start();
	}

	tools->myprintf("## End of mdlStart\n\n");

}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{

	tools->myprintf("## Start of mdlOutputs\n\n");

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );

	if (*blockType == RECEIVE){
		//ed247sfcn::Receive *receive = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		receive->outputs();

	} else if (*blockType == SEND){
		//ed247sfcn::Send *send = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		send->outputs();

	} else {
		//ed247sfcn::Configure *configure = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		configure->outputs();

	}

	tools->myprintf("## End of mdlOutputs\n\n");

}

#define MDL_UPDATE
#ifdef MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid){

	tools->myprintf("## Start of mdlUpdate\n\n");

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == RECEIVE){
		//ed247sfcn::Receive *receive = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		receive->update();

	} else if (*blockType == SEND){
		//ed247sfcn::Send *send = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		send->update();

	} else {
		//ed247sfcn::Configure *configure = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		configure->update();

	}

	tools->myprintf("## End of mdlUpdate\n\n");

}
#endif

static void mdlTerminate(SimStruct *S){

	tools->myprintf("## Start of mdlTerminate\n\n");

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == RECEIVE){
		//ed247sfcn::Receive *receive = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		//receive->terminate();

	} else if (*blockType == SEND){
		//ed247sfcn::Send *send = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		//send->terminate();

	} else {
		//ed247sfcn::Configure *configure = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		configure->terminate();
		delete connector;
		connector = NULL;

	}

	tools->myprintf("## End of mdlTerminate\n\n");

}

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
/* Function: mdlRTW ===========================================================
 */
static void mdlRTW(SimStruct *S)
{
	tools->myprintf("## Start of mdlRTW\n\n");

	char_T configurationFile[512];
	real_T  blockTypeID;
	BLOCK_TYPE_T *blockType;

	real_T portIndex[MAX_SIGNALS];
	real_T refreshIndex[MAX_SIGNALS];
	int_T nSignals;

	blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == SEND){

		//ed247sfcn::Send *send = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		send->RTW(&blockTypeID, &nSignals, portIndex, refreshIndex);
		if (!ssWriteRTWParamSettings(S, 4, 
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID,
			SSWRITE_VALUE_NUM,  "NumSignals",   (real_T) nSignals,
			SSWRITE_VALUE_VECT, "PortIndex",    portIndex, nSignals, 
			SSWRITE_VALUE_VECT, "RefreshIndex", refreshIndex, nSignals)) {
			return; /* An error occurred. */
		}

	} else if (*blockType == RECEIVE){

		//ed247sfcn::Receive *receive = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		receive->RTW(&blockTypeID, &nSignals, portIndex, refreshIndex);
		if (!ssWriteRTWParamSettings(S, 4, 
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID,
			SSWRITE_VALUE_NUM,  "NumSignals",   (real_T) nSignals,
			SSWRITE_VALUE_VECT, "PortIndex",    portIndex, nSignals, 
			SSWRITE_VALUE_VECT, "RefreshIndex", refreshIndex, nSignals)) {
			return; /* An error occurred. */
		}

	} else {

		//ed247sfcn::Configure *configure = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		configure->RTW(&blockTypeID, configurationFile);
		if (!ssWriteRTWParamSettings(S, 2, 
			SSWRITE_VALUE_QSTR, "Filename",     configurationFile,
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID)) {
			return; /* An error occurred. */
		}
	}

	tools->myprintf("## End of mdlRTW\n\n");

}
#endif /* MDL_RTW */

#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif
