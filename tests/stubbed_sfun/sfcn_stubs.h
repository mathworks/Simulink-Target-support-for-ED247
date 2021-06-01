
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

int ssSetSampleTime(SimStruct* S,int i, double v);
int ssSetOffsetTime(SimStruct* S,int i, double v);

#endif