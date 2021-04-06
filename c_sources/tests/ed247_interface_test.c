
#include "ed247_interface.h"
#include <unistd.h>

static void test_assign_data(IO_t *io, int seed);
static void test_free_data(IO_t *io);

void main(int argc, char **argv){

	int i,j,k,l;
	char* configfile;
	int n,seed;

	IO_t *io;

	if (argc < 2){
		myprintf("!! Missing argument : configuration file\n");
		return;
	}
	configfile = argv[1];

	if (argc < 3){
		n = 1;
	} else {
		n = atoi(argv[2]);
	}

	if (argc < 4){
		seed = 1;
	} else {
		seed = atoi(argv[3]);
	}

	myprintf("## Allocate memory\n");
	io_allocate_memory(&io);

	myprintf("## Read configuration : %s\n", configfile);
	read_ed247_configuration(configfile, io, NULL);
	myprintf("\t- Inputs : %d streams, %d signals\n", io->inputs->nstreams, io->inputs->nsignals);
	for (i = 0; i < io->inputs->nstreams; i++){
		myprintf("\t\tStream #%02d : '%s', %d signals\n", i+1, io->inputs->streams[i].name, io->inputs->streams[i].nsignals);
		for (j = 0; j < io->inputs->streams[i].nsignals; j++){
			myprintf("\t\t\tSignal #%02d : '%s'\n", j+1, io->inputs->streams[i].signals[j]->name);
		}
	}
	myprintf("\t- Outputs : %d streams, %d signals\n", io->outputs->nstreams, io->outputs->nsignals);
	for (i = 0; i < io->outputs->nstreams; i++){
		myprintf("\t\tStream #%02d : '%s', %d signals\n", i+1, io->outputs->streams[i].name, io->outputs->streams[i].nsignals);
		for (j = 0; j < io->outputs->streams[i].nsignals; j++){
			myprintf("\t\t\tSignal #%02d : '%s'\n", j+1, io->outputs->streams[i].signals[j]->name);
		}
	}


	myprintf("## Assign test data\n");
	test_assign_data(io,seed);

	myprintf("## Block configuration:\n");
	myprintf("\t - Inputs (%d)\n", io->inputs->nsignals);
	for (i = 0; i < io->inputs->nsignals; i++){
		myprintf("\t\t Name = %s, Size = %d, Type = %d, Sample time = %.3f sec\n", io->inputs->signals[i].name, io->inputs->signals[i].size, io->inputs->signals[i].type, io->inputs->signals[i].sample_time);
	}
	myprintf("\t - Outputs (%d)\n", io->outputs->nsignals);
	for (i = 0; i < io->outputs->nsignals; i++){
		myprintf("\t\t Name = %s, Size = %d, Type = %d, Sample time = %.3f sec\n", io->outputs->signals[i].name, io->outputs->signals[i].size, io->outputs->signals[i].type, io->outputs->signals[i].sample_time);
	}

    myprintf("\n\n========================\nEnter in simulation loop\n--------------\n");

	i = 0;l = 0;
	while (i < n && l < seed && (io->inputs->nsignals > 0 || io->outputs->nsignals > 0)){

		myprintf("## Simulation loop (%d/%d)\n", i, n);

		if (io->inputs->nsignals > 0){

			myprintf("\n\t- Inputs\n");
			send_simulink_to_ed247(io);
			i++;

		}

		if (io->outputs->nsignals > 0){
			myprintf("\n\t- Outputs\n");
			receive_ed247_to_simulink(io,&k);
			l++;
			if (k > 0){
				i++;
				myprintf("\t\t%d values = ", k);
			}

		}

	}

    myprintf("\n--------------\nExit from simulation loop\n========================\n\n");

	myprintf("## Free memory\n");
	test_free_data(io);
	io_free_memory(io);

}

/*
 * TEST UTILITY FUNCTIONS
 */
