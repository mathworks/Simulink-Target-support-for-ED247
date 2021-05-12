#ifndef ED247_SFUN_CONFIGURE__
#define ED247_SFUN_CONFIGURE__

#include "simstruc.h"
#include "ed247_interface.h"

#define ED247_CONFIGURE_INITIALIZE
IO_t* configureInitialize(SimStruct *S);

#define ED247_CONFIGURE_START
void configureStart(SimStruct *S, IO_t *io);

#define ED247_CONFIGURE_OUTPUTS
void configureOutputs(SimStruct *S, IO_t* io);

#define ED247_CONFIGURE_UPDATE
void configureUpdate(SimStruct *S, IO_t* io);

#define ED247_CONFIGURE_TERMINATE
void configureTerminate(SimStruct *S, IO_t* io);

#define ED247_CONFIGURE_RTW
void configureRTW(SimStruct *S, IO_t* io, real_T* blockTypeID, char_T* configurationFile);

#endif