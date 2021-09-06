#ifndef ED247_SFUN_CONFIGURE__
#define ED247_SFUN_CONFIGURE__

#include "simstruc.h"
#include "ed247_interface.h"

namespace ed247sfcn {
    
    class Configure {
        
    private:
        ed247simulink::ED247Connector* _connector;
        ed247simulink::Tools* _tools;
        SimStruct* _S;
        
    public:
        /*
         * CONSTRUCTORS
         */
        Configure(SimStruct *S, ed247simulink::ED247Connector* connector);
        Configure(SimStruct *S, ed247simulink::ED247Connector* connector, ed247simulink::Tools* tools);
        
        /*
         * S-Function API
         */
        configuration_status_t initialize();
        void start();
        void outputs();
        void update();
        void terminate();
        void RTW(real_T* blockTypeID, char_T* configurationFile);
        
    };
    
}

#endif