/*
 * Copyright 2020 The MathWorks, Inc.
 */

#define S_FUNCTION_NAME ed247_sfun /* Defines and Includes */
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "ed247_interface.h"
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

static void mdlInitializeSizes(SimStruct *S)
{
	int i,j;
	int32_T* d;
	
    ssAllowSignalsWithMoreThan2D(S);
	DECL_AND_INIT_DIMSINFO(di);
    
	/*
	 * PARAMETERS
	 */	
    ssSetNumSFcnParams(S, 3);
	if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
		return; /* Parameter mismatch reported by the Simulink engine*/
	}
	ssSetSFcnParamTunable(S, 0, SS_PRM_NOT_TUNABLE);
    ssSetSFcnParamTunable(S, 1, SS_PRM_NOT_TUNABLE);
    ssSetSFcnParamTunable(S, 2, SS_PRM_NOT_TUNABLE);
    
    BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
    if (*blockType == SEND){
        
        myprintf("SEND block (%d)\n", (int) *blockType);
        if (io == NULL) {
            myprintf("No IO defined\n");
            return;
        }
        
        /*
         * INPUTS
         */
        myprintf("%d inputs\n", io->inputs->nsignals);
        if (!ssSetNumInputPorts(S, io->inputs->nsignals)) return;
        for (i = 0; i < io->inputs->nsignals; i++){
            
            di.width	= io->inputs->signals[i].width;
            di.numDims	= io->inputs->signals[i].dimensions;
            d = (int32_T*) malloc(di.numDims*sizeof(int32_T));
            for (j = 0; j < di.numDims; j++){
                d[j] = (int32_T)(io->inputs->signals[i].size[j]);
            }
            di.dims = &(d[0]);
            if(!ssSetInputPortDimensionInfo(S, i, &di)) return;
            
            //ssSetInputPortWidth(S, i, io->inputs->signals[i].width);
            ssSetInputPortDirectFeedThrough(S, i, 1);
            ssSetInputPortDataType(S, i, io->inputs->signals[i].type);
            ssSetInputPortRequiredContiguous(S, i, 1);
            
            free(d);
        }
        
    } else if (*blockType == RECEIVE){
        
        myprintf("RECEIVE block (%d)\n", (int) *blockType);
        if (io == NULL) {
            myprintf("No IO defined\n");
            return;
        }
        
        /*
         * OUTPUTS
         */
        myprintf("%d outputs\n", io->outputs->nsignals);
        if (!ssSetNumOutputPorts(S, io->outputs->nsignals)) return;
        for (i = 0; i < io->outputs->nsignals; i++){
            
            di.width	= io->outputs->signals[i].width;
            di.numDims	= io->outputs->signals[i].dimensions;
            d = (int32_T*) malloc(di.numDims*sizeof(int32_T));
            for (j = 0; j < di.numDims; j++){
                d[j] = (int32_T)(io->outputs->signals[i].size[j]);
            }
            di.dims = &(d[0]);
            if(!ssSetOutputPortDimensionInfo(S, i, &di)) return;
            
            //ssSetOutputPortWidth(S, i, io->outputs->signals[i].width);
            ssSetOutputPortDataType(S, i, io->outputs->signals[i].type);
            
            free(d);
        }
        
    } else {
        
        myprintf("CONFIGURATION block (%d)\n", (int) *blockType);
        
        io_allocate_memory(&io);
        
        char *configurationFile = (char *)( mxGetData(ssGetSFcnParam(S,1)) );
        char *logFile = (char *)( mxGetData(ssGetSFcnParam(S,2)) );
        if (configurationFile != NULL){
            myprintf("Configuration file = '%s'\n", configurationFile);
            myprintf("Log file = '%s'\n", logFile);
            read_ed247_configuration(configurationFile, io, logFile);
            
            myprintf("%d inputs\n", io->outputs->nsignals);
            myprintf("%d outputs\n", io->outputs->nsignals);
        }
        
        /*
        signal_info_t* signal_info = (signal_info_t*)malloc(sizeof(signal_info_t));
        signal_info->io = (void*) io;
        ssSetUserData(S, (void*)signal_info);
        
        if (!ssSetNumDWork(S, 0)) return;
        ssSetNumPWork(S, 1);
        // Register reserved identifiers to avoid name conflict
        if (ssRTWGenIsCodeGen(S) || ssGetSimMode(S)==SS_SIMMODE_EXTERNAL) {
            ssRegMdlInfo(S, "io", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));
        }
        */
        
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
	ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
	ssSetOffsetTime(S, 0, 0.0);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
	int i,ndata,status;
	time_T t = ssGetT(S);

    /*
	signal_info_t* signal_info = (signal_info_t*)ssGetUserData(S);
	IO_t *io = (IO_t *)signal_info->io;
    */
    
    if (io == NULL) {
        myprintf("No IO defined\n");
        return;
    }
    
    BLOCK_TYPE_T * blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
    
    if (*blockType == RECEIVE){
                        
        for (i = 0; i < io->outputs->nsignals; i++){
            myprintf("Receive data #%d\n", i);
            io->outputs->signals[i].valuePtr = (void*)ssGetOutputPortSignal(S,i);
        }
        status = (int)receive_ed247_to_simulink(io,&ndata);
        myprintf("Receive status = %d\n", status);
        
    } else if (*blockType == SEND){
        
        for (i = 0; i < io->inputs->nsignals; i++){
            myprintf("Send data #%d\n", i);
            io->inputs->signals[i].valuePtr = (void*)ssGetInputPortSignal(S,i);
        }
        status = (int)send_simulink_to_ed247(io);
        myprintf("Send status = %d\n", status);
    }

}
static void mdlTerminate(SimStruct *S){

    /*
	if (ssGetPWork(S) != NULL) {
		signal_info_t* signal_info = (signal_info_t*)ssGetUserData(S);
		IO_t *io = (IO_t *)signal_info->io;
		if (io != NULL) {
			io_free_memory(io);
		}
	}
    */

}

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
/* Function: mdlRTW ===========================================================
 */
static void mdlRTW(SimStruct *S)
{
	char *configurationFile = (char *)( mxGetData(ssGetSFcnParam(S,0)) );
	if (!ssWriteRTWParamSettings(S, 1,SSWRITE_VALUE_QSTR, "Filename", configurationFile)) {
		return; /* An error occurred. */
	}
}
#endif /* MDL_RTW */

#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif