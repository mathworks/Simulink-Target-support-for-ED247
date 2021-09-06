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

    ssSetNumPWork(S, 1);
    
    tools = new ed247simulink::Tools();
        
	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == SEND){

        ed247sfcn::Send *obj = new ed247sfcn::Send(S,connector,tools);
                
		tools->myprintf("SEND block (%d)\n", (int) *blockType);
		if (connector == NULL) {
			tools->myprintf("[INITIALIZE] No connector defined\n");
			return;
		}
        obj->initialize();
        ssGetPWork(S)[0] = (void *) obj;

	} else if (*blockType == RECEIVE){

		ed247sfcn::Receive *obj = new ed247sfcn::Receive(S,connector,tools);
        
		tools->myprintf("RECEIVE block (%d)\n", (int) *blockType);
		if (connector == NULL) {
			tools->myprintf("[INITIALIZE] No connector defined\n");
			return;
		}
        obj->initialize();
        ssGetPWork(S)[0] = (void *) obj;
		
	} else {
        		        
		tools->myprintf("CONFIGURATION block (%d)\n", (int_T) *blockType);
        
        char_T *configurationFile = (char_T *)( mxGetData(ssGetSFcnParam(S,1)) );
        char_T *logFile = (char_T *)( mxGetData(ssGetSFcnParam(S,2)) );
        if (configurationFile != NULL){
            
            tools->myprintf("Configuration file = '%s'\n", configurationFile);
            tools->myprintf("Log file = '%s'\n", logFile);
            
            connector = new ed247simulink::ED247Connector(configurationFile,logFile);
        
            ed247sfcn::Configure *obj = new ed247sfcn::Configure(S,connector,tools);            
            obj->initialize();
            ssGetPWork(S)[0] = (void *) obj;
            
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
	if (connector == NULL) {
		tools->myprintf("[START] No connector defined\n");
		return;
	}

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );

	if (*blockType == SEND){
		ed247sfcn::Send *obj = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		obj->start();
		
	} else if (*blockType == RECEIVE){
		ed247sfcn::Receive *obj = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		obj->start();
        
	} else {
		ed247sfcn::Configure *obj = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		obj->start();
	}

}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{

	if (connector == NULL) {
		tools->myprintf("[OUTPUTS] No connector defined\n");
		return;
	}

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );

	if (*blockType == RECEIVE){
		ed247sfcn::Receive *obj = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		obj->outputs();
        
	} else if (*blockType == SEND){
		ed247sfcn::Send *obj = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		obj->outputs();
        
	} else {
		ed247sfcn::Configure *obj = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		obj->outputs();
        
	}

}

#define MDL_UPDATE
#ifdef MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid){
    
	if (connector == NULL) {
		tools->myprintf("No connector defined\n");
		return;
	}

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == RECEIVE){
		ed247sfcn::Receive *obj = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		obj->update();
        
	} else if (*blockType == SEND){
		ed247sfcn::Send *obj = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		obj->update();
        
	} else {
		ed247sfcn::Configure *obj = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		obj->update();
        
	}

}
#endif

static void mdlTerminate(SimStruct *S){
    
	if (connector == NULL) {
		tools->myprintf("No connector defined\n");
		return;
	}

	BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == RECEIVE){
		ed247sfcn::Receive *obj = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		obj->terminate();
        
	} else if (*blockType == SEND){
		ed247sfcn::Send *obj = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		obj->terminate();
        
	} else {
		ed247sfcn::Configure *obj = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		obj->terminate();
        
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

	real_T portIndex[MAX_SIGNALS];
	real_T refreshIndex[MAX_SIGNALS];
	int_T nSignals;

	blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (*blockType == SEND){
        
		ed247sfcn::Send *obj = (ed247sfcn::Send*) ssGetPWork(S)[0]; 
		obj->RTW(&blockTypeID, &nSignals, portIndex, refreshIndex);
        if (!ssWriteRTWParamSettings(S, 4, 
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID,
			SSWRITE_VALUE_NUM,  "NumSignals",   (real_T) nSignals,
			SSWRITE_VALUE_VECT, "PortIndex",    portIndex, nSignals, 
			SSWRITE_VALUE_VECT, "RefreshIndex", refreshIndex, nSignals)) {
            return; /* An error occurred. */
        }

	} else if (*blockType == RECEIVE){
		
        ed247sfcn::Receive *obj = (ed247sfcn::Receive*) ssGetPWork(S)[0]; 
		obj->RTW(&blockTypeID, &nSignals, portIndex, refreshIndex);
        if (!ssWriteRTWParamSettings(S, 4, 
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID,
			SSWRITE_VALUE_NUM,  "NumSignals",   (real_T) nSignals,
			SSWRITE_VALUE_VECT, "PortIndex",    portIndex, nSignals, 
			SSWRITE_VALUE_VECT, "RefreshIndex", refreshIndex, nSignals)) {
            return; /* An error occurred. */
        }

	} else {
        
		ed247sfcn::Configure *obj = (ed247sfcn::Configure*) ssGetPWork(S)[0]; 
		obj->RTW(&blockTypeID, configurationFile);
        if (!ssWriteRTWParamSettings(S, 2, 
			SSWRITE_VALUE_QSTR, "Filename",     configurationFile,
			SSWRITE_VALUE_NUM,  "BlockType",    blockTypeID)) {
            return; /* An error occurred. */
        }
	}
    
}
#endif /* MDL_RTW */

#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif
