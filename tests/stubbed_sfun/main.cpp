
#include "simstruc.h"

#include <stdio.h>
#include <string.h>

#include "ed247_sfun.cpp"

SimStruct configureBlock 	= INIT_SIMSTRUC(0); 
SimStruct receiveBlock 		= INIT_SIMSTRUC(1); 
SimStruct sendBlock 		= INIT_SIMSTRUC(2); 

extern "C" {

	int main(int argc, char *argv[]){

		char filename[512];
		char logfile[512];

		if (argc > 1){strcpy(configureBlock.configuration,argv[1]);}
		if (argc > 2){strcpy(configureBlock.logfile,argv[2]);}
		printf("Filename = '%s'\n", configureBlock.configuration);
		printf("Logfile  = '%s'\n", configureBlock.logfile);

		//
		// mdlInitializeSizes()
		//
		printf("[CONFIGURE] mdlInitializeSizes()\n");
		mdlInitializeSizes(&configureBlock);
		//printf("Inputs : Streams = %d, Signals = %d\n", io->inputs->nstreams, io->inputs->nsignals);
		//printf("Outputs : Streams = %d, Signals = %d\n", io->outputs->nstreams, io->outputs->nsignals);

		printf("[SEND] mdlInitializeSizes() ... ");
		mdlInitializeSizes(&sendBlock);
		printf("Input ports = %d (Output ports = %d)\n", sendBlock.ninports, sendBlock.noutports);

		printf("[RECEIVE] mdlInitializeSizes() ... ");
		mdlInitializeSizes(&receiveBlock);
		printf("Output ports = %d (Input ports = %d)\n", receiveBlock.noutports, receiveBlock.ninports);

		return 0;
	/*
		//
		// mdlInitializeSampleTimes()
		//
		printf("[CONFIGURE] mdlInitializeSampleTimes() ... ");
		mdlInitializeSampleTimes(&configureBlock);
		printf("Done\n");

		printf("[SEND] mdlInitializeSampleTimes() ... ");
		mdlInitializeSampleTimes(&sendBlock);
		printf("Done\n");

		printf("[RECEIVE] mdlInitializeSampleTimes() ... ");
		mdlInitializeSampleTimes(&receiveBlock);
		printf("Done\n");

		//
		// mdlStart()
		//
		printf("[CONFIGURE] mdlStart() ... ");
		mdlStart(&configureBlock);
		printf("Done\n");

		printf("[SEND] mdlStart() ... ");
		mdlStart(&sendBlock);
		printf("Done\n");

		printf("[RECEIVE] mdlStart() ... ");
		mdlStart(&receiveBlock);
		printf("Done\n");

		//
		// mdlOutputs()
		//
		printf("[CONFIGURE] mdlOutputs() ... ");
		mdlOutputs(&configureBlock,0);
		printf("Done\n");

		printf("[SEND] mdlOutputs() ... ");
		mdlOutputs(&sendBlock,0);
		printf("Done\n");

		printf("[RECEIVE] mdlOutputs() ... ");
		mdlOutputs(&receiveBlock,0);
		printf("Done\n");

		//
		// mdlUpdate()
		//
		printf("[CONFIGURE] mdlUpdate() ... ");
		mdlUpdate(&configureBlock,0);
		printf("Done\n");

		printf("[SEND] mdlUpdate() ... ");
		mdlUpdate(&sendBlock,0);
		printf("Done\n");

		printf("[RECEIVE] mdlUpdate() ... ");
		mdlUpdate(&receiveBlock,0);
		printf("Done\n");

		//
		// mdlTerminate()
		//
		printf("[CONFIGURE] mdlTerminate() ... ");
		mdlTerminate(&configureBlock);
		printf("Done\n");

		printf("[SEND] mdlTerminate() ... ");
		mdlTerminate(&sendBlock);
		printf("Done\n");

		printf("[RECEIVE] mdlTerminate() ... ");
		mdlTerminate(&receiveBlock);
		printf("Done\n");
	*/
	}

}


