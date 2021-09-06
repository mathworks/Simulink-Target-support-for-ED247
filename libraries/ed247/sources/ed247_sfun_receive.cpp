#include "ed247_sfun_receive.h"

namespace ed247sfcn {
    
    /*
     * CONSTRUCTORS
     */
    Receive::Receive(SimStruct *S, ed247simulink::ED247Connector* connector){
        _S = S;
        _connector = connector;
        _tools = new ed247simulink::Tools();
    }
    
    Receive::Receive(SimStruct *S, ed247simulink::ED247Connector* connector, ed247simulink::Tools* tools){
        _S = S;
        _connector = connector;
        _tools = tools;
    }
    
    /*
     * PUBLIC METHODS
     */
    void Receive::initialize() {
        
        int i,isig,iport,idim,nports;
        int_T nrefresh;
        int_T *refreshFactor;
        int32_T* d;
        data_characteristics_t *outputs;
        
        ssAllowSignalsWithMoreThan2D(_S);
        DECL_AND_INIT_DIMSINFO(di);
        
        _tools->myprintf("\n\n=== RECEIVE INITIALIZATION START ===\n");
        
        refreshFactor = (int_T *)( mxGetData(ssGetSFcnParam(_S,3)) );
        _tools->myprintf("Refresh factor = %d\n",*refreshFactor);
        
        /*
         * OUTPUTS
         */
        outputs = _connector->getOutputs();
        nrefresh = 0;
        for (i = 0; i < outputs->nsignals && i < MAX_SIGNALS; i++){
            nrefresh += outputs->signals[i].is_refresh;
        }
        
        nports = outputs->nsignals;
        if (nports > MAX_SIGNALS){nports = MAX_SIGNALS;}
        if (*refreshFactor > 0){nports = nports + nrefresh;}
        _tools->myprintf("%d streams\n",outputs->nstreams);
        _tools->myprintf("%d output messages\n", outputs->nsignals);
        _tools->myprintf("%d output ports\n", nports);
        
        if (!ssSetNumOutputPorts(_S, nports)) return;
        
        ssSetNumDWork(_S, outputs->nsignals);
        
        iport = 0;
        for (isig = 0; isig < outputs->nsignals && isig < MAX_SIGNALS; isig++){
            
            //
            // Data port
            //
            _tools->myprintf("Port %d : Signal", iport);
            _tools->myprintf(", Width = %d", outputs->signals[isig].width);
            _tools->myprintf(", Dimensions = %d\n", outputs->signals[isig].dimensions);
            
            di.width	= outputs->signals[isig].width;
            di.numDims	= outputs->signals[isig].dimensions;
            d = (int32_T*) malloc(di.numDims*sizeof(int32_T));
            for (idim = 0; idim < di.numDims && idim < MAX_DIMENSIONS; idim++){
                d[idim] = (int32_T)(outputs->signals[isig].size[idim]);
            }
            di.dims = &(d[0]);
            if(!ssSetOutputPortDimensionInfo(_S, iport, &di)) return;
            
            //ssSetOutputPortWidth(_S, iport, outputs->signals[iport].width);
            ssSetOutputPortDataType(_S, iport, outputs->signals[isig].type);
            
            free(d);
            
            outputs->signals[isig].port_index = iport;
            iport++;
            
            //
            // Refresh ports
            //
            if (*refreshFactor > 0 && outputs->signals[isig].is_refresh == 1){
                
                _tools->myprintf("Port %d : Refresh\n", iport);
                
                ssSetOutputPortVectorDimension(_S, iport, 1);
                ssSetOutputPortDataType(_S, iport, SS_BOOLEAN);
                
                outputs->signals[isig].refresh_index = iport;
                iport++;
                
            } else {
                outputs->signals[isig].refresh_index = -1;
            }
            
            //
            // COUNTER
            //
            ssSetDWorkWidth(_S, isig, 1);
            ssSetDWorkDataType(_S, isig, SS_UINT32);
            
        }
        
        _tools->myprintf("\n=== RECEIVE INITIALIZATION END ===\n\n");
        
    }

    void Receive::start(){
        
        uint32_T *last_update;
        int_T iCounter,nCounter;
        
        _tools->myprintf("Counter initialization: ");
        
        nCounter = ssGetNumDWork(_S);
        for (iCounter = 0; iCounter < nCounter; iCounter++){
            last_update = (uint32_T*) ssGetDWork(_S,iCounter);
            *last_update = MAX_COUNTER;
            _tools->myprintf("[%d] = %d ", iCounter, *last_update);
        }
        _tools->myprintf("\n");
        
    }

