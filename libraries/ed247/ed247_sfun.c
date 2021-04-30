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
	int i,isig,idim,nports;
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
        nports = io->inputs->nsignals;
        for (isig = 0; isig < io->inputs->nsignals; isig++){
            if (io->inputs->signals[isig].is_refresh == 1){
                nports++;
            }
        }
        myprintf("%d streams\n",io->inputs->nstreams);
        myprintf("%d input messages\n", io->inputs->nsignals);
        myprintf("%d input ports\n",  nports);
        
        if (!ssSetNumInputPorts(S, nports)) return;        
        
        isig = 0;
        for (i = 0; i < io->inputs->nsignals; i++){
            
            if (io->inputs->signals[i].is_refresh == 1){
                
                myprintf("Port %d : Refresh\n", isig);
                
                ssSetInputPortVectorDimension(S, isig, 1);
                ssSetInputPortDirectFeedThrough(S, isig, 1);
                ssSetInputPortDataType(S, isig, SS_BOOLEAN);
                ssSetInputPortRequiredContiguous(S, isig, 1);
                
                io->inputs->signals[i].refresh_index = isig;
                isig++;
                
            }
            
            myprintf("Port %d : Signal\n", isig);
            myprintf("\t-Width = %d\n", io->inputs->signals[i].width);
            myprintf("\t-Dimensions = %d\n", io->inputs->signals[i].dimensions);
            
            di.width	= io->inputs->signals[i].width;
            di.numDims	= io->inputs->signals[i].dimensions;
            
            d = (int32_T*) malloc(di.numDims*sizeof(int32_T));
            for (idim = 0; idim < di.numDims; idim++){
                myprintf("\t\t-Dimension #%d = %d\n", idim, io->inputs->signals[i].size[idim]);
                d[idim] = (int32_T)(io->inputs->signals[i].size[idim]);
            }
            di.dims = &(d[0]);
            if(!ssSetInputPortDimensionInfo(S, isig, &di)) return;
            
            ssSetInputPortWidth(S, isig, io->inputs->signals[i].width);
            ssSetInputPortDirectFeedThrough(S, isig, 1);
            ssSetInputPortDataType(S, isig, io->inputs->signals[i].type);
            ssSetInputPortRequiredContiguous(S, isig, 1);
                                                            
            free(d);
            
            io->inputs->signals[i].port_index = isig;            
            isig++;
            
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
        myprintf("%d streams\n", io->outputs->nstreams);
        myprintf("%d outputs\n", io->outputs->nsignals);
        if (!ssSetNumOutputPorts(S, io->outputs->nsignals)) return;
        for (isig = 0; isig < io->outputs->nsignals; isig++){
            
            di.width	= io->outputs->signals[isig].width;
            di.numDims	= io->outputs->signals[isig].dimensions;
            d = (int32_T*) malloc(di.numDims*sizeof(int32_T));
            for (idim = 0; idim < di.numDims; idim++){
                d[idim] = (int32_T)(io->outputs->signals[isig].size[idim]);
            }
            di.dims = &(d[0]);
            if(!ssSetOutputPortDimensionInfo(S, isig, &di)) return;
            
            //ssSetOutputPortWidth(S, isig, io->outputs->signals[isig].width);
            ssSetOutputPortDataType(S, isig, io->outputs->signals[isig].type);
            
            free(d);
        }
        
    } else {
        
        myprintf("CONFIGURATION block (%d)\n", (int_T) *blockType);
        
        io_allocate_memory(&io);
        
        char_T *configurationFile = (char_T *)( mxGetData(ssGetSFcnParam(S,1)) );
        char_T *logFile = (char_T *)( mxGetData(ssGetSFcnParam(S,2)) );
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
	int i,iport,irefresh,ndata,status;
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
            
            iport = io->inputs->signals[i].port_index;
            io->inputs->signals[i].valuePtr = (void*)ssGetInputPortSignal(S,iport);
            
            if (io->inputs->signals[i].is_refresh == 1){     
                int8_T* refresh = (int8_T*)ssGetInputPortSignal(S,io->inputs->signals[i].refresh_index); 
                myprintf("Refresh port #%d = %d\n", io->inputs->signals[i].refresh_index, *refresh);
                io->inputs->signals[i].do_refresh = *refresh;               
            } else {
                io->inputs->signals[i].do_refresh = 1;
            }
            
            if (io->inputs->signals[i].do_refresh == 1){
                myprintf("Send data from port %d to signal %d\n", iport, i);
            }
                        
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
    int_T i;
    char_T  *configurationFile;
    real_T  blockTypeID;
    BLOCK_TYPE_T *blockType;
    
    real_T* portIndex;
    real_T* refreshIndex;
    int_T nSignals;
        
    blockType = (BLOCK_TYPE_T *)( mxGetData(ssGetSFcnParam(S,0)) );
    if (*blockType == SEND){
        
        blockTypeID = 2.0;
        
        nSignals = io->inputs->nsignals;
        portIndex = (real_T*) malloc(sizeof(real_T) * nSignals);
        refreshIndex = (real_T*) malloc(sizeof(real_T) * nSignals);
        
        for (i = 0; i < io->inputs->nsignals; i++){
            
            portIndex[i] = (real_T) io->inputs->signals[i].port_index;
            if (io->inputs->signals[i].is_refresh == 1){ 
                refreshIndex[i] = (real_T) io->inputs->signals[i].refresh_index;
            } else {
                refreshIndex[i] = -1.0;
            }
                        
        }
        
    } else if (*blockType == RECEIVE){
        blockTypeID = 1.0;
    } else {
        blockTypeID = 0.0;
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
    /*
    if (portIndex != NULL){
        free(portIndex);
    }
    if (refreshIndex != NULL){
        free(refreshIndex);
    }
       */ 
}
#endif /* MDL_RTW */

#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif