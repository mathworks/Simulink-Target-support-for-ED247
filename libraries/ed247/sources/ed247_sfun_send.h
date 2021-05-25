#ifndef ED247_SFUN_SEND__
#define ED247_SFUN_SEND__

#include "simstruc.h"
#include "ed247_interface.h"

#define ED247_SEND_INITIALIZE
void sendInitialize(SimStruct *S, IO_t *io);

#define ED247_SEND_START
void sendStart(SimStruct *S, IO_t *io);

#define ED247_SEND_OUTPUTS
void sendOutputs(SimStruct *S, IO_t* io);

#define ED247_SEND_UPDATE
void sendUpdate(SimStruct *S, IO_t* io);

#define ED247_SEND_TERMINATE
void sendTerminate(SimStruct *S, IO_t* io);

#define ED247_SEND_RTW
void sendRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, int_T* nSignals, real_T portIndex[100], real_T refreshIndex[100]);

#endif