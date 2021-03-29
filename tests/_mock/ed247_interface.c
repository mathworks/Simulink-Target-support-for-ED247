/*
 *
 * Test command : 
 *	$ gcc ed247_interface.c -o ed247_interface_test -DTEST -DDEBUG && ./ed247_interface_test.exe <configuration_file>
 *
 * Copyright 2020 The MathWorks, Inc.
 */

#include "ed247_interface.h"

FILE *fptr;

/*
 * ED247 INTERFACE
 */
configuration_status_t read_ed247_configuration(const char* filename, IO_t *io, const char* logfilename){

	char type[100];
	char name[100];
	char datatype[100];
	char sizestr[100];
	char *tmp;
	data_characteristics_t* currentio;

	io->inputs->nsignals	= 0;
	io->inputs->nstreams	= 0;
	io->outputs->nsignals	= 0;
	io->outputs->nstreams	= 0;

	if ((fptr = fopen(filename,"r")) == NULL){
		myprintf("Error! opening file");
		return LOAD_FAILURE;
	}

	while (fscanf(fptr,"%s %s %s %s",type,name,datatype,sizestr)==4){

		if (strcmp(type, "In") == 0) {
			currentio = io->inputs;
		}
		else {
			currentio = io->outputs;
		}
		strcpy(currentio->signals[currentio->nsignals].name,name);
		currentio->signals[currentio->nsignals].type = getDataType(datatype);

		tmp = strtok(sizestr,"x");
		currentio->signals[currentio->nsignals].width = 1;
		currentio->signals[currentio->nsignals].dimensions = 0;

		if (tmp == NULL){
			currentio->signals[currentio->nsignals].size[currentio->signals[currentio->nsignals].dimensions] = 1;
			currentio->signals[currentio->nsignals].width *= currentio->signals[currentio->nsignals].size[currentio->signals[currentio->nsignals].dimensions];
			currentio->signals[currentio->nsignals].dimensions++;
		} else {
			while (tmp != NULL){
				currentio->signals[currentio->nsignals].size[currentio->signals[currentio->nsignals].dimensions] = atoi(tmp);
				currentio->signals[currentio->nsignals].width *= currentio->signals[currentio->nsignals].size[currentio->signals[currentio->nsignals].dimensions];
				currentio->signals[currentio->nsignals].dimensions++;
				tmp = strtok(NULL,"x");
			}
		}

		currentio->nsignals++;

	}
	fclose(fptr);

	return CONFIGURATION_SUCCESS;

}

send_status_t send_simulink_to_ed247(IO_t *io){

	int i, j;

	double			tmpDbl;
	float			tmpFlt;
	char			tmpChar;
	unsigned char	tmpUchar;
	short			tmpShort;
	unsigned short	tmpUshort;
	int				tmpInt;
	unsigned int	tmpUint;
	unsigned char	tmpBool;

	for (i=0; i<io->inputs->nsignals; i++){

		for (j = 0; j < io->inputs->signals[i].width; j++){

			switch (io->inputs->signals[i].type){
				case SS_DOUBLE:
				tmpDbl = *((double*) io->inputs->signals[i].valuePtr + j); 
				myprintf("Input #%d<%s>[%d] = %f\n", i, "double", j, tmpDbl);
				break;
				case SS_SINGLE:
				tmpFlt = *((float*) io->inputs->signals[i].valuePtr + j); 
				myprintf("Input #%d<%s>[%d] = %f\n", i, "single", j, tmpFlt);
				break;
				case SS_INT8:
				tmpChar = *((char*) io->inputs->signals[i].valuePtr + j); 
				myprintf("Input #%d<%s>[%d] = %d\n", i, "int8", j, tmpChar);
				break;
				case SS_UINT8:
				tmpUchar = *((unsigned char*) io->inputs->signals[i].valuePtr + j); 
				myprintf("Input #%d<%s>[%d] = %d\n", i, "uint8", j, tmpUchar);
				break;
				case SS_INT16:
				tmpShort = *((short*) io->inputs->signals[i].valuePtr + j); 
				myprintf("Input #%d<%s>[%d] = %d\n", i, "int16", j, tmpShort);
				break;
				case SS_UINT16:
				tmpUshort = *((unsigned short*) io->inputs->signals[i].valuePtr + j); 
				myprintf("Input #%d<%s>[%d] = %d\n", i, "uint16", j, tmpUshort);
				break;
				case SS_INT32:
				tmpInt = *((int*) io->inputs->signals[i].valuePtr + j); 
				myprintf("Input #%d<%s>[%d] = %d\n", i, "int32", j, tmpInt);
				break;
				case SS_UINT32:
				tmpUint = *((unsigned int*) io->inputs->signals[i].valuePtr + j); 
				myprintf("Input #%d<%s>[%d] = %d\n", i, "uint32", j, tmpUint);
				break;
				case SS_BOOLEAN:
				tmpBool = *((unsigned char*) io->inputs->signals[i].valuePtr + j); 
				myprintf("Input #%d<%s>[%d] = %d\n", i, "boolean", j, tmpBool);
				break;
			}

		}

	}

	return SEND_OK;

}

