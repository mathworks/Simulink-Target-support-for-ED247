
#ifndef SIMSTRUC__
#define SIMSTRUC__

#define INHERITED_SAMPLE_TIME	-1.0F

#define ssAllowSignalsWithMoreThan2D(S)
#define DECL_AND_INIT_DIMSINFO(d)

#define ssSetNumSFcnParams(S, i)
#define ssGetNumSFcnParams(S) 	1
#define ssGetSFcnParamsCount(S) 	1
#define ssSetSFcnParamTunable(S, i, b)

#define ssSetOptions(S, m)

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

#include "sfcn_stubs.h"

#endif