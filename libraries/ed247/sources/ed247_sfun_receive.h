#ifndef ED247_SFUN_RECEIVE__
#define ED247_SFUN_RECEIVE__

#include "simstruc.h"
#include "ed247_interface.h"

#define MAX_COUNTER 10000000

namespace ed247sfcn {
    
    class Receive {
        
    private:
        ed247simulink::ED247Connector* _connector;
        ed247simulink::Tools* _tools;
        SimStruct* _S;
        
    public:
        /*
         * CONSTRUCTORS
         */
        Receive(SimStruct *S, ed247simulink::ED247Connector* connector);
        Receive(SimStruct *S, ed247simulink::ED247Connector* connector, ed247simulink::Tools* tools);
        
        /*
         * S-Function API
         */
        void initialize();
        void start();
        void outputs();
        void update();
        void terminate();
        void RTW(real_T* blockTypeID, int_T* nSignals, real_T portIndex[MAX_SIGNALS], real_T refreshIndex[MAX_SIGNALS]);
        
    };
    
}

#endif