void test_assign_data(IO_t *io,int seed){

	int i,j;
	void *tmp;

	int n = 0;

	for (i=0; i<io->inputs->nsignals; i++){

		n++;
		j = io->inputs->signals[i].size;

		switch (io->inputs->signals[i].type){
			case SS_DOUBLE:
			tmp = (double*) malloc(sizeof(double));
			*((double*)tmp) = (double)((n+j) * 52.5 * seed);
			myprintf("\t\tInput #%d<%s>[%d] = %f\n", i, "double", j, *((double*)tmp));
			break;
			case SS_SINGLE:
			tmp = (float*) malloc(sizeof(float));
			*((float*)tmp) = (float)((n+j) * 0.25 * seed);
			myprintf("\t\tInput #%d<%s>[%d] = %f\n", i, "float", j, *((float*)tmp));
			break;
			case SS_INT8:
			tmp = (char*) malloc(sizeof(char));
			*((char*)tmp) = (char)(-127 + (n+j+seed));
			myprintf("\t\tInput #%d<%s>[%d] = %d\n", i, "char", j, *((char*)tmp));
			break;
			case SS_UINT8:
			tmp = (unsigned char*) malloc(sizeof(unsigned char));
			*((unsigned char*)tmp) = (unsigned char)(0 + (n+j+seed));
			myprintf("\t\tInput #%d<%s>[%d] = %d\n", i, "unsigned char", j, *((unsigned char*)tmp));
			break;
			case SS_INT16:
			tmp = (short*) malloc(sizeof(short));
			*((short*)tmp) = (short)(-1000 + (n+j+seed));
			myprintf("\t\tInput #%d<%s>[%d] = %d\n", i, "short", j, *((short*)tmp));
			break;
			case SS_UINT16:
			tmp = (unsigned short*) malloc(sizeof(unsigned short));
			*((unsigned short*)tmp) = (unsigned short)(1000 + (n+j+seed));
			myprintf("\t\tInput #%d<%s>[%d] = %d\n", i, "unsigned short", j, *((unsigned short*)tmp));
			break;
			case SS_INT32:
			tmp = (int*) malloc(sizeof(int));
			*((int*)tmp) = (int)(-2253 + (n+j+seed));
			myprintf("\t\tInput #%d<%s>[%d] = %d\n", i, "int", j, *((int*)tmp));
			break;
			case SS_UINT32:
			tmp = (unsigned int*) malloc(sizeof(unsigned int));
			*((unsigned int*)tmp) = (unsigned int)(2253 + (n+j+seed));
			myprintf("\t\tInput #%d<%s>[%d] = %d\n", i, "unsigned int", j, *((unsigned int*)tmp));
			break;
			case SS_BOOLEAN:
			tmp = (unsigned char*) malloc(sizeof(unsigned char));
			*((unsigned char*)tmp) = (unsigned char)(((n+j+seed) % 2) > 0);
			myprintf("\t\tInput #%d<%s>[%d] = %d\n", i, "unsigned char", j, *((unsigned char*)tmp));
			break;
		}

		io->inputs->signals[i].valuePtr = tmp;

	}

	for (i=0; i<io->outputs->nsignals; i++){

		switch (io->outputs->signals[i].type){
			case SS_DOUBLE:
			tmp = (double*) malloc(sizeof(double));
			myprintf("\t\tOutput #%d<%s>\n", i, "double");
			break;
			case SS_SINGLE:
			tmp = (float*) malloc(sizeof(float));
			myprintf("\t\tOutput #%d<%s>\n", i, "float");
			break;
			case SS_INT8:
			tmp = (char*) malloc(sizeof(char));
			myprintf("\t\tOutput #%d<%s>\n", i, "char");
			break;
			case SS_UINT8:
			tmp = (unsigned char*) malloc(sizeof(unsigned char));
			myprintf("\t\tOutput #%d<%s>\n", i, "unsigned char");
			break;
			case SS_INT16:
			tmp = (short*) malloc(sizeof(short));
			myprintf("\t\tOutput #%d<%s>\n", i, "short");
			break;
			case SS_UINT16:
			tmp = (unsigned short*) malloc(sizeof(unsigned short));
			myprintf("\t\tOutput #%d<%s>\n", i, "unsigned short");
			break;
			case SS_INT32:
			tmp = (int*) malloc(sizeof(int));
			myprintf("\t\tOutput #%d<%s>\n", i, "int");
			break;
			case SS_UINT32:
			tmp = (unsigned int*) malloc(sizeof(unsigned int));
			myprintf("\t\tOutput #%d<%s>\n", i, "unsigned int");
			break;
			case SS_BOOLEAN:
			tmp = (unsigned char*) malloc(sizeof(unsigned char));
			myprintf("\t\tOutput #%d<%s>\n", i, "unsigned char");
			break;
		}

		memset(tmp,0,sizeof(tmp));
		io->outputs->signals[i].valuePtr = tmp;

	}

}

void test_free_data(IO_t *io){

	int i;

	for (i=0; i<io->inputs->nsignals; i++){
		if (io->inputs->signals[i].valuePtr != NULL){
			free(io->inputs->signals[i].valuePtr);
		}
	}

	for (i=0; i<io->outputs->nsignals; i++){
		if (io->outputs->signals[i].valuePtr != NULL){
			free(io->outputs->signals[i].valuePtr);
		}
	}

}
