#include "mex.h"
#include "ed247_interface.h"

#define N_FIELDNAMES 10
#define MAX_STRING_SIZE 20

/* The gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    char* filename;
    size_t buflen;
    int status;
    IO_t *io;
    
    int i;
    char *fieldnames[N_FIELDNAMES];
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
    
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("ED247:arrayProduct:nlhs","One output required.");
    }
    
    /* make sure the first input argument is char */
    if(!mxIsChar(prhs[0]) || (mxGetM(prhs[0]) != 1 ) ){
        mexErrMsgIdAndTxt("ED247:arrayProduct:notScalar","Input filename must be a non-empty char.");
    }
            
    buflen = mxGetN(prhs[0]) + 1;
    filename = mxMalloc(buflen);
    
    /* Copy the string data into buf. */ 
    status = mxGetString(prhs[0], filename, (mwSize) buflen);
    if (status != 0) {
        mexErrMsgIdAndTxt( "ED247:mxGetString", 
                           "Failed to copy input string into allocated memory.");
    }
    
    
    mexPrintf("The input string is:  %s\n", filename);
    
    io_allocate_memory(&io);
    read_ed247_configuration(filename, io, "");
    
    mexPrintf("%d inputs\n", io->inputs->nsignals);
    mexPrintf("%d outputs\n", io->outputs->nsignals);
    
    plhs[0] = mxCreateStructMatrix(io->inputs->nsignals + io->outputs->nsignals,1,N_FIELDNAMES,(const char**)fieldnames);
    
    //Deallocate memory for the fieldnames
    for (i = 0; i < N_FIELDNAMES; i++){
        mxFree( fieldnames[i] );
    }
    
    mxFree(filename);
    io_free_memory(io);
    
}
