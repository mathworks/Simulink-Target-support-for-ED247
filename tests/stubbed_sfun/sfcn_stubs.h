
#ifndef SFCNSTUBS__
#define SFCNSTUBS__

typedef struct {
	int 			blocktype;
	char 			configuration[512];
	char 			logfile[512];
	int 			refresh;
	float 			sampletime;
	int				ninports;
	int				noutports;
	int				ndworks;
	int				npworks;
	int				nsampletimes;
} SimStruct;
#define INIT_SIMSTRUC(type) { type, "", "", 0, 0.2, 0, 0, 0, 0 }

typedef struct {
	int width;
	int numDims;
	int* dims;
} di_T;
extern di_T di;

extern "C" void* ssGetDWork(SimStruct* S,int n);
extern "C" void** ssGetPWork(SimStruct* S);

extern "C" void* ssGetSFcnParam(SimStruct* S,int n);

extern "C" time_T ssGetSampleTime(SimStruct* S, int n);

extern "C" void* ssGetInputPortSignal(SimStruct* S, int iport);
extern "C" void* ssGetOutputPortSignal(SimStruct* S, int iport);

extern "C" void* mxGetData(void* element);

extern "C" int ssSetNumInputPorts(SimStruct* S,int n);
extern "C" int ssSetNumOutputPorts(SimStruct* S,int n);
extern "C" int ssSetNumDWork(SimStruct* S,int n);
extern "C" int ssSetNumPWork(SimStruct* S,int n);
extern "C" int ssSetNumSampleTimes(SimStruct* S,int n);

extern "C" int ssSetSampleTime(SimStruct* S,int i, double v);
extern "C" int ssSetOffsetTime(SimStruct* S,int i, double v);

#endif