receive_status_t receive_ed247_to_simulink(IO_t *io, int *n){

	int i,j;

	double			tmpDbl;
	float			tmpFlt;
	char			tmpChar;
	unsigned char	tmpUchar;
	short			tmpShort;
	unsigned short	tmpUshort;
	int				tmpInt;
	unsigned int	tmpUint;
	unsigned char	tmpBool;

	static int seed = 0;
	myprintf("Seed = %d\n", seed++);

	for (i=0; i<io->outputs->nsignals; i++){

		for (j = 0; j < io->outputs->signals[i].width; j++){

			switch (io->outputs->signals[i].type){
				case SS_DOUBLE:
				tmpDbl = (double)((seed+j) * 52.5);
				myprintf("Output #%d<%s>[%d] = %f\n", i, "double", j, tmpDbl);
				*((double*)io->outputs->signals[i].valuePtr + j) = tmpDbl;
				break;
				case SS_SINGLE:
				tmpFlt = (float)((seed+j) * 0.25);
				myprintf("Output #%d<%s>[%d] = %f\n", i, "single", j, tmpFlt);
				*((float*)io->outputs->signals[i].valuePtr + j) = tmpFlt;
				break;
				case SS_INT8:
				tmpChar = (char)(-127 + (seed+j));
				myprintf("Output #%d<%s>[%d] = %d\n", i, "int8", j, tmpChar);
				*((char*)io->outputs->signals[i].valuePtr + j) = tmpChar;
				break;
				case SS_UINT8:
				tmpUchar = (unsigned char)(0 + (seed+j));
				myprintf("Output #%d<%s>[%d] = %d\n", i, "uint8", j, tmpUchar);
				*((unsigned char*)io->outputs->signals[i].valuePtr + j) = tmpUchar;
				break;
				case SS_INT16:
				tmpShort = (short)(-1000 + (seed+j));
				myprintf("Output #%d<%s>[%d] = %d\n", i, "int16", j, tmpShort);
				*((short*)io->outputs->signals[i].valuePtr + j) = tmpShort;
				break;
				case SS_UINT16:
				tmpUshort = (unsigned short)(1000 + (seed+j));
				myprintf("Output #%d<%s>[%d] = %d\n", i, "uint16", j, tmpUshort);
				*((unsigned short*)io->outputs->signals[i].valuePtr + j) = tmpUshort;
				break;
				case SS_INT32:
				tmpInt = (int)(-2253 + (seed+j));
				myprintf("Output #%d<%s>[%d] = %d\n", i, "int32", j, tmpInt);
				*((int*)io->outputs->signals[i].valuePtr + j) = tmpInt;
				break;
				case SS_UINT32:
				tmpUint = (unsigned int)(2253 + (seed+j));
				myprintf("Output #%d<%s>[%d] = %d\n", i, "uint32", j, tmpUint);
				*((unsigned int*)io->outputs->signals[i].valuePtr + j) = tmpUint;
				break;
				case SS_BOOLEAN:
				tmpBool = (unsigned char)(((seed+j) % 2) > 0);
				myprintf("Output #%d<%s>[%d] = %d\n", i, "boolean", j, tmpBool);
				*((unsigned char*)io->outputs->signals[i].valuePtr + j) = tmpBool;
				break;
			}

		}

	}

	*n = io->outputs->nsignals;
	return RECEIVE_OK;

}

