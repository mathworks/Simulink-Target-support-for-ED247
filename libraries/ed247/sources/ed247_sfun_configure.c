#include "ed247_sfun_configure.h"

#ifdef ED247_CONFIGURE_INITIALIZE
IO_t* configureInitialize(SimStruct *S){

	IO_t *io;

	ssSetNumDWork(S, 0);

	myprintf("\n\n=== CONFIGURATION INITIALIZATION START ===\n");

	io_allocate_memory(&io);

	char_T *configurationFile = (char_T *)( mxGetData(ssGetSFcnParam(S,1)) );
	char_T *logFile = (char_T *)( mxGetData(ssGetSFcnParam(S,2)) );
	if (configurationFile != NULL){
		myprintf("Configuration file = '%s'\n", configurationFile);
		myprintf("Log file = '%s'\n", logFile);
		read_ed247_configuration(configurationFile, io, logFile);

		myprintf("%d input streams, %d input signals\n", io->inputs->nstreams, io->inputs->nsignals);
		myprintf("%d output streams, %d output signals\n", io->outputs->nstreams, io->outputs->nsignals);
	}

	myprintf("\n=== CONFIGURATION INITIALIZATION END ===\n\n");

	return io;

}
#endif

#ifdef ED247_CONFIGURE_START
void configureStart(SimStruct *S, IO_t *io){}
#endif

#ifdef ED247_CONFIGURE_OUTPUTS
void configureOutputs(SimStruct *S, IO_t* io){}
#endif

#ifdef ED247_CONFIGURE_UPDATE
void configureUpdate(SimStruct *S, IO_t* io){}
#endif

#ifdef ED247_CONFIGURE_TERMINATE
void configureTerminate(SimStruct *S, IO_t* io){}
#endif

#ifdef ED247_CONFIGURE_RTW
void configureRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, char_T* configurationFile){
	      
    char *base;
    
    char_T* filename = (char_T *)( mxGetData(ssGetSFcnParam(S,1)) );
    
    base = strtok(filename, "\\");
    while(base) {
        strcpy(filename, base);
        base = strtok(NULL, "\\");
    }
    strncpy( configurationFile, filename, strlen(filename) + 1 );

    *blockTypeID = 0.0;
    
}
#endif