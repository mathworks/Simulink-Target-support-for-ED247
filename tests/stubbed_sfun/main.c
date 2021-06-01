
#include "simstruc.h"

#include <stdio.h>
#include <string.h>

#include "ed247_sfun.c"

void main(int argc, char *argv[]){

	char filename[512];
	char logfile[512];

	SimStruct configureBlock 	= INIT_SIMSTRUC(0); 
	SimStruct receiveBlock 		= INIT_SIMSTRUC(1); 
	SimStruct sendBlock 		= INIT_SIMSTRUC(2); 

	if (argc > 1){strcpy(configureBlock.configuration,argv[1]);}
	if (argc > 2){strcpy(configureBlock.logfile,argv[2]);}
	printf("Filename = '%s'\n", configureBlock.configuration);
	printf("Logfile  = '%s'\n", configureBlock.logfile);

	/*
	 * mdlInitializeSizes()
	 */
	printf("[CONFIGURE] mdlInitializeSizes()\n");
	mdlInitializeSizes(&configureBlock);
	printf("Inputs : Streams = %d, Signals = %d\n", io->inputs->nstreams, io->inputs->nsignals);
	printf("Outputs : Streams = %d, Signals = %d\n", io->outputs->nstreams, io->outputs->nsignals);

	printf("[SEND] mdlInitializeSizes() ... ");
	mdlInitializeSizes(&sendBlock);
	printf("Input ports = %d (Output ports = %d)\n", sendBlock.ninports, sendBlock.noutports);

	printf("[RECEIVE] mdlInitializeSizes() ... ");
	mdlInitializeSizes(&receiveBlock);
	printf("Output ports = %d (Input ports = %d)\n", receiveBlock.noutports, receiveBlock.ninports);

	/*
	 * mdlInitializeSampleTimes()
	 */
	printf("[CONFIGURE] mdlInitializeSampleTimes() ... ");
	mdlInitializeSampleTimes(&configureBlock);
	printf("Done\n");

	printf("[SEND] mdlInitializeSampleTimes() ... ");
	mdlInitializeSampleTimes(&sendBlock);
	printf("Done\n");

	printf("[RECEIVE] mdlInitializeSampleTimes() ... ");
	mdlInitializeSampleTimes(&receiveBlock);
	printf("Done\n");

	/*
	 * mdlStart()
	 */
	printf("[CONFIGURE] mdlStart() ... ");
	mdlStart(&configureBlock);
	printf("Done\n");

	printf("[SEND] mdlStart() ... ");
	mdlStart(&sendBlock);
	printf("Done\n");

	printf("[RECEIVE] mdlStart() ... ");
	mdlStart(&receiveBlock);
	printf("Done\n");

	/*
	 * mdlOutputs()
	 */
	printf("[CONFIGURE] mdlOutputs() ... ");
	mdlOutputs(&configureBlock,0);
	printf("Done\n");

	printf("[SEND] mdlOutputs() ... ");
	mdlOutputs(&sendBlock,0);
	printf("Done\n");

	printf("[RECEIVE] mdlOutputs() ... ");
	mdlOutputs(&receiveBlock,0);
	printf("Done\n");

	/*
	 * mdlUpdate()
	 */
	printf("[CONFIGURE] mdlUpdate() ... ");
	mdlUpdate(&configureBlock,0);
	printf("Done\n");

	printf("[SEND] mdlUpdate() ... ");
	mdlUpdate(&sendBlock,0);
	printf("Done\n");

	printf("[RECEIVE] mdlUpdate() ... ");
	mdlUpdate(&receiveBlock,0);
	printf("Done\n");

	/*
	 * mdlTerminate()
	 */
	printf("[CONFIGURE] mdlTerminate() ... ");
	mdlTerminate(&configureBlock);
	printf("Done\n");

	printf("[SEND] mdlTerminate() ... ");
	mdlTerminate(&sendBlock);
	printf("Done\n");

	printf("[RECEIVE] mdlTerminate() ... ");
	mdlTerminate(&receiveBlock);
	printf("Done\n");

}

real_T data = 0.0F;
real_T val_real = 0.0F;
int_T val_int = 0;
uint32_T val_uint32 = 0;

void* ssGetDWork(SimStruct* S,int n){
	return (void*) &val_uint32;
}

void* ssGetSFcnParam(SimStruct* S,int n){

	switch (n){

		case 0:
			return (void*) &(S->blocktype);
			break;
		case 1:
			return (void*) S->configuration;
			break;
		case 2:
			return (void*) S->logfile;
			break;
		case 3:
			return (void*) &(S->refresh);
			break;
		case 4:
			return (void*) &(S->sampletime);
			break;
	}

}

time_T ssGetSampleTime(SimStruct* S, int n){
	return 1.0F;
}

void* ssGetOutputPortSignal(SimStruct* S, int iport){
	return (void*) &data;
}

void* ssGetInputPortSignal(SimStruct* S, int iport){
	return (void*) &data;
}

void* mxGetData(void* element){
	return element;
}

int ssSetNumInputPorts(SimStruct* S,int n){
	S->ninports = n;
	return n;
}

int ssSetNumOutputPorts(SimStruct* S,int n){
	S->noutports = n;
	return n;
}

int ssSetNumDWork(SimStruct* S,int n){
	S->ndworks = n;
	return n;
}

int ssSetNumSampleTimes(SimStruct* S,int n){
	S->nsampletimes = n;
	return n;
}

int ssSetSampleTime(SimStruct* S,int i, double v){
	return 0;
}

int ssSetOffsetTime(SimStruct* S,int i, double v){
	return 0;
}
