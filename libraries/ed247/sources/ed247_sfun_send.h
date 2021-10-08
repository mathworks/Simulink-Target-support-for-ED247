#ifndef ED247_SFUN_SEND__
#define ED247_SFUN_SEND__

#include "simstruc.h"
#include "ed247_interface.h"

namespace ed247sfcn {
    
    class Send {
        
    private:
        ed247simulink::ED247Connector* _connector;
        ed247simulink::Tools* _tools;
        SimStruct* _S;
		di_T* _di;

    public:
        /*
         * CONSTRUCTORS
         */
        Send(SimStruct *S, di_T* di, ed247simulink::ED247Connector* connector);
        Send(SimStruct *S, di_T* di, ed247simulink::ED247Connector* connector, ed247simulink::Tools* tools);
        
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
