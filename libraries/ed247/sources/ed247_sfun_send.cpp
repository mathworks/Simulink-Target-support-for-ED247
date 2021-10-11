#include "ed247_sfun_send.h"

namespace ed247sfcn {

	/*
	 * CONSTRUCTORS
	 */
	Send::Send(SimStruct *S, DimsInfo_T* di, ed247simulink::ED247Connector* connector){
		_S = S;
		_di = di;
		_connector = connector;
		_tools = new ed247simulink::Tools();
	}

	Send::Send(SimStruct *S, DimsInfo_T* di, ed247simulink::ED247Connector* connector, ed247simulink::Tools* tools){
		_S = S;
		_di = di;
		_connector = connector;
		_tools = tools;
	}

	/*
	 * PUBLIC METHODS
	 */
	void Send::initialize(){

		int isig,iport,idim,nports;
		int_T* isRefreshEnabled;
		int32_T* d;
		data_characteristics_t *inputs;

		_tools->myprintf("\n\n=== SEND INITIALIZATION START ===\n");

		isRefreshEnabled = (int_T *)( mxGetData(ssGetSFcnParam(_S,3)) );
		//_tools->myprintf("Is refresh enabled = %d\n",isRefreshEnabled == NULL ? -1 : (int_T)*isRefreshEnabled);

		//
		// INPUTS
		//
		inputs = _connector->getInputs();
		if (inputs != NULL){

			_tools->myprintf("%d streams\n",inputs->nstreams);
			_tools->myprintf("%d input messages\n", inputs->nsignals);

			nports = inputs->nsignals;
			for (iport = 0; iport < inputs->nsignals && iport < MAX_SIGNALS; iport++){
				if (inputs->signals[iport].is_refresh == 1 ){ /* && *isRefreshEnabled == 1*/
					nports++;
				}
			}
			_tools->myprintf("%d input ports\n",  nports);

			if (!ssSetNumInputPorts(_S, nports)) return;

			_tools->myprintf("Define port characteristics\n");

			iport = 0;
			for (isig = 0; isig < inputs->nsignals && isig < MAX_SIGNALS; isig++){

				//
				// Data port
				//
				_tools->myprintf("Port %d : Signal\n", iport);
				_tools->myprintf("\t-Width = %d\n", inputs->signals[isig].width);
				_tools->myprintf("\t-Dimensions = %d\n", inputs->signals[isig].dimensions);

				_di->width	= inputs->signals[isig].width;
				_di->numDims	= inputs->signals[isig].dimensions;

				d = (int32_T*) malloc(_di->numDims*sizeof(int32_T));
				for (idim = 0; idim < _di->numDims && idim < MAX_DIMENSIONS; idim++){
					_tools->myprintf("\t\t-Dimension #%d = %d\n", idim, inputs->signals[isig].size[idim]);
					d[idim] = (int32_T)(inputs->signals[isig].size[idim]);
				}
				_di->dims = &(d[0]);
				if(!ssSetInputPortDimensionInfo(_S, iport, _di)) return;

				ssSetInputPortWidth(_S, iport, inputs->signals[isig].width);
				ssSetInputPortDirectFeedThrough(_S, iport, 1);
				ssSetInputPortDataType(_S, iport, inputs->signals[isig].type);
				ssSetInputPortRequiredContiguous(_S, iport, 1);

				free(d);

				inputs->signals[isig].port_index = iport;
				iport++;

				//
				// Refresh ports
				//
				if (inputs->signals[isig].is_refresh == 1){ /* && *isRefreshEnabled == 1 */

					_tools->myprintf("Port %d : Refresh\n", iport);

					ssSetInputPortVectorDimension(_S, iport, 1);
					ssSetInputPortDirectFeedThrough(_S, iport, 1);
					ssSetInputPortDataType(_S, iport, SS_BOOLEAN);
					ssSetInputPortRequiredContiguous(_S, iport, 1);

					inputs->signals[isig].refresh_index = iport;
					iport++;

				} else {
					inputs->signals[isig].refresh_index = -1;
				}
			}
		} else {
			_tools->myprintf("WARNING : Input pointer is NULL\n");
		}

		_tools->myprintf("\n=== SEND INITIALIZATION END ===\n\n");

    }

    void Send::start(){}

    void Send::outputs(){
        
        int isig, iport,status;
        int8_T* refresh;
        data_characteristics_t *inputs;
        
        inputs = _connector->getInputs();
        for (isig = 0; isig < inputs->nsignals && isig < MAX_SIGNALS; isig++){
            
            iport = inputs->signals[isig].port_index;
            inputs->signals[isig].valuePtr = (void*)ssGetInputPortSignal(_S,iport);
            
            if (inputs->signals[isig].is_refresh == 1 && inputs->signals[isig].refresh_index >= 0){
                refresh = (int8_T*)ssGetInputPortSignal(_S,inputs->signals[isig].refresh_index);
                _tools->myprintf("Refresh port #%d = %d\n", inputs->signals[isig].refresh_index, *refresh);
                inputs->signals[isig].do_refresh = *refresh;
            } else {
                inputs->signals[isig].do_refresh = 1;
            }
            
            if (inputs->signals[isig].do_refresh == 1){
                _tools->myprintf("Send data from port %d to signal %d\n", iport, isig);
            }
            
        }
        
        status = (int)_connector->sendSimulinkToED247();
        _tools->myprintf("Send status = %d\n", status);
        
    }

    void Send::update(){}

    void Send::terminate(){}

    void Send::RTW(real_T* blockTypeID, int_T* nSignals, real_T portIndex[MAX_SIGNALS], real_T refreshIndex[MAX_SIGNALS]){
        
        int i;
        data_characteristics_t *inputs;
        
        *blockTypeID = 2.0;
        
        inputs = _connector->getInputs();
        if (inputs->nsignals <= MAX_SIGNALS){
            *nSignals = inputs->nsignals;
        } else {
            *nSignals = MAX_SIGNALS;
        }
        
        for (i = 0; i < *nSignals; i++){
            
            portIndex[i] = (real_T) inputs->signals[i].port_index;
            if (inputs->signals[i].is_refresh == 1 && inputs->signals[i].refresh_index >= 0){
                refreshIndex[i] = (real_T) inputs->signals[i].refresh_index;
            } else {
                refreshIndex[i] = -1.0;
            }
            
        }
        
    }
    
}