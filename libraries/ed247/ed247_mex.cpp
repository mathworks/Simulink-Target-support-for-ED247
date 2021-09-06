#include "mex.h"

#include "ed247_interface.h"
#include "tools.h"

#define N_FIELDNAMES 10
#define MAX_STRING_SIZE 20

void fillStructure(mxArray *S, int index, signal_characteristics_t signal);

/* The gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
					int nrhs, const mxArray *prhs[])
{
	char* filename;
	size_t buflen;
	int status;	
	int nIn, nOut;
	int i,j;
	char *fieldnames[N_FIELDNAMES];

    data_characteristics_t* inputs;
	data_characteristics_t* outputs;
    
    ed247simulink::Tools* tools;
	ed247simulink::ED247Connector* connector;
    
	fieldnames[0] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[0],"name",sizeof("name"));
	fieldnames[1] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[1],"direction",sizeof("direction"));
	fieldnames[2] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[2],"type",sizeof("type"));
	fieldnames[3] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[3],"dimensions",sizeof("dimensions"));
	fieldnames[4] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[4],"width",sizeof("width"));
	fieldnames[5] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[5],"size",sizeof("size"));
	fieldnames[6] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[6],"sample_time",sizeof("sample_time"));
	fieldnames[7] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[7],"validity_duration",sizeof("validity_duration"));
	fieldnames[8] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[8],"signal_type",sizeof("signal_type"));
	fieldnames[9] = (char*)mxMalloc(MAX_STRING_SIZE);
	memcpy(fieldnames[9],"sample_size",sizeof("sample_size"));

	/* check for proper number of arguments */
	if(nrhs!=1) {
		mexErrMsgIdAndTxt("ED247:arrayProduct:nrhs","One inputs required.");
	}

	if(nlhs < 1 ||nlhs > 2) {
		mexErrMsgIdAndTxt("ED247:arrayProduct:nlhs","At least one output is required.");
	}

	/* make sure the first input argument is char */
	if(!mxIsChar(prhs[0]) || (mxGetM(prhs[0]) != 1 ) ){
		mexErrMsgIdAndTxt("ED247:arrayProduct:notScalar","Input filename must be a non-empty char.");
	}

	buflen = mxGetN(prhs[0]) + 1;
	filename = (char*)mxMalloc(buflen);

	/* Copy the string data into buf. */ 
	status = mxGetString(prhs[0], filename, (mwSize) buflen);
	if (status != 0) {
		mexErrMsgIdAndTxt( "ED247:mxGetString", 
							"Failed to copy input string into allocated memory.");
	}
    
    tools = new ed247simulink::Tools();
	connector = new ed247simulink::ED247Connector(filename,*tools);
    
	tools->myprintf("The input string is:  %s\n", filename);

	connector->allocateMemory();
	connector->readED247Configuration();

    inputs = connector->getInputs();
    outputs = connector->getOutputs();
    
	//
	// Configuration structure
	//    
	nIn = inputs->nsignals;
	nOut = outputs->nsignals;
	tools->myprintf("%d input signals, %d output signal\n", nIn, nOut);
	plhs[0] = mxCreateStructMatrix(nIn + nOut,1,N_FIELDNAMES,(const char**)fieldnames);

	for (i = 0; i < nIn; i++){
		tools->myprintf("Fill elements %d/%d\n", i+1, nIn + nOut);
		fillStructure(plhs[0], i, inputs->signals[i]);
	}
	for (j = 0; j < nOut; j++){
		tools->myprintf("Fill elements %d/%d\n", i+j+1, nIn + nOut);
		fillStructure(plhs[0], i+j, outputs->signals[j]);  
	}

	//Deallocate memory for the fieldnames
	for (i = 0; i < N_FIELDNAMES; i++){
		mxFree( fieldnames[i] );
	}

	//
	// List of files
	//
	nIn = inputs->nstreams;
	nOut = outputs->nstreams;
	tools->myprintf("%d input streams, %d output streams\n", nIn, nOut);
	plhs[1] = mxCreateCellMatrix(nIn + nOut, 1);
	for (i = 0; i < nIn; i++){
		tools->myprintf("Store filenames for input stream %d/%d\n", i+1, nIn + nOut);
		mxSetCell(plhs[1],i,mxCreateString(inputs->streams[i].filename));
	}
	for (j = 0; j < nOut; j++){
		tools->myprintf("Store filenames for output stream %d/%d\n", i+j+1, nIn + nOut);
		mxSetCell(plhs[1],i+j,mxCreateString(outputs->streams[j].filename));
	}

	mxFree(filename);
	connector->freeMemory();

}

