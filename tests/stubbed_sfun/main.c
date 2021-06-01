
#include "simstruc.h"

#include <stdio.h>
#include <string.h>

#include "ed247_sfun.c"

void main(int argc, char *argv[]){

	char filename[512];
	char logfile[512];

	SimStruct data = { 0, "", "", 0, 0.2 };
	SimStruct *S = &data;

	if (argc > 1){strcpy(S->configuration,argv[1]);}
	if (argc > 2){strcpy(S->logfile,argv[2]);}
	printf("Filename = '%s'\n", S->configuration);
	printf("Logfile  = '%s'\n", S->logfile);

	mdlInitializeSizes(S);

	printf("Inputs : Streams = %d, Signals = %d\n", io->inputs->nstreams, io->inputs->nsignals);
	printf("Outputs : Streams = %d, Signals = %d\n", io->outputs->nstreams, io->outputs->nsignals);

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