/*
 * MEMORY MANAGEMENT
 */
io_allocation_status_t io_allocate_memory(IO_t ** io){

	if (((*io)=(IO_t*)malloc(sizeof(IO_t))) == NULL) {
		return IO_MALLOC_FAILURE;
	}

	if (((*io)->inputs=(data_characteristics_t*)malloc(sizeof(data_characteristics_t))) == NULL) {
		return INPUTS_MALLOC_FAILURE;
	}

	if (((*io)->outputs=(data_characteristics_t*)malloc(sizeof(data_characteristics_t))) == NULL) {
		return OUTPUTS_MALLOC_FAILURE;
	}

	return IO_ALLOCATION_OK;

}

io_free_status_t io_free_memory(IO_t * io){

	if (io->inputs != NULL) {
		free(io->inputs);
	}
	if (io->outputs != NULL) {
		free(io->outputs);
	} 
	if (io != NULL) {
		free(io);
	}

	return IO_FREE_OK;

}

/*
 * DEBUG / HELPERS
 */
BuiltInDTypeId getDataType(char* type){

	if (strcmp(type,"double") == 0){
		return SS_DOUBLE;
	} else if (strcmp(type,"single") == 0){
		return SS_SINGLE;
	}
	else if (strcmp(type,"int8") == 0){
		return SS_INT8;
	}
	else if (strcmp(type,"uint8") == 0){
		return SS_UINT8;
	}
	else if (strcmp(type,"int16") == 0){
		return SS_INT16;
	}
	else if (strcmp(type,"uint16") == 0){
		return SS_UINT16;
	}
	else if (strcmp(type,"int32") == 0){
		return SS_INT32;
	}
	else if (strcmp(type,"uint32") == 0){
		return SS_UINT32;
	}
	else if (strcmp(type,"boolean") == 0){
		return SS_BOOLEAN;
	}
	else {
		return 9;
	}
}

void myprintf(const char *fmt, ...) {

	int (*printfcn)(const char *, ...);
	char str[1024] = "";
	va_list args;
	va_start(args, fmt);
	vsprintf(str, fmt, args);
	va_end(args);

#ifndef MATLAB_MEX_FILE
	printfcn = &printf;
#else
	printfcn = &ssPrintf;
#endif

#ifdef DEBUG

	(*printfcn)("%s",str);

#endif

}

#ifdef TEST

void main(int argc, char *argv[]){

	int i,j;
	IO_t *io;
	io_allocate_memory(&io);

	if (argc > 1){
		read_ed247_configuration(argv[1], io);
	}
	myprintf("Number of inputs = %d, Number of outputs = %d\n", io->inputs->nsignals, io->outputs->nsignals);

	for (i = 0; i<io->inputs->nsignals; i++){
		myprintf("Input #%d : %s<%d> (%d) [", i+1, io->inputs->signals[i].name, io->inputs->signals[i].type, io->inputs->signals[i].width);
		for (j = 0; j<io->inputs->signals[i].dimensions; j++){
			myprintf("%d,", io->inputs->signals[i].size[j]);
		}
		myprintf("\b]\n");
	}

	for (i = 0; i<io->outputs->nsignals; i++){
		myprintf("Output #%d : %s<%d> (%d) [", i+1, io->outputs->signals[i].name, io->outputs->signals[i].type, io->outputs->signals[i].width);
		for (j = 0; j<io->outputs->signals[i].dimensions; j++){
			myprintf("%d,", io->outputs->signals[i].size[j]);
		}
		myprintf("\b]\n");
	}

	io_free_memory(io);

}

#endif