void fillStructure(mxArray *S, int index, signal_characteristics_t signal){

	int i;
	mxArray *size;
	double  *sizeptr;

	// NAME
	mxSetFieldByNumber(S,index,0, mxCreateString(signal.name));

	// DIRECTION
	if (signal.direction == ED247_DIRECTION_IN){
		mxSetFieldByNumber(S,index,1, mxCreateString("IN"));
	} else if (signal.direction == ED247_DIRECTION_OUT){
		mxSetFieldByNumber(S,index,1, mxCreateString("OUT"));
	} else if (signal.direction == ED247_DIRECTION_INOUT){
		mxSetFieldByNumber(S,index,1, mxCreateString("INOUT"));
	} else {
		mxSetFieldByNumber(S,index,1, mxCreateString("UNKNOWN"));
	}

	// TYPE
	if (signal.type == SS_SINGLE){
		mxSetFieldByNumber(S,index,2, mxCreateString("single"));
	} else if (signal.type == SS_INT8){
		mxSetFieldByNumber(S,index,2, mxCreateString("int8"));
	} else if (signal.type == SS_UINT8){
		mxSetFieldByNumber(S,index,2, mxCreateString("uint8"));
	} else if (signal.type == SS_INT16){
		mxSetFieldByNumber(S,index,2, mxCreateString("int16"));
	} else if (signal.type == SS_UINT16){
		mxSetFieldByNumber(S,index,2, mxCreateString("uint16"));
	} else if (signal.type == SS_INT32){
		mxSetFieldByNumber(S,index,2, mxCreateString("int32"));
	} else if (signal.type == SS_UINT32){
		mxSetFieldByNumber(S,index,2, mxCreateString("uint32"));
	} else if (signal.type == SS_BOOLEAN){
		mxSetFieldByNumber(S,index,2, mxCreateString("logical"));
	} else {
		mxSetFieldByNumber(S,index,2, mxCreateString("double"));
	}

	// DIMENSIONS
	mxSetFieldByNumber(S,index,3, mxCreateDoubleScalar((real_T) signal.dimensions));

	// WIDTH
	mxSetFieldByNumber(S,index,4, mxCreateDoubleScalar((real_T) signal.width));

	// SIZE
	size = mxCreateDoubleMatrix(1,signal.dimensions, mxREAL);
	sizeptr    = mxGetPr(size);
	for(i = 0; i < signal.dimensions; i++){
		sizeptr[i] = (double)signal.size[i];
	}
	mxSetFieldByNumber(S,index,5, size);

	// SAMPLE TIME
	mxSetFieldByNumber(S,index,6, mxCreateDoubleScalar((real_T) signal.sample_time));

	// VALIDITY DURATION
	mxSetFieldByNumber(S,index,7, mxCreateDoubleScalar((real_T) signal.validity_duration));

	// SIGNAL TYPE
	if (signal.stream_type == ED247_STREAM_TYPE_ANALOG){
		mxSetFieldByNumber(S,index,8, mxCreateString("Analog"));
	} else if (signal.stream_type == ED247_STREAM_TYPE_DISCRETE){
		mxSetFieldByNumber(S,index,8, mxCreateString("Discrete"));
	} else if (signal.stream_type == ED247_STREAM_TYPE_NAD){
		mxSetFieldByNumber(S,index,8, mxCreateString("NAD"));
	} else if (signal.stream_type == ED247_STREAM_TYPE_VNAD){
		mxSetFieldByNumber(S,index,8, mxCreateString("VNAD"));
	} else if (signal.stream_type == ED247_STREAM_TYPE_A664){
		mxSetFieldByNumber(S,index,8, mxCreateString("A664"));
	} else if (signal.stream_type == ED247_STREAM_TYPE_A429){
		mxSetFieldByNumber(S,index,8, mxCreateString("A429"));
	} else if (signal.stream_type == ED247_STREAM_TYPE_A825){
		mxSetFieldByNumber(S,index,8, mxCreateString("A825"));
	} else {
		mxSetFieldByNumber(S,index,8, mxCreateString("Undefined"));
	}

	// SAMPLE SIZE
	mxSetFieldByNumber(S,index,9, mxCreateDoubleScalar((real_T) signal.sample_size));

}
