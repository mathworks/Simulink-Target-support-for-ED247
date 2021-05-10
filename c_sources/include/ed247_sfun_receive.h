#ifndef ED247_SFUN_RECEIVE__
#define ED247_SFUN_RECEIVE__

#include "simstruc.h"
#include "ed247_interface.h"

#define MAX_COUNTER 10000000

#define ED247_RECEIVE_INITIALIZE
void receiveInitialize(SimStruct *S, IO_t *io);

#define ED247_RECEIVE_START
void receiveStart(SimStruct *S, IO_t *io);

#define ED247_RECEIVE_OUTPUTS
void receiveOutputs(SimStruct *S, IO_t* io);

#define ED247_RECEIVE_UPDATE
void receiveUpdate(SimStruct *S, IO_t* io);

#define ED247_RECEIVE_TERMINATE
void receiveTerminate(SimStruct *S, IO_t* io);

#define ED247_RECEIVE_RTW
void receiveRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[100], real_T refreshIndex[100]);

#endif