    void Receive::outputs(){
        
        int isig, iport, ndata, status;
        data_characteristics_t *outputs;
        
        int_T *refreshFactor = (int_T *)( mxGetData(ssGetSFcnParam(_S,3)) );
        time_T sampleTime = ssGetSampleTime(_S, 0);
        
        _tools->myprintf("\n\n=== RECEIVE OUTPUTS START ===\n");
        
        outputs = _connector->getOutputs();
        
        //
        // Prepare output (assign pointer to block output)
        //
        for (isig = 0; isig < outputs->nsignals && isig < MAX_SIGNALS; isig++){
            iport = outputs->signals[isig].port_index;
            _tools->myprintf("Attach output signal #%d to port #%d\n", isig, iport);
            outputs->signals[isig].valuePtr = (void*)ssGetOutputPortSignal(_S,iport);
        }
        
        //
        // Receive data
        //
        _tools->myprintf("Receive data");
        status = (int)_connector->receiveED247ToSimulink(&ndata);
        _tools->myprintf(" with status = %d", status);
        
        if (*refreshFactor > 0){
            
            for (isig = 0; isig < outputs->nsignals && isig < MAX_SIGNALS; isig++){
                
                real_T timeFromLastUpdate;
                uint32_T *last_update;
                int8_T* refresh;
                int_T irefresh = outputs->signals[isig].refresh_index;
                
                if (irefresh >= 0){
                    
                    refresh = (int8_T*)ssGetOutputPortSignal(_S,irefresh);
                    
                    last_update = (uint32_T*) ssGetDWork(_S,isig);
                    timeFromLastUpdate = ((real_T) *last_update) * ((real_T) sampleTime);
                    
                    if (outputs->signals[isig].do_refresh == 1 ||
                            timeFromLastUpdate < outputs->signals[isig].validity_duration){
                        *refresh = 1;
                    } else {
                        *refresh = 0;
                    }
                    _tools->myprintf("\t#%d Refresh = %d (Validity duration = %f sec, Time from last update = %f sec)\n", isig, *refresh, outputs->signals[isig].validity_duration, timeFromLastUpdate);
                    
                }
                
            }
            
        } else {
            _tools->myprintf("\tNo refresh");
        }
        
        _tools->myprintf("\n\n=== RECEIVE OUTPUTS END ===\n");
        
    }

    void Receive::update(){
                
        int isig;
        uint32_T *last_update;
        data_characteristics_t *outputs;
        
        _tools->myprintf("\n\n=== RECEIVE UPDATE START ===\n");
        
        _tools->myprintf("Update receive block:\n");
        outputs = _connector->getOutputs();
        for (isig = 0; isig < outputs->nsignals && isig < MAX_SIGNALS; isig++){
            
            _tools->myprintf("\tSignal #%d", isig);
            
            last_update = (uint32_T*) ssGetDWork(_S,isig);
            _tools->myprintf(" : last update = %d | Action = ", *last_update);
            
            if (outputs->signals[isig].do_refresh == 1){
                _tools->myprintf("Reset counter");
                last_update[0] = 0;
            } else if (*last_update < MAX_COUNTER){
                _tools->myprintf("Increment counter");
                last_update[0] = last_update[0]+1;
            } else {
                _tools->myprintf("Counter saturation");
            }
            _tools->myprintf("\n");
            
        }
        
        _tools->myprintf("\n\n=== RECEIVE OUTPUTS END ===\n");
        
    }

    void Receive::terminate(){}
    
    void Receive::RTW(real_T* blockTypeID, int_T* nSignals, real_T portIndex[MAX_SIGNALS], real_T refreshIndex[MAX_SIGNALS]){
        
        int i;
        data_characteristics_t *outputs;
        
        *blockTypeID = 1.0;
        
        outputs = _connector->getOutputs();
        *nSignals = outputs->nsignals;
        if (*nSignals > MAX_SIGNALS){*nSignals = MAX_SIGNALS;}
        for (i = 0; i < *nSignals; i++){
            portIndex[i] = (real_T) outputs->signals[i].port_index;
            refreshIndex[i] = (real_T) outputs->signals[i].refresh_index;
        }
        
    }
    
}