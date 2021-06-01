
#ifndef SIMSTRUC__
#define SIMSTRUC__

#define ssAllowSignalsWithMoreThan2D(S)
#define DECL_AND_INIT_DIMSINFO(d)

#define ssSetNumSFcnParams(S, i)
#define ssGetNumSFcnParams(S) 	1
#define ssGetSFcnParamsCount(S) 	1
#define ssSetSFcnParamTunable(S, i, b)

#define ssSetOptions(S, m)

#define ssSetSampleTime(S,i,v)
#define ssSetOffsetTime(S,i,v)

#define ssSetInputPortDimensionInfo(S, i, d) 1
#define ssSetOutputPortDimensionInfo(S, i, d) 1

#define ssSetInputPortWidth(S, i, w)
#define ssSetInputPortDirectFeedThrough(S, i, b)
#define ssSetInputPortDataType(S, i, t)
#define ssSetInputPortRequiredContiguous(S, i, b)
#define ssSetInputPortVectorDimension(S, i, n)

#define ssSetOutputPortWidth(S,i,n)
#define ssSetOutputPortDataType(S,i,n)
#define ssSetOutputPortVectorDimension(S,i,n)
#define ssSetOutputPortDataType(S,i,n)

#define ssSetDWorkWidth(S,i,n)
#define ssSetDWorkDataType(S,i,n)

#define ssGetNumDWork(S) 1

typedef double real_T;
typedef double time_T;
typedef int int_T;

typedef char char_T;
typedef char int8_T;
typedef unsigned char uint8_T;
typedef short int16_T;
typedef unsigned short uint16_T;
typedef int int32_T;
typedef unsigned int uint32_T;

typedef struct {
	int 			blocktype;
	char 			configuration[512];
	char 			logfile[512];
	int 			refresh;
	float 			sampletime;
	int				ninports;
	int				noutports;
	int				ndworks;
	int				nsampletimes;
} SimStruct;
#define INIT_SIMSTRUC(type) { type, "", "", 0, 0.2, 0, 0, 0, 0 }

typedef struct {
	int width;
	int numDims;
	int* dims;
} di_T;
di_T di;

void* ssGetDWork(SimStruct* S,int n);

void* ssGetSFcnParam(SimStruct* S,int n);

time_T ssGetSampleTime(SimStruct* S, int n);

void* ssGetInputPortSignal(SimStruct* S, int iport);
void* ssGetOutputPortSignal(SimStruct* S, int iport);

void* mxGetData(void* element);

int ssSetNumInputPorts(SimStruct* S,int n);
int ssSetNumOutputPorts(SimStruct* S,int n);
int ssSetNumDWork(SimStruct* S,int n);
int ssSetNumSampleTimes(SimStruct* S,int n);

#endif